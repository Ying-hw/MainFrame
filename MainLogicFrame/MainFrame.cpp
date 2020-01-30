#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "plugin.pb.h"

MainFrame::MainFrame() : QThread(), m_isRunning(true), m_data(nullptr) {
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	connect(this, SIGNAL(ReleaseWidget()), this, SLOT(ReleaseCurrentWidget()));
	connect(this, SIGNAL(InitWidget(bool, const QRect&)), this, SLOT(InitCurrentWidget(bool, const QRect&)));
	g_pSignal->SetUserIdentify(this, User::MAINFRAME);
	this->start();
	StartPluginControl();
}

MainFrame::~MainFrame() {
	m_logFile.close();
	m_isRunning = false;
	m_Loadlib.unload();
	MakePluginsProtobufFile(&m_PluginConfig, true);
}

void MainFrame::ReleaseCurrentWidget() {
	MainWidget::staticThis->m_pWidget->deleteLater();
	delete MainWidget::staticThis->m_pWidget;
	MainWidget::staticThis->m_pWidget = NULL;
	m_Loadlib.unload();
}


void MainFrame::StartPluginControl() {
	m_Loadlib.setFileName(QString("./PluginControl"));
	if (m_Loadlib.load()) {
		void (*pFunction)() = m_Loadlib.resolve("Handle");
		pFunction();
	}
	else
		QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), m_Loadlib.errorString());
}

void MainFrame::ReadPluginConfig() {
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::ReadOnly)) {
		QByteArray protoArray = pluginFile.readAll();
		Allplugins Plugins;
		if (Plugins.ParseFromArray(protoArray.data(), protoArray.size())) {
			for (int i = 0; i < Plugins.mutable_plugin()->size(); i++) {
				PluginInfo info;
				plugins* plugs = Plugins.mutable_plugin(i);
				info.m_str_title = QString::fromStdString(plugs->title());
				info.m_str_name = QString::fromStdString(plugs->name());
				QString strRect = QString::fromStdString(plugs->location());
				QStringList strList = strRect.split(",");
				info.m_rPosition = QRect(strList.at(0).toUInt(),
										 strList.at(1).toUInt(),
										 strList.at(2).toUInt(),
										 strList.at(3).toUInt());
				info.m_str_path = QString::fromStdString(plugs->path());
				info.m_isStart = plugs->isstart();
				for (int i = 0; i < plugs->mutable_child()->size();i++) {
					PluginInfo::ChildProject child;
					plugins_childplugin* childplugin = plugs->mutable_child(i);
					child.m_strTitle = QString::fromStdString(childplugin->title());
					child.m_ChildName = QString::fromStdString(childplugin->childname());
					QString strRect = QString::fromStdString(childplugin->location());
					QStringList strList = strRect.split(",");
					child.m_rPosition = QRect(strList.at(0).toUInt(),
								strList.at(1).toUInt(),
								strList.at(2).toUInt(),
								strList.at(3).toUInt());
					info.m_VecChild.append(child);
				}
				m_PluginConfig.append(info);
			}
			FindPlugin(); 
		} 
		else {
		}
	}
	else {
	}
}

void MainFrame::FindPlugin() {
	std::for_each(m_PluginConfig.begin(), m_PluginConfig.end(),
		[this](const PluginInfo& info){
		if (info.m_isStart) {
			this->m_Loadlib.setFileName(info.m_str_name);
			if (!this->m_Loadlib.load()) {
				QString str = this->m_Loadlib.errorString();
				qDebug() << str;
				WriteLog(str);
				return;
			}
			InitCurrentWidget(true, info.m_rPosition);
		}});
}

void MainFrame::run() {
	m_Mutex_.lock();
	while (m_isRunning) {
		if (m_data == nullptr)
			m_WaitCond.wait(&m_Mutex_);
		else {
			//分发内存消息
			m_data = nullptr;
		}
	}
}

void MainFrame::FreeLib(MainFrame* pthis, const QString& strRect) {
	for (QVector<PluginInfo>::iterator it = pthis->m_PluginConfig.begin();
		it != pthis->m_PluginConfig.end(); it++) {
		if (it->m_str_title == MainWidget::staticThis->m_pWidget->windowTitle()) {
			it->m_rPosition = QRect(strRect.section(",", 0, 0).toUInt(),
				strRect.section(",", 1, 1).toUInt(),
				strRect.section(",", 2, 2).toUInt(),
				strRect.section(",", 3, 3).toUInt());
		}
	}
	emit pthis->ReleaseWidget();
}

void MainFrame::LoadLib(MainFrame* pthis, const QString strTargetName) {
	pthis->m_Loadlib.setFileName("./" + strTargetName);
	if (!pthis->m_Loadlib.load()) {
		QString str = pthis->m_Loadlib.errorString();
		pthis->WriteLog(str);
	}
	else {
		std::for_each(pthis->m_PluginConfig.begin(), pthis->m_PluginConfig.end(), [&](const PluginInfo& info){
			if (info.m_str_name == strTargetName) 
				emit pthis->InitWidget(false, info.m_rPosition);
		});
	}
}

void MainFrame::InitCurrentWidget(bool isProgramStart, const QRect& rect) {
	typedef QWidget* (*pFunction)();
	pFunction pfun = (pFunction)(m_Loadlib.resolve("Handle"));
	if (pfun) {
		QWidget* target = pfun();
		if (isProgramStart)
			MainWidget::staticThis->setMain(target, rect, isProgramStart);
		else
			MainWidget::staticThis->setMain(target, rect);
	}
	else
		qDebug() << QString::fromLocal8Bit("空的");
}

const QRect MainFrame::FindChildUiLocation(QWidget* targetWidget) {
	QRect rect;
	std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [&](const PluginInfo& plugin){
		bool isFind = false;
		if (plugin.m_str_name == targetWidget->objectName()) {
			rect = plugin.m_rPosition;
			return true;
		}
		if (!plugin.m_VecChild.isEmpty()) {
			std::find_if(plugin.m_VecChild.begin(), plugin.m_VecChild.end(), [&, this](const PluginInfo::ChildProject& child){
				if (targetWidget->objectName() == child.m_ChildName) {
					rect = child.m_rPosition;
					isFind = true;
					return isFind;
				}
				return isFind;
			});
			return isFind;
		}
		return isFind;
	});
	return rect.isValid() ? rect : QRect();
}

void MainFrame::MakePluginsProtobufFile(void* source, bool isExit)
{
	Allplugins allPlugins;
	QVector<PluginInfo>* soucrePlugin = (QVector<PluginInfo>*)source;
	for (QVector<PluginInfo>::iterator it = soucrePlugin->begin();
		it != soucrePlugin->end();it++) {
		  plugins* plug = allPlugins.add_plugin();
		  qDebug("0000000000000000000000000000000000");
		  plug->set_isstart(it->m_isStart);
		  qDebug("1111111111111111111111111111111111");
		  plug->set_name(it->m_str_name.toStdString());
		  qDebug("2222222222222222222222222222222222");
		  QString strRect = QString("%1,%2,%3,%4").arg(it->m_rPosition.x()).arg(it->m_rPosition.y()).
			  arg(it->m_rPosition.width()).arg(it->m_rPosition.height());
		  qDebug("3333333333333333333333333333333333");
		  plug->set_location(strRect.toStdString());
		  plug->set_path(it->m_str_path.toStdString());
		  plug->set_title(it->m_str_title.toLocal8Bit().data());
		  for (QVector<PluginInfo::ChildProject>::iterator itChild = it->m_VecChild.begin();
			  itChild != it->m_VecChild.begin();itChild++) {
			  plugins_childplugin* child = plug->add_child();
			  child->set_title(itChild->m_strTitle.toStdString());
			  QString strRect = QString("%1,%2,%3,%4").arg(itChild->m_rPosition.x()).arg(itChild->m_rPosition.y()).
				  arg(itChild->m_rPosition.width()).arg(itChild->m_rPosition.height());
			  child->set_location(strRect.toStdString());
			  child->set_childname(itChild->m_ChildName.toStdString());
		  }
	}
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::WriteOnly)) {
		std::string strAllData = allPlugins.SerializeAsString();
		pluginFile.write(strAllData.c_str());
		pluginFile.close();
	}
	if (!isExit) 
		ReadPluginConfig();
}

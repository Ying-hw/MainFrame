 #include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"

MainFrame::MainFrame() : QObject() {
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	connect(this, SIGNAL(ReleaseWidget()), this, SLOT(ReleaseCurrentWidget()));
	connect(this, SIGNAL(InitWidget(const PluginInfo*)), this, SLOT(InitCurrentWidget(const PluginInfo*)));
	g_pSignal->SetUserIdentify(this, SystemUser::MAINFRAME);
	StartPluginControl();
}

MainFrame::~MainFrame() {
	m_logFile.close();
	m_Loadlib.unload();
	UpdateConfigFile();
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
		QMessageBox::warning(NULL, QString::fromLocal8Bit("´íÎó"), m_Loadlib.errorString());
}

void MainFrame::UpdateConfigFile() {
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::WriteOnly)) {
		pluginFile.write(m_pAllPlugins.SerializeAsString().c_str());
		qDebug() << QString::fromLocal8Bit(m_pAllPlugins.DebugString().c_str());
		pluginFile.close();
	}
	else 
		QMessageBox::warning(NULL, QString::fromLocal8Bit("´íÎó"), pluginFile.errorString());
}

void MainFrame::ReadPluginConfig() {
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::ReadOnly)) {
		QByteArray protoArray = pluginFile.readAll();
		Allplugins Plugins;
		if (Plugins.ParseFromArray(protoArray.data(), protoArray.size()))
			for (int i = 0; i < Plugins.mutable_plugin()->size(); i++) {
				PluginInfo info;
				plugins* plugs = Plugins.mutable_plugin(i);
				info.m_str_name = QString::fromStdString(plugs->name());
				info.m_str_path = QString::fromStdString(plugs->path());
				info.m_isStart = plugs->isstart();
				m_PluginConfig.append(info);
			}
			FindPlugin(); 
		} 
		else {
		}
}

void MainFrame::FindPlugin() {
	std::for_each(m_PluginConfig.begin(), m_PluginConfig.end(),
		[this](const PluginInfo& info){
		if (info.m_isStart) {
			this->m_Loadlib.setFileName(info.m_str_path + "/" + info.m_str_name);
			if (!this->m_Loadlib.load()) {
				QString str = this->m_Loadlib.errorString();
				qDebug() << str;
				WriteLog(str);
				return;
			}
			else {
				m_CurrentWindowName = info.m_str_name;
				InitCurrentWidget(&info);
			}
		}});
}

void MainFrame::FreeLib(MainFrame* pthis) {
	pthis->UpdataGeometry();
	emit pthis->ReleaseWidget();
}

void MainFrame::LoadLib(MainFrame* pthis, const QString strTargetName) {
	PluginInfo* targetPlug =  std::find_if(pthis->m_PluginConfig.begin(), pthis->m_PluginConfig.end(), [&](const PluginInfo& plug){
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		pthis->m_Loadlib.setFileName(targetPlug->m_str_path + "/" + strTargetName);
		if (!pthis->m_Loadlib.load()) {
			QString str = pthis->m_Loadlib.errorString();
			qDebug() << pthis->m_Loadlib.fileName();
			pthis->WriteLog(str);
		}
		else { 
			pthis->m_CurrentWindowName = targetPlug->m_str_name;
			emit pthis->InitWidget(targetPlug);
		}
	}
}

void* MainFrame::LoadLib(MainFrame* pthis, const QString strTargetName, bool noShow)
{
	PluginInfo* targetPlug = std::find_if(pthis->m_PluginConfig.begin(), pthis->m_PluginConfig.end(), [&](const PluginInfo& plug) {
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		pthis->m_Loadlib.setFileName(targetPlug->m_str_path + "/" + strTargetName);
		if (!pthis->m_Loadlib.load()) {
			QString str = pthis->m_Loadlib.errorString();
			qDebug() << pthis->m_Loadlib.fileName();
			pthis->WriteLog(str);
		}
		else {
			typedef QWidget* (*pFunction)();
			pFunction pfun = (pFunction)(pthis->m_Loadlib.resolve("Handle"));
			if (pfun) {
				QWidget* target = pfun();
				return target;
			}
		}
	}
	return nullptr;
}

void MainFrame::InitCurrentWidget(const PluginInfo* targetPlug) {
	typedef QWidget* (*pFunction)();     
	pFunction pfun = (pFunction)(m_Loadlib.resolve("Handle"));
	if (pfun) {
		QWidget* target = pfun();
		const QRect rect = FindChildUiLocation(target, target->metaObject()->className());
		if (targetPlug->m_isStart)
			MainWidget::staticThis->setMain(target, rect, target->windowTitle());
		else 
			MainWidget::staticThis->setMain(target, rect);
	}
	else
		qDebug() << QString::fromLocal8Bit("¿ÕµÄ");
}

const QRect MainFrame::FindChildUiLocation(const QWidget* targetWidget, const QString& ChildName) {
	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		if (m_pAllPlugins.plugin(i).name() == m_CurrentWindowName.toStdString()) 
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size();j++) 
				if (m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->childname() == ChildName.toStdString()) {
					QString strRect = QString::fromStdString(m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->location());
					return STRTOLOCATION(strRect);
				} 

	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		if (m_pAllPlugins.plugin(i).name() == m_CurrentWindowName.toStdString()) {
			plugins_childplugin* pChilds = m_pAllPlugins.mutable_plugin(i)->add_child();
			pChilds->set_childname(ChildName.toStdString());
			pChilds->set_title(targetWidget->windowTitle().toStdString());
			QString strLocation = LOCATIONTOSTR(targetWidget);
			pChilds->set_location(strLocation.toStdString());
		}
	return targetWidget->geometry();
}

void MainFrame::MakePluginsProtobufFile(void* source)
{
	std::vector<PluginInfo>* soucrePlugin = (std::vector<PluginInfo>*)source;
	for (std::vector<PluginInfo>::iterator it = soucrePlugin->begin();
		it != soucrePlugin->end(); it++) {
		plugins* plug = m_pAllPlugins.add_plugin();
		plug->set_isstart(it->m_isStart);
		plug->set_name(it->m_str_name.toStdString());
		plug->set_path(it->m_str_path.toStdString());
	}
	Allplugins locationPlugins;
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::ReadWrite)) {
		QByteArray allLocationInfo = pluginFile.readAll();
		if (locationPlugins.ParseFromArray(allLocationInfo.data(), allLocationInfo.size()))
			if (locationPlugins.plugin_size() != 0)
				for (int j = 0; j < m_pAllPlugins.plugin_size(); j++)
					for (int i = 0; i < locationPlugins.plugin_size(); i++)
						if (m_pAllPlugins.mutable_plugin(j)->name() == locationPlugins.mutable_plugin(i)->name()) {
							m_pAllPlugins.mutable_plugin(j)->set_isstart(locationPlugins.mutable_plugin(i)->isstart());
							for (int index = 0; index < locationPlugins.mutable_plugin(i)->child_size(); index++) {
								plugins_childplugin* pChild = m_pAllPlugins.mutable_plugin(j)->add_child();
								pChild->set_childname(locationPlugins.mutable_plugin(i)->child(index).childname());
								pChild->set_location(locationPlugins.mutable_plugin(i)->child(index).location());
								pChild->set_title(locationPlugins.mutable_plugin(i)->child(index).title());
							}
						}
					}
	std::string strAllData = m_pAllPlugins.SerializeAsString();
	pluginFile.seek(0);
	pluginFile.write(strAllData.c_str());
	pluginFile.close(); 
	ReadPluginConfig(); 
}

void MainFrame::UpdataGeometry() {
	QWidget* currentWidget = MainWidget::staticThis;
	QString strName = MainWidget::staticThis->m_pWidget->metaObject()->className();
	for (int i = 0; i < m_pAllPlugins.plugin_size();i++) {
		for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) {
			plugins_childplugin* plug = m_pAllPlugins.mutable_plugin(i)->mutable_child(j);
			if (QString::fromLocal8Bit(plug->childname().c_str()) == strName) {
				QString strLocation = QString("%1,%2,%3,%4").arg(currentWidget->geometry().x()).arg(currentWidget->y())
					.arg(currentWidget->width()).arg(currentWidget->height());
				plug->set_location(strLocation.toStdString());
			}
		}
	}
}

 #include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include <typeinfo>


MainFrame::MainFrame() : QObject() {
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	connect(this, SIGNAL(ReleaseWidget()), this, SLOT(ReleaseCurrentWidget()));
	connect(this, SIGNAL(InitWidget(const PluginInfo*)), this, SLOT(InitCurrentWidget(const PluginInfo*)));
	g_pSignal->SetUserIdentify(this, User::MAINFRAME);
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
	else {
		QMessageBox::warning(NULL, QString::fromLocal8Bit("´íÎó"), pluginFile.errorString());
	}
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
				info.m_str_title = QString::fromStdString(plugs->title());
				info.m_str_name = QString::fromStdString(plugs->name());
				QString strRect = QString::fromStdString(plugs->location());
				if (!strRect.isEmpty()) {
					QStringList strList = strRect.split(",");
					info.m_rPosition = QRect(strList.at(0).toUInt(),
						strList.at(1).toUInt(),
						strList.at(2).toUInt(),
						strList.at(3).toUInt());
				}
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

void MainFrame::InitCurrentWidget(const PluginInfo* targetPlug) {
	typedef QWidget* (*pFunction)();
	pFunction pfun = (pFunction)(m_Loadlib.resolve("Handle"));
	if (pfun) {
		QWidget* target = pfun();
		if (targetPlug->m_isStart)
			MainWidget::staticThis->setMain(target, targetPlug->m_rPosition, targetPlug->m_str_title);
		else
			MainWidget::staticThis->setMain(target, targetPlug->m_rPosition);
	}
	else
		qDebug() << QString::fromLocal8Bit("¿ÕµÄ");
}

const QRect MainFrame::FindChildUiLocation(const QWidget* targetWidget, const QString& TypeName) {
	for (QVector<PluginInfo>::iterator it_Tag = m_PluginConfig.begin();
		it_Tag != m_PluginConfig.end(); it_Tag++) 
		if (it_Tag->m_str_name == TypeName) 
			return it_Tag->m_rPosition;

	for (QVector<PluginInfo>::iterator it_Tag = m_PluginConfig.begin();
		it_Tag != m_PluginConfig.end(); it_Tag++) 
		for (QVector<PluginInfo::ChildProject>::iterator it_ = it_Tag->m_VecChild.begin();
			it_ != it_Tag->m_VecChild.end();it_++) 
			if (it_->m_ChildName == TypeName) 
				return it_->m_rPosition;

	for (QVector<PluginInfo>::iterator it_Tag = m_PluginConfig.begin();
		it_Tag != m_PluginConfig.end(); it_Tag++) 
			if (it_Tag->m_str_name == m_CurrentWindowName) {
				PluginInfo::ChildProject child;
				child.m_ChildName = TypeName;
				child.m_rPosition = targetWidget->geometry();
				child.m_strTitle = targetWidget->windowTitle();
				it_Tag->m_VecChild.append(child);
			}
	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		if (m_pAllPlugins.plugin(i).name() == m_CurrentWindowName.toStdString()) {
			plugins_childplugin* pChilds = m_pAllPlugins.mutable_plugin(i)->add_child();
			pChilds->set_childname(TypeName.toStdString());
			pChilds->set_title(targetWidget->windowTitle().toStdString());
			QString strLocation = QString("%1,%2,%3,%4").arg(targetWidget->geometry().x())
				.arg(targetWidget->geometry().y()).arg(targetWidget->geometry().width())
				.arg(targetWidget->geometry().height());
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
		plug->set_title(it->m_str_title.toLocal8Bit().data());
		for (QVector<PluginInfo::ChildProject>::iterator itChild = it->m_VecChild.begin();
			itChild != it->m_VecChild.begin(); itChild++) {
			plugins_childplugin* child = plug->add_child();
			child->set_title(itChild->m_strTitle.toStdString());
			child->set_childname(itChild->m_ChildName.toStdString());
		}
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
							m_pAllPlugins.mutable_plugin(j)->set_location(locationPlugins.mutable_plugin(i)->location());
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
	QString strTitle = QString::fromLocal8Bit(currentWidget->windowTitle().toStdString().c_str());
	for (int i = 0; i < m_pAllPlugins.plugin_size();i++) {
		if (m_pAllPlugins.mutable_plugin(i)->name() == m_CurrentWindowName.toStdString()) {
			QString strLocation = QString("%1,%2,%3,%4").arg(currentWidget->geometry().x()).arg(currentWidget->y())
				.arg(currentWidget->width()).arg(currentWidget->height());
			m_pAllPlugins.mutable_plugin(i)->set_location(strLocation.toStdString());
			break;
		}
		for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) {
			plugins_childplugin* plug = m_pAllPlugins.mutable_plugin(i)->mutable_child(j);
			if (QString::fromLocal8Bit(plug->title().c_str()) == strTitle) {
				QString strLocation = QString("%1,%2,%3,%4").arg(currentWidget->geometry().x()).arg(currentWidget->y())
					.arg(currentWidget->width()).arg(currentWidget->height());
				plug->set_location(strLocation.toStdString());
			}
		}
	}
	for (int i = 0; i < m_PluginConfig.size();i++) {
		if (m_PluginConfig.at(i).m_str_name == m_CurrentWindowName) {
			QString strLocation = QString("%1,%2,%3,%4").arg(currentWidget->geometry().x()).arg(currentWidget->y())
				.arg(currentWidget->width()).arg(currentWidget->height());
			const_cast<QRect&>(m_PluginConfig.at(i).m_rPosition) = currentWidget->geometry();
			break;
		}
		for (int j = 0; j < m_PluginConfig.at(i).m_VecChild.size();j++) {
			if (m_PluginConfig.at(i).m_VecChild.at(j).m_strTitle == strTitle) {
				QRect lastRect = const_cast<QRect&>(currentWidget->geometry());
				const_cast<QRect&>(m_PluginConfig.at(i).m_VecChild.at(j).m_rPosition) = lastRect;
			}
		}
	}
}

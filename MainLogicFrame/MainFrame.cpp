 #include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "MessageThread.h"

MainFrame::MainFrame() : QObject(), m_pMsgThread(NULL) {
	m_pMsgThread = new MessageThread(this);
	m_pMsgThread->start();
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	connect(this, SIGNAL(ReleaseWidget(const QString&)), this, SLOT(ReleaseCurrentWidget(const QString&)));
	connect(this, SIGNAL(InitWidget(const PluginInfo*)), this, SLOT(InitCurrentWidget(const PluginInfo*)));
	g_pSignal->SetUserIdentify(this, SystemUser::MAINFRAME);
	StartPluginControl();
}

MainFrame::~MainFrame() {
	m_logFile.close();
	for (QLibrary* lib : m_LstLoadlib) {
		lib->unload();
		delete lib;
		lib = NULL;
	}
	UpdateConfigFile();
	if (m_pMsgThread)
	{
		delete m_pMsgThread;
		m_pMsgThread = NULL;
	}
	for (MainWidget* Tgt: m_mapMainWidget)
	{
		if (Tgt) {
			delete Tgt;
			Tgt = NULL;
		}
	}
}

// \todo待完成，当有多个插件加载的时候释放目标错误
void MainFrame::ReleaseCurrentWidget(const QString& strPlugName) {
	if (m_mapMainWidget.contains(m_mapAbstractWidget[strPlugName])) {
		delete m_mapMainWidget[m_mapAbstractWidget[strPlugName]];
		m_mapMainWidget.remove(m_mapAbstractWidget[strPlugName]);
		m_mapAbstractWidget.remove(strPlugName);
	}
	
	for (QList<QLibrary*>::iterator it = m_LstLoadlib.begin();
		it != m_LstLoadlib.end();it++) {
		if ((*it)->fileName().contains(strPlugName)) {
			(*it)->unload();
			m_LstLoadlib.erase(it);
			return;
		}
	}
}


void MainFrame::StartPluginControl() {
	QLibrary lib(QString("./PluginControl"));
	if (lib.load()) {
		void (*pFunction)() = lib.resolve("Handle");
		pFunction();
	}
	else
		QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), lib.errorString());
}

void MainFrame::UpdateConfigFile() {
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::WriteOnly)) {
		pluginFile.write(m_pAllPlugins.SerializeAsString().c_str());
		qDebug() << QString::fromLocal8Bit(m_pAllPlugins.DebugString().c_str());
		pluginFile.close();
	}
	else 
		QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), pluginFile.errorString());
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
			QLibrary* lib = new QLibrary(info.m_str_path + "/" + info.m_str_name);
			if (!lib->load()) {
				QString strError = lib->errorString();
				qDebug() << strError;
				m_logFile.write(strError.toLocal8Bit());
				return;
			}
			else {
				this->m_LstLoadlib.append(lib);
				InitCurrentWidget(&info);
			}
		}});
}

void MainFrame::FreeLib(QWidget* pTgt) {
	for (QMap<QString, AbstractWidget*>::iterator it = m_mapAbstractWidget.begin();
		it != m_mapAbstractWidget.end();it++) 
		if ((*it) == pTgt)
			FreeLib(it.key());
}

void MainFrame::FreeLib(const QString& strPlugName)
{
	UpdataGeometry();
	emit ReleaseWidget(strPlugName);
}

void MainFrame::LoadLib(const QString strTargetName) {
	PluginInfo* targetPlug =  std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [&](const PluginInfo& plug){
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		for (QList<QLibrary*>::iterator it = m_LstLoadlib.begin();
			it != m_LstLoadlib.end();it++) 
			if ((*it)->fileName().contains(targetPlug->m_str_name)) 
				return;
		QLibrary* lib = new QLibrary(targetPlug->m_str_path + "/" + strTargetName);
		if (!lib->load()) {
			QString strError = lib->errorString();
			qDebug() << lib->fileName();
			m_logFile.write(strError.toLocal8Bit());
		}
		else { 
			m_LstLoadlib.append(lib);
			emit InitWidget(targetPlug);
		}
	}
}

void* MainFrame::LoadLib(const QString strTargetName, bool noShow)
{
	PluginInfo* targetPlug = std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [&](const PluginInfo& plug) {
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		QLibrary* lib = new QLibrary(targetPlug->m_str_path + "/" + strTargetName);
		if (!lib->load()) {
			QString strError = lib->errorString();
			qDebug() << lib->fileName();
			m_logFile.write(strError.toLocal8Bit());
		}
		else {
			typedef QWidget* (*pFunction)();
			pFunction pfun = (pFunction)(lib->resolve("Handle"));
			if (pfun) 
				return pfun();
		}
	}
	return nullptr;
}

void MainFrame::Initialize_NetInterface(AbstractNetWork* net)
{
	net->initCommunication();
}

SignalQueue* MainFrame::GetTgtSigQueueInstance(AbstractWidget* pTgtChild)
{
	return m_mapMainWidget[pTgtChild]->m_pSigQueue;
}

void MainFrame::InitCurrentWidget(const PluginInfo* targetPlug) {
	typedef QWidget* (*pFunction)();     
	pFunction pfun = (pFunction)(m_LstLoadlib.last()->resolve("Handle"));
	if (pfun) {
		QWidget* target = pfun();
		m_mapAbstractWidget[targetPlug->m_str_name] = qobject_cast<AbstractWidget*>(target);
		const QRect rect = FindChildUiLocation(target, target->metaObject()->className(), "");
		MainWidget* pmainWidget = new MainWidget;
		m_mapMainWidget[qobject_cast<AbstractWidget*>(target)] = pmainWidget;
		pmainWidget->setMain(qobject_cast<AbstractWidget*>(target), rect, target->windowTitle());
		pmainWidget->InitAanimation();
		pmainWidget->show();
	}
	else
		qDebug() << QString::fromLocal8Bit("空的");
}

const QRect MainFrame::FindChildUiLocation(const QWidget* targetWidget, const QString& ChildName, const QString& strParent) {
	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		for(QList<QLibrary*>::iterator it = m_LstLoadlib.begin();it != m_LstLoadlib.end();it++)
		if ((*it)->fileName().contains(QString::fromStdString(m_pAllPlugins.plugin(i).name())))
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size();j++) 
				if (m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->childname() == ChildName.toStdString()) {
					QString strRect = QString::fromStdString(m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->location());
					return STRTOLOCATION(strRect)
				} 

	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		if (m_pAllPlugins.plugin(i).name() == strParent.toStdString()) {
			plugins_childplugin* pChilds = m_pAllPlugins.mutable_plugin(i)->add_child();
			pChilds->set_childname(ChildName.toStdString());
			pChilds->set_title(targetWidget->windowTitle().toStdString());
			QString strLocation = LOCATIONTOSTR(targetWidget);
			pChilds->set_location(strLocation.toStdString());
		}
	return targetWidget->geometry();
}

bool MainFrame::CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type)
{
	MainWidget* mainWidget = static_cast<MainWidget*>(g_pSignal->ReturnUser(SystemUser::MAINWIDGET));
	if (m_mapAbstractWidget.contains(strPlugName) && m_mapAbstractWidget[strPlugName] == mainWidget->GetInstance()) {
		m_pMsgThread->pushinQueue(m_mapAbstractWidget[strPlugName], type);
		return true;
	}
	if (m_mapAbstractWidget.contains(strChildName) && m_mapAbstractWidget[strChildName] == mainWidget->GetInstance()) {
		m_pMsgThread->pushinQueue(m_mapAbstractWidget[strChildName], type);
		return true;
	}
	return false;
}

bool MainFrame::CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName)
{
	MainWidget* mainWidget = static_cast<MainWidget*>(g_pSignal->ReturnUser(SystemUser::MAINWIDGET));
	return m_mapAbstractWidget.contains(strPlugName) && m_mapAbstractWidget[strPlugName] == mainWidget->GetInstance()
		|| m_mapAbstractWidget.contains(strChildName) && m_mapAbstractWidget[strChildName] == mainWidget->GetInstance();
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
	MainWidget* currentWidget = static_cast<MainWidget*>(g_pSignal->ReturnUser(SystemUser::MAINWIDGET));
	QString strName = currentWidget->GetInstance()->metaObject()->className();
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

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
	connect(this, SIGNAL(InitWidget(const PluginInfo*)), this, SLOT(LinkCurrentWidgetInterface(const PluginInfo*)));
	g_pSignal->SetUserIdentify(this, SystemUser::MAINFRAME);
	ConfigPlug();
	//StartPluginControl();
}

MainFrame::~MainFrame() {
	m_logFile.close();
	UpdateConfigFile();
	if (m_pMsgThread) {
		delete m_pMsgThread;
		m_pMsgThread = NULL;
	}
	for (MainWidget* Tgt: m_mapMainWidget)
		if (Tgt) {
			delete Tgt;
			Tgt = NULL;
		}
	for (QLibrary* lib : m_LstLoadlib) {
		lib->unload();
		delete lib;
		lib = NULL;
	}
}

// \todo待完成，当有多个插件加载的时候释放目标错误
void MainFrame::ReleaseCurrentWidget(const QString& strPlugName) {
	if (m_mapMainWidget.contains(m_mapAbstractWidget[strPlugName])) {
		MainWidget* Tgt = m_mapMainWidget[m_mapAbstractWidget[strPlugName]];
		m_mapMainWidget.remove(m_mapAbstractWidget[strPlugName]);
		m_mapAbstractWidget.remove(strPlugName);
		delete Tgt;
		Tgt = NULL;
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
		pluginFile.close();
	}
	else 
		QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), pluginFile.errorString());
}

void MainFrame::FindPlugin() {
	std::for_each(m_PluginConfig.begin(), m_PluginConfig.end(),
		[this](const PluginInfo& info){
		if (info.m_isStart) {
			QLibrary* lib = new QLibrary(info.m_str_path + "/" + info.m_str_name);
			if (!lib->load()) {
				QString strError = lib->errorString();
				m_logFile.write(strError.toLocal8Bit());
				return;
			}
			else {
				this->m_LstLoadlib.append(lib);
				LinkCurrentWidgetInterface(&info);
			}
		}});
}

void MainFrame::FreeLib(AbstractWidget* pTgt) {
	for (QMap<QString, AbstractWidget*>::iterator it = m_mapAbstractWidget.begin();
		it != m_mapAbstractWidget.end();it++) 
		if ((*it) == pTgt)
			FreeLib(it.key());
}

void MainFrame::FreeLib(const QString& strPlugName)
{
	UpdataGeometry(strPlugName);
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
			m_logFile.write(strError.toLocal8Bit());
		}
		else { 
			m_LstLoadlib.append(lib);
			emit InitWidget(targetPlug);
		}
	}
}

AbstractWidget* MainFrame::LoadLib(const QString strTargetName, bool noShow)
{
	PluginInfo* targetPlug = std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [&](const PluginInfo& plug) {
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		QLibrary* lib = new QLibrary(targetPlug->m_str_path + "/" + strTargetName);
		if (!lib->load()) {
			QString strError = lib->errorString();
			m_logFile.write(strError.toLocal8Bit());
		}
		else {
			typedef AbstractWidget* (*pFunction)();
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

void MainFrame::Initialize_WidgetInterface(AbstractWidget* pTgtWidget, const QString& strParentName)
{
	m_mapAbstractWidget[pTgtWidget->metaObject()->className()] = pTgtWidget;
	const QRect rect = GetTargetLocation(pTgtWidget, pTgtWidget->metaObject()->className(), strParentName);
	MainWidget* pmainWidget = new MainWidget;
	m_mapMainWidget[pTgtWidget] = pmainWidget;
	pmainWidget->setMain(pTgtWidget, rect, pTgtWidget->windowTitle());
}

SignalQueue* MainFrame::GetTgtSigQueueInstance(AbstractWidget* pTgtChild)
{
	return m_mapMainWidget[pTgtChild]->m_pSigQueue;
}

void MainFrame::LinkCurrentWidgetInterface(const PluginInfo* targetPlug) {
	typedef AbstractWidget* (*pFunction)();     
	pFunction pfun = (pFunction)(m_LstLoadlib.last()->resolve("Handle"));
	if (pfun) 
		Initialize_WidgetInterface(pfun(), targetPlug->m_str_name);
	else
		qDebug() << QString::fromLocal8Bit("空的");
}

const QRect MainFrame::GetTargetLocation(const AbstractWidget* targetWidget, const QString& ChildName, const QString& strParent) {
	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size(); i++) 
		for (QList<QLibrary*>::iterator it = m_LstLoadlib.begin(); it != m_LstLoadlib.end(); it++)
			if ((*it)->fileName().contains(QString::fromStdString(m_pAllPlugins.plugin(i).name())))
				for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) 
					if (m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->childname() == ChildName.toStdString()) {
						QString strRect = QString::fromStdString(m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->location());
						return STRTOLOCATION(strRect)
					}
	for (int i = 0; i < m_pAllPlugins.plugin_size(); i++) 
		if (m_pAllPlugins.plugin(i).name() == strParent.toStdString()) {
			plugins_childplugin* pChilds = m_pAllPlugins.mutable_plugin(i)->add_child();
			pChilds->set_childname(ChildName.toStdString());
			pChilds->set_title(targetWidget->windowTitle().toStdString());
			QString strLocation = LOCATIONTOSTR(targetWidget);
			pChilds->set_location(strLocation.toStdString());
		}
	return targetWidget->geometry();
}

bool MainFrame::SendMsgThread(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type)
{
	bool isRuning = CheckPlugIsRuning(strPlugName, strChildName);
	if (isRuning)
		m_pMsgThread->pushinQueue(m_mapAbstractWidget[strChildName], type);
	return isRuning;
}

bool MainFrame::CheckPlugIsRuning(const QString& strPlugName, const QString& strChildName)
{
	for (int i = 0; i < m_pAllPlugins.plugin_size(); i++) 
		if (m_pAllPlugins.mutable_plugin(i)->name() == strPlugName.toStdString()) 
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size();j++) 
				if (m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->childname() == strChildName.toStdString())
					if (m_mapAbstractWidget.contains(strChildName))
						for (QMap<AbstractWidget*, MainWidget*>::iterator it = m_mapMainWidget.begin();it != m_mapMainWidget.end(); it++)
							if (it.key() == m_mapAbstractWidget[strChildName])
								return true;
	return false;
}

void MainFrame::UpdataGeometry(AbstractWidget* Tgt) {
	QString strName = Tgt->metaObject()->className();
	qDebug() << "strName::" << m_mapMainWidget.size() << "   " << m_mapMainWidget << "   " << Tgt;
	for (int i = 0; i < m_pAllPlugins.mutable_plugin()->size();i++) {
		for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) {
			plugins_childplugin* plug = m_pAllPlugins.mutable_plugin(i)->mutable_child(j);
			if (QString::fromStdString(plug->childname()) == strName) {
				QString strLocation = QString("%1,%2,%3,%4").arg(m_mapMainWidget[Tgt]->geometry().x()).arg(m_mapMainWidget[Tgt]->y())
					.arg(m_mapMainWidget[Tgt]->width()).arg(m_mapMainWidget[Tgt]->height());
				plug->set_location(strLocation.toStdString());
			}
		}
	}
}
	

void MainFrame::UpdataGeometry(const QString& strPlugName)
{
	for (int i = 0; i < m_pAllPlugins.plugin_size(); i++)
		if (m_pAllPlugins.mutable_plugin(i)->name() == strPlugName.toStdString())
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++)
				if (m_mapAbstractWidget.contains(QString::fromStdString(m_pAllPlugins.mutable_plugin(i)->mutable_child(j)->childname())))
					UpdataGeometry(m_mapAbstractWidget[strPlugName]);
}

int MainFrame::RemoveWidget(MainWidget* mainWidget)
{
	m_mapMainWidget.remove(mainWidget->GetInstance());
	return std::count_if(m_mapMainWidget.begin(), m_mapMainWidget.end(), [](const MainWidget* mainWidget) {
		return !mainWidget->isHidden();
	});
}

const QString MainFrame::GetParentName(const AbstractWidget* ChildWidget)
{
	auto it = std::find_if(m_mapAbstractWidget.begin(), m_mapAbstractWidget.end(), [ChildWidget](const AbstractWidget* AbsWidget) {
		return AbsWidget == ChildWidget;
	});
	if (it != m_mapAbstractWidget.end())
	{
		for (int i = 0; i < m_pAllPlugins.plugin_size(); i++) {
			if (m_pAllPlugins.mutable_plugin(i)->name() == it.key().toStdString())
				return it.key();
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) {
				plugins_childplugin* plug = m_pAllPlugins.mutable_plugin(i)->mutable_child(j);
				if (QString::fromStdString(plug->childname()) == it.key()) {
					return it.key();
				}
			}
		}
	}
	return "";
}

void MainFrame::ConfigPlug() {
	QDomNode node = ReadXML();
	QString strname = node.nodeName();
	while (!node.isNull()) {
		if (node.hasChildNodes()) {
			QDomNode ChildNode = node.firstChild();
			PluginInfo plug;
			while (!ChildNode.isNull()) {
				plug << ChildNode.toElement().text();
				ChildNode = ChildNode.nextSibling();
			}
			m_PluginConfig.push_front(plug);
		}
		node = node.nextSibling();
	}
	QFile configfile(QString(CONFIG) + "plugin");
	bool isOpend = configfile.open(QIODevice::ReadOnly);
	Allplugins configPlug;
	if (isOpend) {
		std::string strContent = configfile.readAll().toStdString();
		isOpend = configPlug.ParseFromString(strContent);
	}
	for (QVector<PluginInfo>::iterator it = m_PluginConfig.begin();
		it != m_PluginConfig.end(); it++) {
		plugins* plug = m_pAllPlugins.add_plugin();
		plug->set_isstart(it->m_isStart);
		plug->set_name(it->m_str_name.toStdString());
		plug->set_path(it->m_str_path.toStdString());
		for (int i = 0; isOpend && i < configPlug.plugin_size(); i++)
			if (configPlug.mutable_plugin(i)->name() == plug->name())
				for (int j = 0; j < configPlug.mutable_plugin(i)->child_size(); j++) {
					plugins_childplugin* child = plug->add_child();
					child->set_childname(configPlug.mutable_plugin(i)->mutable_child(j)->childname());
					child->set_location(configPlug.mutable_plugin(i)->mutable_child(j)->location());
					child->set_title(configPlug.mutable_plugin(i)->mutable_child(j)->title());
				}
	}
	if (configfile.isOpen())
		configfile.close();
	qDebug() << QString::fromStdString(m_pAllPlugins.DebugString());
	FindPlugin();
}

QDomNode MainFrame::ReadXML() {
	QDomNode nodeFirst;
	static QDomDocument DomRoot;
	QFile PluginCon(QString(CONFIG) + "plugin.xml");
	if (PluginCon.open(QIODevice::ReadOnly)) {
		QByteArray PlugContent = PluginCon.readAll();
		DomRoot.setContent(PlugContent);
		PluginCon.close();
		QDomNode node = DomRoot.documentElement();
		node = node.firstChild();
		while (node.nodeName() != "pluginconfig")
			node = node.nextSibling();
		node = node.firstChild();
		nodeFirst = node;
	}
	return nodeFirst;
}
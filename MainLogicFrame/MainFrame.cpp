 #include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "MessageThread.h"

MainFrame::MainFrame() : QObject(), m_pMsgThread(NULL), m_pTargetQueue(NULL) {
	m_pMsgThread = new MessageThread(this);
	m_pMsgThread->start();
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	connect(this, SIGNAL(ReleaseWidget(const QString&, bool)), this, SLOT(ReleaseCurrentWidget(const QString&, bool)), Qt::DirectConnection);
	connect(this, SIGNAL(InitWidget(const PluginInfo*)), this, SLOT(LinkCurrentWidgetInterface(const PluginInfo*)));
	g_pSignal->SetUserIdentify(this, SystemUser::MAINFRAME);
	ConfigPlug();
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

// 删除释放目标
void MainFrame::ReleaseCurrentWidget(const QString& strPlugName, bool isParent) {
	if (m_mapAbstractWidget.contains(strPlugName) && m_mapMainWidget.contains(m_mapAbstractWidget[strPlugName])) {
		MainWidget* Tgt = m_mapMainWidget[m_mapAbstractWidget[strPlugName]];
		for (QMap<QString, AbstractNetWork*>::iterator it = m_mapAbstractNet.begin(); it != m_mapAbstractNet.end(); it++) 
			if (it.key() == strPlugName) {
				m_mapAbstractNet.erase(it);
				break;
			}
		m_mapMainWidget.remove(m_mapAbstractWidget[strPlugName]);
		m_mapAbstractWidget.remove(strPlugName);
		delete Tgt;
		Tgt = NULL;
	}
	for (QList<QLibrary*>::iterator it = m_LstLoadlib.begin();
		isParent && it != m_LstLoadlib.end();it++) {
		if ((*it)->fileName().contains(strPlugName)) {
			(*it)->unload();
			m_LstLoadlib.erase(it);
			return;
		}
	}
}


void MainFrame::UpdateConfigFile() {
	QFile pluginFile("../Data/Config/plugin");
	if (pluginFile.open(QIODevice::WriteOnly)) {
		pluginFile.write(m_pAllPlugins.SerializeAsString().c_str());
		pluginFile.close();
	}
	else 
		WriteLog(LogGrade::Error, pluginFile.errorString());
}

void MainFrame::FindPlugin() {
	std::for_each(m_PluginConfig.begin(), m_PluginConfig.end(),
		[this](const PluginInfo& info){
		if (info.m_isStart) {
			QLibrary* lib = new QLibrary(info.m_str_path + "/" + info.m_str_name);
			if (!lib->load()) {
				QString strError = lib->errorString();
				WriteLog(LogGrade::SeriousError, strError);
				return;
			}
			else {
				this->m_LstLoadlib.append(lib);
				LinkCurrentWidgetInterface(&info);
			}
		}});
}

void MainFrame::LoadLib(const QString strTargetName) {
	PluginInfo* targetPlug =  std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [&](const PluginInfo& plug){
		return plug.m_str_name == strTargetName;
	});
	if (targetPlug) {
		for (QList<QLibrary*>::iterator it = m_LstLoadlib.begin();it != m_LstLoadlib.end();it++) 
			if ((*it)->fileName().contains(targetPlug->m_str_name)) 
				return;
		QLibrary* lib = new QLibrary(targetPlug->m_str_path + "/" + strTargetName);
		if (!lib->load()) {
			QString strError = lib->errorString();
			WriteLog(LogGrade::SeriousError, strError);
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
			WriteLog(LogGrade::SeriousError, strError);
		}
		else {
			AbstractWidget* (*pFunction)() = (AbstractWidget * (*)())(lib->resolve("Handle"));
			if (pFunction) {
				AbstractWidget* pAbsWidget = pFunction();
				pAbsWidget->m_pInstanceWidget = pAbsWidget;
				return pAbsWidget;
			}
		}
	}
	return nullptr;
}

void MainFrame::Initialize_NetInterface(AbstractNetWork* net, const QString& strChildName)
{
	if (strChildName != QString::fromLocal8Bit("未知")) 
		m_mapAbstractNet[strChildName] = net;
}

void MainFrame::Initialize_WidgetInterface(AbstractWidget* pTgtChildWidget, const QString& strParentName)
{
	m_mapAbstractWidget[pTgtChildWidget->metaObject()->className()] = pTgtChildWidget;
	const QRect rect = GetNewTargetLocation(pTgtChildWidget, pTgtChildWidget->metaObject()->className(), strParentName);
	MainWidget* pmainWidget = new MainWidget;
	m_mapMainWidget[pTgtChildWidget] = pmainWidget;
	pmainWidget->setMain(pTgtChildWidget, rect, pTgtChildWidget->windowTitle());
}

SignalQueue* MainFrame::GetTgtSigQueueInstance(const AbstractWidget* pTgtChild)
{
	AbstractWidget* widget = const_cast<AbstractWidget*>(pTgtChild);
	if (!m_mapMainWidget.contains(widget)) {
		MainWidget* pmainWidget = new MainWidget;
		m_mapMainWidget[widget] = pmainWidget;
	}
	return m_mapMainWidget[widget]->m_pSigQueue;
}

void MainFrame::LinkCurrentWidgetInterface(const PluginInfo* targetPlug) {
	typedef AbstractWidget* (*pFunction)();     
	pFunction pfun = (pFunction)(m_LstLoadlib.last()->resolve("Handle"));
	if (pfun) {
		AbstractWidget* widget = pfun();
		if (m_pTargetQueue) {
			emit m_pTargetQueue->hide_Window();
			emit ReleaseWidget(GetMyselfName(m_pTargetQueue->m_ParamInfo.m_pOldWidget), true); 
			m_pTargetQueue = NULL;
		}
		widget->m_pInstanceWidget = widget;
		Initialize_WidgetInterface(widget, targetPlug->m_str_name);
	}
	else
		WriteLog(LogGrade::Warning, m_LstLoadlib.last()->errorString());
}

const QRect MainFrame::GetNewTargetLocation(const AbstractWidget* targetWidget, const QString& ChildName, const QString& strParent) {
	qDebug() << "mingcheng qwi " << ChildName << strParent;
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

int MainFrame::GetShowWidgetCount(MainWidget* mainWidget)
{
	return std::count_if(m_mapMainWidget.begin(), m_mapMainWidget.end(), [](const MainWidget* mainWidget) {
		return !mainWidget->isHidden();
	});
}


bool MainFrame::isParent(const QString& strTarget)
{
	return std::find_if(m_PluginConfig.begin(), m_PluginConfig.end(), [strTarget](const PluginInfo& plugin) {
		return plugin.m_str_name == strTarget;
	});
}

const QString MainFrame::GetParentName(const AbstractWidget* ChildWidget)
{
	auto it = std::find_if(m_mapAbstractWidget.begin(), m_mapAbstractWidget.end(), [ChildWidget](const AbstractWidget* AbsWidget) {
		return AbsWidget == ChildWidget->m_pInstanceWidget;
	});
	if (it != m_mapAbstractWidget.end()) 
		for (int i = 0; i < m_pAllPlugins.plugin_size(); i++) {
			if (m_pAllPlugins.mutable_plugin(i)->name() == it.key().toStdString())
				return m_pAllPlugins.mutable_plugin(i)->name().c_str();
			for (int j = 0; j < m_pAllPlugins.mutable_plugin(i)->child_size(); j++) {
				plugins_childplugin* plug = m_pAllPlugins.mutable_plugin(i)->mutable_child(j);
				if (QString::fromStdString(plug->childname()) == it.key())
					return m_pAllPlugins.mutable_plugin(i)->name().c_str();
			}
		}
	return QString::fromLocal8Bit("未知");
}

const QString MainFrame::GetMyselfName(const AbstractWidget* AbsWidget)
{
	auto it = std::find_if(m_mapAbstractWidget.begin(), m_mapAbstractWidget.end(), [AbsWidget](const AbstractWidget* widget) {
		return widget == AbsWidget;
	});
	return it != m_mapAbstractWidget.end() ? it.key() : QString::fromLocal8Bit("未知");
}

void MainFrame::WriteLog(LogGrade Grade, const QString& strLog)
{
	QString strCurrentDate = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss");
	QString strGrade;
	switch (Grade)
	{
	case LogGrade::Error:
		strGrade = QString::fromLocal8Bit("错误");
		break;
	case  LogGrade::Tips:
		strGrade = QString::fromLocal8Bit("提示");
		break;
	case LogGrade::SeriousError:
		strGrade = QString::fromLocal8Bit("严重错误");
		break;
	case LogGrade::Warning:
		strGrade = QString::fromLocal8Bit("警告");
		break;
	default:
		strGrade = QString::fromLocal8Bit("未知等级");
		break;
	}
	QString strWriteText = QString::fromLocal8Bit("%1 等级：%2 日志：%3").arg(strCurrentDate).arg(strGrade).arg(strLog);
	strWriteText += "\n";
	m_logFile.write(strWriteText.toLocal8Bit());
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
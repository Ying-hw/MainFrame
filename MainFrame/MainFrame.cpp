#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"

MainFrame::MainFrame() : QThread(), m_isRunning(true), m_data(nullptr)
{
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	m_logFile.open(QIODevice::Append);
	ReadPluginConfig();
	connect(this, SIGNAL(ReleaseWidget()), this, SLOT(ReleaseCurrentWidget()));
	connect(this, SIGNAL(InitWidget(bool)), this, SLOT(InitCurrentWidget(bool)));
	g_pSignal->SetUserIdentify(this, User::MAINFRAME);
	this->start();
}

MainFrame::~MainFrame(){
	m_logFile.close();
	m_isRunning = false;
	m_Loadlib.unload();
}

void MainFrame::ReleaseCurrentWidget() {
	MainWidget::staticThis->m_pWidget->deleteLater();
	delete MainWidget::staticThis->m_pWidget;
	MainWidget::staticThis->m_pWidget = NULL;
	m_Loadlib.unload();
}

void MainFrame::ReadPluginConfig() {
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
			m_Plugin.push_front(plug);
		}
		node = node.nextSibling();
	}
	LoadPlugin();
}

void MainFrame::LoadPlugin() {
	std::for_each(m_Plugin.begin(), m_Plugin.end(),
		[this](const PluginInfo& info){
		if (info.m_isStart) {
			this->m_Loadlib.setFileName(info.m_str_name);
			if (!this->m_Loadlib.load()) {
				QString str = this->m_Loadlib.errorString();
				qDebug() << str;
				WriteLog(str);
				return;
			}
			InitCurrentWidget(true);
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
	QDomNode node = pthis->ReadXML();
	while (!node.isNull()) {
		if (node.hasChildNodes()) {
			if (!node.childNodes().at(3).nodeValue().toInt()){
				QDomNode ChildNode = node.firstChild();
				while (!ChildNode.isNull()) {
					QString strTarget = ChildNode.toElement().tagName();
					if (strTarget == "invalid")
						ChildNode.setNodeValue(QString::number(1));
					if (strTarget == "rect")
						ChildNode.setNodeValue(strRect);
					ChildNode = ChildNode.nextSibling();
				}
			}
		}
		node = node.nextSibling();
	}
	QString strPlugin = QString(CONFIG) + "plugin.xml";
	FILE* Pfile = fopen(strPlugin.toStdString().c_str(), "r+");
	if (Pfile) {
		QTextStream text(Pfile, QIODevice::WriteOnly);
		text.setCodec("utf-8");
		text << node.toDocument();
		fclose(Pfile);
	}
	emit pthis->ReleaseWidget();
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

void MainFrame::LoadLib(MainFrame* pthis, const QString strTargetName) {
	
	pthis->m_Loadlib.setFileName("./" + strTargetName);
	if (!pthis->m_Loadlib.load()) {
		QString str = pthis->m_Loadlib.errorString();
		pthis->WriteLog(str);
	}
	else {
		std::for_each(pthis->m_Plugin.begin(), pthis->m_Plugin.end(), [&](const PluginInfo& info){
			if (info.m_str_name == strTargetName) 
				g_Rects = info.m_rPosition;
		});
		emit pthis->InitWidget(false);
	}
}

void MainFrame::InitCurrentWidget(bool isProgramStart) {
	typedef QWidget* (*pFunction)(void);
	pFunction pfun = (pFunction)(m_Loadlib.resolve("Handle"));
	if (pfun)
		if (isProgramStart)
			MainWidget::staticThis->setMain(pfun());
		else
			MainWidget::staticThis->setMain(pfun(), g_Rects);
	else
		qDebug() << QString::fromLocal8Bit("空的");
}
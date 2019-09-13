#include "stdafx.h"
#include "MainFrame.h"

MainFrame::MainFrame() : QThread(), m_isRunning(true), m_data(nullptr)
{
	QString strDate = QDate::currentDate().toString(Qt::ISODate);
	m_logFile.setFileName(QString(LOG) + strDate + ".txt");
	if (m_logFile.open(QIODevice::Append)) {
		QString strTips = QString::fromLocal8Bit("欢迎回来\r\n");
		WriteLog(strTips);
		m_logFile.close();
	}
	ReadPluginConfig();
	SignalQueue* singalThread = new SignalQueue;
	singalThread->start();
	this->start();
}

MainFrame::~MainFrame(){
	m_logFile.close();
	m_isRunning = false;
}

void MainFrame::ReadPluginConfig() {
	QFile PluginCon(QString(CONFIG) + "plugin.xml");
	if (PluginCon.open(QIODevice::ReadOnly)) {
		QByteArray PlugContent = PluginCon.readAll();
		QDomDocument DomRoot;
		DomRoot.setContent(PlugContent);
		PluginCon.close();
		QDomNode node = DomRoot.documentElement();
		node = node.firstChild();
		while (node.nodeName() != "pluginconfig") 
				node = node.nextSibling();
		node = node.firstChild();
		while (!node.isNull()) {
			if (node.hasChildNodes()) {
				QDomNode ChildNode = node.firstChild();
				PluginInfo plug;
				while (!ChildNode.isNull()) {
					plug << ChildNode.toElement().text();
 					ChildNode = ChildNode.nextSibling();
				}
				m_vecPluginInfo.append(plug);
			}
			node = node.nextSibling();
		}
	}
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


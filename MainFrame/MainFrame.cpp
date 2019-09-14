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
	else
	{
		QString errstr = m_logFile.errorString();
	}
	ReadPluginConfig();
	g_pSignal->SetUserIdentify(this, User::MAINFRAME);
	this->start();
}

MainFrame::~MainFrame(){
	m_logFile.close();
	m_isRunning = false;
	m_Loadlib.unload();
}

void MainFrame::ReadPluginConfig() {
	QVector<PluginInfo> lstPlugin;
	QDomNode node = ReadXML();
	while (!node.isNull()) {
		if (node.hasChildNodes()) {
			QDomNode ChildNode = node.firstChild();
			PluginInfo plug;
			while (!ChildNode.isNull()) {
				plug << ChildNode.toElement().text();
				ChildNode = ChildNode.nextSibling();
			}
			lstPlugin.push_front(plug);
		}
		node = node.nextSibling();
	}
	LoadPlugin(lstPlugin);
}

void MainFrame::LoadPlugin(const QVector<PluginInfo>& vecPluginInfo) {
	std::for_each(vecPluginInfo.begin(), vecPluginInfo.end(),
		[this](const PluginInfo& info){
		if (!info.m_bIsIvalid) {
			this->m_Loadlib.setFileName(info.m_str_name);
			if (!this->m_Loadlib.load())
			{
				QString str = this->m_Loadlib.errorString();
				qDebug() << str;
			}
			QFunctionPointer pFun = this->m_Loadlib.resolve("Handle");
			if (pFun)
				pFun();
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

void MainFrame::FreeLib(MainFrame* pthis, QString strRect) {
	pthis->m_Loadlib.unload();
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
	FILE* Pfile = fopen(strcat(CONFIG, "plugin.xml"), "r+");
	if (Pfile) {
		QTextStream text(Pfile, QIODevice::WriteOnly);
		text.setCodec("utf-8");
		text << node.toDocument();
		fclose(Pfile);
	}
}

QDomNode MainFrame::ReadXML() {
	QDomNode nodeFirst;
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
		nodeFirst = node;
	}
	return nodeFirst;
}

void MainFrame::LoadLib(MainFrame* pthis) {

}

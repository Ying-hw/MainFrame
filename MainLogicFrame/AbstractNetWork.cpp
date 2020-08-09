#include "stdafx.h"
#include "AbstractNetWork.h"
#include "MacroDefine.h"
#include "HintFrameWidget.h"

AbstractNetWork::AbstractNetWork(ProtoType Type, QHostAddress addrs, int port, QObject* parent) :
	QObject(parent), m_prototype(Type), m_pReply(nullptr), isNeytErrorWidgetShow(false), m_Tcp(NULL) {
	m_addrInfo.m_addr = addrs;
	m_addrInfo.m_port = port;
}

AbstractNetWork::~AbstractNetWork(){
	
}


int AbstractNetWork::SendMsg(const QString& strContent)
{
	int size = 0;
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		size = m_Tcp->write(strContent.toUtf8());
		break;
	case ProtoType::UDP:
		size = m_Udp.write(strContent.toUtf8());
		break;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
	if (size == -1)
		isNeytErrorWidgetShow = false;
	return size;
}

void AbstractNetWork::initCommunication()
{
	timer.setInterval(3000);
	connect(&timer, SIGNAL(timeout()), this, SLOT(SelectNetabOnline()));
	timer.start();
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		m_Tcp = new QTcpSocket();
		connect(m_Tcp, SIGNAL(connected()), this, SLOT(connected()));
		connect(m_Tcp, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
		m_Tcp->connectToHost(m_addrInfo.m_addr, m_addrInfo.m_port);
		break;
	case ProtoType::UDP:
		connect(&m_Udp, SIGNAL(connected()), this, SLOT(connected()));
		connect(&m_Udp, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
		m_Udp.connectToHost(m_addrInfo.m_addr, m_addrInfo.m_port);
		break;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
}

void* AbstractNetWork::ReturnCurrentTargetSocket()
{
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		return (void*)&m_Tcp;
	case ProtoType::UDP:
		return (void*)&m_Udp;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
	return NULL;
}

void AbstractNetWork::SetAddrInfo(QHostAddress host, int port)
{
	m_addrInfo.m_addr = host;
	m_addrInfo.m_port = port;
}

void AbstractNetWork::ReleaseCommuncation()
{
	timer.stop();
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		if (m_Tcp->isOpen())
			m_Tcp->close();
		m_Tcp->deleteLater();
		delete m_Tcp;
		m_Tcp = NULL;
		break;
	case ProtoType::UDP:
		m_Udp.close();
		break;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
	deleteLater();
	qDebug() << "111111111111111111111111111111111";
}

void AbstractNetWork::ProcessError()
{

}

void AbstractNetWork::connected()
{
	QString strHintError = QString::fromLocal8Bit("连接成功！！！");
	qDebug() << strHintError;
}

void AbstractNetWork::SetCommunicationProtocol(ProtoType type)
{
	m_prototype = type;
}

void AbstractNetWork::SelectNetabOnline()
{
	QHostInfo::lookupHost("www.baidu.com", this, SLOT(processSelectResult(QHostInfo)));
}

void AbstractNetWork::processSelectResult(QHostInfo host)
{
	if (host.error() != QHostInfo::NoError) {
		if (!isNeytErrorWidgetShow) {
			QDesktopWidget destop;
			HintFrameWidget* hint = new HintFrameWidget(host.errorString(), destop.rect().center(), this);
			hint->show();
			ProcessError();
		}
		isNeytErrorWidgetShow = true;
	}
	else
		isNeytErrorWidgetShow = false;
}
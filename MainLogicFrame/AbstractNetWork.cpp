#include "stdafx.h"
#include "AbstractNetWork.h"
#include "MacroDefine.h"
#include "HintFrameWidget.h"

AbstractNetWork::AbstractNetWork(ProtoType Type, QString addrs, int port, QObject* parent) :
	QThread(parent), m_prototype(Type), m_pReply(nullptr), m_Tcp(NULL), m_size(0) {
	m_addrInfo.m_strAddr = addrs;
	m_addrInfo.m_port = port;
	DnsDispose* dns = new DnsDispose(addrs, this);
	connect(dns, SIGNAL(NetError(const QString &)), this, SLOT(ProcessError(const QString&)));
	dns->ExcuteInitNetFunction(this);
	m_mutex.lock();
}

AbstractNetWork::~AbstractNetWork(){

}


int AbstractNetWork::SendMsg()
{
	int size = 0;
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		if (!m_Tcp)
			m_waitMutex.wait(&m_mutex);
		size = m_Tcp->write(m_strContent.c_str());
		break;
	case ProtoType::UDP:
		size = m_Udp.writeDatagram(m_strContent.c_str(), QHostAddress(m_addrInfo.m_strAddr), m_addrInfo.m_port);
		break;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
	return size;
}

void AbstractNetWork::initCommunication(QHostAddress strAddr)
{
	if (!m_Tcp)
	{
		switch (m_prototype)
		{
		case ProtoType::FTP:
			break;
		case ProtoType::HTTP:
			break;
		case ProtoType::TCP:
		{
			m_Tcp = new QTcpSocket();
			connect(m_Tcp, SIGNAL(connected()), this, SLOT(connected()));
			connect(m_Tcp, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
			m_Tcp->connectToHost(strAddr, m_addrInfo.m_port);
		}
			break;
		case ProtoType::UDP:
			connect(&m_Udp, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
			break;
		case ProtoType::SMTP:
			break;
		default:
			break;
		}
		m_waitMutex.wakeOne();
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
		return (void*)m_Tcp;
	case ProtoType::UDP:
		return (void*)&m_Udp;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
	return NULL;
}

void AbstractNetWork::ReleaseCommuncation()
{
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

void AbstractNetWork::run()
{
	SendMsg();
}

int AbstractNetWork::Send(std::string& strContent)
{
	
	m_strContent = strContent.c_str();
	if (m_Tcp)
		return SendMsg();
	else 
		start();
	return 0;
}

void AbstractNetWork::ProcessError(const QString& strError)
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




DnsDispose::DnsDispose(QString addrs, QObject* parent /*= 0*/) : QThread(parent), m_strAddr(addrs), m_isNetErrorShow(false), m_NetWork(NULL)
{
	
	m_Timer.setInterval(3000);
	connect(&m_Timer, &QTimer::timeout, this, [this]() {
		start();
	});
	m_Timer.start();
	start();
}

DnsDispose::~DnsDispose()
{
	wait();
	m_Timer.stop();
}

void DnsDispose::run()
{
	if (!m_strAddr.isEmpty())
		QHostInfo::lookupHost(m_strAddr, this, SLOT(processDnsResult(QHostInfo)));
}

void DnsDispose::ExcuteInitNetFunction(AbstractNetWork* that)
{
	m_NetWork = that;
}

void DnsDispose::StopTime()
{
	m_Timer.stop();
	wait();
}

void DnsDispose::processDnsResult(QHostInfo host)
{
	if (host.error() != QHostInfo::NoError) {
		if (!m_isNetErrorShow) {
			QDesktopWidget destop;
			HintFrameWidget* hint = new HintFrameWidget(host.errorString(), destop.rect().center());
			hint->show();
			emit NetError(host.errorString());
		}
		m_isNetErrorShow = true;
	}
	else {
		m_isNetErrorShow = false;
		m_addr = host.addresses().first();
		if (m_NetWork)
			m_NetWork->initCommunication(m_addr);
	}
}


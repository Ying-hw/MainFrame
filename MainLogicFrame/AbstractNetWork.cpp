#include "stdafx.h"
#include "AbstractNetWork.h"
#include "MacroDefine.h"
#include "HintFrameWidget.h"
#include <QHostInfo>

AbstractNetWork::AbstractNetWork(ProtoType Type, QHostAddress addrs, int port, QObject* parent) :
	QObject(parent), m_prototype(Type), m_pReply(nullptr), isNeytErrorWidgetShow(false) {
	m_addrInfo.m_addr = addrs;
	m_addrInfo.m_port = port;
}

AbstractNetWork::~AbstractNetWork(){
	switch (m_prototype)
	{
	case ProtoType::FTP:
		break;
	case ProtoType::HTTP:
		break;
	case ProtoType::TCP:
		m_Tcp.close();
		break;
	case ProtoType::UDP:
		m_Udp.close();
		break;
	case ProtoType::SMTP:
		break;
	default:
		break;
	}
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
		size = m_Tcp.write(strContent.toUtf8());
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
		connect(&m_Tcp, SIGNAL(connected()), this, SLOT(connected()));
		connect(&m_Tcp, SIGNAL(readyRead()), this, SLOT(RecvMessage()));
		m_Tcp.connectToHost(m_addrInfo.m_addr, m_addrInfo.m_port);
		break;
	case ProtoType::UDP:
		connect(&m_Udp, SIGNAL(connected()), this, SLOT(connected()));
		connect(&m_Udp, SIGNAL(readyRead()), this, SLOT(RecvMessage()));
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
		break;
	case ProtoType::UDP:
		return (void*)&m_Udp;
		break;
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

void AbstractNetWork::ProcessError()
{

}

void AbstractNetWork::connected()
{
	QString strHintError = QString::fromLocal8Bit("连接成功！！！");
	qDebug() << strHintError;
}

void AbstractNetWork::SelectNetabOnline()
{
	QHostInfo::lookupHost("www.baidu.com", this, SLOT(processSelectResult(QHostInfo)));
}

void AbstractNetWork::processSelectResult(QHostInfo host)
{
	if (host.error() != QHostInfo::NoError) {
		if (!isNeytErrorWidgetShow) {
			HintFrameWidget* hint = new HintFrameWidget(host.errorString(), this);
			hint->show();
			ProcessError();
		}
		isNeytErrorWidgetShow = true;
	}
	else
		isNeytErrorWidgetShow = false;
}
#include "stdafx.h"
#include "NetWork.h"
#include "MacroDefine.h"
#include <QHostInfo>

NetWork::NetWork(QAbstractSocket::SocketType Type) : m_Type(Type){}

NetWork::~NetWork(){}

void NetWork::InitIpPort(const QString& strIp, quint32 port)
{
	qRegisterMetaType<QAbstractSocket::SocketError>();
	if (m_Type == QAbstractSocket::TcpSocket) {
		m_Tcp.connectToHost(QHostAddress(strIp), port);
		connect(&m_Tcp, SIGNAL(disconnected()), this, SLOT(disconnect()));
		connect(&m_Tcp, SIGNAL(connected()), this, SLOT(connected()));
		connect(&m_Tcp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(NetworkHaveError(QAbstractSocket::SocketError)));
		connect(&m_Tcp, SIGNAL(readyRead()), this, SLOT(RecvMessage()));
	}
	else if (m_Type == QAbstractSocket::UdpSocket) {
		m_Udp.connectToHost(QHostAddress(strIp), port);
		connect(&m_Udp, SIGNAL(disconnected()), this, SLOT(disconnect()));
		connect(&m_Udp, SIGNAL(connected()), this, SLOT(connected()));
		connect(&m_Udp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(NetworkHaveError(QAbstractSocket::SocketError)));
		connect(&m_Udp, SIGNAL(readyRead()), this, SLOT(RecvMessage()));
	}
}

void NetWork::disconnect()
{
	QString strHintError = QString::fromLocal8Bit("连接失败");
	qDebug() << strHintError;
}

void NetWork::connected()
{
	QString strHintError = QString::fromLocal8Bit("连接成功！！！");
	qDebug() << strHintError;
}

void NetWork::NetworkHaveError(QAbstractSocket::SocketError)
{
	QString strHintError = QString::fromLocal8Bit("网络错误, 请检查网络连接");
	qDebug() << strHintError;
	//SEND_SIGNAL(Signal_::CUSTOMTIPSFRAME, strHintError);
}

void NetWork::RecvMessage()
{
	if (m_Type == QAbstractSocket::TcpSocket) 
		if (m_Tcp.bytesAvailable()) {
			m_MessageArray.resize(m_Tcp.bytesAvailable());
			m_Tcp.read(m_MessageArray.data(), m_MessageArray.size());	
		}
	else if (m_Type == QAbstractSocket::UdpSocket) 
		if (m_Udp.bytesAvailable()) {
			m_MessageArray.resize(m_Udp.bytesAvailable());
			m_Udp.read(m_MessageArray.data(), m_MessageArray.size());
		}
}

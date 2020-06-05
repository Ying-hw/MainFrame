#include "stdafx.h"
#include "NetWorkWidget.h"

NetWorkWidget::NetWorkWidget(QWidget* parent /*= 0*/) : QWidget(parent)
{
	QUrl url;
	url.setScheme("ftp");
	url.setHost("ftp://192.168.1.48");
	QNetworkRequest Request(url);
	m_pReply = m_AccessMan.get(Request);
}

NetWorkWidget::~NetWorkWidget() {}

void NetWorkWidget::InitIpPort(const QString& strIp, quint32 port)
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

void NetWorkWidget::disconnect()
{
	QString strHintError = QString::fromLocal8Bit("连接失败");
	qDebug() << strHintError;
}

void NetWorkWidget::connected()
{
	QString strHintError = QString::fromLocal8Bit("连接成功！！！");
	qDebug() << strHintError;
}

void NetWorkWidget::NetworkHaveError(QAbstractSocket::SocketError)
{
	QString strHintError = QString::fromLocal8Bit("网络错误, 请检查网络连接");
	qDebug() << strHintError;
	//SEND_SIGNAL(Signal_::CUSTOMTIPSFRAME, strHintError);
}

void NetWorkWidget::RecvMessage()
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

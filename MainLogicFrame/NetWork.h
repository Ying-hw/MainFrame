#ifndef NETWORK_H
#define NETWORK_H

#include "MainFrame_global.h"
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MAINFRAME_EXPORT NetWork : public QObject
{
public:
	NetWork(QAbstractSocket::SocketType Type, QObject* parent = 0);
	~NetWork();

protected:
	void InitIpPort(const QString& strIp, quint32 port);
	QByteArray m_MessageArray;
protected slots:
	virtual void disconnect();
	virtual void connected();
	virtual	void NetworkHaveError(QAbstractSocket::SocketError);
	virtual void RecvMessage();
private:
	QTcpSocket m_Tcp;
	QUdpSocket m_Udp;
	QAbstractSocket::SocketType m_Type;	
	QNetworkAccessManager m_AccessMan;
	QNetworkReply* m_pReply;
};

#endif // NETWORK_H

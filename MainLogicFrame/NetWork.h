#ifndef NETWORK_H
#define NETWORK_H

#include "MainFrame_global.h"
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

class MAINFRAME_EXPORT NetWork : public QObject
{
	Q_OBJECT
public:
	NetWork(QAbstractSocket::SocketType Type);
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
};

#endif // NETWORK_H

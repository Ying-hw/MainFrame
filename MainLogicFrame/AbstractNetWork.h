#ifndef NETWORK_H
#define NETWORK_H

#include "MainFrame_global.h"
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>

struct socketAddrInfo {
	socketAddrInfo(QHostAddress addr, int port) : m_addr(addr), m_port(port) {}
	socketAddrInfo() {};
	QHostAddress m_addr;
	int m_port;
};

class MAINFRAME_EXPORT AbstractNetWork : public QObject
{
	Q_OBJECT
public:
	enum class ProtoType {
		UDP,
		TCP,
		FTP,
		SMTP,
		HTTP
	};
	AbstractNetWork(ProtoType Type, QHostAddress addrs, int port, QObject* parent = 0);
	virtual ~AbstractNetWork();
	virtual int SendMsg(const QString& strContent);
	virtual void initCommunication();
	void* ReturnCurrentTargetSocket();
	void SetAddrInfo(QHostAddress host, int port);
	void SetCommunicationProtocol(ProtoType type);
protected slots:
	virtual int RecvMessage() = 0;
	virtual void ProcessError();
	virtual void connected();
private slots:
	void SelectNetabOnline();
	void processSelectResult(QHostInfo host);
private:
	QTcpSocket m_Tcp;
	QUdpSocket m_Udp;
	ProtoType m_prototype;
	QNetworkAccessManager m_AccessMan;
	QNetworkReply* m_pReply;
	QTimer timer;
	bool isNeytErrorWidgetShow;
	socketAddrInfo m_addrInfo;
};

#endif // NETWORK_H

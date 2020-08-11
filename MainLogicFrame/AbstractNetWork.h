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
	socketAddrInfo(QString addr, int port) : m_strAddr(addr), m_port(port) {}
	socketAddrInfo() {};
	QString m_strAddr;
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
	AbstractNetWork(ProtoType Type, QString addrs, int port, QObject* parent = 0);
	virtual ~AbstractNetWork();
	virtual int SendMsg(const QString& strContent);
	void initCommunication(QHostAddress addr);
	void* ReturnCurrentTargetSocket();
	void SetCommunicationProtocol(ProtoType type);
	void StartTimer();
	void ReleaseCommuncation();
protected slots:
	virtual int RecvMsg() = 0;
	virtual void ProcessError();
	virtual void connected();
private slots:
	void CheckNetIsOnline();
	void processSelectResult(QHostInfo host);
private:
	QTcpSocket* m_Tcp;
	QUdpSocket m_Udp;
	ProtoType m_prototype;
	QNetworkAccessManager m_AccessMan;
	QNetworkReply* m_pReply;
	QTimer m_timer;
	bool m_isNetErrorShow;
	socketAddrInfo m_addrInfo;
};

#endif // NETWORK_H

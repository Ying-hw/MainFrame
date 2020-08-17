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
class AbstractNetWork;

class MAINFRAME_EXPORT DnsDispose : public QThread {
	Q_OBJECT
public:
	DnsDispose(QString addrs, QObject* parent = 0);
	~DnsDispose();

	void run();
	void ExcuteInitNetFunction(AbstractNetWork* that);
	void StopTime();
signals:
	void NetError(const QString &strError);
public slots:
	void processDnsResult(QHostInfo host);
private:
	QString m_strAddr;
	bool m_isNetErrorShow;
	AbstractNetWork* m_NetWork;
	QHostAddress m_addr;
	QTimer m_Timer;
};



class MAINFRAME_EXPORT AbstractNetWork : public QThread
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
	virtual int SendMsg();
	void* ReturnCurrentTargetSocket();
	void SetCommunicationProtocol(ProtoType type);
	void ReleaseCommuncation();
	void run();
	int Send(std::string& strContent);
	void initCommunication(QHostAddress addr);
protected slots:
	virtual int RecvMsg() = 0;
	virtual void ProcessError(const QString& strError);
	virtual void connected();
private:
	QTcpSocket* m_Tcp;
	QUdpSocket m_Udp;
	ProtoType m_prototype;
	QNetworkAccessManager m_AccessMan;
	QNetworkReply* m_pReply;
	socketAddrInfo m_addrInfo;
	QMutex m_mutex;
	QWaitCondition m_waitMutex;
	std::string m_strContent;
	int m_size;
};

#endif // NETWORK_H

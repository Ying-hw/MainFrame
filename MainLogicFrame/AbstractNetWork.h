#ifndef NETWORK_H
#define NETWORK_H

#include "MainFrame_global.h"
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>

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
	AbstractNetWork(ProtoType Type, QObject* parent = 0);
	virtual ~AbstractNetWork();
protected:
	virtual int SendMsg(const QString& strContent);
	virtual void initCommunication(ProtoType type, QHostAddress address, int port);
protected slots:
	virtual int RecvMessage() = 0;
	virtual void ProcessError() = 0;
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
};

#endif // NETWORK_H

#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "Signal.h"
#include <QObject>

class SignalQueue;
class MainWidget;
extern MAINFRAME_EXPORT SignalQueue* g_pSignal;
#define GETINSTANCE SignalQueue::GetTargetInstance

struct ParamInfo{
	ParamInfo() :params(NULL), perious(NULL) {}
	ParamInfo(void * param, QString strTarget):params(param), strTgtName(strTarget) {}
	void *params;
	QWidget *perious;
	QString m_strPerious;
	QString strTgtName;
};

class MAINFRAME_EXPORT SignalQueue : public QThread
{
	Q_OBJECT
public:
	SignalQueue();
	~SignalQueue();

	void run();
	void selectSignal(QPair<Signal_, void *> p);
	void push_queue(QPair<Signal_, void *> p);
	static void doit();
	void Send_Message(Signal_ signal_, void *param); 
	void Send_Message(Signal_ signal_, void *widget, const QString strChild, const QString strParent); 
	void Send_Message(Signal_ signal_, void *param, QWidget* that);
	static void* GetTargetInstance(QString strTarget);
	void SetUserIdentify(void *, SystemUser user);
	void DeleteAll(MainWidget* pTgtWidget);
	void *ReturnUser(SystemUser user);
	void removeUser(SystemUser SysUser);
	template<typename T> void Recv_Message(Signal_ SIG, T t, QWidget* that);
	template<typename T> void Recv_Message(Signal_ SIG, T t);
	template<typename T> void Recv_Message(T* t, const QString strParent);
signals:
	void hide_Window();
	void close_Window();
	void ExitSystem();
	void minWindow();
	void maxWindow();
	void ReloadUI(QWidget* that, const QRect &rect);
	void MakeFile(void *source);
	void UpdateWindowGeometry();
private:
	bool m_isRuning;
	QQueue<QPair<Signal_, void *>> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	QMap<SystemUser, void*> m_mapUser; 
	ParamInfo m_ParamInfo;
	Signal_ m_CurrentSignal;
};

template<typename T>
void SignalQueue::Recv_Message(Signal_ SIG, T t)
{
	Send_Message(SIG, t);
}

template<typename T>
void SignalQueue::Recv_Message(T* t, const QString strParent)
{
	qDebug() << t->metaObject()->className();
	Send_Message(Signal_::RELOADUI, t, t->metaObject()->className(), strParent);
}

template<typename T>
void SignalQueue::Recv_Message(Signal_ SIG, T t, QWidget* that)
{
	Send_Message(Signal_::SWITCHPLUGIN, t, that);
}

#endif  //SIGNALQUEUE__


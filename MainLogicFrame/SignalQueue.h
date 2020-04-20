#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "Signal.h"
#include <QObject>

class SignalQueue;
extern MAINFRAME_EXPORT SignalQueue* g_pSignal;

#define SENDSIGNAL SignalQueue::Recv_Message

struct ParamInfo{
	void *params;
	QString strType;
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
	static void Send_Message(Signal_ signal_, void *param);
	static void Send_Message(Signal_ signal_, void *widget, QString strWidgetType);
	void SetUserIdentify(void *, User user);
	void DeleteAll();
	void *ReturnUser(User user);
	template<typename T> static void Recv_Message(Signal_ SIG, T t);
	template<typename T> static void Recv_Message(T* t);
signals:
	void close_Window(bool isClose);
	void close_Window();
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
	QMap<User, void*> m_mapUser;
	ParamInfo m_ParamInfo;
};

template<typename T>
void SignalQueue::Recv_Message(Signal_ SIG, T t)
{
	Send_Message(SIG, (void *)t);
}

template<typename T>
void SignalQueue::Recv_Message(T* t)
{
	qDebug() << t->metaObject()->className();
	Send_Message(Signal_::RELOADUI, t, t->metaObject()->className());
}

#endif  //SIGNALQUEUE__

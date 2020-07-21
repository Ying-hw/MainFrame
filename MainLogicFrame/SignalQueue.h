#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "Signal.h"
#include <QObject>

class SignalQueue;
class MainWidget;
class AbstractWidget;

extern MAINFRAME_EXPORT SignalQueue* g_pSignal;
#define GETINSTANCE SignalQueue::GetTargetInstance

struct ParamInfo{
	ParamInfo() :m_Params(NULL), m_PreviousWidget(NULL) {}
	ParamInfo(void * param, QString strTarget) : m_Params(param), strTgtName(strTarget) {}
	void *m_Params;
	AbstractWidget *m_PreviousWidget;
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
	void Send_Message(Signal_ signal_, void *widget, const QString strChild, AbstractWidget* Tgt); 
	void Send_Message(Signal_ signal_, void *param, AbstractWidget* that);
	static void* GetTargetInstance(QString strTarget);
	void SetUserIdentify(void *, SystemUser user);
	void DeleteAll(MainWidget* pTgtWidget);
	void *ReturnUser(SystemUser user);
	void removeUser(SystemUser SysUser);
	template<typename T> void Recv_Message(Signal_ SIG, T t, AbstractWidget* that);
	template<typename T> void Recv_Message(Signal_ SIG, T t);
	template<typename T> void Recv_Message(Signal_ SI, T* t, AbstractWidget* Tgt, bool isUi);
signals:
	void hide_Window();
	void close_Window();
	void ExitSystem();
	void minWindow();
	void maxWindow();
	void showWindow(AbstractWidget* that, const QString& strParentName);
	void UpdateWindowGeometry(AbstractWidget* that);
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
void SignalQueue::Recv_Message(Signal_ SIG, T* t, AbstractWidget* TgtWidget, bool isUi)
{
	qDebug() << t->metaObject()->className();
	Send_Message(SIG, t, t->metaObject()->className(), TgtWidget);
}

template<typename T>
void SignalQueue::Recv_Message(Signal_ SIG, T t, AbstractWidget* that)
{
	Send_Message(Signal_::SWITCHPLUGIN, t, that);
}

#endif  //SIGNALQUEUE__


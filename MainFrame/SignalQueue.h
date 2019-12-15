#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"

#include "Signal.h"



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
	void SetUserIdentify(void *, User user);
signals:
	void close_Window();
	void exit_Window();
	void minWindow();
	void maxWindow();
	void RELOADUI(QWidget* that, const QRect &rect);
private:
	bool m_isRuning;
	QQueue<QPair<Signal_, void *>> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	QMap<User, void*> m_mapUser;
};

#endif  //SIGNALQUEUE__


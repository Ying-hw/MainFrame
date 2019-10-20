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
	void selectSignal(const Signal_ signal_);
	void push_queue(Signal_ signal_);
	static void doit();
	static void Send_Message(Signal_ signal_);
	void SetUserIdentify(void *, User user);
signals:
	void close_Window();
	void minWindow();
	void maxWindow();
private:
	bool m_isRuning;
	QQueue<Signal_> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	QMap<User, void*> m_mapUser;
};

#endif  //SIGNALQUEUE__


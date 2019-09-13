#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__

#include "Signal.h"

class SignalQueue : public QThread
{
public:
	SignalQueue();
	~SignalQueue();

	void run();
	void selectSignal(const Signal_ signal_);
	void push_queue(Signal_ signal_);
	static void doit();
private:
	bool m_isRuning;
	QQueue<Signal_> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
};

#endif  //SIGNALQUEUE__


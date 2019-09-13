#include "stdafx.h"
#include "SignalQueue.h"


SignalQueue::SignalQueue() : QThread(), m_isRuning(true)
{
	m_Mutex.lock();
}


SignalQueue::~SignalQueue()
{
	m_isRuning = false;
}

void SignalQueue::push_queue(Signal_ signal_) {
	m_queue.enqueue(signal_);
	m_waitMutex.wakeOne();
}

void SignalQueue::run() {
	while (m_isRuning) {
		if (m_queue.isEmpty())
			m_waitMutex.wait(&m_Mutex);
		else
			while (!m_queue.isEmpty())
				selectSignal(m_queue.dequeue());
	}
}

void SignalQueue::selectSignal(Signal_ signal_) {
	switch (signal_) {
	case Signal_::WINDOWCLOSE:
		break;
	case Signal_::WINDOWEXIT:
		//atexit(doit);
		//exit();
		break;
	case Signal_::WINDOWMAX:
		break;
	case Signal_::WINDOWMIN:
		break;
	case Signal_::WINDOWSHOW:
		break;
	case Signal_::ISFIXEDSIZE:
		break;
	case Signal_::IGNORESIGNAL:
		break;
	case Signal_::FREEPLUG:
		break;
	case Signal_::LOADPLUG:
		break;
	case Signal_::WRITELOG:
		break;
	default:
		break;
	}
}

void SignalQueue::doit() {

}
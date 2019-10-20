#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "SignalQueue.h"
#include "MainFrame_global.h"
SignalQueue* g_pSignal;

SignalQueue::SignalQueue() : QThread(), m_isRuning(true)
{
	g_pSignal = this;
	m_Mutex.lock();
}


SignalQueue::~SignalQueue()
{
	m_isRuning = false;
}

void SignalQueue::Send_Message(Signal_ signal_) {
	g_pSignal->push_queue(signal_);
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
		emit close_Window();
		break;
	case Signal_::WINDOWEXIT:
		//atexit(doit);
		//exit();
		break;
	case Signal_::WINDOWMAX:
		break;
	case Signal_::WINDOWMIN:
		break;
	case Signal_::ISFIXEDSIZE:
		break;
	case Signal_::IGNORESIGNAL:
		break;
	case Signal_::FREEPLUG:
	{
		const QRect rect = static_cast<MainWidget*>(m_mapUser[User::MAINWIDGET])->geometry();
		QString strRect = QString::number(rect.x()) + QString::number(rect.y()) +
			QString::number(rect.width()) + QString::number(rect.height());
		MainFrame::FreeLib(static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]), strRect);
	}
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

void SignalQueue::SetUserIdentify(void *pIdentify, User user) {
	m_mapUser[user] = pIdentify;
	if (user == User::MAINWIDGET)
		connect(this, SIGNAL(close_Window()), (MainWidget*)pIdentify
			, SLOT(closeWindow()));
}
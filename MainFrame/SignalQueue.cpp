#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "SignalQueue.h"
SignalQueue* g_pSignal;


SignalQueue::SignalQueue() : QThread(), m_isRuning(true)
{
	g_pSignal = this;
	m_Mutex.lock();
}


SignalQueue::~SignalQueue()
{
	m_isRuning = false;
	wait();
}

void SignalQueue::Send_Message(Signal_ signal_, void* param) {
	QPair<Signal_, void *> p;
	p.first = signal_;
	p.second = param;
	g_pSignal->push_queue(p);
}

void SignalQueue::push_queue(QPair<Signal_, void *> p) {
	m_queue.enqueue(p);
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

void SignalQueue::selectSignal(QPair<Signal_, void *> p) {
	switch (p.first) {
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
		emit minWindow();
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
	case Signal_::RELOADUI:
	{
		QWidget *that = static_cast<QWidget*>(p.second);
		QRect rect = that->rect();
		emit RELOADUI(that, rect);
	}		
		break;
	default:
		break;
	}
}

void SignalQueue::doit() {
	
}

void SignalQueue::SetUserIdentify(void *pIdentify, User user) {
	m_mapUser[user] = pIdentify;
	if (user == User::MAINWIDGET) {
		connect(this, SIGNAL(close_Window()), (MainWidget*)pIdentify
			, SLOT(closeWindow()));
		connect(this, SIGNAL(minWindow()), (MainWidget*)pIdentify
			, SLOT(minWindow()));
		connect(this, SIGNAL(RELOADUI(QWidget*, const QRect &)), (MainWidget*)pIdentify
			, SLOT(setMain(QWidget*, const QRect &)));
	}
}

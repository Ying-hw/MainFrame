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
}

void SignalQueue::Send_Message(Signal_ signal_, void* param) {
	QPair<Signal_, void *> p;
	p.first = signal_;
	p.second = param;
	g_pSignal->push_queue(p);
}

void SignalQueue::Send_Message(Signal_ signal_, void *param, QString strParamType) {
	g_pSignal->m_ParamInfo.params = param;
	g_pSignal->m_ParamInfo.strType = strParamType;
	Send_Message(signal_, &g_pSignal->m_ParamInfo);
}

void SignalQueue::push_queue(QPair<Signal_, void *> p) {
	while (!m_queue.isEmpty());
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
		emit UpdateWindowGeometry();
		emit close_Window();
		break;
	case Signal_::WINDOWEXIT:
		emit UpdateWindowGeometry();
		emit close_Window(true);
		break;
	case Signal_::WINDOWMAX:
		break;
	case Signal_::WINDOWMIN:
		emit minWindow();
		break;
	case Signal_::IGNORESIGNAL:
		break;
	case Signal_::FREEPLUG:
	{
		const QRect rect = static_cast<MainWidget*>(m_mapUser[User::MAINWIDGET])->geometry();
		QString strRect = QString("%1,%2,%3,%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
		MainFrame::FreeLib(static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]));
	}
		break;
	case Signal_::LOADPLUG:
	{
		QString strpTarget = (char *)p.second;
		MainFrame::LoadLib(static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]), strpTarget);
	}
		break;
	case Signal_::WRITELOG:
	{
		MainFrame* frame = static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]);
		frame->WriteLog((const char *)p.second);
	}
		break;
	case Signal_::RELOADUI:
	{
		QWidget *that = static_cast<QWidget*>(g_pSignal->m_ParamInfo.params);
		static_cast<MainFrame*>(m_mapUser[User::MAINFRAME])->UpdataGeometry();
		QRect rect = static_cast<MainFrame*>(m_mapUser[User::MAINFRAME])
			->FindChildUiLocation(that, g_pSignal->m_ParamInfo.strType);
		emit close_Window(false);
		msleep(500);
		emit ReloadUI(that, rect);
	}		
		break;
	case Signal_::SWITCHPLUGIN:
	{
		emit close_Window(true); 
		msleep(500);
		MainFrame::FreeLib(static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]));
		QString strpTarget = (char *)p.second;
		MainFrame::LoadLib(static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]), strpTarget);
	}
		break;
	case Signal_::PLUGINNAMECHANGED:

		break;
	case Signal_::MAKEPLUGINFILE:
		emit MakeFile(p.second);
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
		connect(this, SIGNAL(close_Window()), MainWidget::staticThis
			, SLOT(closeWindow()));
		connect(this, SIGNAL(minWindow()), MainWidget::staticThis
			, SLOT(showMinimized()));
		connect(this, SIGNAL(ReloadUI(QWidget*, const QRect &)), MainWidget::staticThis
			, SLOT(setMain(QWidget*, const QRect &)));
	}	
	else {
		connect(this, SIGNAL(MakeFile(void *)), (MainFrame*)pIdentify, SLOT(MakePluginsProtobufFile(void*)));
		connect(this, SIGNAL(UpdateWindowGeometry()), (MainFrame*)pIdentify, SLOT(UpdataGeometry()));
	}
}

void SignalQueue::DeleteAll() {
	delete static_cast<MainWidget*>(m_mapUser[User::MAINWIDGET]);
	delete static_cast<MainFrame*>(m_mapUser[User::MAINFRAME]);
	delete this;
}

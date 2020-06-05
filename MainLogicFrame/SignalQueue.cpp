#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "SignalQueue.h"
SignalQueue* g_pSignal;


SignalQueue::SignalQueue() : QThread(), m_isRuning(true), m_CurrentSignal(Signal_::INVALID)
{
	g_pSignal = this;
	m_Mutex.lock();
}

SignalQueue::~SignalQueue()
{
	m_isRuning = false;
	m_Mutex.unlock();
}

void SignalQueue::Send_Message(Signal_ signal_, void* param) { 
	g_pSignal->m_CurrentSignal = signal_;    
	QPair<Signal_, void *> p;
	p.first = signal_;
	p.second = param;
	g_pSignal->push_queue(p);
}

void SignalQueue::Send_Message(Signal_ signal_, void *widget, QString strWidgetType) {
	g_pSignal->m_ParamInfo.params = widget;
	g_pSignal->m_ParamInfo.strType = strWidgetType;
	Send_Message(signal_, &g_pSignal->m_ParamInfo);
}          

void* SignalQueue::GetTargetInstance(QString strTarget)
{
	return MainFrame::LoadLib(static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME]), strTarget, true);
}

void SignalQueue::push_queue(QPair<Signal_, void *> p) {
	while (!m_queue.isEmpty());
	m_queue.enqueue(p);
	m_waitMutex.wakeOne();
}

void SignalQueue::run() {
	while (m_isRuning) 
		if (m_queue.isEmpty())
			m_waitMutex.wait(&m_Mutex);
		else
			while (!m_queue.isEmpty()) 
				selectSignal(m_queue.dequeue());
}

void SignalQueue::selectSignal(QPair<Signal_, void *> p) { 
	switch (p.first) {
	case Signal_::WINDOWCLOSE:
	case Signal_::WINDOWEXIT:
		emit UpdateWindowGeometry();  
		emit ExitSystem();
		break;
	case Signal_::WINDOWMAX:
		break;
	case Signal_::WINDOWMIN:
		emit minWindow();
		break;
	case Signal_::IGNORESIGNAL:
		break;
	case Signal_::FREEPLUG:
		MainFrame::FreeLib(static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME]));
		break;
	case Signal_::LOADPLUG: 
	{
		QString strpTarget = (char *)p.second; 
		MainFrame::LoadLib(static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME]), strpTarget);
	}
		break;
	case Signal_::WRITELOG:
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->WriteLog((const char *)p.second);
		break;
	case Signal_::RELOADUI:
	{
		ParamInfo* paraminfo = (ParamInfo *)p.second;
		QWidget *that = static_cast<QWidget*>(paraminfo->params);
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->UpdataGeometry();
		QRect rect = static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->FindChildUiLocation(that, paraminfo->strType);
		emit hide_Window();
		msleep(800);
		emit ReloadUI(that, rect);
	}		
		break;
	case Signal_::SWITCHPLUGIN:
	{ 
		ParamInfo* paraminfo = (ParamInfo *)p.second;
		if (!paraminfo->isAllShow) {
			emit close_Window();
			MainFrame::FreeLib(static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME]));
		}
		MainFrame::LoadLib(static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME]), (char*)paraminfo->params);
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

void SignalQueue::SetUserIdentify(void *pIdentify, SystemUser SystemUser) {
	m_mapUser[SystemUser] = pIdentify;
	switch (SystemUser)
	{
	case SystemUser::MAINFRAME:
		connect(this, SIGNAL(MakeFile(void *)), (MainFrame*)pIdentify, SLOT(MakePluginsProtobufFile(void*)));
		connect(this, SIGNAL(UpdateWindowGeometry()), (MainFrame*)pIdentify, SLOT(UpdataGeometry()));
		break;
	case SystemUser::MAINWIDGET:
		connect(this, SIGNAL(ExitSystem()), MainWidget::staticThis, SLOT(closeWindow()));
		connect(this, SIGNAL(minWindow()), MainWidget::staticThis, SLOT(showMinimized()));
		connect(this, SIGNAL(ReloadUI(QWidget*, const QRect &)), MainWidget::staticThis, SLOT(setMain(QWidget*, const QRect &)));
		connect(this, SIGNAL(close_Window()), MainWidget::staticThis, SLOT(close()));
		connect(this, SIGNAL(hide_Window()), MainWidget::staticThis, SLOT(hide()));
		break;
	case SystemUser::MESSAGE:
		break;
	default:
		break;
	}
}

void SignalQueue::DeleteAll() {
	delete static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]);
	delete static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME]);
	m_isRuning = false;
	m_waitMutex.wakeOne();
	deleteLater();
	delete this;
}

void * SignalQueue::ReturnUser(SystemUser SystemUser) {
	return m_mapUser[SystemUser];
}


#include "stdafx.h"
#include "MainFrame.h"
#include "MainWidget.h"
#include "SignalQueue.h"

SignalQueue* g_pSignal = NULL;


SignalQueue::SignalQueue() : QThread(), m_isRuning(true), m_CurrentSignal(Signal_::INVALID)
{
	if (!g_pSignal) 
		g_pSignal = this;
	m_Mutex.lock();
}

SignalQueue::~SignalQueue()
{
	m_isRuning = false;
	m_waitMutex.wakeOne();
	wait();
}

void SignalQueue::Send_Message(Signal_ signal_, void* param) { 
	m_CurrentSignal = signal_;  
	QPair<Signal_, void *> p;
	p.first = signal_;
	p.second = param;
	push_queue(p);
}

void SignalQueue::Send_Message(Signal_ SIG, AbstractNetWork* pNet, QString strName)
{
	m_ParamInfo.m_Params = pNet;
	m_ParamInfo.strTgtName = strName;
	Send_Message(SIG, &m_ParamInfo);
}

void SignalQueue::Send_Message(Signal_ SIG, void* param, const AbstractWidget* widget)
{
	m_ParamInfo.m_Params = param;
	m_ParamInfo.m_pOldWidget = const_cast<AbstractWidget*>(widget);
	Send_Message(SIG, &m_ParamInfo);
}

void* SignalQueue::GetTargetInstance(QString strTarget)
{
	return static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->LoadLib(strTarget, true);
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
		emit g_pSignal->UpdateWindowGeometry(static_cast<AbstractWidget*>(p.second));  
		emit ExitSystem();
		break;
	case Signal_::CLOSE:
		emit close_Window();
		break;
	case Signal_::WINDOWMAX:
		break;
	case Signal_::WINDOWMIN:
		emit minWindow();
		break;
	case Signal_::FREEPLUG:
		static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->FreeLib(*(QString*)p.second);
		break;
	case Signal_::LOADPLUG: 
	{
		QString strpTarget = (char *)p.second; 
		static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->LoadLib(strpTarget);
	}
		break;
	case Signal_::WRITELOG:
		//static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->WriteLog((const char *)p.second);		
		break;
	case Signal_::SWITCHPLUGIN:
	{ 
		ParamInfo* paraminfo = (ParamInfo *)p.second;
		emit hide_Window();
		static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->MainFrame::FreeLib(paraminfo->m_pOldWidget);
		static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->LoadLib((char*)paraminfo->m_Params);		
	}
		break;
	case Signal_::INITIALIZENETWORK:
	{
		ParamInfo* paraminfo = (ParamInfo*)p.second;
		emit g_pSignal->InitNet((AbstractNetWork*)paraminfo->m_Params, paraminfo->strTgtName);
	}
		break;
	case Signal_::PLUGINNAMECHANGED:

		break;
	case Signal_::SHOW_ABSTRACTWIDGET:
	{
		AbstractWidget *that = static_cast<AbstractWidget*>(p.second);
		MainFrame* pFrame = static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME]);
		emit g_pSignal->showWindow(that, pFrame->GetParentName(that));
	}
		break;
	default:
		break;
	}
}

void SignalQueue::doit() {
	
}

void SignalQueue::SetUserIdentify(void *pIdentify, SystemUser SysUser) {
	m_mapUser[SysUser] = pIdentify;
	switch (SysUser)
	{
	case SystemUser::MAINFRAME:
		connect(this, SIGNAL(UpdateWindowGeometry(AbstractWidget*)), (MainFrame*)pIdentify, SLOT(UpdataGeometry(AbstractWidget*)));	
		connect(this, SIGNAL(showWindow(AbstractWidget*, const QString&)), (MainFrame*)pIdentify, SLOT(Initialize_WidgetInterface(AbstractWidget*, const QString&)));
		connect(this, SIGNAL(InitNet(AbstractNetWork*, const QString&)), (MainFrame*)pIdentify, SLOT(Initialize_NetInterface(AbstractNetWork*, const QString&)));
		break;
	case SystemUser::MAINWIDGET:
		connect(this, SIGNAL(ExitSystem()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(closeWindow()));
		connect(this, SIGNAL(minWindow()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(showMin()));
		connect(this, SIGNAL(close_Window()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(close()));
		connect(this, SIGNAL(hide_Window()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(hide()));
		break;
	case SystemUser::MESSAGE:
		break;
	default:
		break;
	}
}

void SignalQueue::DeleteAll(MainWidget* pTgtWidget) {
	m_mapUser.erase(std::remove_if(m_mapUser.begin(), m_mapUser.end(), [pTgtWidget](void* arg) {
		return static_cast<MainWidget*>(arg) == pTgtWidget;
	}));
	MainFrame* frame = static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME]);
	int ResidueCount = frame->GetShowWidgetCount(pTgtWidget);
	const QString strName = frame->GetMyselfName(pTgtWidget->GetInstance());
	emit frame->ReleaseWidget(strName, frame->isParent(strName));
	if (ResidueCount > 0)
		return;
	m_isRuning = false;
	m_waitMutex.wakeOne();
	deleteLater();
	delete frame;
	frame = NULL;
	auto init = g_pSignal->ReturnUser(SystemUser::MESSAGE);
	if (init) {
		delete init;
		init = NULL;
	}
	delete g_pSignal;
	g_pSignal = NULL;
}

void * SignalQueue::ReturnUser(SystemUser SystemUser) {

	return m_mapUser.contains(SystemUser) ? m_mapUser[SystemUser] : nullptr;
}

void SignalQueue::removeUser(SystemUser sysUser)
{
	m_mapUser.remove(sysUser);
}


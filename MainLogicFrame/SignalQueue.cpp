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
	sleep(1);
	deleteLater();
}

void SignalQueue::Send_Message(Signal_ signal_, void* param) { 
	m_CurrentSignal = signal_;    
	QPair<Signal_, void *> p;
	p.first = signal_;
	p.second = param;
	push_queue(p);
}

void SignalQueue::Send_Message(Signal_ signal_, void *widget, const QString strChild, const QString strParent, AbstractWidget* Tgt) {
	m_ParamInfo.params = widget;
	m_ParamInfo.strTgtName = strChild;
	m_ParamInfo.m_strPerious = strParent;
	m_ParamInfo.perious = Tgt;
	Send_Message(signal_, &m_ParamInfo);
}          

void SignalQueue::Send_Message(Signal_ signal_, void *param, AbstractWidget* that)
{
	m_ParamInfo.params = param;
	m_ParamInfo.perious = that;
	Send_Message(signal_, &m_ParamInfo);
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
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->FreeLib(*(QString*)p.second);
		break;
	case Signal_::LOADPLUG: 
	{
		QString strpTarget = (char *)p.second; 
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->LoadLib(strpTarget);
	}
		break;
	case Signal_::WRITELOG:
		//static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->WriteLog((const char *)p.second);
		break;
	case Signal_::RELOADUI:
	{
		ParamInfo* paraminfo = (ParamInfo *)p.second;
		AbstractWidget *that = static_cast<AbstractWidget*>(paraminfo->params);
		static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->UpdataGeometry(paraminfo->perious);
		QRect rect = static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->FindChildUiLocation(that, paraminfo->strTgtName, paraminfo->m_strPerious);
		emit hide_Window();
		msleep(800);
		emit ReloadUI(that, rect);
	}		
		break;
	case Signal_::SWITCHPLUGIN:
	{ 
		ParamInfo* paraminfo = (ParamInfo *)p.second;
		emit close_Window();
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->MainFrame::FreeLib(paraminfo->perious);
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->LoadLib((char*)paraminfo->params);
	}
		break;
	case Signal_::INITIALIZENETWORK:
		static_cast<MainFrame*>(m_mapUser[SystemUser::MAINFRAME])->Initialize_NetInterface((AbstractNetWork*)p.second);
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

void SignalQueue::SetUserIdentify(void *pIdentify, SystemUser SysUser) {
	m_mapUser[SysUser] = pIdentify;
	switch (SysUser)
	{
	case SystemUser::MAINFRAME:
		connect(this, SIGNAL(MakeFile(void *)), (MainFrame*)pIdentify, SLOT(MakePluginsProtobufFile(void*)));
		connect(this, SIGNAL(UpdateWindowGeometry()), (MainFrame*)pIdentify, SLOT(UpdataGeometry()));
		break;
	case SystemUser::MAINWIDGET:
		connect(this, SIGNAL(ExitSystem()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(closeWindow()));
		connect(this, SIGNAL(minWindow()), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(showMinimized()));
		connect(this, SIGNAL(ReloadUI(AbstractWidget*, const QRect &)), static_cast<MainWidget*>(m_mapUser[SystemUser::MAINWIDGET]), SLOT(setMain(AbstractWidget*, const QRect &)));
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
	static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->UpdataGeometry(pTgtWidget->GetInstance());
	for (QMap<SystemUser, void*>::iterator it = m_mapUser.begin();
		it != m_mapUser.end();it++) 
		if (static_cast<MainWidget*>((*it)) == pTgtWidget) {
			m_mapUser.erase(it);
			break;
		}
	int size = static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME])->RemoveWidget(pTgtWidget);
	delete pTgtWidget;
	pTgtWidget = NULL;
	if (size > 0) 
		return;
	delete static_cast<MainFrame*>(g_pSignal->m_mapUser[SystemUser::MAINFRAME]);
	m_isRuning = false;
	m_waitMutex.wakeOne();
	deleteLater();
}

void * SignalQueue::ReturnUser(SystemUser SystemUser) {

	return m_mapUser.contains(SystemUser) ? m_mapUser[SystemUser] : nullptr;
}

void SignalQueue::removeUser(SystemUser sysUser)
{
	m_mapUser.remove(sysUser);
}


#include "stdafx.h"
#include "AbstractObject.h"
#include "SignalQueue.h"
#include "MainFrame.h"


AbstractObject::AbstractObject() : m_pObjectInstance(nullptr)
{

}

AbstractObject::~AbstractObject()
{

}

void AbstractObject::OnMessage()
{

}


void AbstractObject::Log(LogGrade grade, const QString& strTgtLog)
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	frame->WriteLog(grade, strTgtLog);
}

bool AbstractObject::PlugIsRuning(const QString& strPlug, const QString& strInstance) const
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->CheckPlugIsRuning(strPlug, strInstance);
}

void AbstractObject::SendSIG(Signal_ sig, void *arg, Signal_Type type) const
{
	MainFrame* frame = static_cast<MainFrame*>(g_pSignal->ReturnUser(SystemUser::MAINFRAME));
	SignalQueue* pTgtQueue = frame->GetTgtSigQueueInstance(this);
	switch (sig)
	{
	case Signal_::INVALID:
		break;
	case Signal_::SWITCHPLUGIN:
		pTgtQueue->Send_Message(sig, arg, this);
		break;
	default:
		pTgtQueue->Send_Message(sig, arg);
		break;
	}
}
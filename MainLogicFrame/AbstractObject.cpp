#include "stdafx.h"
#include "AbstractObject.h"
#include "SignalQueue.h"
#include "MainFrame.h"


AbstractObject::AbstractObject(QObject* parent /*= 0*/)
{

}

AbstractObject::~AbstractObject()
{

}

void AbstractObject::OnMessage()
{

}

void AbstractObject::OnClose()
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
	case Signal_::INITIALIZENETWORK:
		if (type == Signal_Type::CMD)
			frame->Initialize_NetInterface((AbstractNetWork*)arg, this->metaObject()->className());
		else
			pTgtQueue->Send_Message(sig, (AbstractNetWork*)arg, this->metaObject()->className());
		break;
	case Signal_::SWITCHPLUGIN:
		pTgtQueue->Send_Message(sig, arg, this);
		break;
	default:
		pTgtQueue->Send_Message(sig, arg);
		break;
	}
}
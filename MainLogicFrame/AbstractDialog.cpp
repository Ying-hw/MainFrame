#include "stdafx.h"
#include "AbstractDialog.h"
#include "MainFrame.h"
#include "SignalQueue.h"

AbstractDialog::AbstractDialog(QDialog* parent) : QDialog(parent)
{
}

AbstractDialog::~AbstractDialog()
{
}

void AbstractDialog::OnMessage()
{

}

void AbstractDialog::OnClose()
{

}

void AbstractDialog::Log(LogGrade grade, const QString& strTgtLog)
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	frame->WriteLog(grade, strTgtLog);
}

bool AbstractDialog::PlugIsRuning(const QString& strPlug, const QString& strInstance) const
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->CheckPlugIsRuning(strPlug, strInstance);
}

void AbstractDialog::SendSIG(Signal_ sig, void *arg, Signal_Type type) const
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

void AbstractDialog::closeEvent(QCloseEvent* event)
{
	OnClose();
}

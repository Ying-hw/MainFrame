#include "stdafx.h"
#include "AbstractWidget.h"
#include "SignalQueue.h"
#include "MainFrame.h"

AbstractWidget::AbstractWidget(QWidget* parent /*= 0*/)
{

}

AbstractWidget::~AbstractWidget()
{
	OnClose();
}

void AbstractWidget::OnMessage()
{

}

void AbstractWidget::OnClose()
{

}

void AbstractWidget::Log(const QString& strTgtLog)
{

}

int AbstractWidget::CalculateResolution(const int& size)
{
	return 0;
}

bool AbstractWidget::PlugIsRuning(const QString& strPlug, const QString& strInstance)
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->CheckPlugIsRuning(strPlug, strInstance);
}

void AbstractWidget::SendSIG(Signal_ sig, void *arg)
{
	MainFrame* frame = static_cast<MainFrame*>(g_pSignal->ReturnUser(SystemUser::MAINFRAME));
	SignalQueue* pTgtQueue =  frame->GetTgtSigQueueInstance(this);
	switch (sig)
	{
	case Signal_::RELOADUI:
		pTgtQueue->Recv_Message((AbstractWidget*)arg, this);
		break;
	case Signal_::INVALID:
		break;
	case Signal_::SWITCHPLUGIN:
		pTgtQueue->Recv_Message(sig, arg, this);
		break;
	default:
		pTgtQueue->Recv_Message(sig, arg);
		break;
	}
}


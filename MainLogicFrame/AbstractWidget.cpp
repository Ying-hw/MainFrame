#include "stdafx.h"
#include "AbstractWidget.h"
#include "SignalQueue.h"
#include "MainFrame.h"

AbstractWidget::AbstractWidget(QWidget* parent /*= 0*/)
{

}

AbstractWidget::~AbstractWidget()
{

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
	case Signal_::SHOW_ABSTRACTWIDGET:
		pTgtQueue->Recv_Message(sig, (AbstractWidget*)arg, this, true);
		break;
	case Signal_::INVALID:
		break;
	case Signal_::SWITCHPLUGIN:
		pTgtQueue->Recv_Message(sig, arg, this);
		break;
	case Signal_::INITIALIZENETWORK:
		pTgtQueue->Recv_Message(sig, (AbstractNetWork*)arg, this->metaObject()->className());
		break;
	default:
		pTgtQueue->Recv_Message(sig, arg);
		break;
	}
}


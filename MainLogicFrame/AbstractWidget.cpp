#include "stdafx.h"
#include "AbstractWidget.h"
#include "SignalQueue.h"
#include "MainFrame.h"

AbstractWidget* AbstractWidget::m_pInstanceWidget = NULL;
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

void AbstractWidget::Log(const QString& strTgtLog) const
{

}

int AbstractWidget::CalculateResolution(const int& size) const
{
	return 0;
}

bool AbstractWidget::PlugIsRuning(const QString& strPlug, const QString& strInstance) const
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->CheckPlugIsRuning(strPlug, strInstance);
}

void AbstractWidget::SendSIG(Signal_ sig, void *arg) const
{
	MainFrame* frame = static_cast<MainFrame*>(g_pSignal->ReturnUser(SystemUser::MAINFRAME));
	SignalQueue* pTgtQueue =  frame->GetTgtSigQueueInstance(this);
	switch (sig)
	{
	case Signal_::INVALID:
		break;
	case Signal_::INITIALIZENETWORK:
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


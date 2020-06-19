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

bool AbstractWidget::PlugIsRuning(const QString& strPlug, const QString& strInstance)
{
	MainFrame* frame = (MainFrame *)g_pSignal->ReturnUser(SystemUser::MAINFRAME);
	return frame->CheckIsRuningPlug(strPlug, strInstance);
}


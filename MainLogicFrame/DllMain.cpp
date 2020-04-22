#include "stdafx.h"
#include "MainWidget.h"
#include "MainFrame.h"

MainWidget* MainWidget::staticThis = new MainWidget;

extern "C" Q_DECL_EXPORT void Handle() {
	SignalQueue* singalThread = new SignalQueue;
	singalThread->start();
	MainFrame *w = new MainFrame;
	MainWidget::staticThis->InitAanimation();
}

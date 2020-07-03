#include "stdafx.h"
#include "MainFrame.h"

extern "C" Q_DECL_EXPORT void Handle() {
	SignalQueue* singalThread = new SignalQueue;
	singalThread->start();
	MainFrame *w = new MainFrame;
}

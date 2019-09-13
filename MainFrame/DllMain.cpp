#include "stdafx.h"
#include "MainFrame.h"

extern "C" Q_DECL_EXPORT void Handle() {
	MainFrame *w = new MainFrame;
}

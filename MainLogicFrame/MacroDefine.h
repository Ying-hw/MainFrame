#ifndef __MACRODEFINE_H__
#define __MACTODEFINE_H__
#include "SignalQueue.h"
#include "Signal.h"
#include <QString>

#define SIGNALVALUE
#define PARAM		
#define SEND_SIGNAL(SIGNALVALUE, PARAM) \
if (SIGNALVALUE == Signal_::RELOADUI) \
	SENDSIGNAL(SIGNALVALUE, PARAM, typeid(PARAM).name()); \
else \
	SENDSIGNAL(SIGNALVALUE, PARAM); 

	 

#endif  //__MACTODEFINE_H__
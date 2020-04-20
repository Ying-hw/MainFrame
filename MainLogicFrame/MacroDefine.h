#ifndef __MACRODEFINE_H__
#define __MACTODEFINE_H__
#include "SignalQueue.h"
#include "Signal.h"
#include <QString>
#include <QObject>

#define SIGNAL_VALUE
#define PARAM		

#define SEND_SIGNAL(SIGNAL_VALUE, PARAM) \
if constexpr (SIGNAL_VALUE == Signal_::RELOADUI) \
	SENDSIGNAL(PARAM); \
else \
	SENDSIGNAL(SIGNAL_VALUE, PARAM);
	
#endif  //__MACTODEFINE_H__
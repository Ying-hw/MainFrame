#ifndef __MACRODEFINE_H__
#define __MACTODEFINE_H__
#include "SignalQueue.h"
#include "Signal.h"

#define SIGNALVALUE
#define PARAM		
#define SENDMESSAGE(SIGNALVALUE, PARAM) SignalQueue::Send_Message(SIGNALVALUE, PARAM) 

#endif  //__MACTODEFINE_H__
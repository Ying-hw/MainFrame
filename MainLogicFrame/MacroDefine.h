#ifndef __MACRODEFINE_H__
#define __MACTODEFINE_H__
#include "SignalQueue.h"
#include "Signal.h"
#include <QString>
#include "Message.h"
#ifndef MAINFRAME_LIB
#include "sqlPlugin.h"
#endif

#define SEND_MESSAGE		CommonTemplate::Send_Message
#define GET_MESSAGE			CommonTemplate::InitType::Get

#ifndef MAINFRAME_LIB
	#define  GET_DATA sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->GetData
    #define	 EXECUTE	sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->ExecuteSql
	#define  OPEN_DATATBASE sqlPlugin::DataLib::GetDataLibInstance()->openDataLib
#endif


#define SIGNAL_VALUE
#define PARAM		
#define PARAM1

#define SEND_SIGNAL(SIGNAL_VALUE, PARAM) \
if constexpr (SIGNAL_VALUE == Signal_::RELOADUI) \
	SENDSIGNAL(PARAM); \
else \
	SENDSIGNAL(SIGNAL_VALUE, PARAM);
	
#endif  //__MACTODEFINE_H__
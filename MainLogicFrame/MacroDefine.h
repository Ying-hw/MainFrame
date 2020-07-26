#ifndef __MACTODEFINE_H__
#define __MACTODEFINE_H__

#include "MessageTemplate.h"
#ifndef MAINFRAME_LIB
#include "sqlPlugin.h"
#endif

#define SEND_MESSAGE		CommonTemplate::Send_Message
#define GET_MESSAGE			CommonTemplate::InitType::Get


#ifndef MAINFRAME_LIB
	#define  GET_DATA sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->GetData
    #define	 EXECUTE	sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->ExecuteSql
    #define  UPDATE_IMAGE sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->InsertImage
	#define  OPEN_DATATBASE sqlPlugin::DataLib::GetDataLibInstance()->openDataLib
#endif

#endif  //__MACTODEFINE_H__
#ifndef __MACTODEFINE_H__
#define __MACTODEFINE_H__

//#include "MessageTemplate.h"
#ifndef MAINFRAME_LIB
#include "sqlPlugin.h"
#endif

#define SEND_MESSAGE		CommonTemplate::Send_Message
#define GET_MESSAGE			CommonTemplate::InitType::Get

#define   CONFIG   "../Data/Config/"
#define   LOG      "../Data/Log/"
#define   IMAGE    "../Data/Image/"

#define QWIDGET    QWidget
#define QMAINWINDOW     QMainWindow



#define LOCATION
#define STRTOLOCATION(LOCATION) QRect(LOCATION.section(",",0,0).toInt(), \
									LOCATION.section(",", 1, 1).toInt(), \
									LOCATION.section(",", 2, 2).toInt(), \
									LOCATION.section(",", 3, 3).toInt());   

#define LOCATIONTOSTR(LOCATION) QString("%1,%2,%3,%4").arg(LOCATION->geometry().x()) \
										.arg(LOCATION->geometry().y()).arg(LOCATION->geometry().width()) \
										.arg(LOCATION->geometry().height());   ///< ¾ØÐÎ×ªÎª×Ö·û´®£¬´ý·ÏÆú

#define    SIG_WINDOWCLOSE			 0x010
#define    SIG_CLOSE                 0X020
#define	   SIG_WINDOWMAX			 0x030
#define	   SIG_WINDOWMIN			 0x040
#define	   SIG_WINDOWEXIT			 0x055
#define	   SIG_FREEPLUG				 0x065
#define	   SIG_LOADPLUG				 0x075
#define	   SIG_WRITELOG				 0x080
#define	   SIG_PLUGINNAMECHANGED     0x090
#define    SIG_INITIALIZENETWORK     0x0100
#define    SIG_SHOW_ABSTRACTWIDGET   0x0110
#define	   SIG_SWITCHPLUGIN			 0x0120
#define	   SIG_INVALID				(SIG_SWITCHPLUGIN) + 1


#ifndef MAINFRAME_LIB
	#define  GET_DATA			sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->GetData
    #define	 EXECUTE			sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->ExecuteSql
    #define  UPDATE_IMAGE		sqlPlugin::DataLib::GetDataLibInstance()->GetSelectInstance()->InsertImage
	#define  OPEN_DATATBASE		sqlPlugin::DataLib::GetDataLibInstance()->ConnectServer
	#define  GET_DATALIBTHREAD  sqlPlugin::DataLib::GetDataLibInstance()
    #define  GET_OPENRESULT     sqlPlugin::DataLib::GetDataLibInstance()->GetOpenResult
#endif

enum class Signal_ {
	WINDOWCLOSE				= SIG_WINDOWCLOSE,
	CLOSE					= SIG_CLOSE,
	WINDOWMAX				= SIG_WINDOWMAX,
	WINDOWMIN				= SIG_WINDOWMIN,
	WINDOWEXIT				= SIG_WINDOWEXIT,
	FREEPLUG				= SIG_FREEPLUG,
	LOADPLUG				= SIG_LOADPLUG,
	WRITELOG				= SIG_WRITELOG,
	PLUGINNAMECHANGED		= SIG_PLUGINNAMECHANGED,
	INITIALIZENETWORK		= SIG_INITIALIZENETWORK,
	SHOW_ABSTRACTWIDGET		= SIG_SHOW_ABSTRACTWIDGET,
	SWITCHPLUGIN			= SIG_SWITCHPLUGIN,
	INVALID					= SIG_INVALID
};

enum class SystemUser {
	MAINFRAME,
	MESSAGE,
	MAINWIDGET,
};

enum class Signal_Type {
	CMD,
	THREAD
};

enum class LogGrade {
	SeriousError,
	Error,
	Warning,
	Tips
};


#endif  //__MACTODEFINE_H__
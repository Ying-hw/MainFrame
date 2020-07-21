#ifndef SIGNAL__
#define SIGNAL__

#define    SIG_WINDOWCLOSE			 0x010
#define	   SIG_WINDOWMAX			 0x020
#define	   SIG_WINDOWMIN			 0x030
#define	   SIG_WINDOWEXIT			 0x045
#define	   SIG_FREEPLUG				 0x055
#define	   SIG_LOADPLUG				 0x065
#define	   SIG_WRITELOG				 0x070
#define	   SIG_RELOADUI				 0x080
#define	   SIG_PLUGINNAMECHANGED     0x090
#define    SIG_INITIALIZENETWORK     0X100
#define    SIG_SHOW_ABSTRACTWIDGET   0x110
#define	   SIG_SWITCHPLUGIN			SIG_WINDOWEXIT | SIG_FREEPLUG | SIG_LOADPLUG
#define	   SIG_INVALID				(SIG_SWITCHPLUGIN) + 1

enum class Signal_ {
	WINDOWCLOSE =			SIG_WINDOWCLOSE,
	WINDOWMAX =				SIG_WINDOWMAX,
	WINDOWMIN =				SIG_WINDOWMIN,
	WINDOWEXIT =			SIG_WINDOWEXIT,
	FREEPLUG =				SIG_FREEPLUG,
	LOADPLUG =				SIG_LOADPLUG,
	WRITELOG =				SIG_WRITELOG,
	RELOADUI =				SIG_RELOADUI,
	PLUGINNAMECHANGED =		SIG_PLUGINNAMECHANGED,
	INITIALIZENETWORK =		SIG_INITIALIZENETWORK,
	SHOW_ABSTRACTWIDGET =   SIG_SHOW_ABSTRACTWIDGET,
	SWITCHPLUGIN =			SIG_SWITCHPLUGIN,
	INVALID =				SIG_INVALID
};
 
enum class SystemUser{
	MAINFRAME, 
	MESSAGE,
	MAINWIDGET,
};


#endif //SIGNAL__
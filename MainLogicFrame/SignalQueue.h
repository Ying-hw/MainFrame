#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "MacroDefine.h"

class SignalQueue;
class MainWidget;
class AbstractWidget;
class AbstractNetWork;

extern MAINFRAME_EXPORT SignalQueue* g_pSignal;
#define GETINSTANCE SignalQueue::GetTargetInstance

struct ParamInfo{
	ParamInfo() :m_Params(NULL), m_pOldWidget(NULL){}
	ParamInfo(void * param, QString strTarget) : m_Params(param), strTgtName(strTarget), m_pOldWidget(NULL){}
	void *m_Params;
	AbstractWidget* m_pOldWidget;
	QString strTgtName;
};

class MAINFRAME_EXPORT SignalQueue : public QThread
{
	Q_OBJECT
public:
	SignalQueue();
	~SignalQueue();

	void run();
	void selectSignal(QPair<Signal_, void *> p);
	void push_queue(QPair<Signal_, void *> p);
	static void doit();
	void Send_Message(Signal_ signal_, void *param); 
	void Send_Message(Signal_ SIG, AbstractNetWork* pNet, QString strName);
	void Send_Message(Signal_ SIG, void* param, const AbstractWidget* widget);
	static void* GetTargetInstance(QString strTarget);
	void SetUserIdentify(void *, SystemUser user);
	void *ReturnUser(SystemUser user);
	void removeUser(SystemUser SysUser);
	void DeleteAll(MainWidget* pTgtWidget);
signals:
	void hide_Window();
	void close_Window();
	void ExitSystem();
	void minWindow();
	void maxWindow();
	void showWindow(AbstractWidget* that, const QString& strParentName);
	void UpdateWindowGeometry(AbstractWidget* that);
	void InitNet(AbstractNetWork* net, const QString& strTgtName);
private:
	bool m_isRuning;
	QQueue<QPair<Signal_, void *>> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	QMap<SystemUser, void*> m_mapUser; 
	ParamInfo m_ParamInfo;
	Signal_ m_CurrentSignal;
	friend class MainFrame;
};

#endif  //SIGNALQUEUE__


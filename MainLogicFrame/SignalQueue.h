#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "Signal.h"

#define SENDSIGNAL SignalQueue::Send_Message

struct ParamInfo{
	void *params;
	QString strType;
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
	static void Send_Message(Signal_ signal_, void *param);
	static void Send_Message(Signal_ signal_, void *param, QString strParamType);
	void SetUserIdentify(void *, User user);
	void DeleteAll();
signals:
	void close_Window(bool isClose);
	void close_Window();
	void minWindow();
	void maxWindow();
	void ReloadUI(QWidget* that, const QRect &rect);
	void MakeFile(void *source);
	void UpdateWindowGeometry();
private:
	bool m_isRuning;
	QQueue<QPair<Signal_, void *>> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	QMap<User, void*> m_mapUser;
	ParamInfo m_ParamInfo;
};

#endif  //SIGNALQUEUE__


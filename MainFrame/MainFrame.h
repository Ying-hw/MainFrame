#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "SignalQueue.h"
 
extern SignalQueue* g_pSignal;

enum class LogGrade{
	SeriousError,
	Error,
	Warning,
	Tips
};

class Q_DECL_EXPORT MainFrame : public QThread
{
public:
	MainFrame();
	~MainFrame();

	inline void SendSharesCentent(char* centent) {
		m_data = centent;
		m_WaitCond.wakeOne();
	}
	void run();
	static void FreeLib(MainFrame* pthis, QString strRect);
	static void LoadLib(MainFrame* pthis);
private:
	void ReadPluginConfig();
	void loadManage();
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}
	void LoadPlugin(const QVector<PluginInfo>& m_vecPluginInfo);
	QDomNode ReadXML();
private:
	QFile m_logFile;
	bool m_isRunning;
	char *m_data;
	QMutex m_Mutex_;
	QLibrary m_Loadlib;
	QWaitCondition m_WaitCond;
};

#endif // MAINFRAME_H

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "Signal.h"
#include "SignalQueue.h"
 

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
private:
	void ReadPluginConfig();
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}
private:
	QVector<PluginInfo> m_vecPluginInfo;
	QFile m_logFile;
	bool m_isRunning;
	char *m_data;
	QMutex m_Mutex_;
	QWaitCondition m_WaitCond;
};

#endif // MAINFRAME_H

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
	Q_OBJECT
public:
	MainFrame();
	~MainFrame();

	inline void SendSharesCentent(char* centent) {
		m_data = centent;
		m_WaitCond.wakeOne();
	}
	void run();
	static void FreeLib(MainFrame* pthis, const QString& strRect);
	static void LoadLib(MainFrame* pthis, const QString strTargetName);
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}
signals:
	void ReleaseWidget();
	void InitWidget(bool isProgramStart);
private:
	void ReadPluginConfig();
	void loadManage();
	void LoadPlugin();
	QDomNode ReadXML();
private slots:
	void ReleaseCurrentWidget();
	void InitCurrentWidget(bool isProgramStart);
private:
	QFile m_logFile;
	bool m_isRunning;
	char *m_data;
	QMutex m_Mutex_;
	QLibrary m_Loadlib;
	QWaitCondition m_WaitCond;
	QVector<PluginInfo> m_Plugin;
};


#endif // MAINFRAME_H

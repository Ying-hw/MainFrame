#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "SignalQueue.h"
#include "plugin.pb.h"
 
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
	const QRect FindChildUiLocation(const QWidget* targetWidget, const QString& TypeName);
	static void FreeLib(MainFrame* pthis);
	static void LoadLib(MainFrame* pthis, const QString strTargetName);
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}
public slots:
	void MakePluginsProtobufFile(void* source);
	void UpdataGeometry();
signals:
	void ReleaseWidget();
	void InitWidget(const PluginInfo* targetPlug);
private:
	void ReadPluginConfig();
	void FindPlugin();
	void StartPluginControl();
	void UpdateConfigFile();
private slots:
	void ReleaseCurrentWidget();
	void InitCurrentWidget(const PluginInfo* targetPlug);
private:
	QFile m_logFile;
	bool m_isRunning;
	char *m_data;
	QMutex m_Mutex_;
	QLibrary m_Loadlib;
	QWaitCondition m_WaitCond;
	QVector<PluginInfo> m_PluginConfig;
	QString m_CurrentWindowName;
	Allplugins m_pAllPlugins;
};
#endif // MAINFRAME_H

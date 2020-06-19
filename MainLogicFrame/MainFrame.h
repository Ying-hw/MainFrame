#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "SignalQueue.h"
#include "plugin.pb.h"
#include "AbstractNetWork.h"
#include "AbstractWidget.h"

#define   CONFIG   "../Data/Config/"
#define   LOG      "../Data/Log/"
#define   IMAGE    "../Data/Image/"

#define LOCATION
#define STRTOLOCATION(LOCATION) QRect(LOCATION.section(",",0,0).toInt(), \
									LOCATION.section(",", 1, 1).toInt(), \
									LOCATION.section(",", 2, 2).toInt(), \
									LOCATION.section(",", 3, 3).toInt());   

#define LOCATIONTOSTR(LOCATION) QString("%1,%2,%3,%4").arg(LOCATION->geometry().x()) \
										.arg(LOCATION->geometry().y()).arg(LOCATION->geometry().width()) \
										.arg(LOCATION->geometry().height());   ///< ����תΪ�ַ�����������

enum class LogGrade{
	SeriousError,
	Error,
	Warning,
	Tips
};

namespace CommonTemplate {
	class InitType;
}
class MessageThread;
class MAINFRAME_EXPORT MainFrame : public QObject
{
	Q_OBJECT
public:
	
	/// \brief ���캯��
	MainFrame();
	
	/// \brief ��������
	~MainFrame();

	/// \brief �����ӽڵ�Ľ������λ��
	/// ����鲻�����������һ���ڵ㣬����������Ĭ��λ��
	/// \param[in] targetWidget Ŀ�����
	/// \param[in] ChildName �ӽڵ�����
	/// \retval ���ؽ���λ��
	const QRect FindChildUiLocation(const QWidget* targetWidget, const QString& TypeName);

	/// \brief �������Ĳ������ʵ���Ƿ�������״̬��
	/// ��������Ĳ������ʵ�����ڼ��ص�״̬��������Ϣ�̣߳����ϲ�����������Ϣ
	/// \param[in] strPlugName �������
	/// \param[in] strChildName ʵ������
	/// \param[in] type ��Ϣʵ��
	/// \retval ����true��������Ĳ�����ƻ���ʵ���������������У�����falseû��������
	bool CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type);

	/// \brief �������Ĳ������ʵ���Ƿ�������״̬��
	/// ��������Ĳ������ʵ�����ڼ��ص�״̬��������Ϣ�̣߳����ϲ�����������Ϣ
	/// \param[in] strPlugName �������
	/// \param[in] strChildName ʵ������
	/// \retval ����true��������Ĳ�����ƻ���ʵ���������������У�����falseû��������
	bool CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName);

	/// \brief �ͷ��Ѿ����صĲ��
	/// \param[in] pthis thisָ��
	static void FreeLib(MainFrame* pthis);

	/// \brief ���ز��
	/// \param[in] pthis thisָ��
	/// \param[in] strTargetName Ŀ��������
	static void LoadLib(MainFrame* pthis, const QString strTargetName);

	/// \brief ������
	static void* LoadLib(MainFrame* pthis, const QString strTargetName, bool noShow);

	/// \brief ��ʼ�����������
	/// ͨ������ָ��ָ�����࣬ʵ�ֶ�̬�麯�����ԣ������ɵ�������ӿ�
	/// \param[in] net ���̳е�����
	static void InitializeNetInterface(AbstractNetWork* net);

	/// ������
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}

public slots:
	void MakePluginsProtobufFile(void* source);  //������
	
	/// \brief ���´���λ��
	void UpdataGeometry();
signals:
	/// \brief �ͷŲ���ź�
	void ReleaseWidget();

	/// \brief ��ʼ������еĽ���
	/// \param[in] targetPlug Ŀ������Ϣ
	void InitWidget(const PluginInfo* targetPlug);
private:
	/// \brief ��ȡ��������ļ�
	void ReadPluginConfig();

	/// \brief ���Ҳ��
	/// ��������������ڲ�����������������ȼ���ʶ
	void FindPlugin();

	/// \brief ������
	void StartPluginControl();

	/// \brief ���������ļ� 
	void UpdateConfigFile();
private slots:
	/// \brief �ͷŵ�ǰ�������
	void ReleaseCurrentWidget();

	/// \brief ��ʼ����ǰ�������
	/// \param[in] targetPlug ��ǰ���
	void InitCurrentWidget(const PluginInfo* targetPlug);
private:
	QFile m_logFile;										///< ��־�ļ�
	QLibrary m_Loadlib;										///< ���ؿ�
	QVector<PluginInfo> m_PluginConfig;						///< ���в����Ϣ
	QString m_CurrentloadedPlugName;         				///< ��ǰ�������
	Allplugins m_pAllPlugins;								///< ������
	QMap<QString, AbstractWidget*> m_mapAbstractWidget;		///< ��������е�ʵ������-����������
	MessageThread* m_pMsgThread;							///< ��Ϣ�߳�
};

#endif // MAINFRAME_H

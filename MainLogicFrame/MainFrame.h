#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "SignalQueue.h"
#include "plugin.pb.h"
#include "AbstractNetWork.h"
#include "AbstractWidget.h"
#include <QLibrary>

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
	/// \param[in] strParent ��������
	/// \retval ���ؽ���λ��
	const QRect GetTargetLocation(const AbstractWidget* targetWidget, const QString& ChildName, const QString& strParent);

	/// \brief �������Ĳ������ʵ���Ƿ�������״̬��
	/// ��������Ĳ������ʵ�����ڼ��ص�״̬��������Ϣ�̣߳����ϲ�����������Ϣ
	/// \param[in] strPlugName �������
	/// \param[in] strChildName ʵ������
	/// \param[in] type ��Ϣʵ��
	/// \retval ����true��������Ĳ�����ƻ���ʵ���������������У�����falseû��������
	bool SendMsgThread(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type);

	/// \brief �������Ĳ������ʵ���Ƿ�������״̬��
	/// ��������Ĳ������ʵ�����ڼ��ص�״̬��������Ϣ�̣߳����ϲ�����������Ϣ
	/// \param[in] strPlugName �������
	/// \param[in] strChildName ʵ������
	/// \retval ����true��������Ĳ�����ƻ���ʵ���������������У�����falseû��������
	bool CheckPlugIsRuning(const QString& strPlugName, const QString& strChildName);

	/// \brief �ͷ��Ѿ����صĲ��
	/// \param[in] pthis thisָ��
	/// \param[in] that Ŀ��ʵ��
	void FreeLib(AbstractWidget* that);

	/// \brief �ͷ��Ѿ����صĲ��
	/// \param[in] strPlugName �������
	void FreeLib(const QString& strPlugName);

	/// \brief ���ز��
	/// \param[in] pthis thisָ��
	/// \param[in] strTargetName Ŀ��������
	void LoadLib(const QString strTargetName);

	/// \brief ���ز��ʵ�� ------- ������
	AbstractWidget* LoadLib(const QString strTargetName, bool noShow);

	/// \brief ��ȡĿ��������źż�
	/// \param[in] pTgtChild Ŀ������
	/// \retval �����źż�ʵ��
	SignalQueue* GetTgtSigQueueInstance(AbstractWidget* pTgtChild);

	/// \brief ���¸ò�����еĴ���λ��
	/// \param strPlugName �������
	void UpdataGeometry(const QString& strPlugName);

	/// \brief ɾ����Ӧ��widget
	/// ���ͷ�֮ǰɾ����Ӧ�Ľ��棬���ҷ���ʣ������
	/// \param[in] mainWidget ��Ӧ��widget
	/// \retval ����ʣ������
	int RemoveWidget(MainWidget* mainWidget);

	/// \brief ���������ȡ���������
	/// \param[in] ChildWidget ����
	/// \retval ���ظ��������
	const QString GetParentName(const AbstractWidget* ChildWidget);

	/// \brief ��ʼ�����������
	/// ͨ������ָ��ָ�����࣬ʵ�ֶ�̬�麯�����ԣ������ɵ�������ӿ�
	/// \param[in] net ���̳е�����
	void Initialize_NetInterface(AbstractNetWork* net);

public slots:
	
	/// \brief ��ʼ��widget������ʾ
	/// \param[in] pTgtWidget Ŀ��Widget
	/// \param[in] strChildName ��Ӧ��������
	void Initialize_WidgetInterface(AbstractWidget* pTgtWidget, const QString& strParentName);

	/// \brief ���´���λ��
	/// \param Tgt ����λ��
	void UpdataGeometry(AbstractWidget* Tgt);

signals:
	/// \brief �ͷŲ���ź�
	/// \param[in] strPlugName �������
	void ReleaseWidget(const QString& strPlugName);

	/// \brief ��ʼ������еĽ���
	/// \param[in] targetPlug Ŀ������Ϣ
	void InitWidget(const PluginInfo* targetPlug);
	
private:
	/// \brief ��ȡ��������ļ�
	void ConfigPlug();

	/// \brief ���Ҳ��
	/// ��������������ڲ�����������������ȼ���ʶ
	void FindPlugin();

	/// \brief ������
	void StartPluginControl();

	/// \brief ���������ļ� 
	void UpdateConfigFile();

	/// \brief ��ȡXML�ļ�
	/// \retval ���ض�ȡ���ĸ��ڵ�
	QDomNode ReadXML();

private slots:
	/// \brief �ͷŵ�ǰ�������
	/// \param[in] strPlugName �������
	void ReleaseCurrentWidget(const QString& strPlugName);

	/// \brief ��ʼ����ǰ�������
	/// \param[in] targetPlug ��ǰ���
	void LinkCurrentWidgetInterface(const PluginInfo* targetPlug);
private:
	QFile m_logFile;										///< ��־�ļ�
	QList<QLibrary*> m_LstLoadlib;							///< ���ؿ�
	QVector<PluginInfo> m_PluginConfig;						///< ���в����Ϣ
	QMap<AbstractWidget*, MainWidget*> m_mapMainWidget;     ///< �Ӵ��ںͿ�ܴ���
	Allplugins m_pAllPlugins;								///< ������
	QMap<QString, AbstractWidget*> m_mapAbstractWidget;		///< ��������е�ʵ������-����������
	MessageThread* m_pMsgThread;							///< ��Ϣ�߳�
};

#endif // MAINFRAME_H

#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "MacroDefine.h"
#include "AbstractObject.h"
/*!
 * \file SignalQueue.h
 *
 * \brief �����ź��࣬���������߳̽����ϲ㷢�����źŲ����д���
 *
 * \author yhw
 * \date 2021/09/12 13:16
 *
 * 
 */
class SignalQueue;
class MainWidget;
class AbstractWidget;
class AbstractNetWork;
class AbstractObject;

extern MAINFRAME_EXPORT SignalQueue* g_pSignal;

#define GETINSTANCE SignalQueue::GetTargetInstance

/*!
 * \class ParamInfo
 *
 * \brief ������Ϣ�����ɲ����ͷ��ͷ��ͽ��շ�����
 *
 * \author yhw
 * \date ���� 
 */
struct ParamInfo{
	/// \brief ���캯��
	ParamInfo() :m_Params(NULL), m_pOldWidget(NULL){}
	
	/// \brief ���캯��
	/// \param[in] param �źŲ���
	/// \param[in] strTarget ��������
	ParamInfo(void * param, QString strTarget) : m_Params(param), strTgtName(strTarget), m_pOldWidget(NULL){} 

	void *m_Params;          ///< ����
	AbstractObject* m_pOldWidget;   ///< ���ͷ�
	QString strTgtName;      ///< ���շ�
};

/*!
 * \class SignalQueue
 *
 * \brief �źŽ��ն���
 *
 *
 * \author yhw
 * \date 2021��9��12�� 
 */
class MAINFRAME_EXPORT SignalQueue : public QThread
{
	Q_OBJECT
public:

	/// \brief ���캯��
	SignalQueue();

	/// \brief ��������
	~SignalQueue();

	/// \brief �߳����к���
	void run();

	/// \brief ���Ͳ������źţ��ⲿ����
	/// \param[in] signal_ �ź�
	/// \param[in] param �źŲ���
	void Send_Message(Signal_ signal_, void *param); 

	void Send_Message(Signal_ SIG, AbstractNetWork* pNet, QString strName);

	/// \brief ���Ͳ������źţ��ⲿ����
	/// \param[in] SIG �ź�
	/// \param[in] param �źŲ���
	/// \param[in] widget Ŀ�����
	void Send_Message(Signal_ SIG, void* param, const AbstractObject* widget);

	/// \brief ��ȡĿ��ʵ��������Ŀ�궯̬�⣬���ص������
	/// \param[in] strTarget Ŀ�궯̬������
	/// \retval ���ظö�̬��ʵ��
	static void* GetTargetInstance(QString strTarget);

	/// \brief ���Ͳ������źţ��ⲿ����
	/// \param[in] signal_ �ź�
	/// \param[in] param �źŲ���
	void SetUserIdentify(void *, SystemUser user);

	/// \brief ����ʹ���źŶ��е�ʹ����
	/// \param[in] user �û���
	/// \retval ���ظ��û�ʵ��
	void *ReturnUser(SystemUser user);

	/// \brief ɾ��ʹ���źŶ��е�ʹ����
	/// \param[in] user �û���
	void removeUser(SystemUser SysUser);

	/// \brief ɾ������ʵ��
	/// \param[in] pTgtWidget Ŀ���ϲ�ʵ��
	void DeleteAll(MainWidget* pTgtWidget);
signals:
	/// \brief ���ش���
	void hide_Window();
	/// \brief �رմ���
	void close_Window();
	/// \brief �˳�ϵͳ
	void ExitSystem();
	/// \brief ��С������
	void minWindow();
	/// \brief ��󻯴���
	void maxWindow();
	/// \brief ���Ͳ������źţ��ⲿ����
	/// \param[in] that Ŀ�괰��
	/// \param[in] strParentName ����������
	void showWindow(AbstractWidget* that, const QString& strParentName);
	/// \brief ���´���λ��
	/// \param[in] that Ŀ�괰��
	void UpdateWindowGeometry(AbstractWidget* that);
	
	void InitNet(AbstractNetWork* net, const QString& strTgtName);

private:
	/// \brief ѡ���źţ�����ִ��
	/// \param[in] p �ź������
	void selectSignal(QPair<Signal_, void *> p);

	/// \brief ���������ź�ѹ�����
	/// \param[in] p �ź������
	void push_queue(QPair<Signal_, void *> p);

private:
	bool m_isRuning;    ///< �߳����б�־
	QQueue<QPair<Signal_, void *>> m_queue;   ///< �ź����������
	QWaitCondition m_waitMutex;               ///< ��������
	QMutex m_Mutex;							  ///< ������
	QMap<SystemUser, void*> m_mapUser;        ///< �źŶ��е�ʹ����ӳ��
	ParamInfo m_ParamInfo;					  ///< ������Ϣ��
	friend class MainFrame;                   ///< ��Ԫ��
};

#endif  //SIGNALQUEUE__


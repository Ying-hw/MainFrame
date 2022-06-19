#ifndef  __ABSTRACTOBJECT__
#define  __ABSTRACTOBJECT__

#include <QObject>

#include "MainFrame_global.h"
#include "SignalQueue.h"
#include <QString>
#include "TreeStruct.h"

class MAINFRAME_EXPORT AbstractObject 
{

public:
	AbstractObject();
	~AbstractObject();

	/// \brief ����Ϣ����
	/// �ں����ڲ�ִ��get���������յ���Ϣ�������ȴ�����
	virtual void OnMessage();


	/// \brief ���log
	/// \brief Grade ��־�ȼ�
	/// \param[in] strTgtLog ��־����
	static void Log(LogGrade Grade, const QString& strTgtLog);

	/// \brief ���ݸ����Ĳ��ʵ���ж��Ƿ�������
	/// \param[in] strPlug �������
	/// \param[in] strInstance ���ʵ������
	bool PlugIsRuning(const QString& strPlug, const QString& strInstance) const;

	/// \brief �����ź�
	/// �ź����Ϳɷ�Ϊ�������ͺ��߳����ͣ��������ͷ�������ִ�У��ȴ�ִ����ɣ��߳�����ִ��ϵͳ�̣߳����ȴ���Ĭ��Ϊ�߳�
	/// \param[in] sig �ź�����
	/// \param[in] arg �źŲ���
	/// \param[in] type �ź�����
	void SendSIG(Signal_ sig, void* arg, Signal_Type type = Signal_Type::THREAD) const;

private:
	friend class MainFrame;					///< ��Ԫ�࣬�ɷ���˽�еĺ���
	AbstractObject* m_pObjectInstance;		///< root���ڵ�
	friend class Tree_ApiFun::TreeFunction;  ///< �ڲ����ݽṹ
};

#endif

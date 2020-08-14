#ifndef  __ABSTRACTWIDGET__
#define  __ABSTRACTWIDGET__
#include "MainFrame_global.h"
#include "SignalQueue.h"

class MAINFRAME_EXPORT AbstractWidget : public QWidget
{
	Q_OBJECT
public:
	enum class LogGrade {
		SeriousError,
		Error,
		Warning,
		Tips
	};
	/// \brief ���캯��
	/// \param[in] parent ������
	AbstractWidget(QWidget* parent = 0);

	/// \brief ��������������ֹ�ڴ�й©
	virtual ~AbstractWidget();

	/// \brief ����Ϣ����
	/// �ں����ڲ�ִ��get���������յ���Ϣ�������ȴ�����
	virtual void OnMessage();

	/// \brief ���ڹر��¼�
	virtual void OnClose();

	/// \brief ���log
	/// \brief Grade ��־�ȼ�
	/// \param[in] strTgtLog ��־����
	static void Log(LogGrade Grade, const QString& strTgtLog);

	/// \brief ����ֱ��ʴ�С�����ҷ��ؼ����ĺ��ʴ�С
	/// \param[in] size ԭ���Ĵ�С
	/// \retval ���ؼ�����ֵ
	int CalculateResolution(const int& size) const;

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
	static AbstractWidget* m_pInstanceWidget;
	friend class MainFrame;
};

#endif


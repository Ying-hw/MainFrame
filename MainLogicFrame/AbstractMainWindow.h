#ifndef  __ABSTRACTMAINWINDOW__
#define  __ABSTRACTMAINWINDOW__
/*!
 * \file AbstractMainWindow.h
 *
 * \brief �ṩ���Ӳ˵������ڻ���
 *
 * \author yhw
 * \date 2021/09/12 13:11
 *
 *
 */
#include "MainFrame_global.h"
#include "SignalQueue.h"
#include <QMainWindow>
#include "AbstractObject.h"
#include <QCloseEvent>

/*!
 * \class AbstractMainWindow
 *
 * \brief �ṩ���Ӳ˵������ڻ���
 *
 * \author yhw
 * \date 2021��9��12��
 */
class MAINFRAME_EXPORT AbstractMainWindow : public QMainWindow, public AbstractObject
{
	Q_OBJECT

public:
	AbstractMainWindow(QMainWindow* parent = 0);
	~AbstractMainWindow();

	/// \brief ����Ϣ����
	/// �ں����ڲ�ִ��get���������յ���Ϣ�������ȴ�����
	virtual void OnMessage();

	/// \brief ���ڹر��¼�
	virtual void OnClose();

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

	void closeEvent(QCloseEvent* event);
	friend class MainFrame;   ///< ��Ԫ��
};

#endif

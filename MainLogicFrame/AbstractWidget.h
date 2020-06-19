#ifndef  __ABSTRACTWIDGET__
#define  __ABSTRACTWIDGET__
#include "MainFrame_global.h"


class MAINFRAME_EXPORT AbstractWidget : public QWidget
{
	Q_OBJECT
public:
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

	/// \brief ���ݸ����Ĳ��ʵ���ж��Ƿ�������
	/// \param[in] strPlug �������
	/// \param[in] strInstance ���ʵ������
	bool PlugIsRuning(const QString& strPlug, const QString& strInstance);
};

#endif


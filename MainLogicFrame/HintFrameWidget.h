#ifndef __HINTFRAMEWIDGET__
#define __HINTFRAMEWIDGET__

/*!
 * \file HintFrameWidget.h
 *
 * \brief ��ʾ���� 
 *
 * \author yhw
 * \date 2021/09/12 15:08
 *
 * 
 */

#include "MainFrame_global.h"
#include "AbstractWidget.h"
#include "AbstractNetWork.h"

/*!
 * \class HintFrameWidget
 *
 * \brief ��ʾ����
 *
 * \author yhw
 * \date 2021��9��12��
 */
class MAINFRAME_EXPORT HintFrameWidget : public QWidget
{
	Q_OBJECT
	enum class HintType{ BLOCK, NOBLOCK };  ///< ���
public:
	/// \brief ���캯��
	/// \param[in] strHint ��ʾ���� 
	/// \param[in] parent ������
	/// \param[in] type Ĭ��NOBLOCK
	HintFrameWidget(QString strHint, AbstractWidget* parent, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief ���캯��
	/// \param[in] strHint ��ʾ���� 
	/// \param[in] originPoint ��ʼλ��
	/// \param[in] parent ������
	/// \param[in] type Ĭ��NOBLOCK
	HintFrameWidget(QString strHint, QPoint originPoint, AbstractNetWork* parent, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief ���캯��
	/// \param[in] strHint ��ʾ���� 
	/// \param[in] originPoint ��ʼλ��
	/// \param[in] type Ĭ��NOBLOCK
	HintFrameWidget(QString strHint, QPoint originPoint, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief ��������
	~HintFrameWidget();

	/// \brief ��ʼ����ʾ����Ϣ
	/// \param[in] strHint ��ʾ����
	void initBlockModel(const QString& strHint);

	/// \brief ��ʼ����С
	void initSize();

	/// \brief ��ʾ�¼�
	/// �ڴ����ڴ洴���ɹ�֮����ʾ֮ǰ
	/// \param[in] event �¼�����
	void showEvent(QShowEvent *event);

	/// \brief ��ͼ�¼�
	/// �ڴ��ڱ仯��ʱ�����
	/// \param[in] event �¼�����
	void paintEvent(QPaintEvent* event);
private slots:
	/// \brief �رմ���
	void CloseHintWindow();
private:
	QString m_strHintText;  ///< ��ʾ����
	QTimer m_TimeClose;     ///< ��ʱ��
	AbstractWidget* m_ParentWidget;   ///< ������
};

#endif //__HINTFRAMEWIDGET__


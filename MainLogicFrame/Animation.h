#ifndef ANIMATION_H
#define ANIMATION_H

/*!
 * \file Animation.h
 *
 * \brief �ṩ�����Ķ���Ч��
 *
 * \author yhw
 * \date 2021/09/12 15:00
 *
 * 
 */
#include "MainFrame_global.h"

/*!
 * \class Animation
 *
 * \brief ������
 *
 * \author yhw
 * \date 2021��9��12��
 */
class MAINFRAME_EXPORT Animation : public QWidget
{
	Q_OBJECT
public:
	/// \brief ���캯��
	/// \param[in] parent ������
	Animation(QWidget *parent = 0);

	/// \brief ��������
	~Animation();

	/// \brief ������ʾ�¼����ڴ��ڴ���֮ǰ��ʾ
	/// \param[in] event �¼�����
	void showEvent(QShowEvent *event);

	/// \brief ���ö����Ƿ���ʾ
	/// \param[in] isShow Ĭ����ʾ
	void setAnimation(bool isShow = true);

	/// \brief �����¼�
	/// \param[in] event �¼�����
	void hideEvent(QHideEvent *event);

	/// \brief ���ڹر��¼�
	/// \param[in] event �¼�����
	void closeEvent(QCloseEvent *event);

	///< ��ʼ������
	void InitAanimation();
public slots:

	/// \brief �رն���
	/// \param[in] closeHide �رջ�����ʾ
	void closeAnimation(bool closeHide);
private:
	bool m_IsShow; ///< �Ƿ���ʾ����Ч��
	std::shared_ptr<QPropertyAnimation> m_Animation_Opacity, m_Animation_Geometry;   ///< ������
};


#endif // ANIMATION_H

#ifndef ANIMATION_H
#define ANIMATION_H

/*!
 * \file Animation.h
 *
 * \brief 提供基本的动画效果
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
 * \brief 动画类
 *
 * \author yhw
 * \date 2021年9月12日
 */
class MAINFRAME_EXPORT Animation : public QWidget
{
	Q_OBJECT
public:
	/// \brief 构造函数
	/// \param[in] parent 父窗口
	Animation(QWidget *parent = 0);

	/// \brief 析构函数
	~Animation();

	/// \brief 窗口显示事件，在窗口创建之前显示
	/// \param[in] event 事件参数
	void showEvent(QShowEvent *event);

	/// \brief 设置动画是否显示
	/// \param[in] isShow 默认显示
	void setAnimation(bool isShow = true);

	/// \brief 隐藏事件
	/// \param[in] event 事件参数
	void hideEvent(QHideEvent *event);

	/// \brief 窗口关闭事件
	/// \param[in] event 事件参数
	void closeEvent(QCloseEvent *event);

	///< 初始化动画
	void InitAanimation();
public slots:

	/// \brief 关闭动画
	/// \param[in] closeHide 关闭或者显示
	void closeAnimation(bool closeHide);
private:
	bool m_IsShow; ///< 是否显示动画效果
	std::shared_ptr<QPropertyAnimation> m_Animation_Opacity, m_Animation_Geometry;   ///< 动画类
};


#endif // ANIMATION_H

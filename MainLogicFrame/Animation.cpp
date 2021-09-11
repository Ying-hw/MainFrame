#include "stdafx.h"
#include "Animation.h"

Animation::Animation(QWidget *parent)
	: QWidget(parent), m_IsShow(true),
	  m_Animation_Opacity( NULL ),
	  m_Animation_Geometry( NULL )
{
	
}

Animation::~Animation()
{

}

void Animation::showEvent(QShowEvent *event) {
	if (!m_IsShow) {
		m_IsShow = true;
		return;
	}
	QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
	if (m_Animation_Opacity->startValue() == 1)
	{
		m_Animation_Opacity->setStartValue(0);
		m_Animation_Opacity->setEndValue(1);
	}
	group->addAnimation(&*m_Animation_Opacity);
	group->addAnimation(&*m_Animation_Geometry);
	group->start();
	QWidget::showEvent(event);
}

void Animation::setAnimation(bool isShow /*= true*/)
{
	m_IsShow = isShow;
}

void Animation::hideEvent(QHideEvent *event)
{
	closeAnimation(false);
	QWidget::hideEvent(event);
}

void Animation::closeEvent(QCloseEvent *event)
{
	closeAnimation(true);
}

void Animation::closeAnimation(bool closeHide) {
	if (!m_IsShow) {
		m_IsShow = true;
		return;
	}
	m_Animation_Opacity->setDuration(700);
	m_Animation_Opacity->setStartValue(1);
	m_Animation_Opacity->setEndValue(0);
	m_Animation_Opacity->setEasingCurve(QEasingCurve::Linear);
	m_Animation_Opacity->start();
}

void Animation::InitAanimation()
{
	if (!m_Animation_Opacity.get() && !m_Animation_Geometry.get())
	{
		m_Animation_Opacity = std::make_shared<QPropertyAnimation>(new QPropertyAnimation(this, "windowOpacity"));
		m_Animation_Geometry = std::make_shared<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));
	}

	m_Animation_Opacity->setDuration(700);
	m_Animation_Opacity->setStartValue(0);
	m_Animation_Opacity->setEndValue(1);
	m_Animation_Opacity->setEasingCurve(QEasingCurve::Linear);

	QDesktopWidget deskTop;
	m_Animation_Geometry->setDuration(200);
	m_Animation_Geometry->setStartValue(QRect(0, 0, deskTop.width(), deskTop.height()));
	m_Animation_Geometry->setEndValue(this->geometry());
}

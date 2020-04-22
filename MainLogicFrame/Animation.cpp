#include "stdafx.h"
#include "Animation.h"
#include "MainWidget.h"

Animation::Animation(QWidget *parent)
	: QWidget(parent), m_IsShow(true),
	  m_Animation_Opacity( NULL ),
	  m_Animation_Geometry( NULL )
{
	
}

Animation::~Animation()
{
	delete m_Animation_Opacity;
	delete m_Animation_Geometry;
	m_Animation_Geometry = NULL;
	m_Animation_Opacity = NULL;
}

void Animation::showEvent(QShowEvent *event) {
	if (!m_IsShow)
		return;

	QParallelAnimationGroup* group = new QParallelAnimationGroup(this);;
	group->addAnimation(m_Animation_Opacity);
	group->addAnimation(m_Animation_Geometry);
	group->start();

	QGraphicsDropShadowEffect *m_pShadow = new QGraphicsDropShadowEffect(MainWidget::staticThis);
	m_pShadow->setColor(QColor(0, 0, 0, 85));
	m_pShadow->setBlurRadius(10);
	m_pShadow->setOffset(4, 4);
	MainWidget::staticThis->setGraphicsEffect(m_pShadow);
	//m_IsShow = false;
}

void Animation::setAnimation(bool isShow /*= true*/)
{
	m_IsShow = isShow;
}

void Animation::hideEvent(QHideEvent *event)
{
	//closeAnimation(false);
}

void Animation::closeAnimation(bool closeHide) {
	if (!m_IsShow)
		return;
 	if (closeHide) 
 		connect(m_Animation_Opacity, &QPropertyAnimation::finished, this, &QWidget::close);
 	else 
 		connect(m_Animation_Opacity, &QPropertyAnimation::finished, MainWidget::staticThis, &QWidget::hide);
	m_Animation_Opacity->start();
	//m_IsShow = false;
}

void Animation::InitAanimation()
{
	if (m_Animation_Opacity) {
		delete m_Animation_Opacity;
		m_Animation_Opacity = NULL;
	}
	if (m_Animation_Geometry) {
		delete m_Animation_Geometry;
		m_Animation_Geometry = NULL;
	}
	m_Animation_Opacity = new QPropertyAnimation(MainWidget::staticThis, "windowOpacity");
	m_Animation_Geometry = new QPropertyAnimation(MainWidget::staticThis, "geometry");

	m_Animation_Opacity->setDuration(400);
	m_Animation_Opacity->setStartValue(0);
	m_Animation_Opacity->setEndValue(1);
	m_Animation_Opacity->setEasingCurve(QEasingCurve::Linear);

	QDesktopWidget deskTop;
	m_Animation_Geometry->setDuration(300);
	m_Animation_Geometry->setStartValue(QRect(0, 0, deskTop.width(), deskTop.height()));
	m_Animation_Geometry->setEndValue(MainWidget::staticThis->geometry());
}

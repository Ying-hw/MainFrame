#include "stdafx.h"
#include "HintFrameWidget.h"

HintFrameWidget::HintFrameWidget(QString strHint, QPoint originPoint, QObject* parent) : m_strHintText(strHint)
{
	setWindowFlags(Qt::FramelessWindowHint);
	initSize(originPoint);
}

void HintFrameWidget::initSize(const QPoint& originPoint)
{
	QFont f;
	f.setFamily("Microsoft YaHei");
	f.setPixelSize(18);  //此处应该根据系统的分辨率调整
	QFontMetrics fm(f);
	QRect re = fm.boundingRect(m_strHintText);
	re.setTopLeft(originPoint);
	re.setSize(QSize(re.width(), re.height() * 3));
	this->setGeometry(re);
}

void HintFrameWidget::showEvent(QShowEvent *event)
{
	QPropertyAnimation* showOpacity = new QPropertyAnimation(this, "windowOpacity");   
	showOpacity->setDuration(1000);
	showOpacity->setStartValue(0);
	showOpacity->setEndValue(1);
	showOpacity->setEasingCurve(QEasingCurve::Linear);
	showOpacity->start();

	QPainter paint(this);
	paint.drawText(rect(), Qt::AlignCenter, m_strHintText);
	QWidget::showEvent(event);
}

void HintFrameWidget::closeEvent(QCloseEvent *event)
{
	QPropertyAnimation* closeOpacity = new QPropertyAnimation(this, "windowOpacity");
	closeOpacity->setDuration(1000);
	closeOpacity->setStartValue(1);
	closeOpacity->setEndValue(0);
	closeOpacity->setEasingCurve(QEasingCurve::Linear);
	closeOpacity->start();
	QWidget::closeEvent(event);
}


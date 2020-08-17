#include "stdafx.h"
#include "HintFrameWidget.h"

extern SignalQueue* g_pSignal;

HintFrameWidget::HintFrameWidget(QString strHint, AbstractWidget* parent) : m_strHintText(strHint), m_ParentWidget(parent), QWidget(parent)
{
	//setStyleSheet("background-color: black;");
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	initSize();
	connect(&m_TimeClose, &QTimer::timeout, this, &HintFrameWidget::CloseHintWindow);
	m_TimeClose.start(3000);
}

HintFrameWidget::HintFrameWidget(QString strHint, QPoint originPoint, AbstractNetWork* parent) : m_strHintText(strHint)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	QFont f;
	f.setFamily("Microsoft YaHei");
	f.setPixelSize(18);  //此处应该根据系统的分辨率调整
	QFontMetrics fm(f);
	QRect re = fm.boundingRect(m_strHintText);
	re.setSize(QSize(re.width(), re.height() * 3));
	re.moveCenter(originPoint);
	this->setGeometry(re);
}

HintFrameWidget::HintFrameWidget(QString strHint, QPoint originPoint) : m_strHintText(strHint)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	QFont f;
	f.setFamily("Microsoft YaHei");
	f.setPixelSize(18);  //此处应该根据系统的分辨率调整
	QFontMetrics fm(f);
	QRect re = fm.boundingRect(m_strHintText);
	re.setSize(QSize(re.width(), re.height() * 3));
	re.moveCenter(originPoint);
	this->setGeometry(re);
}

HintFrameWidget::~HintFrameWidget()
{
	
}

void HintFrameWidget::initSize()
{
	QFont f;
	f.setFamily("Microsoft YaHei");
	f.setPixelSize(14);  //此处应该根据系统的分辨率调整
	QFontMetrics fm(f);
	QRect re = fm.boundingRect(m_strHintText);
	QPoint cen = m_ParentWidget->rect().center();
	cen.setX(m_ParentWidget->rect().center().x() - re.width());
	re.moveTo(cen);
	re.setWidth(re.width() * 3);
	re.setHeight(re.height() * 2);
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
	QWidget::showEvent(event);
}

void HintFrameWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint;
	paint.begin(this);
	QColor colors = QColor(Qt::GlobalColor::white);
	QPen pen(colors);
	paint.setPen(pen);
	paint.drawText(rect(), Qt::AlignCenter, m_strHintText);
	paint.setBrush(QBrush(QColor(1,1,1,180)));
	paint.drawRect(rect());

	paint.end();
	QWidget::paintEvent(event);
}

void HintFrameWidget::CloseHintWindow()
{
	QPropertyAnimation* closeOpacity = new QPropertyAnimation(this, "windowOpacity");
	closeOpacity->setDuration(1000);
	closeOpacity->setStartValue(1);
	closeOpacity->setEndValue(0);
	closeOpacity->setEasingCurve(QEasingCurve::Linear);
	closeOpacity->start();
	close();
}


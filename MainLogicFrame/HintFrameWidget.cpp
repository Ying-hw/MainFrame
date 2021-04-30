#include "stdafx.h"
#include "HintFrameWidget.h"
#include <QProgressBar>

extern SignalQueue* g_pSignal;

HintFrameWidget::HintFrameWidget(QString strHint, AbstractWidget* parent, HintFrameWidget::HintType type/* = HintType::NOBLOCK*/) : m_strHintText(strHint), m_ParentWidget(parent), QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	if (type == HintType::BLOCK) {
		initBlockModel(strHint);
		return;
	}
	connect(&m_TimeClose, &QTimer::timeout, this, &HintFrameWidget::CloseHintWindow);
	m_TimeClose.start(3000);
	initSize();
}

HintFrameWidget::HintFrameWidget(QString strHint, QPoint originPoint, AbstractNetWork* parent, HintFrameWidget::HintType type /*= HintType::NOBLOCK*/) : m_strHintText(strHint)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	if (type == HintType::BLOCK) {
		initBlockModel(strHint);
		return;
	}
	QFont f;
	f.setFamily("Microsoft YaHei");
	f.setPixelSize(18);  //此处应该根据系统的分辨率调整
	QFontMetrics fm(f);
	QRect re = fm.boundingRect(m_strHintText);
	re.setSize(QSize(re.width(), re.height() * 3));
	re.moveCenter(originPoint);
	this->setGeometry(re);
}

HintFrameWidget::HintFrameWidget(QString strHint, QPoint originPoint, HintFrameWidget::HintType type/* = HintType::NOBLOCK*/) : m_strHintText(strHint)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	if (type == HintType::BLOCK) {
		initBlockModel(strHint);
		return;
	}
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

void HintFrameWidget::initBlockModel(const QString& strHint)
{
	setObjectName("HintFrameWidget");

	QGridLayout* lay = new QGridLayout();
	QLabel* lab = new QLabel(strHint, this);
	QProgressBar* bar = new QProgressBar(this);
	bar->setRange(0, 0);
						
	QSpacerItem* It = new QSpacerItem(40, 20);
	QSpacerItem* Its = new QSpacerItem(40, 20);
	QHBoxLayout* hBox = new QHBoxLayout(); 
	
	hBox->addItem(It);
	hBox->addWidget(lab);
	hBox->addItem(Its);

	lay->addLayout(hBox, 0, 0);
	lay->addWidget(bar);
	this->setLayout(lay);
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


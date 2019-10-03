#include "stdafx.h"
#include "MainWidget.h"

QRect g_Rects;

void TitleBar::retranslateUi() {
	this->setWindowTitle(QString::fromLocal8Bit("登录"));
	labPicture->setText( QString::fromLocal8Bit("测试"));
	labText->setText( QString::fromLocal8Bit("测试"));
	BtnMin->setText( QString::fromLocal8Bit("缩小"));
	BtnClose->setText( QString::fromLocal8Bit("关闭"));
}

void TitleBar::setMain(QWidget* pMain) {
	m_pWidget = pMain;
	m_pWidget->setStyleSheet("background-color:rgb(20,225,205);");
	setStyleSheet("background-color:rgb(20,225,205);");
	setInitUi(); 
	retranslateUi(); 
	update();
	repaint();
	show();
}

void TitleBar::mousePressEvent(QMouseEvent* event) {
	QPoint point = event->pos();
	if (pHbLayout->contentsRect().contains(point)) {
		m_point = event->pos();
		m_isPress = true;
	}
	else
		m_isPress = false;
}

void TitleBar::mouseMoveEvent(QMouseEvent* event) {
	if (m_isPress) {
		for (int i = 0; i < 10000; i++);
		move(mapToGlobal(event->pos() - m_point));
	}
}

void TitleBar::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleBar::setInitUi() {
	setWindowFlags(Qt::FramelessWindowHint);
	this->resize(g_Rects.width(), g_Rects.height());
	gridLayout_2 = new QGridLayout(this);

	pHbLayout = new QHBoxLayout(this);

	labPicture = new QLabel(this);
	pHbLayout->addWidget(labPicture);

	labText = new QLabel(this);
	pHbLayout->addWidget(labText);

	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	pHbLayout->addItem(horizontalSpacer);

	BtnMin = new QPushButton(this);
	pHbLayout->addWidget(BtnMin);

	BtnClose = new QPushButton(this);
	pHbLayout->addWidget(BtnClose);

	pHbLayout->setContentsMargins(QMargins(0,0,0,10));
	
	m_pWidget->setParent(this);
	m_pWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	gridLayout_2->addLayout(pHbLayout, 0, 0);
	gridLayout_2->addWidget(m_pWidget, 1, 0);
	gridLayout_2->setContentsMargins(QMargins(0,0,0,0));
	gridLayout_2->setSpacing(0);
	setLayout(gridLayout_2);
}

MainWidget::MainWidget(QWidget* widget):
QWidget(widget), m_pBar(new TitleBar(widget))
{
	SetQApplicationQss();
	SetInitSize();
}


MainWidget::~MainWidget()
{
}

void MainWidget::SetInitSize() {
	this->setGeometry(g_Rects.x(), g_Rects.y(), g_Rects.height(), g_Rects.width());
	resize(g_Rects.height(), g_Rects.width());
}

void MainWidget::SetQApplicationQss() {
	QFile QssFile(QString(CONFIG) + "DefaultQss.qss");
	if (!QssFile.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告！"),
			QString::fromLocal8Bit("读取配置文件失败！！！"), QMessageBox::Ok);
		return;
	}
	else {
		m_strQssConfig = QssFile.readAll();
		qApp->setStyleSheet(m_strQssConfig);
	}
}
void MainWidget::showEvent(QShowEvent* event) {
	this->resize(g_Rects.height(), g_Rects.width());
}

void MainWidget::closeWindow() {
	if (QMessageBox::information(this, QString::fromLocal8Bit("是否退出！"),
		QString::fromLocal8Bit("客官真的要退出吗！"), QMessageBox::Yes | QMessageBox::No)
		== QMessageBox::No) {
		return;
	}
	else
		close();
}

void MainWidget::CloseWind(MainWidget* pthis) {
	pthis->closeWindow();
}

void MainWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	p.begin(this);
	QLine line;
	line.setPoints(QPoint(0, 0), QPoint(10000, 0));
	QPen pen(Qt::black);
	p.setPen(pen);
	p.drawLine(line);
	p.end();
}




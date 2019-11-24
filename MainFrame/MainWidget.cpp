#include "stdafx.h"
#include "MainWidget.h"

QRect g_Rects;


void MainWidget::setMain(QWidget* pMain) {
	m_pWidget = pMain;
	Set_Qss();
	setInitUi(g_Rects); 
	g_pSignal->SetUserIdentify(this, User::MAINWIDGET);
	this->setWindowIcon(QIcon(QString(IMAGE) + "Titlepicture.JPG"));
	show();
}

void MainWidget::setMain(QWidget* pMain, const QRect& rect)
{
	m_pWidget = pMain;
	this->resize(rect.width(), rect.height());
	gridLayout_2->addWidget(m_pWidget, 1, 0);
	gridLayout_2->setContentsMargins(QMargins(0, 0, 0, 0));
	gridLayout_2->setSpacing(0);
	setLayout(gridLayout_2);
	this->setWindowIcon(QIcon(QString(IMAGE) + "Titlepicture.JPG"));
	show();
}

void MainWidget::mousePressEvent(QMouseEvent* event) {
	QPoint point = event->pos();
	if (pHbLayout->contentsRect().contains(point)) {
		m_point = point;
		m_isPress = true;
	}
	else
		m_isPress = false;
}

void MainWidget::paintEvent(QPaintEvent* event) {
	QLine* lines = new QLine;
	lines->setP1(QPoint(0, 10));
	lines->setP2(QPoint(700, 10));
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);

	p.setPen(QColor( 20, 100, 100));
	p.drawLine(*lines);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWidget::mouseMoveEvent(QMouseEvent* event) {
	if (m_isPress) 
		move(mapToGlobal(event->pos() - m_point));
}

void MainWidget::Set_Qss() {
	setWindowFlags(Qt::FramelessWindowHint);
	QFile QssFile(QString(CONFIG) + "DefaultQss9.qss");
	if (!QssFile.open(QIODevice::ReadOnly))
		QMessageBox::warning(this, QString::fromLocal8Bit("警告！"),
			QString::fromLocal8Bit("读取配置文件失败！！！"), QMessageBox::Ok);
	else {
		m_strQssConfig = QssFile.readAll();
		qApp->setStyleSheet(m_strQssConfig);
		QssFile.close();
	}
}

void MainWidget::closeWindow() {
	if (QMessageBox::information(this, QString::fromLocal8Bit("是否退出！"),
		QString::fromLocal8Bit("客官真的要退出吗！"),
		QMessageBox::Yes | QMessageBox::No)
		== QMessageBox::No)
		return;
	else
		close();
}

void MainWidget::minWindow() {
	this->showMinimized();
}

void MainWidget::setInitUi(const QRect& rect) {
	if (BtnClose != NULL)
	{
		BtnClose->close();
		BtnMin->close();
		BtnSet->close();
	}
	
	qDebug() << rect.width();
	qDebug() << rect.height();

	this->resize(rect.width(), rect.height());
	gridLayout_2 = new QGridLayout(this);

	pHbLayout = new QHBoxLayout(this);

	BtnPicture = new QPushButton(this);
	BtnPicture->setIcon(QIcon(QString(IMAGE) + "Titlepicture.JPG"));
	BtnPicture->setIconSize(QSize(45, 45));
	BtnPicture->setStyleSheet("QPushButton{border:none;}");
	pHbLayout->addWidget(BtnPicture);

	QPushButton *BtnText = new QPushButton(this);
	BtnText->setText("QQ");
	BtnText->setObjectName("BtnText");
	BtnText->setFlat(true);
	BtnText->setEnabled(false);
	pHbLayout->addWidget(BtnText);

	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20,
		QSizePolicy::Expanding, QSizePolicy::Minimum);
	pHbLayout->addItem(horizontalSpacer);

	BtnSet = new QPushButton(this);
	BtnSet->setIcon(QIcon(QString(IMAGE) + "Gear.png"));
	BtnSet->setFlat(true);
	pHbLayout->addWidget(BtnSet);

	BtnMin = new QPushButton(this);
	BtnMin->setIcon(QIcon(QString(IMAGE) + "Minus.png"));
	BtnMin->setFlat(true);
	pHbLayout->addWidget(BtnMin); 

	BtnClose = new QPushButton(this);
	BtnClose->setIcon(QIcon(QString(IMAGE) + "Standby.png"));
	connect(BtnClose, &QPushButton::clicked, [](){
		SignalQueue::Send_Message(Signal_::WINDOWCLOSE, nullptr);
	});
	connect(BtnMin, &QPushButton::clicked, [](){
		SignalQueue::Send_Message(Signal_::WINDOWMIN, nullptr);
	});

	BtnClose->setFlat(true);
	pHbLayout->addWidget(BtnClose);

	BtnClose->setObjectName("BtnClose");
	BtnMin->setObjectName("BtnMin");
	BtnSet->setObjectName("BtnSet");
	BtnClose->show();
	BtnMin->show();
	BtnSet->show();

	if (m_pWidget->windowTitle() != "LoginSystem") BtnSet->hide();
	
	m_pWidget->setParent(this);
	m_pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	gridLayout_2->addLayout(pHbLayout, 0, 0);
	gridLayout_2->addWidget(m_pWidget, 1, 0);
	gridLayout_2->setContentsMargins(QMargins(0,0,0,0));
	gridLayout_2->setSpacing(0);


	setLayout(gridLayout_2);
}






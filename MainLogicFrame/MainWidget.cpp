#include "stdafx.h"
#include "MainWidget.h"
#include "Animation.h"

void MainWidget::setMain(QWidget* pMain, const QRect& rect, const QString& strTitle) {
	m_pWidget = pMain;
	connect(g_pSignal, SIGNAL(close_Window(bool)), static_cast<Animation*>(m_pWidget), SLOT(closeAnimation(bool)));
	if (BtnSet)
		if (m_pWidget->windowTitle() != "LoginSystem") BtnSet->hide();
		else BtnSet->show();
	Set_Qss();
	setInitUi(rect);
	g_pSignal->SetUserIdentify(this, User::MAINWIDGET);
	this->setWindowTitle(strTitle.toLocal8Bit().data());
	this->setWindowIcon(QIcon(QString(IMAGE) + "Titlepicture.JPG"));
	show();
	TDragProxy*	mpDragProxy = new TDragProxy(this);
	mpDragProxy->SetBorderWidth(5, 5, 5, 5);
}

void MainWidget::setMain(QWidget* pMain, const QRect& rect)
{
	m_pWidget = pMain;
	if (BtnSet)
		if (m_pWidget->windowTitle() != "LoginSystem") BtnSet->hide();
		else BtnSet->show();
	!rect.isValid() ? this->setGeometry(pMain->geometry()) : this->setGeometry(rect);
	m_pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	gridLayout_2->addWidget(m_pWidget, 1, 0);
	gridLayout_2->setContentsMargins(QMargins(3, 0, 3, 3));
	gridLayout_2->setSpacing(0);
	setLayout(gridLayout_2);
	this->setWindowTitle(pMain->windowTitle());
	this->setWindowIcon(QIcon(QString(IMAGE) + "Titlepicture.JPG"));
	show();
}

void MainWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWidget::Set_Qss() {
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint 
		| Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	
	QFile QssFile(QString(CONFIG) + "DefaultQss.qss");
	if (!QssFile.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告！"),
			QssFile.errorString(), QMessageBox::Ok);
	}
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
	else {
		close();
		g_pSignal->DeleteAll();
	}
}

MainWidget::MainWidget(QWidget *ject /*= 0*/) : QWidget(ject), m_pWidget(nullptr), m_isTopMenuPress(false), 
		BtnClose(NULL), BtnSet(NULL), 
		m_isPress(false), glx(MAXINT32) {
}

void MainWidget::setInitUi(const QRect& rect) {
	if (BtnClose != NULL) {
		BtnClose->close();
		BtnMin->close();
		BtnSet->close();
	}
	rect.isValid() ? this->setGeometry(rect) : this->setGeometry(m_pWidget->geometry());
 	m_pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 	//setStyleSheet("color:rgb(30,100,100);");

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
	else BtnSet->show();
	gridLayout_2->addLayout(pHbLayout, 0, 0);
	gridLayout_2->addWidget(m_pWidget, 1, 0);
	gridLayout_2->setContentsMargins(QMargins(3,0,3,3));
	setLayout(gridLayout_2);
}



TDragProxy::TDragProxy(QWidget *parent)
	:QObject((QObject*)parent), m_bDragEnable(true), m_bBorderMini(false)
{
	m_proxyWidget = parent;
	m_cursorTimerId = m_top = m_right = m_bottom = m_left = 0;

	m_proxyWidget->setMouseTracking(true);
	m_proxyWidget->installEventFilter(this);    // 代理窗体事件

	m_mousePressed = false;
	m_regionPressed = Unknown;
}

TDragProxy::~TDragProxy()
{
}

void TDragProxy::SetBorderWidth(int top, int right, int bottom, int left)
{
	m_top = top;
	m_right = right;
	m_bottom = bottom;
	m_left = left;

	if (m_top == 1 && m_right == 1 && m_bottom == 1 && m_left == 1)
		m_bBorderMini = true;
	MakeRegions();
}

void TDragProxy::UpdateGeometry(int x, int y, int w, int h)
{
	int minWidth = m_proxyWidget->minimumWidth();
	int minHeight = m_proxyWidget->minimumHeight();
	int maxWidth = m_proxyWidget->maximumWidth();
	int maxHeight = m_proxyWidget->maximumHeight();

	if (w < minWidth || w > maxWidth || h < minHeight || h > maxHeight)
		return;
	m_proxyWidget->setGeometry(x, y, w, h);
}

bool TDragProxy::eventFilter(QObject* obj, QEvent* event)
{
	if (!m_bDragEnable)
		return QObject::eventFilter(obj, event);
	QEvent::Type eventType = event->type();
	if (eventType == QEvent::MouseMove) {
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		QPoint curPosLocal = mouseEvent->pos();
		TDragProxy::WidgetRegion regionType = HitTest(curPosLocal);
		QPoint curPosGlobal = m_proxyWidget->mapToGlobal(curPosLocal);

		if (!m_mousePressed) {   // 鼠标未按下
			switch (regionType)
			{
			case Top:
			case Bottom:
				m_proxyWidget->setCursor(Qt::SizeVerCursor);
				break;
			case TopRight:
			case LeftBottom:
				m_proxyWidget->setCursor(Qt::SizeBDiagCursor);
				break;
			case Right:
			case Left:
				m_proxyWidget->setCursor(Qt::SizeHorCursor);
				break;
			case RightBottom:
			case LeftTop:
				m_proxyWidget->setCursor(Qt::SizeFDiagCursor);
				break;
			default:
				m_proxyWidget->setCursor(Qt::ArrowCursor);
				break;
			}

			StartCursorTimer();
		}
		else    // 鼠标已按下
			if (m_regionPressed == Inner)
				m_proxyWidget->move(m_originGeo.topLeft() + curPosGlobal - m_originPosGlobal);
			else if (m_regionPressed == Top) {
				int dY = curPosGlobal.y() - m_originPosGlobal.y();
				UpdateGeometry(m_originGeo.x(), m_originGeo.y() + dY, m_originGeo.width(), m_originGeo.height() - dY);
			}
			else if (m_regionPressed == TopRight) {
				QPoint dXY = curPosGlobal - m_originPosGlobal;
				UpdateGeometry(m_originGeo.x(), m_originGeo.y() + dXY.y(), m_originGeo.width() + dXY.x(), m_originGeo.height() - dXY.y());
			}
			else if (m_regionPressed == Right) {
				int dX = curPosGlobal.x() - m_originPosGlobal.x();
				UpdateGeometry(m_originGeo.x(), m_originGeo.y(), m_originGeo.width() + dX, m_originGeo.height());
			}
			else if (m_regionPressed == RightBottom) {
				QPoint dXY = curPosGlobal - m_originPosGlobal;
				UpdateGeometry(m_originGeo.x(), m_originGeo.y(), m_originGeo.width() + dXY.x(), m_originGeo.height() + dXY.y());
			}
			else if (m_regionPressed == Bottom) {
				int dY = curPosGlobal.y() - m_originPosGlobal.y();
				UpdateGeometry(m_originGeo.x(), m_originGeo.y(), m_originGeo.width(), m_originGeo.height() + dY);
			}
			else if (m_regionPressed == LeftBottom) {
				QPoint dXY = curPosGlobal - m_originPosGlobal;
				UpdateGeometry(m_originGeo.x() + dXY.x(), m_originGeo.y(), m_originGeo.width() - dXY.x(), m_originGeo.height() + dXY.y());
			}
			else if (m_regionPressed == Left) {
				int dX = curPosGlobal.x() - m_originPosGlobal.x();
				UpdateGeometry(m_originGeo.x() + dX, m_originGeo.y(), m_originGeo.width() - dX, m_originGeo.height());
			}
			else if (m_regionPressed == LeftTop) {
				QPoint dXY = curPosGlobal - m_originPosGlobal;
				UpdateGeometry(m_originGeo.x() + dXY.x(), m_originGeo.y() + dXY.y(), m_originGeo.width() - dXY.x(), m_originGeo.height() - dXY.y());
			}
	}
	else if (eventType == QEvent::MouseButtonPress) {
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if (mouseEvent->button() == Qt::LeftButton) {
			m_mousePressed = true;

			QPoint curPos = mouseEvent->pos();
			m_regionPressed = HitTest(curPos);

			m_originPosGlobal = m_proxyWidget->mapToGlobal(curPos);
			m_originGeo = m_proxyWidget->geometry();

			StopCursorTimer();
		}
	}
	else if (eventType == QEvent::MouseButtonRelease) {
		m_mousePressed = false;
		m_regionPressed = Unknown;

		m_proxyWidget->setCursor(Qt::ArrowCursor);
	}
	else if (eventType == QEvent::Resize) 
		MakeRegions();
	else if (eventType == QEvent::Leave) {
		m_proxyWidget->setCursor(Qt::ArrowCursor);
		StopCursorTimer();
	}
	else if (eventType == QEvent::Timer) {
		QTimerEvent* timerEvent = (QTimerEvent*)event;
		if (timerEvent->timerId() == m_cursorTimerId)
			if (m_regions[Inner].contains(m_proxyWidget->mapFromGlobal(QCursor::pos()))) {
				m_proxyWidget->setCursor(Qt::ArrowCursor);
				StopCursorTimer();
			}
	}
	return QObject::eventFilter(obj, event);
}

void TDragProxy::StartCursorTimer()
{
	StopCursorTimer();
	m_cursorTimerId = m_proxyWidget->startTimer(50);
}

void TDragProxy::StopCursorTimer()
{
	if (m_cursorTimerId != 0) {
		m_proxyWidget->killTimer(m_cursorTimerId);
		m_cursorTimerId = 0;
	}
}

void TDragProxy::MakeRegions()
{
	int width = m_proxyWidget->width();
	int height = m_proxyWidget->height();

	if (m_bBorderMini) {
		m_regions[Top] = QRect(2, 0, width - 4, 1);
		m_regions[TopRight] = QRect(width - 2, 0, 2, 2);
		m_regions[Right] = QRect(width - 1, 2, 1, height - 4);
		m_regions[RightBottom] = QRect(width - 2, height - 2, 2, 2);
		m_regions[Bottom] = QRect(2, height - 1, width - 4, 1);
		m_regions[LeftBottom] = QRect(0, height - 2, 2, 2);
		m_regions[Left] = QRect(0, 2, 1, height - 4);
		m_regions[LeftTop] = QRect(0, 0, 2, 2);
		m_regions[Inner] = QRect(2, 2, width - 4, height - 4);
	}
	else {
		m_regions[Top] = QRect(m_left, 0, width - m_left - m_right, m_top);
		m_regions[TopRight] = QRect(width - m_right, 0, m_right, m_top);
		m_regions[Right] = QRect(width - m_right, m_top, m_right, height - m_top - m_bottom);
		m_regions[RightBottom] = QRect(width - m_right, height - m_bottom, m_right, m_bottom);
		m_regions[Bottom] = QRect(m_left, height - m_bottom, width - m_left - m_right, m_bottom);
		m_regions[LeftBottom] = QRect(0, height - m_bottom, m_left, m_bottom);
		m_regions[Left] = QRect(0, m_top, m_left, height - m_top - m_bottom);
		m_regions[LeftTop] = QRect(0, 0, m_left, m_top);
		m_regions[Inner] = QRect(m_left, m_top, width - m_left - m_right, height - m_top - m_bottom);
	}
}

TDragProxy::WidgetRegion TDragProxy::HitTest(const QPoint& pos)
{
	for (int i = 0; i < 9; i++) {
		const QRect rect = m_regions[i];
		if (rect.contains(pos))
			return TDragProxy::WidgetRegion(i);
	}
	return Unknown;
}

void TDragProxy::SetDragEnable(bool bEnable)
{
	m_bDragEnable = bEnable;
}




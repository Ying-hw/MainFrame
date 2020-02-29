#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"
#include "MainFrame_global.h"
// #include <QChar>
// #define PADDING 2
// enum Direction { UP = 0, DOWN = 1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

extern SignalQueue* g_pSignal;

// class FrameWidget : public QWidget {
// public:
// 	FrameWidget(QWidget*parent = 0):QWidget(parent) {
// 		this->setMouseTracking(true);
// 		installEventFilter(this);
// 	}
// 	bool eventFilter(QObject *watched, QEvent *event) {
// 		if (watched == this && event->type() == QEvent::Enter) {
// 				QCursor cur;
// 				this->setCursor(cur);
// 		}
// 		return QWidget::eventFilter(watched, event);
// 	}
// };

class MAINFRAME_EXPORT MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *ject = 0);

	void setInitUi(const QRect &rect);
	void setMain(QWidget* pMain, const QRect& rect, const QString& strTitle);
// 	void mousePressEvent(QMouseEvent* event);
// 	void mouseMoveEvent(QMouseEvent* event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void Set_Qss();
	void paintEvent(QPaintEvent* event);
	void region(const QPoint &cursorGlobalPoint);

public:
	static MainWidget* staticThis;
	QWidget* m_pWidget;
	//FrameWidget* m_pFrameWidget;

public slots: 
	void closeWindow();
	void setMain(QWidget* pMain, const QRect& rect);
private:
	QString m_strQssConfig;
	QPoint m_point;
	bool m_isTopMenuPress;
	bool m_isPress;
	QGridLayout *gridLayout_2;
	//QGridLayout *frameGrid;
	QHBoxLayout *pHbLayout;
	QPushButton *BtnPicture;
	QPushButton *BtnSet;
	QPushButton *BtnMin;
	QPushButton *BtnClose;

// 	bool isLeftPressDown;  // 判断左键是否按下
// 	QPoint dragPosition;   // 窗口移动拖动时需要记住的点 
// 	QChar::Direction dir;        // 窗口大小改变时，记录改变方向

	int glx;
	int px;
	int py;
	int pxx;
	int pyy;
	int hx;
	int hy;
	int kx;
	int ky;
};

class TDragProxy :public QObject
{
	Q_OBJECT

public:
	TDragProxy(QWidget* parent);
	~TDragProxy();

protected:
	enum WidgetRegion
	{
		Top = 0,
		TopRight,
		Right,
		RightBottom,
		Bottom,
		LeftBottom,
		Left,
		LeftTop,
		Inner,
		Unknown
	};

public:
	void SetBorderWidth(int top, int right, int bottom, int left);//设置四周边框宽度
	void SetDragEnable(bool bEnable); //最大化无拖拽效果

protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);

	void MakeRegions();
	WidgetRegion HitTest(const QPoint& pos);
	void UpdateGeometry(int x, int y, int w, int h);

	//鼠标从边框快速移到窗体内子控件上，可能会造成鼠标样式未改变，这里使用计时器监控
	void StartCursorTimer();
	void StopCursorTimer();

private:
	QWidget* m_proxyWidget; //代理的窗体
	int m_top, m_right, m_bottom, m_left; //四周宽度
	QRect m_regions[9]; //九宫格，对应9个区域

	QPoint m_originPosGlobal; //拖拽前鼠标位置
	QRect m_originGeo; //拖拽前窗体位置和大小

	bool m_mousePressed; //鼠标是否按下
	WidgetRegion m_regionPressed; //记录鼠标按下时所点击的区域

	int m_cursorTimerId;

	bool m_bDragEnable;
	bool m_bBorderMini; //边框为1时，处理四对角位置
};


#endif //MAINWIDGET


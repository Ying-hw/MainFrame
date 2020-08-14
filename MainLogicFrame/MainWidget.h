#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"
#include "MainFrame_global.h"
#include "Animation.h"
#include "AbstractWidget.h"

extern SignalQueue* g_pSignal;

class MAINFRAME_EXPORT MainWidget : public Animation {
	Q_OBJECT
public:
	MainWidget(QWidget *ject = 0);
	~MainWidget();
	void setInitUi(const QRect &rect);
	void setMain(AbstractWidget* pMain, const QRect& rect, const QString& strTitle);
	void Set_Qss();
	void paintEvent(QPaintEvent* event);
	AbstractWidget* GetInstance();

public slots: 
	void closeWindow();
	void showMin();
private:
	AbstractWidget* m_pWidget;
	QString m_strQssConfig;
	QGridLayout *gridLayout_2;
	QHBoxLayout *pHbLayout;
	QPushButton *BtnPicture;
	QPushButton *BtnMin;
	QPushButton *BtnClose;
	SignalQueue* m_pSigQueue;
	friend class MainFrame;
};

class TDragProxy :public QObject
{
	Q_OBJECT

public:
	TDragProxy(QWidget* parent);
	~TDragProxy();
	enum WidgetRegion{Top,TopRight,Right,RightBottom,Bottom,LeftBottom,Left,LeftTop,Inner,Unknown};

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


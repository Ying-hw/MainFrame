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

// 	bool isLeftPressDown;  // �ж�����Ƿ���
// 	QPoint dragPosition;   // �����ƶ��϶�ʱ��Ҫ��ס�ĵ� 
// 	QChar::Direction dir;        // ���ڴ�С�ı�ʱ����¼�ı䷽��

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
	void SetBorderWidth(int top, int right, int bottom, int left);//�������ܱ߿���
	void SetDragEnable(bool bEnable); //�������קЧ��

protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);

	void MakeRegions();
	WidgetRegion HitTest(const QPoint& pos);
	void UpdateGeometry(int x, int y, int w, int h);

	//���ӱ߿�����Ƶ��������ӿؼ��ϣ����ܻ���������ʽδ�ı䣬����ʹ�ü�ʱ�����
	void StartCursorTimer();
	void StopCursorTimer();

private:
	QWidget* m_proxyWidget; //����Ĵ���
	int m_top, m_right, m_bottom, m_left; //���ܿ��
	QRect m_regions[9]; //�Ź��񣬶�Ӧ9������

	QPoint m_originPosGlobal; //��קǰ���λ��
	QRect m_originGeo; //��קǰ����λ�úʹ�С

	bool m_mousePressed; //����Ƿ���
	WidgetRegion m_regionPressed; //��¼��갴��ʱ�����������

	int m_cursorTimerId;

	bool m_bDragEnable;
	bool m_bBorderMini; //�߿�Ϊ1ʱ�������ĶԽ�λ��
};


#endif //MAINWIDGET


#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"
#include "MainFrame_global.h"
#include "Animation.h"

#define   CONFIG   "../Data/Config/"
#define   LOG      "../Data/Log/"
#define   IMAGE    "../Data/Image/"

extern SignalQueue* g_pSignal;

class MAINFRAME_EXPORT MainWidget : public Animation {
	Q_OBJECT
public:
	MainWidget(QWidget *ject = 0);

	void setInitUi(const QRect &rect);
	void setMain(QWidget* pMain, const QRect& rect, const QString& strTitle);
	void Set_Qss();
	void paintEvent(QPaintEvent* event);

public:
	static MainWidget* staticThis;
	QWidget* m_pWidget;
public slots: 
	void closeWindow();
	void setMain(QWidget* pMain, const QRect& rect);
private:
	QString m_strQssConfig;
	QGridLayout *gridLayout_2;
	QHBoxLayout *pHbLayout;
	QPushButton *BtnPicture;
	QPushButton *BtnSet;
	QPushButton *BtnMin;
	QPushButton *BtnClose;
};

class TDragProxy :public QObject
{
	Q_OBJECT

public:
	TDragProxy(QWidget* parent);
	~TDragProxy();
	enum WidgetRegion{Top,TopRight,Right,RightBottom,Bottom,LeftBottom,Left,LeftTop,Inner,Unknown};

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


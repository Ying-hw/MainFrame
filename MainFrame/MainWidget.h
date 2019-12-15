#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"
#include "MainFrame_global.h"


extern SignalQueue* g_pSignal;

class MAINFRAME_EXPORT MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *ject = 0) : QWidget(ject),
		m_pWidget(nullptr), m_isPress(false), BtnClose(NULL) {}

	void setInitUi(const QRect &rect);
	void setMain(QWidget* pMain); 
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void Set_Qss();
	void paintEvent(QPaintEvent* event);
	static MainWidget* staticThis;
	QWidget* m_pWidget;
public slots: 
	void closeWindow();
	void setMain(QWidget* pMain, const QRect& rect);
private:
	QString m_strQssConfig;
	QPoint m_point;
	bool m_isPress;
	QGridLayout *gridLayout_2;
	QHBoxLayout *pHbLayout;
	QPushButton *BtnPicture;
	QPushButton *BtnSet;
	QPushButton *BtnMin;
	QPushButton *BtnClose;
};


#endif //MAINWIDGET


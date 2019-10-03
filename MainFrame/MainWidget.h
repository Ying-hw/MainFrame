#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"
#include <QGridLayout>


#define MAIN
#define SET_MAIN(MAIN)    set_Main(MAIN);
extern SignalQueue* g_pSignal;

class Q_DECL_EXPORT TitleBar : public QWidget
{
public:
	TitleBar(QWidget *ject = 0) : QWidget(ject), m_pWidget(nullptr), m_isPress(false)
		{ installEventFilter(this); }

	void setInitUi();

	void retranslateUi();

	void setMain(QWidget* pMain); 

	void paintEvent(QPaintEvent* event);

	void mousePressEvent(QMouseEvent* event);
	
	void mouseMoveEvent(QMouseEvent* event);
	
private:
	QPoint m_point;
	bool m_isPress;
	QWidget* m_pWidget;
	QGridLayout *gridLayout_2;
	QHBoxLayout *pHbLayout;
	QLabel *labPicture;
	QLabel *labText;
	QSpacerItem *horizontalSpacer;
	QPushButton *BtnMin;
	QPushButton *BtnClose;
	
};

class Q_DECL_EXPORT MainWidget : public QWidget
{
public:
	MainWidget(QWidget* widget = 0);

	~MainWidget();

	void SetQApplicationQss();

	static void CloseWind(MainWidget* pthis);
	
protected:
	void SetInitSize();

	void showEvent(QShowEvent* event);

	virtual void closeWindow();

	void set_Main(QWidget* pMain) { m_pBar->setMain(pMain); }

	void paintEvent(QPaintEvent* event);

	
private:
	TitleBar *m_pBar;
	QString m_strQssConfig;
	QWidget* m_pTitleBar;
};


#endif //MAINWIDGET


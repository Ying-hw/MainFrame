#ifndef __HINTFRAMEWIDGET__
#define __HINTFRAMEWIDGET__
#include "MainFrame_global.h"
#include "AbstractWidget.h"
#include "AbstractNetWork.h"

class MAINFRAME_EXPORT HintFrameWidget : public QWidget
{
	Q_OBJECT
public:
	HintFrameWidget(QString strHint, AbstractWidget* parent);
	HintFrameWidget(QString strHint, QPoint originPoint, AbstractNetWork* parent);
	~HintFrameWidget();
	void initSize();
	void showEvent(QShowEvent *event);
	void paintEvent(QPaintEvent* event);
private slots:
	void CloseHintWindow();
private:
	QString m_strHintText;
	QTimer m_TimeClose;
	AbstractWidget* m_ParentWidget;
};

#endif //__HINTFRAMEWIDGET__


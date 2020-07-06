#ifndef __HINTFRAMEWIDGET__
#define __HINTFRAMEWIDGET__
#include "MainFrame_global.h"

class MAINFRAME_EXPORT HintFrameWidget : public QWidget
{
	Q_OBJECT
public:
	HintFrameWidget(QString strHint, QPoint originPoint, QObject* parent = 0);
	void initSize(const QPoint& originPoint);
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);
private:
	QString m_strHintText;
};

#endif //__HINTFRAMEWIDGET__


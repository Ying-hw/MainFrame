#ifndef __HINTFRAMEWIDGET__
#define __HINTFRAMEWIDGET__

class HintFrameWidget : public QWidget
{
	Q_OBJECT
public:
	HintFrameWidget(QString strHint, QObject* parent = 0);
	void initSize();
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);
private:
	QString m_strHintText;
};

#endif //__HINTFRAMEWIDGET__


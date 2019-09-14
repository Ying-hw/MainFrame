#ifndef MAINWIDGET__
#define MAINWIDGET__
#include "Signal.h"
#include "SignalQueue.h"

extern SignalQueue* g_pSignal;

class Q_DECL_EXPORT MainWidget : public QWidget
{
public:
	MainWidget(QWidget* widget);
	~MainWidget();
	void SetQApplicationQss();
	static void CloseWind(MainWidget* pthis);
protected:
	void SetInitSize();
	void showEvent(QShowEvent* event);
	virtual void closeWindow();
private:
	QString m_strQssConfig;
};
#endif //MAINWIDGET


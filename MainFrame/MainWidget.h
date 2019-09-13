#ifndef MAINWIDGET
#define MAINWIDGET

class Q_DECL_EXPORT MainWidget : public QWidget
{
public:
	MainWidget(QWidget* widget = 0);
	~MainWidget();
	void SetQApplicationQss();
	
private:
	QString m_strQssConfig;
};
#endif //MAINWIDGET


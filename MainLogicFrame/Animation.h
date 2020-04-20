#ifndef ANIMATION_H
#define ANIMATION_H
#include "MainFrame_global.h"

class MAINFRAME_EXPORT Animation : public QWidget
{
	Q_OBJECT
public:
	Animation(QWidget *parent = 0);
	~Animation();
	void showEvent(QShowEvent* event);
	void setAnimation(bool isShow = true);
	void hideEvent(QHideEvent *event);
public slots:
	void closeAnimation(bool closeHide);
private:
	bool m_IsShow;
};

#endif // ANIMATION_H

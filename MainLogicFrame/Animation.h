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
public slots:
	void closeAnimation(bool closeHide);
};

#endif // ANIMATION_H

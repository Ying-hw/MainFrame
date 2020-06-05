#ifndef ANIMATION_H
#define ANIMATION_H
#include "MainFrame_global.h"
#include <QPropertyAnimation>

class MAINFRAME_EXPORT Animation : public QWidget
{
	Q_OBJECT
public:
	Animation(QWidget *parent = 0);
	~Animation();
	void showEvent(QShowEvent *event);
	void setAnimation(bool isShow = true);
	void hideEvent(QHideEvent *event);
	void closeEvent(QCloseEvent *event);
	void InitAanimation();
public slots:
	void closeAnimation(bool closeHide);
private:
	bool m_IsShow;
	QPropertyAnimation* m_Animation_Opacity;
	QPropertyAnimation* m_Animation_Geometry;
};


#endif // ANIMATION_H

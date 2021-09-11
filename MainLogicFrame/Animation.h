#ifndef ANIMATION_H
#define ANIMATION_H
#include "MainFrame_global.h"


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
	std::shared_ptr<QPropertyAnimation> m_Animation_Opacity, m_Animation_Geometry;
};


#endif // ANIMATION_H

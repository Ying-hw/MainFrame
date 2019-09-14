#include "stdafx.h"
#include "MainWidget.h"

QRect g_Rects;

MainWidget::MainWidget(QWidget* widget):
	QWidget(widget)
{
	SetQApplicationQss();
	SetInitSize();
	g_pSignal->SetUserIdentify(this, User::MAINWIDGET);
}


MainWidget::~MainWidget()
{
}

void MainWidget::SetInitSize() {
	this->setGeometry(g_Rects.x(), g_Rects.y(), g_Rects.height(), g_Rects.width());
	resize(g_Rects.height(), g_Rects.width());
}

void MainWidget::SetQApplicationQss() {
	QFile QssFile(QString(CONFIG) + "DefaultQss.qss");
	if (!QssFile.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告！"),
			QString::fromLocal8Bit("读取配置文件失败！！！"), QMessageBox::Ok);
		return;
	}
	else {
		m_strQssConfig = QssFile.readAll();
		qApp->setStyleSheet(m_strQssConfig);
	}
}
void MainWidget::showEvent(QShowEvent* event) {
	this->resize(g_Rects.height(), g_Rects.width());
}

void MainWidget::closeWindow() {
	if (QMessageBox::information(this, QString::fromLocal8Bit("是否退出！"),
		QString::fromLocal8Bit("客官真的要退出吗！"), QMessageBox::Yes | QMessageBox::No)
		== QMessageBox::No) {
		return;
	}
	else
		close();
}

void MainWidget::CloseWind(MainWidget* pthis) {
	pthis->closeWindow();
}

#include "stdafx.h"
#include "MainWidget.h"


MainWidget::MainWidget(QWidget* widget):
	QWidget(widget)
{
	SetQApplicationQss();
}


MainWidget::~MainWidget()
{
}

void MainWidget::SetQApplicationQss() {
	QFile QssFile(QString(CONFIG) + "DefaultQss.qss");
	if (!QssFile.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, QString::fromLocal8Bit("���棡"),
			QString::fromLocal8Bit("��ȡ�����ļ�ʧ�ܣ�����"), QMessageBox::Ok);
		return;
	}
	else {
		m_strQssConfig = QssFile.readAll();
		qApp->setStyleSheet(m_strQssConfig);
	}
}

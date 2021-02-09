#include <QApplication>
#include <QDebug>
#include <QLibrary>
#include <QTranslator>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	a.setAttribute(Qt::AA_ShareOpenGLContexts);
	QTranslator tran;
	if(tran.load("../Data/qt_zh_CN.qm")) a.installTranslator(&tran);
	QLibrary lib("./MainLogicFrame.dll");
	if (lib.load()) {
		QFunctionPointer fun = lib.resolve("Handle");
		if (fun) 
			fun();
	}
	else {
		qDebug() << lib.errorString();
	}
	return a.exec();
}

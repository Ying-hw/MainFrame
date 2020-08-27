#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QLibrary>


int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
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

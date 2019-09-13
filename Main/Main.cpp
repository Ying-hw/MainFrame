#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QLibrary>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QLibrary lib("./MainFrame.dll");
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

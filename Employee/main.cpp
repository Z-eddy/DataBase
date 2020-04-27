#include "Employee.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Employee w;
	w.show();
	return a.exec();
}

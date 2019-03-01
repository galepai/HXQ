#include "ReportTable.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ReportTable w;
	w.show();
	return a.exec();
}

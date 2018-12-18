#include "notepad.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    a.setStyle("fusion");
	NotePad w;
	w.show();
	//FindDialog w;
	//w.show();
	return a.exec();
}

#include "notepad.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(icons);
	QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    a.setStyle("fusion");
	NotePad w;
	w.show();
	//FindDialog w;
	//w.show();
	return a.exec();
}

//main_cpp
//It is the project of OOP course
//by Zhu Jingsen, Chai Ziwei, Zhang Ruizhe, Lin Haohong and Wu Tong.
//It is a editor called JinsLime.
//developed with QT 5.12

#include "notepad.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(icons);
	QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    a.setStyle("fusion");
	NotePad w;
	w.show();
	return a.exec();
}

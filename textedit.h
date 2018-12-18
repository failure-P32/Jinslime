#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QtWidgets>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);
    //   :QTextEdit(parent)
    //{}

public slots:
    //void setColor();
    //void setTextFont(const QFont &);
    void setCurrentFontSize(int);

private:

};

#endif // TEXTEDIT_H

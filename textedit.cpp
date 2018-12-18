#include "textedit.h"

TextEdit::TextEdit(QWidget *parent)
    :QTextEdit(parent)
{}

/*void TextEdit::setTextFont(const QFont &f)
{
    setFont(f);
    qDebug() << "Set Font to " << f.toString();
}*/
void TextEdit::setCurrentFontSize(int size)
{
    setFontPointSize(size);
}

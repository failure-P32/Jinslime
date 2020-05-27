//linenumberarea_h
//The functions in this file is used to display the line number

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QtWidgets>

class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(QTextEdit *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTextEdit *codeEditor;
};
#endif // LINENUMBERAREA_H

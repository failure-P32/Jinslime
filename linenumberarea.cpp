
#include "textedit.h"

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize((qobject_cast<TextEdit*>(codeEditor))->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((qobject_cast<TextEdit*>(codeEditor))->lineNumberAreaPaintEvent(event));
}


#include "textedit.h"

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
}

TextEdit::TextEdit(QWidget *parent, bool activateCompleter)
    :QTextEdit(parent), completerEnabled(activateCompleter), c(nullptr)
{
    if(completerEnabled)
        initCompleter();
    setAlignment(Qt::AlignJustify);
}

QCompleter* TextEdit::completer() const
{
    return c;
}

/*void TextEdit::setTextFont(const QFont &f)
{
    setFont(f);
    qDebug() << "Set Font to " << f.toString();
}*/
void TextEdit::setCurrentFontSize(int size)
{
    setFontPointSize(size);
}
void TextEdit::setCompleterEnabled(bool activate)
{
    completerEnabled = activate;
}

void TextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cursor = textCursor();
    if(completerEnabled)
        handleCompleterEvent(e);
    else
        QTextEdit::keyPressEvent(e);
    switch(e->key())
    {
    case Qt::Key_BraceLeft:
        cursor.insertText("}");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        break;
    case Qt::Key_BracketLeft:
        cursor.insertText("]");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        break;
    case Qt::Key_ParenLeft:
        cursor.insertText(")");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        break;
    case Qt::Key_Apostrophe:
        cursor.insertText("\'");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        break;
    case Qt::Key_QuoteDbl:
        cursor.insertText("\"");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        break;
    default:
        break;
    }
}

void TextEdit::initCompleter()
{
    if (c)
    {
        disconnect(c, 0, this, 0);
        delete c;
    }
    QStringList keywords;
    //32 C language keywords
    //Optional: adding C++ keywords
    keywords << "auto" << "short" << "int" << "long" <<
                "float" << "double" << "char" << "struct" <<
                "union" << "enum" << "typedef" << "const" <<
                "unsigned" << "signed" << "extern" << "register" <<
                "static" << "volatile" << "void" << "else" <<
                "switch" << "case" << "for" << "do" <<
                "while" << "goto" << "continue" << "break" <<
                "default" << "sizeof" << "return" << "if";
    c = new QCompleter(keywords, this);
    c->setWidget(this);
    //completer->setWidget(this);
    connect(c, SIGNAL(activated(QString)), this, SLOT(on_completer_activated(QString)));
}

QString TextEdit::currentWord()
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();
    if(word.isEmpty())
        word = " ";
    return word;
}

void TextEdit::on_completer_activated(const QString &completeWord)
{
    QString lastWord = currentWord();
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, lastWord.size());
    cursor.deleteChar();
    cursor.insertText(completeWord);
}

void TextEdit::handleCompleterEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key())
       {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
      return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = currentWord();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                    || eow.contains(e->text().right(1))))
    {
      c->popup()->hide();
      return;
    }

    if (completionPrefix != c->completionPrefix()) {
      c->setCompletionPrefix(completionPrefix);
      c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
              + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

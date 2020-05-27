#include "textedit.h"

/*
 * @brief: constructor to initialize the widget and the status of completer
 * @param: pointer to QWidget and the status of completer
 * @return: void
 */
TextEdit::TextEdit(QWidget *parent, bool activateCompleter)
    :QTextEdit(parent), c(nullptr), completerEnabled(activateCompleter)
{
    if(completerEnabled)
        initCompleter();
    setAlignment(Qt::AlignJustify);

    // create the line numbers
    lineNumberArea = new LineNumberArea(this);
    ///
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    ///
    updateLineNumberAreaWidth(0);


}

/*
 * @brief: set the current fontsize
 * @param: fontsize integer
 * @return: void
 */
void TextEdit::setCurrentFontSize(int size)
{
    qDebug() << "Set font size to " << size;
    setFontPointSize(size);
}

/*
 * @brief: enable the auto completer function depending on different file types
 * @param: completers status and file type
 * @return: void
 */
void TextEdit::setCompleterEnabled(bool activate, FileType fileType)
{
    this->fileType = fileType;
    completerEnabled = activate;
    qDebug()<<"setCompleterEnabled"<<fileType;

    keywords.clear();
    switch(fileType)
    {
    case CFile:
        keywords << "auto" << "short" << "int" << "long" <<
                    "float" << "double" << "char" << "struct" <<
                    "union" << "enum" << "typedef" << "const" <<
                    "unsigned" << "signed" << "extern" << "register" <<
                    "static" << "volatile" << "void" << "else" <<
                    "switch" << "case" << "for" << "do" <<
                    "while" << "goto" << "continue" << "break" <<
                    "default" << "sizeof" << "return" << "if";
        break;

    case CppFile:
        keywords << "asm" << "do" << "if" << "return" << "typedef"<<
                    "auto" << "double" << "inline" << "short"<<
                    "typeid" << "bool" << "dynamic_cast" << "int" <<
                    "signed" << "typename" << "break" << "else" <<
                    "long" << "sizeof" << "union" << "case" << "enum" <<
                    "mutable" << "static" << "unsigned" << "catch" <<
                    "explicit" << "namespace" << "static_cast" << "using"<<
                    "char" << "export" << "new" <<"struct" << "virtual" <<
                    "extern" << "operator" << "switch" << "void" <<
                    "cosnt" << "false" << "private" << "template" <<
                    "volatile" << "const_cast" << "float" << "protected" <<
                    "this" << "wchar_t" << "continue" << "for" << "public" <<
                    "throw" << "while" << "defalut" << "friend" << "register"<<
                    "true" << "delete" << "goto" << "reinterpret_cast" <<"try" <<
                    "continue";
        break;
    case PythonFile:
        keywords << "False" << "None" << "True" << "and" << "as" << "assert" << "break" << "class" << "continue" <<
                    "def" << "del" << "elif" << "else" << "except" << "finally" << "for" << "from" << "global" <<
                    "if" << "import" << "in" << "is" << "lambda" << "nonlocal" << "not" << "or" << "pass" <<
                    "raise" << "return" << "try" << "while" << "with" << "yield";
        break;

    case JavaFile:
        keywords << "abstract" << "assert" << "do" << "if" << "return" << "typedef"<<
                    "auto" << "double" << "inline" << "short"<< "boolean" << "byte" <<
                    "typeid" << "bool" << "dynamic_cast" << "int" << "continue" << "interface"
                    "signed" << "typename" << "break" << "else" << "package" << "synchronized" <<
                    "long" << "sizeof" << "union" << "case" << "enum" << "strictfp"
                    "mutable" << "static" << "unsigned" << "catch" << "transient"
                    "explicit" << "namespace" << "static_cast" << "using"<< "implements"
                    "char" << "export" << "new" <<"struct" << "virtual" << "final"
                    "extern" << "operator" << "switch" << "void" << "instanceof" <<
                    "cosnt" << "false" << "private" << "template" << "extends"
                    "volatile" << "const_cast" << "float" << "protected" <<
                    "this"  << "continue" << "for" << "public" << "finally"
                    "throw" << "while" << "defalut" << "true" << "delete" <<
                    "goto" << "transient" <<"try";
        break;

    case HTMLFile:
        keywords << "html>  <\\html>" << "head>  <\\head>" << "title>  <\\title>" <<
                    "style>  <\\style>" << "body>  <\\body>" << "center>  <\\center>" <<
                    "table>  <\\table>";
        break;
    }

    if(activate)
    {
        if(!c)
            initCompleter();
        else
        {
            QStringListModel *model = qobject_cast<QStringListModel*>(c->model());
            model->setStringList(keywords);
        }
    }
}

/*
 * @brief: count the tabs
 * @param: none
 * @return: number of current tabs
 */
int TextEdit::countTabs()
{
    QTextCursor curs = textCursor();
    curs.select(QTextCursor::LineUnderCursor);
    QString lastLine = curs.selectedText();
    int count = lastLine.count("\t");
    if (lastLine.contains(QRegularExpression("\\{[ |\t]*$")) ||
        lastLine.contains(QRegularExpression("if[ |\t]*\\(.*\\)[ |\t]*$")) ||
        lastLine.contains(QRegularExpression("else[ |\t]*\\(.*\\)[ |\t]*$")) ||
        lastLine.contains(QRegularExpression("while[ |\t]*\\(.*\\)[ |\t]*$")) ||
        lastLine.contains(QRegularExpression("for[ |\t]*\\(.*\\)[ |\t]*$")) ||
        lastLine.contains(QRegularExpression(":[ |\t]*$")))
        ++count;
    return count;
}

void TextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cursor = textCursor();
    int tabs = countTabs();
    if(completerEnabled)
        handleCompleterEvent(e);
    else
        QTextEdit::keyPressEvent(e);
    if (e->isAccepted() && e->key() == Qt::Key_Return)
        cursor.insertText(QString(tabs, '\t'));
    switch(e->key())
    {
    case Qt::Key_BraceLeft:
        cursor.insertText("\n");
        cursor.insertText(QString(tabs, '\t'));
        cursor.insertText("}");
        cursor.movePosition(QTextCursor::PreviousRow);
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
/*
 * @brief: initialize the completer
 * @param: none
 * @return: void
 */
void TextEdit::initCompleter()
{
    c = new QCompleter(keywords, this);
    c->setWidget(this);
    //completer->setWidget(this);
    connect(c, SIGNAL(activated(QString)), this, SLOT(on_completer_activated(QString)));
}

/*
 * @brief: find the current word being editted
 * @param: none
 * @return: the word of QString type
 */
QString TextEdit::currentWord()
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();
    if(word.isEmpty())
        word = " ";
    return word;
}

/*
 * @brief: complete the selected word and move the cursor to the proper situation
 * @param: the word(s) needed to be completed
 * @return: void
 */
void TextEdit::on_completer_activated(const QString &completeWord)
{
    QString lastWord = currentWord();

    QTextCursor cursor = textCursor();
    qDebug() << "Complete: " << completeWord;
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, lastWord.size());
    cursor.deleteChar();
    cursor.insertText(completeWord);
    if(HTMLFile == fileType)
    {
        for(int i = 0; i < 3; ++i)
            cursor.movePosition(QTextCursor::PreviousWord);
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
    }
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

QString TextEdit::currentFile() const
{
    return curFile;
}

void TextEdit::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
}


int TextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 +  fontMetrics().width(QLatin1Char('9')) * (digits);

    return space;
}

void TextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void TextEdit::updateLineNumberArea(QRectF /*rect_f*/)
{
    TextEdit::updateLineNumberArea();
}
void TextEdit::updateLineNumberArea(int /*slider_pos*/)
{
    TextEdit::updateLineNumberArea();
}
void TextEdit::updateLineNumberArea()
{
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    // Addjust slider to alway see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(static_cast<int>((dy-this->document()->documentMargin())));

}


void TextEdit::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


int TextEdit::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(245, 245, 245));  //background color
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = static_cast<int>(this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition());
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
         additional_margin = static_cast<int>(this->document()->documentLayout()->blockBoundingRect(prev_block) .translated(0, translate_y).intersected(this->viewport()->geometry()).height());

    // Shift the starting point
    top += additional_margin;

    int bottom = top + static_cast<int>(this->document()->documentLayout()->blockBoundingRect(block).height());

    QColor col_1(13, 51, 255);      // 当前行颜色
    QColor col_0(0, 49, 83);    // Other lines

    // Draw the numbers
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-5, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(this->document()->documentLayout()->blockBoundingRect(block).height());
        ++blockNumber;
    }

}


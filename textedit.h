//textedit_h
//functions related to the edit panel

#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include "linenumberarea.h"

class LineNumberArea;

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr, bool activateCompleter = true);

    // display the line number in the linenumber area
    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();


    QCompleter *completer() const;

    QString currentFile() const;

    enum FileType
    {
        TextFile = 0, CFile, CppFile, CHeaderFile, HTMLFile,
        MarkdownFile, JavaScriptFile, JavaFile, PythonFile
    };

public slots:
    void setCurrentFontSize(int);
    void setCompleterEnabled(bool, FileType fileType);
    void setCurrentFile(const QString &);
    void resizeEvent(QResizeEvent *e) override;

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;

private:
    FileType fileType;
    QCompleter *c;
    QString curFile;
    QWidget *lineNumberArea;
    void initCompleter();
    QString currentWord();
    QStringList keywords;

    int countTabs();

    bool completerEnabled;

    void handleCompleterEvent(QKeyEvent *e);

private slots:
    void on_completer_activated(const QString &);
    // 绘制行号
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();

};



#endif // TEXTEDIT_H

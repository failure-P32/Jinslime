#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QtWidgets>

class LineNumberArea;



class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr, bool activateCompleter = true);
    //   :QTextEdit(parent)
    //{}

    // 绘制行号
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    QCompleter *completer() const;

public slots:
    //void setColor();
    //void setTextFont(const QFont &);
    void setCurrentFontSize(int);
    void setCompleterEnabled(bool);

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;

private:
    QCompleter *c;
    QWidget *lineNumberArea;
    void initCompleter();
    QString currentWord();

    bool completerEnabled;

    void handleCompleterEvent(QKeyEvent *e);

private slots:
    void on_completer_activated(const QString &);
};



#endif // TEXTEDIT_H

//notepad_h
//functions that implements kernel functions

#pragma once
#include "textedit.h"
#include "dialogs.h"
#include "highlighter.h"
#include "markdown.h"
#include <QtWidgets/QApplication>
class NotePad : public QMainWindow
{
	Q_OBJECT

public:
	NotePad(QWidget *parent = Q_NULLPTR);

    enum FileType
    {
        TextFile, CFile, CppFile, CHeaderFile, HTMLFile,
        MarkdownFile, JavaScriptFile, JavaFile, PythonFile, ObCFile, PHPFile,
        PerlFile, GoFile, BatFile, RubyFile, HaskellFile, RustFile
    };

private:

    TextEdit *text;
    //QTextBrowser *browser;
	QString curFile;
    enum { RecentFileNum = 5 };
    FileType fileType;
    QStringList recentFiles;

    int curSuffix;

    QTabWidget *mainWidget;
    //QStringList curFiles;

	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveasAction;
    QAction *saveAllAction;
    QAction *recentFileActions[RecentFileNum];

    QAction *undoAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *cutAction;
	QAction *findAction;
	QAction *replaceAction;
    QAction *fontAction;
    QAction *fontColorAction;

    QAction *runAction;
    QAction *viewHtmlAction;
    QAction *viewMarkdownAction;

    QActionGroup *codecActions;

	QMenu *fileMenu;
	QMenu *editMenu;
    QMenu *utilityMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
    QToolBar *fontToolBar;

    QLabel *countLabel;

    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;

	FindDialog *findDialog;
	ReplaceDialog *replaceDialog;
    BrowserDialog *browserDialog;
	
	void createActions();
	void createMenus();
	void createToolBars();
    void createStatusBar();
    void updateRecentFiles(const QString &);
	bool okToContinue();
    bool okToCloseTab(int);
	void setCurrentFile(const QString &);
	bool open(const QString &);
	bool save(const QString &);
    void connectTextEdit();
    void disconnectTextEdit();
    void updateModified();

    Highlighter *highlighter;


private slots:
	bool newFile();
	bool openFile();
    bool openRecentFile();
	bool saveFile();
	bool saveAs();

    bool saveAll();
	void textModified();
	void showFindDialog();
	void findPrevious(const QString &, Qt::CaseSensitivity);
	void findNext(const QString &, Qt::CaseSensitivity);
    void findPrevious(const QRegExp &);
    void findNext(const QRegExp &);
	void showReplaceDialog();
	bool replaceNext(const QString &, const QString &, Qt::CaseSensitivity, bool showMsg = true);
	void replaceAll(const QString &, const QString &, Qt::CaseSensitivity);
    void showFontDialog();
    void showColorDialog();
    bool compileAndRun();
    void showBrowser(const QString &);
    void viewHtml();
    void viewMarkdown();
    void textCount();
    void on_codecAction_triggered();

    void setCurrentTab(int);
    bool closeTab(int);

protected:
	void closeEvent(QCloseEvent *event) override;
};

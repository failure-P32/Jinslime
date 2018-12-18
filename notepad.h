#pragma once
#include <memory>
#include "textedit.h"
#include "dialogs.h"

class NotePad : public QMainWindow
{
	Q_OBJECT

public:
	NotePad(QWidget *parent = Q_NULLPTR);

private:
    TextEdit *text;
	QString content;
	QString curFile;

	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveasAction;

    QAction *undoAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *cutAction;
	QAction *findAction;
	QAction *replaceAction;
    QAction *fontAction;
    QAction *fontColorAction;

	QMenu *fileMenu;
	QMenu *editMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
    QToolBar *fontToolBar;

    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;

	FindDialog *findDialog;
	ReplaceDialog *replaceDialog;
	
	void createActions();
	void createMenus();
	void createToolBars();
	bool okToContinue();
	void setCurrentFile(const QString &);
	bool open(const QString &);
	bool save(const QString &);

private slots:
	bool newFile();
	bool openFile();
	bool saveFile();
	bool saveAs();
	void textModified();
	void showFindDialog();
	void findPrevious(const QString &, Qt::CaseSensitivity);
	void findNext(const QString &, Qt::CaseSensitivity);
	void showReplaceDialog();
	bool replaceNext(const QString &, const QString &, Qt::CaseSensitivity, bool showMsg = true);
	void replaceAll(const QString &, const QString &, Qt::CaseSensitivity);
    void showFontDialog();
    void showColorDialog();

protected:
	void closeEvent(QCloseEvent *event) override;
};

#include "notepad.h"

NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent), content(), findDialog(nullptr), replaceDialog(nullptr)
{
    text = new TextEdit(this);
    text->setFont(QFont("微软雅黑", 16));
	setCentralWidget(text);
    fontComboBox = new QFontComboBox(this);
    fontSizeSpinBox = new QSpinBox(this);
    fontSizeSpinBox->setValue(16);
    resize(QSize(1280, 720));
	setCurrentFile(QString());
	connect(text, SIGNAL(textChanged()), this, SLOT(textModified()));
    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont &)), text, SLOT(setCurrentFont(QFont)));
    connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), text, SLOT(setCurrentFontSize(int)));
	createActions();
	createMenus();
	createToolBars();
}
void NotePad::createActions()
{
	newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(QKeySequence::New);
    newAction->setIcon(QIcon("icons/file.png"));
	newAction->setToolTip(tr("Create a new text file."));

	openAction = new QAction(tr("&Open"), this);
	openAction->setShortcut(QKeySequence::Open);
    openAction->setIcon(QIcon("icons/folder-open.png"));
	openAction->setToolTip(tr("Open a text file."));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
    saveAction->setIcon(QIcon("icons/save.png"));
	saveAction->setToolTip(tr("Save the current text file."));

	saveasAction = new QAction(tr("&Save as"), this);
	saveasAction->setShortcut(QKeySequence::SaveAs);

    undoAction = new QAction(tr("&Undo"), this);

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setIcon(QIcon("icons/copy.png"));
    copyAction->setToolTip(tr("Copy the selected text."));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setIcon(QIcon("icons/paste.png"));
    pasteAction->setToolTip(tr("Paste the copied text."));

	cutAction = new QAction(tr("&Cut"), this);
	cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setIcon(QIcon("icons/cut.png"));
    cutAction->setToolTip(tr("Cut the selected text."));

	findAction = new QAction(tr("&Find"), this);
	findAction->setShortcut(QKeySequence::Find);
    findAction->setIcon(QIcon("icons/search.png"));

	replaceAction = new QAction(tr("&Replace"), this);
	replaceAction->setShortcut(QKeySequence::Replace);
    replaceAction->setIcon(QIcon("icons/binoculars.png"));

    fontAction = new QAction(tr("F&ont"), this);
    fontAction->setIcon(QIcon("icons/font.png"));

    fontColorAction = new QAction(tr("Color"), this);
    fontColorAction->setIcon(QIcon("icons/dashboard.png"));
    fontColorAction->setToolTip(tr("Set font color."));

    //fontAction = new QAction(tr("&Replace"), this);

	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(saveasAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    connect(undoAction, SIGNAL(triggered(bool)), text, SLOT(undo()));
    connect(copyAction, SIGNAL(triggered(bool)), text, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered(bool)), text, SLOT(paste()));
    connect(cutAction, SIGNAL(triggered(bool)), text, SLOT(cut()));
    connect(findAction, SIGNAL(triggered(bool)), this, SLOT(showFindDialog()));
    connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(showReplaceDialog()));
    connect(fontAction, SIGNAL(triggered(bool)), this, SLOT(showFontDialog()));
    connect(fontColorAction, SIGNAL(triggered(bool)), this, SLOT(showColorDialog()));
}
void NotePad::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addActions({ newAction,openAction,saveAction,saveasAction });
	editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addActions({ undoAction,copyAction,pasteAction,cutAction });
	editMenu->addSeparator();
	editMenu->addActions({ findAction,replaceAction });
}
void NotePad::createToolBars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addActions({ newAction,openAction,saveAction });
    //fileToolBar->addSeparator();
	editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addActions({ copyAction,pasteAction,cutAction,findAction,replaceAction });
    fontToolBar = addToolBar(tr("F&ont"));
    fontToolBar->addActions({ fontAction,fontColorAction });
    fontToolBar->addSeparator();
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontSizeSpinBox);

}
bool NotePad::okToContinue()
{
	if (isWindowModified())
	{
		int r = QMessageBox::warning(this, tr("NotePad"), tr("The text has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
			return saveFile();
		else if (r == QMessageBox::Cancel)
			return false;
	}
	return true;
}
void NotePad::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	QString shownFile = curFile.isEmpty() ? "Untitled" : curFile;
	setWindowModified(false);
	setWindowTitle(tr("%1[*] - %2").arg(shownFile).arg(tr("NotePad")));
}
//Events
void NotePad::closeEvent(QCloseEvent *event)
{
	if (okToContinue())
		event->accept();
	else
		event->ignore();
}
//Slots
bool NotePad::newFile()
{
	if (okToContinue())
	{
		text->clear();
		setCurrentFile(QString());
		return true;
	}
	else
		return false;
}
bool NotePad::openFile()
{
	if (okToContinue())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open text"), ".", tr("Text files (*.txt)"));
		if (!fileName.isEmpty())
			return open(fileName);
		return false;
	}
	return false;
}
bool NotePad::saveFile()
{
	if (curFile.isEmpty())
		return saveAs();
	return save(curFile);
}
bool NotePad::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".", tr("Text files (*.txt)"));
	if (!fileName.isEmpty())
		return save(fileName);
	return false;
}
void NotePad::textModified()
{
	setWindowModified(true);
}
bool NotePad::open(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("NotePad"), tr("Cannot read file %1.\n%2").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream in(&file);
	QString content;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	content = in.readAll();
	text->setPlainText(content);
	setCurrentFile(fileName);
	QApplication::restoreOverrideCursor();
	return true;
}
bool NotePad::save(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("NotePad"), tr("Cannot save file %1.\n%2").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString content = text->toPlainText();
	for (auto itr = content.cbegin(); itr < content.cend(); itr++)
		if (*itr == '\n')
			out << endl;
		else
			out << *itr;
	setCurrentFile(fileName);
	QApplication::restoreOverrideCursor();
	//out << endl;
	return true;
}
void NotePad::showFindDialog()
{
	if (!findDialog)
	{
		findDialog = new FindDialog(this);
        qDebug() << "New find dialog";
		connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), this, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
		connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), this, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}
void NotePad::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    qDebug() << cs;
	QFlags<QTextDocument::FindFlag> flag = (cs == Qt::CaseInsensitive) ? QTextDocument::FindBackward : (QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
    qDebug() << flag;
    bool findSucc = text->find(str, flag);
    //text->setTextCursor(text->document()->find(str, text->textCursor(), flag));
    if (!findSucc)
        QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(str), QMessageBox::StandardButton::Ok);
}
void NotePad::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    QFlags<QTextDocument::FindFlag> flag = (cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively;
	bool findSucc = text->find(str, flag);
	if (!findSucc)
		QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(str), QMessageBox::StandardButton::Ok);
}
void NotePad::showReplaceDialog()
{
	if (!replaceDialog)
	{
		replaceDialog = new ReplaceDialog(this);
        qDebug() << "New replace dialog";
        connect(replaceDialog, SIGNAL(replaceNext(const QString &, const QString &, Qt::CaseSensitivity, bool)),
			this, SLOT(replaceNext(const QString &, const QString &, Qt::CaseSensitivity, bool)));
		connect(replaceDialog, SIGNAL(replaceAll(const QString &, const QString &, Qt::CaseSensitivity)), 
			this, SLOT(replaceAll(const QString &, const QString &, Qt::CaseSensitivity)));
		connect(replaceDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), this, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
	}
	replaceDialog->show();
	replaceDialog->raise();
	replaceDialog->activateWindow();
}
bool NotePad::replaceNext(const QString &find, const QString &replace, Qt::CaseSensitivity cs, bool showMsg)
{
	QFlags<QTextDocument::FindFlag> flag = (cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively;
    qDebug() << "Replace Next";
	bool findSucc = text->find(find, flag);
	if (!findSucc)
	{
		if(showMsg)
			QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(find), QMessageBox::StandardButton::Ok);
		return false;
	}
	/*QTextCursor textCursor = text->textCursor();
	textCursor.deleteChar();
	textCursor.insertText(replace);
	text->setTextCursor(textCursor);*/
	text->textCursor().deleteChar();
	text->textCursor().insertText(replace);
	return true;
}
void NotePad::replaceAll(const QString &find, const QString &replace, Qt::CaseSensitivity cs)
{
	QFlags<QTextDocument::FindFlag> flag = (cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively;
	text->moveCursor(QTextCursor::Start);
	bool findSucc = text->find(find, flag);
	if (!findSucc)
	{
		QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(find), QMessageBox::StandardButton::Ok);
		return;
	}
	int count = 0;
	text->moveCursor(QTextCursor::Start);
	while (replaceNext(find, replace, cs, false))
		count++;
	QMessageBox::information(this, tr("NotePad"), tr("%1 text%2 been replaced").arg(count).arg(count == 1 ? " has" : "s have"), QMessageBox::StandardButton::Ok);
}
void NotePad::showFontDialog()
{
    QFontDialog *fontDialog = new QFontDialog(this);
    qDebug() << "New font dialog";
    fontDialog->setCurrentFont(text->currentFont());

    if (QDialog::Accepted == fontDialog->exec())
        text->setCurrentFont(fontDialog->currentFont());

    delete fontDialog;
}
void NotePad::showColorDialog()
{
    QColorDialog *colorDialog = new QColorDialog(this);
    qDebug() << "New color dialog";
    colorDialog->setCurrentColor(text->textColor());

    if(QDialog::Accepted == colorDialog->exec())
        text->setTextColor(colorDialog->currentColor());

    delete colorDialog;
}

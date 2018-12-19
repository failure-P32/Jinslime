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
    setCurrentFile(QString()); //Empty file
    connect(text, SIGNAL(textChanged()), this, SLOT(textModified())); //Modifidied
    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont &)), text, SLOT(setCurrentFont(QFont))); //Set font
    connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), text, SLOT(setCurrentFontSize(int))); //Change font size
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

    for(int i = 0; i < RecentFileNum; ++i)
    {
        recentFileActions[i] = new QAction(this);
        connect(recentFileActions[i], SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
    }

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setIcon(QIcon("icons/undo.png"));

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
    runAction = new QAction(tr("Compile and run"), this);
    runAction->setIcon(QIcon("icons/run.png"));

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

    connect(runAction, SIGNAL(triggered(bool)), this, SLOT(compileAndRun()));
}
void NotePad::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addActions({ newAction,openAction,saveAction,saveasAction });
    fileMenu->addSeparator();
    for(int i = 0; i < RecentFileNum; ++i)
    {
        fileMenu->addAction(recentFileActions[i]);
        recentFileActions[i]->setVisible(false);
    }
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
    fontToolBar->addSeparator();
    fontToolBar->addAction(runAction);
}
bool NotePad::okToContinue()
//Ask if user close without saving
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
    QString shownFile = curFile.isEmpty() ? "Untitled" : QFileInfo(fileName).fileName();
    //Use QFileInfo::fileName to cut the path of the file name when showing.
	setWindowModified(false);
	setWindowTitle(tr("%1[*] - %2").arg(shownFile).arg(tr("NotePad")));
}
//Events
void NotePad::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) //Ask if save
		event->accept();
	else
		event->ignore();
}
//Slots
bool NotePad::newFile()
{
    if (okToContinue()) //Ask if save
	{
		text->clear();
        setCurrentFile(QString()); //Empty file
		return true;
	}
	else
		return false;
}
bool NotePad::openFile()
{
    if (okToContinue()) //Ask if save
	{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open text"), ".",
                                        tr("Text files (*.txt);;C/C++ source files (*.c *.cpp);;All files (*.*)"));
		if (!fileName.isEmpty())
			return open(fileName);
		return false;
	}
	return false;
}
bool NotePad::openRecentFile()
{
    if(okToContinue())
    {
        QString fileName = qobject_cast<QAction*>(sender())->data().toString();
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".",
                                    tr("Text files (*.txt);;C/C++ source files (*.c *.cpp);;All files (*.*)"));
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
    updateRecentFiles(fileName);
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
    updateRecentFiles(fileName);
	//out << endl;
	return true;
}
void NotePad::updateRecentFiles(const QString &fileName)
{
    //for(int i = 0; i < recentFiles.size(); ++i)
    for(auto itr = recentFiles.begin(); itr < recentFiles.end();) //Update recent file list
    {
        if(QFile::exists(*itr))
            ++itr;
        else
            itr = recentFiles.erase(itr);
    }
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    for(int i = 0; i < RecentFileNum; ++i)
    {
        if(i < recentFiles.size())
        {
            recentFileActions[i]->setText(QFileInfo(recentFiles[i]).fileName());
            recentFileActions[i]->setData(recentFiles[i]);
            recentFileActions[i]->setVisible(true);
        }
        else
            recentFileActions[i]->setVisible(false);
    }
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
    text->setCurrentFont(QFontDialog::getFont(nullptr, text->currentFont(), this, tr("Set Font")));
}
void NotePad::showColorDialog()
{
    text->setTextColor(QColorDialog::getColor(text->textColor(), this, tr("Set Font Color")));
}
bool NotePad::compileAndRun()
{
    QFileInfo fileInfo(curFile);
    QString compiler = "gcc ";
    if(fileInfo.suffix() != "c" && fileInfo.suffix() != "cpp")
    {
        QMessageBox::warning(this, tr("Warning"), tr("Only C or C++ source file is supported to be compiled and run!"), QMessageBox::Ok);
        return false;
    }
    if(fileInfo.suffix() == "cpp")
        compiler = "g++ ";
    if(okToContinue())
    {
        QString cmd = "cd /d " + fileInfo.path() + " && " +
                compiler + fileInfo.fileName() + " -o " + fileInfo.baseName() + " && " +
                fileInfo.baseName();
        qDebug() << cmd;
        if(system(cmd.toLatin1().data()))
        {
            QMessageBox::warning(this, tr("Error"), tr("Error while compiling or running!"), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    return false;
}

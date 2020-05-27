#include "notepad.h"
#include "markdown.h"
#include <cassert>
#include <QDebug>

/*
 * @brief: constructor of the NotePad class derived from QMainWindow
 * @param: QWidget pointer
 * @return: void
 */
NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent), fileType(TextFile), findDialog(nullptr), replaceDialog(nullptr), browserDialog(new BrowserDialog(this)),
      fontComboBox(new QFontComboBox(this)), fontSizeSpinBox(new QSpinBox(this)), text(nullptr), mainWidget(new QTabWidget(this)), curFile()
{
    //text = new TextEdit(this, false);
    //text->setFont(QFont("微软雅黑", 16));
    //setCentralWidget(text);

    mainWidget->setTabsClosable(true);
    setCentralWidget(mainWidget);
    fontSizeSpinBox->setValue(16);
    fontComboBox->setCurrentFont(QFont("Consolas", 16));
    browserDialog->resize(QSize(1280, 720));
    resize(QSize(1280, 720));

    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), browserDialog, SLOT(setCurrentFont(QFont)));//Set font
    connect(mainWidget, SIGNAL(currentChanged(int)), this, SLOT(setCurrentTab(int)));
    connect(mainWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	createActions();
	createMenus();
	createToolBars();
    highlighter = new Highlighter(nullptr);

    // create the status bar
    createStatusBar();
    //setWindowModified(false);
}
/*
 * @brief: count the number of words in this text and display it on the down right side of the window
 * @param: none
 * @return: void
 */
void NotePad::textCount()
{
    if(text)
        countLabel->setText("Current Words:"+ QString::number(text->toPlainText().length()));
   // qDebug()<<content.toUtf8().size();
}

void NotePad::createStatusBar()
{
    statusBar();
    countLabel = new QLabel(this->statusBar());
    statusBar()->addPermanentWidget(countLabel);
}

/*
 * @brief: create actions of file option
 * @param: QWidget pointer
 * @return: void
 */
void NotePad::createActions()
{
	newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(QKeySequence::New);
    newAction->setIcon(QIcon(":/icons/file.png"));
    newAction->setToolTip(tr("Create a new text file"));

	openAction = new QAction(tr("&Open"), this);
	openAction->setShortcut(QKeySequence::Open);
    openAction->setIcon(QIcon(":/icons/folder-open.png"));
    openAction->setToolTip(tr("Open a text file"));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
    saveAction->setIcon(QIcon(":/icons/save.png"));
    saveAction->setToolTip(tr("Save the current text file"));

	saveasAction = new QAction(tr("&Save as"), this);
	saveasAction->setShortcut(QKeySequence::SaveAs);

    saveAllAction = new QAction(tr("Save All"), this);

    for(int i = 0; i < RecentFileNum; ++i)
    {
        recentFileActions[i] = new QAction(this);
        connect(recentFileActions[i], SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
    }

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":/icons/undo.png"));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setIcon(QIcon(":/icons/copy.png"));
    copyAction->setToolTip(tr("Copy the selected text"));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setIcon(QIcon(":/icons/paste.png"));
    pasteAction->setToolTip(tr("Paste the copied text"));

	cutAction = new QAction(tr("&Cut"), this);
	cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setIcon(QIcon(":/icons/cut.png"));
    cutAction->setToolTip(tr("Cut the selected text"));

	findAction = new QAction(tr("&Find"), this);
	findAction->setShortcut(QKeySequence::Find);
    findAction->setIcon(QIcon(":/icons/search.png"));

	replaceAction = new QAction(tr("&Replace"), this);
	replaceAction->setShortcut(QKeySequence::Replace);
    replaceAction->setIcon(QIcon(":/icons/binoculars.png"));

    fontAction = new QAction(tr("F&ont"), this);
    fontAction->setIcon(QIcon(":/icons/font.png"));

    fontColorAction = new QAction(tr("Color"), this);
    fontColorAction->setIcon(QIcon(":/icons/dashboard.png"));
    fontColorAction->setToolTip(tr("Set font color"));

    runAction = new QAction(tr("Compile and run"), this);
    runAction->setIcon(QIcon(":/icons/run.png"));

    viewHtmlAction = new QAction(tr("View HTML"), this);
    viewMarkdownAction = new QAction(tr("View Markdown"), this);

    saveAction->setEnabled(false);
    saveasAction->setEnabled(false);

    connect(newAction, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(saveasAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(saveAllAction, SIGNAL(triggered(bool)), this, SLOT(saveAll()));

    undoAction->setEnabled(false);
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
    cutAction->setEnabled(false);
    findAction->setEnabled(false);
    replaceAction->setEnabled(false);

    connect(findAction, SIGNAL(triggered(bool)), this, SLOT(showFindDialog()));
    connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(showReplaceDialog()));
    connect(fontAction, SIGNAL(triggered(bool)), this, SLOT(showFontDialog()));
    connect(fontColorAction, SIGNAL(triggered(bool)), this, SLOT(showColorDialog()));

    connect(runAction, SIGNAL(triggered(bool)), this, SLOT(compileAndRun()));
    connect(viewHtmlAction, SIGNAL(triggered(bool)), this, SLOT(viewHtml()));
    connect(viewMarkdownAction, SIGNAL(triggered(bool)), this, SLOT(viewMarkdown()));

    codecActions = new QActionGroup(this);
    QAction *codecAction = new QAction("UTF-8", this);
    codecAction->setCheckable(true);
    codecAction->setChecked(true);
    codecActions->addAction(codecAction);
    connect(codecAction, SIGNAL(triggered(bool)), this, SLOT(on_codecAction_triggered()));
    codecAction = new QAction("GB2312", this);
    codecAction->setCheckable(true);
    codecActions->addAction(codecAction);
    connect(codecAction, SIGNAL(triggered(bool)), this, SLOT(on_codecAction_triggered()));
    codecAction = new QAction("GBK", this);
    codecAction->setCheckable(true);
    codecActions->addAction(codecAction);
    connect(codecAction, SIGNAL(triggered(bool)), this, SLOT(on_codecAction_triggered()));
}

/*
 * @brief: connect the signals from NotePad with the slots in the textedit.cpp or highlighter.cpp
 * @param: none
 * @return: void
 */
void NotePad::connectTextEdit()
{
    if(text)
    {
        text->setFont(fontComboBox->currentFont());
        text->setCurrentFont(fontComboBox->currentFont());
        connect(text, SIGNAL(textChanged()), this, SLOT(textModified())); //Modifidied
        connect(text, SIGNAL(textChanged()), this, SLOT(textCount())); //Word count
        connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), text, SLOT(setCurrentFont(QFont)));
        connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), text, SLOT(setCurrentFontSize(int))); //Change font size
        saveAction->setEnabled(true);
        saveasAction->setEnabled(true);
        undoAction->setEnabled(true);
        copyAction->setEnabled(true);
        pasteAction->setEnabled(true);
        cutAction->setEnabled(true);
        findAction->setEnabled(true);
        replaceAction->setEnabled(true);
        connect(undoAction, SIGNAL(triggered(bool)), text, SLOT(undo()));
        connect(copyAction, SIGNAL(triggered(bool)), text, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered(bool)), text, SLOT(paste()));
        connect(cutAction, SIGNAL(triggered(bool)), text, SLOT(cut()));
        highlighter->setDocument(text->document());

        // change the keywords of the highligter and completer to fit in the current filetype
        highlighter->setKeyWords(static_cast<Highlighter::FileType>(curSuffix));
        text->setCompleterEnabled(true, static_cast<TextEdit::FileType>(curSuffix));
    }
}

/*
 * @brief: disconnect the signals from NotePad off the slots in the textedit.cpp or highlighter.cpp
 * @param: none
 * @return: void
 */
void NotePad::disconnectTextEdit()
{
    if(text)
    {
        disconnect(text, SIGNAL(textChanged()), this, SLOT(textModified())); //Modifidied
        disconnect(text, SIGNAL(textChanged()), this, SLOT(textCount())); //Word count
        disconnect(fontComboBox, SIGNAL(currentFontChanged(QFont)), text, SLOT(setCurrentFont(QFont)));
        disconnect(fontSizeSpinBox, SIGNAL(valueChanged(int)), text, SLOT(setCurrentFontSize(int))); //Change font size
        disconnect(undoAction, SIGNAL(triggered(bool)), text, SLOT(undo()));
        disconnect(copyAction, SIGNAL(triggered(bool)), text, SLOT(copy()));
        disconnect(pasteAction, SIGNAL(triggered(bool)), text, SLOT(paste()));
        disconnect(cutAction, SIGNAL(triggered(bool)), text, SLOT(cut()));
        highlighter->setDocument(nullptr);
    }
}

/*
 * @brief: create the menu on the top of the window
 * @param: none
 * @return: void
 */
void NotePad::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addActions({ newAction,openAction,saveAction,saveasAction,saveAllAction });
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
    utilityMenu = menuBar()->addMenu(tr("&Utilities"));
    QMenu *cOrCppMenu = utilityMenu->addMenu(tr("Programming"));
    cOrCppMenu->addAction(runAction);
    QMenu *htmlMenu = utilityMenu->addMenu(tr("HTML"));
    htmlMenu->addAction(viewHtmlAction);
    QMenu *markdownMenu = utilityMenu->addMenu(tr("Markdown"));
    markdownMenu->addAction(viewMarkdownAction);
    QMenu *codecMenu = utilityMenu->addMenu(tr("Set codec"));
    codecMenu->addActions(codecActions->actions());
}

/*
 * @brief: create the tool bars along with their icon
 * @param: none
 * @return: void
 */
void NotePad::createToolBars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addActions({ newAction,openAction,saveAction });
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

/*
 * @brief: create the message box to ask user if they would like to close without saving
 * @param: none
 * @return: a bool which stands for the saving status
 */
bool NotePad::okToContinue()
{
	if (isWindowModified())
	{
		int r = QMessageBox::warning(this, tr("NotePad"), tr("The text has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
        {
            bool ret = true;
            while (mainWidget->count())
            {
                if (!saveFile())
                    ret = false;
                TextEdit *oldText = text;
                int index = mainWidget->currentIndex();
                assert(text == mainWidget->widget(index));
                mainWidget->removeTab(index);
                setCurrentTab(mainWidget->currentIndex());
                delete oldText;
            }
            return ret;
        }
		else if (r == QMessageBox::Cancel)
			return false;
	}
	return true;
}

/*
 * @brief: create the file saving dialog
 * @param: none
 * @return: a bool of the saving status of current file
 */
bool NotePad::okToCloseTab(int index)
{
    TextEdit *textEdit = qobject_cast<TextEdit*>(mainWidget->widget(index));
    if (!textEdit)
        return false;
    if (textEdit->document()->isModified())
    {
        int r = QMessageBox::warning(this, tr("NotePad"), tr("The text has been modified.\n"
            "Do you want to save your changes?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            QString fileName = textEdit->currentFile();
            if (fileName.isEmpty())
            {
                fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".",
                                        tr("All files (*.*);;Text files (*.txt);;C/C++ source files (*.c *.cpp);;JavaScript files (*.js);;Python files (*.py);;Java files (*.java);;HTML files (*.html);;Markdown files (*.md)"));
                textEdit->setCurrentFile(fileName);
                if (textEdit == text)
                    setCurrentFile(fileName);
                mainWidget->setTabText(index, QFileInfo(fileName).fileName());
            }
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("NotePad"), tr("Cannot save file %1.\n%2").arg(fileName).arg(file.errorString()));
                return false;
            }
            QTextStream out(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QString content = textEdit->toPlainText();
            for (auto itr = content.cbegin(); itr < content.cend(); itr++)
                if (*itr == '\n')
                    out << endl;
                else
                    out << *itr;
            textEdit->document()->setModified(false);
            mainWidget->setTabText(index, mainWidget->tabText(index).remove("*"));
            QApplication::restoreOverrideCursor();
            updateRecentFiles(fileName);
            updateModified();
            return true;
        }
        else if (r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

/*
 * @brief: identify the current file by its suffix in multi-file edit
 * @param: reference to file's name
 * @return: void
 */
void NotePad::setCurrentFile(const QString &fileName)
{
    QFileInfo info(fileName);
	curFile = fileName;
    if(text)
        text->setCurrentFile(fileName);
    QString shownFile = curFile.isEmpty() ? "Untitled" : info.fileName();
    //Use QFileInfo::fileName to cut the path of the file name when showing.
    QString suffix = info.suffix();
    //Get suffix to decide the file type
    //set the fileType member of the NotePad
    if(suffix == "c")
        curSuffix = fileType   = CFile;
    else if(suffix == "cpp")
        curSuffix = fileType  = CppFile;
    else if(suffix == "h" || suffix == "hpp")
        curSuffix = fileType  = CHeaderFile;
    else if(suffix == "html")
        curSuffix = fileType  = HTMLFile;
    else if(suffix == "md")
        curSuffix = fileType  = MarkdownFile;
    else if(suffix == "py")
        curSuffix = curSuffix = fileType  = PythonFile;
    else if(suffix == "java")
        curSuffix = fileType  = JavaFile;
    else if(suffix == "js")
        curSuffix = fileType  = JavaScriptFile;
    else
        fileType = TextFile;
    switch(fileType)
    {
    case CFile:
    case CppFile:
    case CHeaderFile:
        if(text)
        {
            text->setCompleterEnabled(true, static_cast<TextEdit::FileType>(curSuffix));
            highlighter->setDocument(text->document());
            highlighter->setKeyWords(static_cast<Highlighter::FileType>(curSuffix));
        }
        break;

    case PythonFile:
    case JavaFile:
    case JavaScriptFile:
    case HTMLFile:
        if(text)
        {
            text->setCompleterEnabled(true, static_cast<TextEdit::FileType>(curSuffix));
            highlighter->setDocument(text->document());
            highlighter->setKeyWords(static_cast<Highlighter::FileType>(curSuffix));
        }
        break;
    default:
        if(text)
        {
            text->setCompleterEnabled(false, static_cast<TextEdit::FileType>(0));
            highlighter->setDocument(nullptr);
        }
        break;
    }
    updateModified();
    mainWidget->setTabText(mainWidget->currentIndex(), shownFile);
	setWindowTitle(tr("%1[*] - %2").arg(shownFile).arg(tr("NotePad")));

}

/*
 * @brief: carry out the close procedure of a file
 * @param: none
 * @return: void
 */
void NotePad::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) //Ask if save
		event->accept();
	else
		event->ignore();
}
/*
 * @brief: create the a new file with empty text and other initialization
 * @param: none
 * @return: bool of the status  when creating a new file
 */
bool NotePad::newFile()
{
    if (text)
        disconnectTextEdit();
    text = new TextEdit(this, false);
    connectTextEdit();
    //text->setCurrentFont(fontComboBox->currentFont());
    mainWidget->addTab(text, tr("Untitled"));
    mainWidget->setCurrentWidget(text);
    text->setFont(fontComboBox->currentFont());
    text->setCurrentFont(fontComboBox->currentFont());
    text->setFocus();
    text->document()->setModified(false);
    setCurrentFile(QString()); //Set the current file to the Empty file
    return true;
}

/*
 * @brief: open a file of certain type by calling for the open function followed
 * @param: none
 * @return: bool of the status  when opening that selected file
 */
bool NotePad::openFile()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text"), ".",
                                    tr("All files (*.*);;Text files (*.txt);;C/C++ source files (*.c *.cpp);;JavaScript files (*.js);;Python files (*.py);;Java files (*.java);;HTML files (*.html);;Markdown files (*.md)"));
    if (!fileName.isEmpty())
        return open(fileName);
    return false;
}

/*
 * @brief: open a recent file recorded by data function in qaction.h
 * @param: none
 * @return: bool of the status  when opening that selected file
 */
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

/*
 * @brief: save the change of current file, and call for saveAs function if the file is never saved before
 * @param: none
 * @return: bool of the status  when saving the current file
 */
bool NotePad::saveFile()
{
    if (!text)
        return false;
	if (curFile.isEmpty())
		return saveAs();
	return save(curFile);
}

/*
 * @brief: save the current file as a different names and types
 * @param: none
 * @return: bool of the status when saving the current file
 */
bool NotePad::saveAs()
{
    if (!text)
        return false;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".",
                                    tr("All files (*.*);;Text files (*.txt);;C/C++ source files (*.c *.cpp);;JavaScript files (*.js);;Python files (*.py);;Java files (*.java);;HTML files (*.html);;Markdown files (*.md)"));
	if (!fileName.isEmpty())
		return save(fileName);
	return false;
}

/*
 * @brief: save all files opened and not saved yet
 * @param: none
 * @return: bool of the status when saving all the current files
 */
bool NotePad::saveAll()
{
    bool ret = true;
    for (int i = 0; i < mainWidget->count(); ++i)
    {
        TextEdit *textEdit = qobject_cast<TextEdit*>(mainWidget->widget(i));
        QString fileName = textEdit->currentFile();
        if (fileName.isEmpty())
        {
            fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".",
                                    tr("All files (*.*);;Text files (*.txt);;C/C++ source files (*.c *.cpp);;JavaScript files (*.js);;Python files (*.py);;Java files (*.java);;HTML files (*.html);;Markdown files (*.md)"));
            textEdit->setCurrentFile(fileName);
        }
        QFile file(textEdit->currentFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // throw the warning if the QString of the filename is not normative
            QMessageBox::warning(this, tr("NotePad"), tr("Cannot save file %1.\n%2").arg(fileName).arg(file.errorString()));
            ret = false;
            continue;
        }
        QTextStream out(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString content = textEdit->toPlainText();
        for (auto itr = content.cbegin(); itr < content.cend(); itr++)
            if (*itr == '\n')
                out << endl;
            else
                out << *itr;
        textEdit->document()->setModified(false);
        mainWidget->setTabText(i, QFileInfo(fileName).fileName());
        QApplication::restoreOverrideCursor();
        updateRecentFiles(fileName);
    }
    setWindowModified(false);
    return ret;
}

void NotePad::textModified()
{
    if (text->document()->isModified())
    {
        setWindowModified(true);
        if(!mainWidget->tabText(mainWidget->currentIndex()).contains("*"))
            mainWidget->setTabText(mainWidget->currentIndex(), mainWidget->tabText(mainWidget->currentIndex()) + "*");
    }
    else
    {
        setWindowModified(false);
        mainWidget->setTabText(mainWidget->currentIndex(), mainWidget->tabText(mainWidget->currentIndex()).remove("*"));
    }
}

/*
 * @brief: save all files opened and not saved yet
 * @param: none
 * @return: bool of the status when saving all the current files
 */
bool NotePad::open(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("NotePad"), tr("Cannot read file %1.\n%2").arg(fileName).arg(file.errorString()));
		return false;
	}
    if (text)
        disconnectTextEdit();
    text = new TextEdit(this, false);
    text->setCurrentFont(fontComboBox->currentFont());
    connectTextEdit();
    int index = mainWidget->addTab(text, QString());
    mainWidget->setCurrentIndex(index);
    text->setFocus();
    qDebug() << "Open file " << fileName;
	QTextStream in(&file);
	QString content;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	content = in.readAll();
    qDebug() << "Complete Reading";
    qDebug() << text;
	text->setPlainText(content);
    qDebug() << "Complete Reading";
    text->document()->setModified(false);
	setCurrentFile(fileName);
	QApplication::restoreOverrideCursor();
    updateRecentFiles(fileName);
	return true;
}

/*
 * @brief: save all files opened and not saved yet
 * @param: none
 * @return: bool of the status when saving all the current files
 */
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
    text->document()->setModified(false);
	setCurrentFile(fileName);
    connectTextEdit();
	QApplication::restoreOverrideCursor();
    updateRecentFiles(fileName);
    updateModified();
	//out << endl;
	return true;
}
void NotePad::updateRecentFiles(const QString &fileName)
{
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
        connect(findDialog, SIGNAL(findNext(QRegExp)), this, SLOT(findNext(QRegExp)));
        connect(findDialog, SIGNAL(findPrevious(QRegExp)), this, SLOT(findPrevious(QRegExp)));
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

/*
 * @brief: find the word before current position
 * @param: none
 * @return: reference to the word needed to be found
 */
void NotePad::findPrevious(const QRegExp &r)
{
    if (text)
    {
        bool findSucc = text->find(r, QTextDocument::FindBackward);
        if (!findSucc)
            QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(r.pattern()), QMessageBox::StandardButton::Ok);
    }
}

/*
 * @brief: find the word after current position
 * @param: none
 * @return: reference to the word needed to be found
 */
void NotePad::findNext(const QRegExp &r)
{
    if (text)
    {
        bool findSucc = text->find(r);
        if (!findSucc)
            QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(r.pattern()), QMessageBox::StandardButton::Ok);
    }
}

/*
 * @brief: regular expression matching before the current position(Overloaded)
 * @param: none
 * @return: reference to the word needed to be found and the form of regular expression
 */
void NotePad::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    QFlags<QTextDocument::FindFlag> flag = (cs == Qt::CaseInsensitive) ? QTextDocument::FindBackward : (QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
    if (text)
    {
        bool findSucc = text->find(str, flag);
        if (!findSucc)
            QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(str), QMessageBox::StandardButton::Ok);
    }
}

/*
 * @brief: regular expression matching after the current position(Overloaded)
 * @param: none
 * @return: reference to the word needed to be found and the form of regular expression
 */
void NotePad::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    QFlags<QTextDocument::FindFlag> flag = static_cast<QTextDocument::FindFlag>((cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively);
    if (text)
    {
        bool findSucc = text->find(str, flag);
        if (!findSucc)
            QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(str), QMessageBox::StandardButton::Ok);
    }
}

// replace part is similar to find: find them and replace it with certain word
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
    QFlags<QTextDocument::FindFlag> flag = static_cast<QTextDocument::FindFlag>((cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively);
    qDebug() << "Replace Next";
	bool findSucc = text->find(find, flag);
	if (!findSucc)
	{
		if(showMsg)
			QMessageBox::information(this, tr("NotePad"), tr("Can't find \"%1\".").arg(find), QMessageBox::StandardButton::Ok);
		return false;
	}
	text->textCursor().deleteChar();
	text->textCursor().insertText(replace);
	return true;
}

void NotePad::replaceAll(const QString &find, const QString &replace, Qt::CaseSensitivity cs)
{
    QFlags<QTextDocument::FindFlag> flag = static_cast<QTextDocument::FindFlag>((cs == Qt::CaseInsensitive) ? 0 : QTextDocument::FindCaseSensitively);
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
    QFont font = QFontDialog::getFont(nullptr, text->currentFont(), this, tr("Set Font"));
    if (text)
    {
        QTextCursor curs = text->textCursor();
        QTextCharFormat format = curs.charFormat();
        curs.select(QTextCursor::Document);
        format.setFont(font);
        curs.mergeCharFormat(format);
    }
    QTextBrowser *browser = browserDialog->browser();
    QTextCursor curs = browser->textCursor();
    QTextCharFormat format = curs.charFormat();
    curs.select(QTextCursor::Document);
    format.setFont(font);
    curs.mergeCharFormat(format);

    fontSizeSpinBox->setValue(font.pointSize());
}

void NotePad::showColorDialog()
{
    text->setTextColor(QColorDialog::getColor(text->textColor(), this, tr("Set Font Color")));
}

/*
 * @brief: judge the type of current file and compile it with certain compliler
 * @param: none
 * @return: the status of compiling procedure
 */
bool NotePad::compileAndRun()
{
    QFileInfo fileInfo(curFile);
    QString compiler;
    QString runner;
    switch (fileType) {
    case CFile:
        compiler = "gcc ";
        break;
    case CppFile:
        compiler = "g++ ";
        break;
    case JavaFile:
        compiler = "javac ";
        runner = "java ";
        break;
    case ObCFile:
        compiler = "gcc -framework Cocoa ";
        break;
    case PHPFile:
        runner = "php ";
        break;
    case PythonFile:
        runner = "python -u ";
        break;
    case PerlFile:
        runner = "perl ";
        break;
    case GoFile:
        runner = "go run ";
        break;
    case BatFile:
        runner = "cmd /c ";
        break;
    case RubyFile:
        runner = "ruby ";
        break;
    case HaskellFile:
        runner = "runhaskell ";
        break;
    case RustFile:
        compiler = "rustc ";
        break;
    default:
        QMessageBox::warning(this, tr("Warning"), tr("This file is not supported to be compiled and run!"), QMessageBox::Ok);
        return false;
    }
    if (okToCloseTab(mainWidget->currentIndex()))
    {
        QString cmd;
        switch (fileType)
        {
        case CFile: case CppFile: case ObCFile:
            cmd = "cd /d " + fileInfo.path() + " && " +
                compiler + fileInfo.fileName() + " -o " + fileInfo.baseName() + " && " +
                fileInfo.baseName() + " && pause";
            break;
        case JavaFile:
            cmd = "cd /d " + fileInfo.path() + " && " +
                compiler + fileInfo.fileName() + " && " +
                runner + fileInfo.baseName() + " && pause";
            break;
        case PHPFile: case PythonFile: case PerlFile: case GoFile: case BatFile: case RubyFile: case HaskellFile: case RustFile:
            cmd = "cd /d " + fileInfo.path() + " && " +
                runner + fileInfo.fileName() + " && pause";
            break;
        default:
            break;
        }

        if (system(cmd.toLatin1().data()))
        {
            QMessageBox::warning(this, tr("Error"), tr("Error while compiling or running!"), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    return false;
}

/*
 * @brief: judge the filetype and view the HtmlFile with a browser
 * @param: none
 * @return: void
 */
void NotePad::viewHtml()
{
    QString htmlText;
    if(fileType != HTMLFile)
    {
        QMessageBox::warning(this, tr("Warning"), tr("This file isn't an HTML file."), QMessageBox::Ok);
        return;
    }
    if(fileType == HTMLFile)
    {
        if (!okToCloseTab(mainWidget->currentIndex()))
            return;
        htmlText = text->toPlainText();
    }
    showBrowser(htmlText);
}

/*
 * @brief: judge the filetype and view the mdFile with a browser
 * @param: none
 * @return: void
 */void NotePad::viewMarkdown()
{
    QString htmlText;
    TextEdit *textEdit = qobject_cast<TextEdit*>(mainWidget->currentWidget());
    QString fileName = textEdit->currentFile();
    if(fileType != MarkdownFile)
    {
        QMessageBox::warning(this, tr("Warning"), tr("This file isn't an Markdown file."), QMessageBox::Ok);
        return;
    }
    if(fileType == MarkdownFile)
    {
        if (!okToCloseTab(mainWidget->currentIndex()))
            return;
        ViewMarkdown(fileName.toStdString());
        qDebug()<<fileName;
        QFile html(QFileInfo(fileName).baseName() + ".html");
        html.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream In(&html);
        htmlText = In.readAll();
    }
    showBrowser(htmlText);
}
void NotePad::showBrowser(const QString &htmlText)
{
    browserDialog->setCurrentFont(text->currentFont());
    browserDialog->setFont(text->currentFont());
    browserDialog->browser()->setFont(text->currentFont());
    browserDialog->setText(htmlText);
    browserDialog->show();
    qDebug() << "View HTML";
}
void NotePad::on_codecAction_triggered()
{
    QAction *codecAction = qobject_cast<QAction *>(sender());
    qDebug() << "Set codec: " << codecAction->text();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codecAction->text().toLatin1()));
    open(curFile);
}

/*
 * @brief: set the tab user is currently using
 * @param: none
 * @return: current index of mainWidget
 */
void NotePad::setCurrentTab(int index)
{
    //Q_STATIC_ASSERT(text == mainWidget->widget(index));
    disconnectTextEdit();
    if (index >= 0)
        text = qobject_cast<TextEdit*>(mainWidget->widget(index));
    else
        text = nullptr;
    if(text)
    {
        setCurrentFile(text->currentFile());
        connectTextEdit();
    }
    else
    {
        setCurrentFile(QString());
        setWindowModified(false);
    }
}

/*
 * @brief: close a tab and the corresponding file
 * @param: current index of mainWidget
 * @return: status of closing the tab
 */
bool NotePad::closeTab(int index)
{
    if(!okToCloseTab(index))
        return false;
    TextEdit *oldText = qobject_cast<TextEdit*>(mainWidget->widget(index));
    //assert(text == mainWidget->widget(index));
    mainWidget->removeTab(index);
    if (oldText == text)
        setCurrentTab(mainWidget->currentIndex());
    delete oldText;
    updateModified();
    if (mainWidget->count() == 0)
    {
        saveAction->setEnabled(false);
        saveasAction->setEnabled(false);
        undoAction->setEnabled(false);
        copyAction->setEnabled(false);
        pasteAction->setEnabled(false);
        cutAction->setEnabled(false);
        findAction->setEnabled(false);
        replaceAction->setEnabled(false);
    }
    return true;
}
/*
 * @brief: update the modification of each widget
 * @param: none
 * @return: void
 */
void NotePad::updateModified()
{
    for (int i = 0; i < mainWidget->count(); ++i)
    {
        TextEdit *textEdit = qobject_cast<TextEdit*>(mainWidget->widget(i));
        if (textEdit->document()->isModified())
        {
            setWindowModified(true);
            return;
        }
    }
    setWindowModified(false);
}

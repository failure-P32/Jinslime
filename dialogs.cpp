

#include "dialogs.h"

/*
 * @brief: This function is the constructor of FindDialog.
 * @param: The pointer to the QWidget Class.
 * @return: void.
 */
FindDialog::FindDialog(QWidget *parent)
	:QDialog(parent)
{
	lineEdit = new QLineEdit(this);
	findButton = new QPushButton(tr("&Find"), this);
	findButton->setEnabled(false);
	findButton->setDefault(true);
	cancelButton = new QPushButton(tr("&Cancel"), this);
	label = new QLabel(tr("Find &what:"), this);
	label->setBuddy(lineEdit);
	caseCheckBox = new QCheckBox(tr("C&ase sensitive"), this);
    regExpCheckBox = new QCheckBox(tr("Reg Expression"), this);
	directionGroupBox = new QGroupBox(tr("Direction"), this);
	nextCheckBox = new QCheckBox(tr("&Next"), this);
	previousCheckBox = new QCheckBox(tr("&Previous"), this);
	directionButtonGroup = new QButtonGroup(this);
	directionButtonGroup->addButton(nextCheckBox);
	directionButtonGroup->addButton(previousCheckBox);
	directionButtonGroup->setExclusive(true);
	nextCheckBox->setChecked(true);

    // connect click fine & cancel signals to their slot
	connect(findButton, SIGNAL(clicked()), this, SLOT(findButtonClicked()));
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    // create the horizontal or verticle box objects for the find widget
	QHBoxLayout *directionLayout = new QHBoxLayout;
	directionLayout->addWidget(nextCheckBox);
	directionLayout->addWidget(previousCheckBox);
	directionGroupBox->setLayout(directionLayout);
	QHBoxLayout *topLeftLayout = new QHBoxLayout;
	topLeftLayout->addWidget(label);
	topLeftLayout->addWidget(lineEdit);
    QVBoxLayout *downLeftCheckBoxLayout = new QVBoxLayout;
    downLeftCheckBoxLayout->addWidget(caseCheckBox);
    downLeftCheckBoxLayout->addWidget(regExpCheckBox);
	QHBoxLayout *downLeftLayout = new QHBoxLayout;
    downLeftLayout->addLayout(downLeftCheckBoxLayout);
	downLeftLayout->addStretch();
	downLeftLayout->addWidget(directionGroupBox);
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addLayout(topLeftLayout);
	leftLayout->addLayout(downLeftLayout);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(findButton);
	rightLayout->addWidget(cancelButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);
}

/*
 * @brief: the function is to sense the click option of "find" button
 * @param: none
 * @return: void
 */
void FindDialog::findButtonClicked()
{
	Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
	if (nextCheckBox->isChecked())
    {
        if (regExpCheckBox->isChecked())
            emit findNext(QRegExp(lineEdit->text(), cs));
        else
            emit findNext(lineEdit->text(), cs);
    }
	else
    {
        if (regExpCheckBox->isChecked())
            emit findPrevious(QRegExp(lineEdit->text(), cs));
        else
            emit findPrevious(lineEdit->text(), cs);
    }
}

void FindDialog::enableFindButton()
{
	findButton->setEnabled(!lineEdit->text().isEmpty());
}

/*
 * @brief: This function is the constructor of ReplaceDialog.
 * @param: The pointer to the QWidget Class.
 * @return: void.
 */
ReplaceDialog::ReplaceDialog(QWidget *parent)
	:QDialog(parent)
{
	contentEdit = new QLineEdit(this);
	contentLabel = new QLabel(tr("Find &what:"), this);
	contentLabel->setBuddy(contentEdit);
	replaceEdit = new QLineEdit(this);
	replaceLabel = new QLabel(tr("&Repalce by:"), this);
	replaceLabel->setBuddy(replaceEdit);

	findButton = new QPushButton(tr("&Find next"), this);
	replaceNextButton = new QPushButton(tr("Replace &next"), this);
	replaceAllButton = new QPushButton(tr("Replace &all"), this);
	findButton->setEnabled(false);
	replaceNextButton->setEnabled(false);
	replaceAllButton->setEnabled(false);
	cancelButton = new QPushButton(tr("&Cancel"), this);
	caseCheckBox = new QCheckBox(tr("Case &sensitive"), this);

    // connect click fine &  replace & cancelbutton & contentedit & replace edit signals to their slot
    connect(findButton, SIGNAL(clicked()), this, SLOT(findButtonClicked()));
	connect(replaceNextButton, SIGNAL(clicked()), this, SLOT(replaceNextButtonClicked()));
	connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(replaceAllButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(contentEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton()));
	connect(replaceEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableReplaceButtons()));

    // create the horizontal or verticle box objects for the replace widget
	QHBoxLayout *contentEditLayout = new QHBoxLayout;
	contentEditLayout->addWidget(contentLabel);
	contentEditLayout->addWidget(contentEdit);
	QHBoxLayout *replaceEditLayout = new QHBoxLayout;
	replaceEditLayout->addWidget(replaceLabel);
	replaceEditLayout->addWidget(replaceEdit);
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addLayout(contentEditLayout);
	leftLayout->addLayout(replaceEditLayout);
	leftLayout->addStretch();
	leftLayout->addWidget(caseCheckBox);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(findButton);
	rightLayout->addWidget(replaceNextButton);
	rightLayout->addWidget(replaceAllButton);
	rightLayout->addWidget(cancelButton);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);
}

// several functions of the replace widget
void ReplaceDialog::findButtonClicked()
{
	Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
	emit findNext(contentEdit->text(), cs);
}
void ReplaceDialog::replaceNextButtonClicked()
{
	Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit replaceNext(contentEdit->text(), replaceEdit->text(), cs, true);
}
void ReplaceDialog::replaceAllButtonClicked()
{
	Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
	emit replaceAll(contentEdit->text(), replaceEdit->text(), cs);
}
void ReplaceDialog::enableFindButton()
{
	findButton->setEnabled(!contentEdit->text().isEmpty());
}
void ReplaceDialog::enableReplaceButtons()
{
	replaceNextButton->setEnabled(!replaceEdit->text().isEmpty());
	replaceAllButton->setEnabled(!replaceEdit->text().isEmpty());
}

BrowserDialog::BrowserDialog(QWidget *parent)
    :QDialog(parent), b(new QTextBrowser(this))
{
    qDebug()<<"browserdialog";
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(b);
    setLayout(layout);
}

/*
 * @brief: set the current text of the file
 * @param: const reference to the QString text
 * @return: none
 */
void BrowserDialog::setText(const QString &text)
{
    b->setHtml(text);
}

/*
 * @brief: set the current font of the file
 * @param: const reference to the QFont font
 * @return: none
 */
void BrowserDialog::setCurrentFont(const QFont &font)
{
    b->setCurrentFont(font);
}

QTextBrowser *BrowserDialog::browser() const
{
    return b;
}

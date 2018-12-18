#pragma once
#include <QtWidgets>
class FindDialog
	:public QDialog
{
	Q_OBJECT
public:
	FindDialog(QWidget *parent = nullptr);

signals:
	void findPrevious(const QString &, Qt::CaseSensitivity);
	void findNext(const QString &, Qt::CaseSensitivity);

private:
	QLineEdit *lineEdit;
	QPushButton *findButton;
	QPushButton *cancelButton;
	QLabel *label;
	QCheckBox *caseCheckBox;
	QGroupBox *directionGroupBox;
	QCheckBox *nextCheckBox;
	QCheckBox *previousCheckBox;
	QButtonGroup *directionButtonGroup;

private slots:
	void findButtonClicked();
	void enableFindButton();
};

class ReplaceDialog
	:public QDialog
{
	Q_OBJECT
public:
	ReplaceDialog(QWidget *parent = nullptr);

signals:
	void findNext(const QString &, Qt::CaseSensitivity);
    void replaceNext(const QString &, const QString &, Qt::CaseSensitivity, bool);
	void replaceAll(const QString &, const QString &, Qt::CaseSensitivity);

private:
	QLineEdit *contentEdit;
	QLineEdit *replaceEdit;
	QPushButton *findButton;
	QPushButton *replaceNextButton;
	QPushButton *replaceAllButton;
	QPushButton *cancelButton;
	QLabel *contentLabel;
	QLabel *replaceLabel;
	QCheckBox *caseCheckBox;

private slots:
	void findButtonClicked();
	void replaceNextButtonClicked();
	void replaceAllButtonClicked();
	void enableFindButton();
	void enableReplaceButtons();
};

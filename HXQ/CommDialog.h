#ifndef COMMDIALOG_H
#define COMMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "TestDialog.h"

namespace Ui {
class CommDialog;
}

class CommDialog : public QDialog
{
    Q_OBJECT

public:
	explicit CommDialog(QWidget *parent = 0);
	~CommDialog();

private:
	Ui::CommDialog *ui;

	void setBtnQss(QPushButton *btn,
		QString normalColor, QString normalTextColor,
		QString hoverColor, QString hoverTextColor,
		QString pressedColor,QString pressedTextColor);

	void setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor);

public slots:
	void ChangeStyle();
	void DebugDialog();
	void ChangeMode(int mode);
	void start();
	void stop();

};

#endif // CommDialog_H

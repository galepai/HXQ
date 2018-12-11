#ifndef ENGINEERDIALOG_H
#define ENGINEERDIALOG_H

#include <QDialog>

namespace Ui {
class EngineerDialog;
}

class EngineerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EngineerDialog(QWidget *parent = 0);
    ~EngineerDialog();

	bool correct() const { return m_bCorrect; }
	void setCorrect(bool corrent) { m_bCorrect = corrent; }

public slots:
	void login();
	

private:
    Ui::EngineerDialog *ui;
	bool m_bCorrect;
};

#endif // ENGINEERDIALOG_H

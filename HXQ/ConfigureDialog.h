#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>


namespace Ui {
class ConfigureDialog;
}

class ConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDialog(QWidget *parent = 0);
    ~ConfigureDialog();

private:
    Ui::ConfigureDialog *ui;

private slots:
	void currentRowChanged(int currentRow);
};


#endif // CONFIGUREDIALOG_H

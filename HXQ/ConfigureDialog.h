#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>

class ConfigSerial : public QWidget
{
	Q_OBJECT

public:
	explicit ConfigSerial(QWidget *parent = 0);

	void CreateUi();

	void ReadIni();

private slots:
	void SavePortToIni();


private:
	QComboBox* PortComboBox;
	QComboBox* BaudComboBox;
	QComboBox* DataBitsComboBox;

};

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
	ConfigSerial configSerial;

private slots:
	void currentRowChanged(int currentRow);
};


#endif // CONFIGUREDIALOG_H

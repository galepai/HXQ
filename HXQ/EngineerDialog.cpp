#include "EngineerDialog.h"
#include "ui_EngineerDialog.h"
#include "Func.h"

EngineerDialog::EngineerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EngineerDialog), m_bCorrect(false)
{
    ui->setupUi(this);
	//
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(login()));
	
	
}

EngineerDialog::~EngineerDialog()
{
    delete ui;
}

void EngineerDialog::login()
{

	if (ui->lineEdit->text() == QString("ycgd"))
	{
		m_bCorrect = true;
		this->close();
	}
	else
	{
		ui->label_error->setText(G2U("ÃÜÂë´íÎó£¡"));
	}

}


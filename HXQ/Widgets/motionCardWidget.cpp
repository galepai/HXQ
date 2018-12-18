#include "motionCardWidget.h"
#include "ui_motionCardWidget.h"
#include "Func.h"
#include <QMessageBox>

motionCardWidget::motionCardWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::motionCardWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveMotionCardToIni()));
	ReadIni();
}

motionCardWidget::~motionCardWidget()
{
    delete ui;
}

void motionCardWidget::ReadIni()
{

	QVariant Value;
	ReadConfigure("config.ini", "MotionCard", "Ip", Value);
	ui->lineEdit_Ip->setText(Value.toString());
	
	//ReadConfigure("config.ini", "Raw", "Raw_Path", Value);
	//lineEdit_Raw_Path->setText(Value.toString());

}

void motionCardWidget::SaveMotionCardToIni()
{

	//double time_Start = (double)clock();

	if (ui->lineEdit_Ip->text()!="")
	{
		WriteConfigure("config.ini", "MotionCard", "Ip", ui->lineEdit_Ip->text());
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("���ƿ���Ϣ�ɹ�д�������ļ�"));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("��Ϣ"), G2U("���ƿ���Ϣд��ʧ�ܣ����������Ƿ���ȷ��"));
	}

}
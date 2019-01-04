#include "motionCardWidget.h"
#include "ui_motionCardWidget.h"
#include "Func.h"
#include "ConstParam.h"
#include "GalilThread.h"
#include <QMessageBox>

motionCardWidget::motionCardWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::motionCardWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveMotionCardToIni()));
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(OnTest()));
	ReadIni();
}

motionCardWidget::~motionCardWidget()
{
    delete ui;
}

void motionCardWidget::ReadIni()
{

	//QVariant Value;
	//ReadConfigure("config.ini", "MotionCard", "Ip", Value);
	//ui->lineEdit_Ip->setText(Value.toString());
	
	
	QString type, value;

	ReadXmlElementText(QString(XML_MotionCard), QString(Node_MotionCard), QString(MotionCard_ip), type, value);
	ui->lineEdit_Ip->setText(value);

}

void motionCardWidget::SaveMotionCardToIni()
{

	//double time_Start = (double)clock();

	//if (ui->lineEdit_Ip->text()!="")
	//{
	//	WriteConfigure("config.ini", "MotionCard", "Ip", ui->lineEdit_Ip->text());
	//	QMessageBox::StandardButton reply;
	//	reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("���ƿ���Ϣ�ɹ�д�������ļ�"));
	//}
	//else
	//{
	//	QMessageBox::StandardButton reply;
	//	reply = QMessageBox::warning(this, G2U("��Ϣ"), G2U("���ƿ���Ϣд��ʧ�ܣ����������Ƿ���ȷ��"));
	//}


	if (UpdateXmlNodeText(QString(XML_MotionCard), QString(Node_MotionCard), QString(MotionCard_ip), ui->lineEdit_Ip->text()) == ChhXml::UpdateOK)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("�ع�����ɹ�д�������ļ�"));
	}

}

void motionCardWidget::OnTest()
{

	Galil_Thread galil;
	QString Revision;
	if(galil.Open(ui->lineEdit_Ip->text()))
	{
		galil.CmdT("\x12\x16", Revision);
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("���ƿ���Ϣ"), Revision + (G2U("\r\n���ӳɹ���")));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("��Ϣ"), G2U("���ƿ�����ʧ��"));
	}

}

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
	//	reply = QMessageBox::information(this, G2U("信息"), G2U("控制卡信息成功写入配置文件"));
	//}
	//else
	//{
	//	QMessageBox::StandardButton reply;
	//	reply = QMessageBox::warning(this, G2U("信息"), G2U("控制卡信息写入失败，请检测设置是否正确！"));
	//}


	if (UpdateXmlNodeText(QString(XML_MotionCard), QString(Node_MotionCard), QString(MotionCard_ip), ui->lineEdit_Ip->text()) == ChhXml::UpdateOK)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
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
		reply = QMessageBox::information(this, G2U("控制卡信息"), Revision + (G2U("\r\n连接成功！")));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("信息"), G2U("控制卡连接失败"));
	}

}

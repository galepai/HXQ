#include "ComDialog.h"
#include "ui_ComDialog.h"
#include "DeltaThread.h"
#include "Func.h"

ComDialog::ComDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::ComDialog)
{
    ui->setupUi(this);

	connect(ui->QPushButton_OpenComm, SIGNAL(clicked()), this, SLOT(OpenCommThread()));
	connect(ui->QPushButton_Send_SencondQueue, SIGNAL(clicked()), this, SLOT(Send())); 
	connect(ui->QPushButton_TimeLag, SIGNAL(clicked()), this, SLOT(SetTimeLag()));
	connect(ui->lineEdit_Slave, SIGNAL(textChanged(const QString &)), this, SLOT(GenLRC(const QString &)));
	connect(ui->lineEdit_FunctionCode, SIGNAL(textChanged(const QString &)), this, SLOT(GenLRC(const QString &)));
	connect(ui->lineEdit_StartAddress, SIGNAL(textChanged(const QString &)), this, SLOT(GenLRC(const QString &)));
	connect(ui->lineEdit_OtherInfo, SIGNAL(textChanged(const QString &)), this, SLOT(GenLRC(const QString &)));
	if (Delta_Thread::GetSerialPort())
		ui->QPushButton_OpenComm->setText(G2U("串口已打开"));

}

ComDialog::~ComDialog()
{
    delete ui;
}


void ComDialog::OpenCommThread()
{
	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();


	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::OneQueryToDefalutQuene);

	if (Delta_Thread::GetSerialPort() == nullptr)
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));  
		thread->start();
		ui->QPushButton_OpenComm->setText(G2U("串口已打开"));
	}
	
}

void ComDialog::Send()
{	
	QString data = ui->lineEdit_Slave->text() + ui->lineEdit_FunctionCode->text() + ui->lineEdit_StartAddress->text() + ui->lineEdit_OtherInfo->text();

	if (!data.isEmpty())
	{
		Delta_Thread::AddOneQueueInfo(data.toUpper().toStdString());
	}
		
}

//void ComDialog::GenLRC()
//{
//	ui->lineEdit_Slave->setText(ui->lineEdit_Slave->text().toUpper());
//	ui->lineEdit_FunctionCode->setText(ui->lineEdit_FunctionCode->text().toUpper());
//	ui->lineEdit_StartAddress->setText(ui->lineEdit_StartAddress->text().toUpper());
//	ui->lineEdit_OtherInfo->setText(ui->lineEdit_OtherInfo->text().toUpper());
//
//	QString data = ui->lineEdit_Slave->text() + ui->lineEdit_FunctionCode->text() + ui->lineEdit_StartAddress->text() + ui->lineEdit_OtherInfo->text();
//	ui->lineEdit_LRC->setText(Gen_Delta_Ascii_CR(data.toUpper().toStdString()).c_str());
//}

void ComDialog::readyDataSlot(QByteArray)
{

}

void ComDialog::GenLRC(const QString & str)
{
	ui->lineEdit_Slave->setText(ui->lineEdit_Slave->text().toUpper());
	ui->lineEdit_FunctionCode->setText(ui->lineEdit_FunctionCode->text().toUpper());
	ui->lineEdit_StartAddress->setText(ui->lineEdit_StartAddress->text().toUpper());
	ui->lineEdit_OtherInfo->setText(ui->lineEdit_OtherInfo->text().toUpper());

	QString data = ui->lineEdit_Slave->text() + ui->lineEdit_FunctionCode->text() + ui->lineEdit_StartAddress->text() + ui->lineEdit_OtherInfo->text();
	ui->lineEdit_LRC->setText(Gen_Delta_Ascii_CR(data.toUpper().toStdString()).c_str());

}

void ComDialog::SetTimeLag()
{
	int time = ui->lineEdit_TimeLag->text().toInt();
	Delta_Thread::SetTimeLag(time);
}
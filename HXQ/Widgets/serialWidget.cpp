#include "serialWidget.h"
#include "ui_serialWidget.h"
#include "Func.h"
#include <QtSerialPort\QSerialPortInfo>
#include <QMessageBox>

serialWidget::serialWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::serialWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	CreateData();
	ReadIni();
}

serialWidget::~serialWidget()
{
    delete ui;
}

void serialWidget::ReadIni()
{

	QVariant Value;
	ReadConfigure("config.ini", "Port", "Port", Value);
	int index = ui->PortComboBox->findText(Value.toString());
	ui->PortComboBox->setCurrentIndex(index);

	ReadConfigure("config.ini", "Port", "Baud", Value);
	index = ui->BaudComboBox->findText(Value.toString());
	ui->BaudComboBox->setCurrentIndex(index);

	ReadConfigure("config.ini", "Port", "DataBits", Value);
	index = ui->DataBitsComboBox->findText(Value.toString());
	ui->DataBitsComboBox->setCurrentIndex(index);
	
	//ReadConfigure("config.ini", "Raw", "Raw_Path", Value);
	//lineEdit_Raw_Path->setText(Value.toString());

}

void serialWidget::SavePortToIni()
{

	//double time_Start = (double)clock();

	if (ui->PortComboBox->currentText().length()
		&& ui->BaudComboBox->currentText().length()
		&& ui->DataBitsComboBox->currentText().length())
	{
		WriteConfigure("config.ini", "Port", "Port", ui->PortComboBox->currentText());
		WriteConfigure("config.ini", "Port", "Baud", ui->BaudComboBox->currentText());
		WriteConfigure("config.ini", "Port", "DataBits", ui->DataBitsComboBox->currentText());
		//double time_End = (double)clock();

		//qDebug() << (time_End - time_Start) / 1000.0 << "s";
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("串口信息成功写入配置文件"));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("信息"), G2U("串口信息写入失败，请检测设置是否正确！"));
	}

}

void serialWidget::CreateData()
{
	QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
	if (infos.isEmpty())
	{
		ui->PortComboBox->addItem("");
		return;
	}
	foreach(QSerialPortInfo info, infos)
	{
		ui->PortComboBox->addItem(info.portName());
	}

	ui->BaudComboBox->addItem("4800");
	ui->BaudComboBox->addItem("9600");
	ui->BaudComboBox->addItem("19200");

	ui->DataBitsComboBox->addItem("6");
	ui->DataBitsComboBox->addItem("7");
	ui->DataBitsComboBox->addItem("8");

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SavePortToIni()));

}

#include "ConfigureDialog.h"
#include "ui_ConfigureDialog.h"
#include "Func.h"
#include <QtSerialPort\QSerialPortInfo>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include "Widgets\motionCardWidget.h"
#include "Widgets\cameraWidget.h"
#include "Widgets\modelWidget.h"

ConfigureDialog::ConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));

	ui->listWidget->setViewMode(QListView::IconMode);
	ui->listWidget->setIconSize(QSize(96, 84));
	ui->listWidget->setMovement(QListView::Static);
	//ui->listWidget->setMaximumWidth(128);
	ui->listWidget->setSpacing(12);
	

	QListWidgetItem *configButton = new QListWidgetItem(ui->listWidget);
	configButton->setIcon(QIcon(":/icon/Resources/icon/01.ico"));
	configButton->setText(G2U("运动控制"));
	configButton->setTextAlignment(Qt::AlignHCenter);
	configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *configButton2 = new QListWidgetItem(ui->listWidget);
	configButton2->setIcon(QIcon(":/icon/Resources/icon/03.ico"));
	configButton2->setText(G2U("曝光设置"));
	configButton2->setTextAlignment(Qt::AlignHCenter);
	configButton2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *configButton3 = new QListWidgetItem(ui->listWidget);
	configButton3->setIcon(QIcon(":/icon/Resources/icon/05.ico"));
	configButton3->setText(G2U("图像处理"));
	configButton3->setTextAlignment(Qt::AlignHCenter);
	configButton3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	ui->stackedWidget->addWidget(new motionCardWidget);
	ui->stackedWidget->addWidget(new cameraWidget);
	ui->stackedWidget->addWidget(new modelWidget);

	connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));

	HIDDLE_DIALOG_BUTTON
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::currentRowChanged(int currentRow)
{
	ui->stackedWidget->setCurrentIndex(currentRow);
}

ConfigSerial::ConfigSerial(QWidget *parent) :
	QWidget(parent)
{
	
	PortComboBox = new QComboBox();
	BaudComboBox = new QComboBox();
	DataBitsComboBox = new QComboBox();

	CreateUi();
	ReadIni();

}

void ConfigSerial::ReadIni()
{

	QVariant Value;
	ReadConfigure("config.ini", "Port", "Port", Value);
	int index = PortComboBox->findText(Value.toString());
	PortComboBox->setCurrentIndex(index);

	ReadConfigure("config.ini", "Port", "Baud", Value);
	index = BaudComboBox->findText(Value.toString());
	BaudComboBox->setCurrentIndex(index);

	ReadConfigure("config.ini", "Port", "DataBits", Value);
	index = DataBitsComboBox->findText(Value.toString());
	DataBitsComboBox->setCurrentIndex(index);

	//ReadConfigure("config.ini", "Raw", "Raw_Path", Value);
	//lineEdit_Raw_Path->setText(Value.toString());

}

void ConfigSerial::SavePortToIni()
{

	double time_Start = (double)clock();

	if (PortComboBox->currentText().length()
		&& BaudComboBox->currentText().length()
		&& DataBitsComboBox->currentText().length())
	{
		WriteConfigure("config.ini", "Port", "Port", PortComboBox->currentText());
		WriteConfigure("config.ini", "Port", "Baud", BaudComboBox->currentText());
		WriteConfigure("config.ini", "Port", "DataBits", DataBitsComboBox->currentText());
		double time_End = (double)clock();

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("串口信息成功写入配置文件"));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("信息"), G2U("串口信息写入失败，请检测设置是否正确！"));
	}

}

void ConfigSerial::CreateUi()
{
	QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
	if (infos.isEmpty())
	{
		PortComboBox->addItem("");
		return;
	}
	foreach(QSerialPortInfo info, infos)
	{
		PortComboBox->addItem(info.portName());
	}

	BaudComboBox->addItem("4800");
	BaudComboBox->addItem("9600");
	BaudComboBox->addItem("19200");

	DataBitsComboBox->addItem("6");
	DataBitsComboBox->addItem("7");
	DataBitsComboBox->addItem("8");

	QGroupBox *configGroup = new QGroupBox(G2U("串口设置"));

	QHBoxLayout* portLayout = new QHBoxLayout();
	QLabel* portLabel = new QLabel(G2U("端口号:"));
	portLayout->addWidget(portLabel);
	portLayout->addWidget(PortComboBox);

	QHBoxLayout* baudLayout = new QHBoxLayout();
	QLabel* baudLabel = new QLabel(G2U("波特率:"));
	baudLayout->addWidget(baudLabel);
	baudLayout->addWidget(BaudComboBox);

	QHBoxLayout* dataBitsLayout = new QHBoxLayout();
	QLabel* dataBitsLabel = new QLabel(G2U("数据位:"));
	dataBitsLayout->addWidget(dataBitsLabel);
	dataBitsLayout->addWidget(DataBitsComboBox);

	QVBoxLayout* groupLayout = new QVBoxLayout();
	groupLayout->addLayout(portLayout);
	groupLayout->addLayout(baudLayout);
	groupLayout->addLayout(dataBitsLayout);

	configGroup->setLayout(groupLayout);

	
	QPushButton* confirmButton = new QPushButton(G2U("设置"));
	confirmButton->setMinimumHeight(25);
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addSpacing(20);
	buttonLayout->addWidget(confirmButton);
	buttonLayout->addStretch(1);
	


	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(configGroup);
	mainLayout->addLayout(buttonLayout);
	//mainLayout->addWidget(buttonLayout);
	mainLayout->addStretch(1);

	setLayout(mainLayout);

	connect(confirmButton, SIGNAL(clicked()), this, SLOT(SavePortToIni()));

}
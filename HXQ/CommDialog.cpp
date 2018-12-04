#include "CommDialog.h"
#include "ui_CommDialog.h"
#include "DeltaThread.h"
#include "Func.h"
#include <QPushButton>
#include <QLineEdit>

CommDialog::CommDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::CommDialog)
{
    ui->setupUi(this);

	/*setBtnQss(ui->btn1, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
	setBtnQss(ui->btn2, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
	setBtnQss(ui->btn3, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");*/
	//setTxtQss(ui->lineEdit, "#DCE4EC", "#34495E");
	connect(ui->btn_skin, SIGNAL(clicked()), this, SLOT(ChangeStyle()));
	connect(ui->btn_debug, SIGNAL(clicked()), this, SLOT(DebugDialog()));
	connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui->verticalSlider_mode, SIGNAL(valueChanged(int)), this, SLOT(ChangeMode(int)));
	
	if (Delta_Thread::GetSerialPort() == nullptr)
	{
		ui->btn_stop->setEnabled(false);
		ui->btn_start->setEnabled(true);
		ui->btn_debug->setEnabled(false);
	}
		
	else
	{
		ui->btn_stop->setEnabled(true);
		ui->btn_start->setEnabled(false);
		ui->btn_debug->setEnabled(false);
	}
}


void CommDialog::start()
{
	ui->btn_start->setEnabled(false);
	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	Delta_Thread::AddDefaultQueueInfo("00050500FF00");
	Delta_Thread::AddDefaultQueueInfo("00050501FF00");
	Delta_Thread::AddDefaultQueueInfo("00050502FF00");
	Delta_Thread::AddDefaultQueueInfo("00050503FF00");
	Delta_Thread::AddDefaultQueueInfo("00050504FF00");

	if (Delta_Thread::GetSerialPort() == nullptr)
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		//connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		thread->start();
		ui->btn_stop->setEnabled(true);
		ui->btn_debug->setEnabled(false);
		ui->verticalSlider_mode->setEnabled(false);
	}

}

void CommDialog::stop()
{
	Delta_Thread::StopRun(true);

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->verticalSlider_mode->setEnabled(true);
}

void CommDialog::ChangeMode(int mode)
{
	if (mode == 1)
	{
		ui->btn_debug->setEnabled(false);
		ui->btn_start->setEnabled(true);
	}
	else
	{
		ui->btn_debug->setEnabled(true);
		ui->btn_start->setEnabled(false);
	}
		
}

void CommDialog::DebugDialog()
{
	TestDialog dlg(this);
	dlg.exec();
}

void CommDialog::ChangeStyle()
{
	QFile qss("Resources/qss/css.h");
	if (qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
}

CommDialog::~CommDialog()
{
    delete ui;
}

void CommDialog::setBtnQss(QPushButton *btn,
	QString normalColor, QString normalTextColor,
	QString hoverColor, QString hoverTextColor,
	QString pressedColor, QString pressedTextColor)

{
	QStringList qss;
	qss.append(QString("QPushButton{border-style:none;padding:10px;border-radius:5px;color:%1;background:%2;}").arg(normalTextColor).arg(normalColor));
	qss.append(QString("QPushButton:hover{color:%1;background:%2;}").arg(hoverTextColor).arg(hoverColor));
	qss.append(QString("QPushButton:pressed{color:%1;background:%2;}").arg(pressedTextColor).arg(pressedColor));
	btn->setStyleSheet(qss.join(""));
}

void CommDialog::setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor)

{
	QStringList qss;
	/*QLineEdit{
		border: 1px solid rgb(41, 57, 85);   # 边框1px宽，颜色为深紫色
		border-radius: 3px;					# 给定3px边框圆角
		background : white;			# 背景色定为白色吧
		selection-background-color: green;	# 这个属性设定文本选中时的文本背景色
		font-size: 14px;  # 文本的大小
	}*/
	qss.append(QString("QLineEdit{border:1px solid rgb(41, 57, 85);\
							border-radius: 3px;\
							background: black; \
							selection-background-color: green; \
							color: %1;\
							font-size: 14px;}")
							.arg("#ff0000"));
	qss.append(QString("QLineEdit{border-style:none;padding:6px;border-radius:5px;border:2px solid %1;}").arg(normalColor));
	qss.append(QString("QLineEdit:focus{border:2px solid %1;}").arg(focusColor));
	txt->setStyleSheet(qss.join(""));

}
#include "TestDialog.h"
#include "ui_TestDialog.h"
#include "DeltaThread.h"
#include "Func.h"
#include "ConstParam.h"
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>

//#define ADD_DefaultQueueInfo Delta_Thread::AddDefaultQueueInfo(READ_Y_OUTPUT);	//读Y00-Y67

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::TestDialog)
{
    ui->setupUi(this);
	connect(ui->btn_return, SIGNAL(clicked()), this, SLOT(CloseWindow()));
	connect(ui->btn_zhuashou_left, SIGNAL(clicked()), this, SLOT(OnZhuaShouLeft()));
	connect(ui->btn_zhuashou_right, SIGNAL(clicked()), this, SLOT(OnZhuaShouRight()));
	connect(ui->btn_zhuashou_up, SIGNAL(clicked()), this, SLOT(OnZhuaShouUp()));
	connect(ui->btn_zhuashou_down, SIGNAL(clicked()), this, SLOT(OnZhuaShouDown()));
	connect(ui->btn_boshou_up, SIGNAL(clicked()), this, SLOT(OnBoShouUp()));
	connect(ui->btn_boshou_down, SIGNAL(clicked()), this, SLOT(OnBoShouDown()));
	connect(ui->btn_boshou_shuiping_up, SIGNAL(clicked()), this, SLOT(OnBoShouShiPingUp()));
	connect(ui->btn_boshou_shuiping_down, SIGNAL(clicked()), this, SLOT(OnBoShouShiPingDown()));

	connect(ui->btn_hand_open, SIGNAL(clicked()), this, SLOT(OnHandOpen()));
	connect(ui->btn_hand_close, SIGNAL(clicked()), this, SLOT(OnHandClose()));

	connect(ui->btn_protect_up, SIGNAL(clicked()), this, SLOT(OnProtectUp()));
	connect(ui->btn_protect_down, SIGNAL(clicked()), this, SLOT(OnProtectDown()));

	connect(ui->btn_vision_up, SIGNAL(clicked()), this, SLOT(OnVisionUp()));
	connect(ui->btn_vision_down, SIGNAL(clicked()), this, SLOT(OnVisionDown()));
	connect(ui->btn_rotate_start, SIGNAL(clicked()), this, SLOT(OnRotateStart()));
	connect(ui->btn_rotate_stop, SIGNAL(clicked()), this, SLOT(OnRotateStop()));

	connect(ui->btn_paoguang, SIGNAL(clicked()), this, SLOT(OnPaoGuang()));

	connect(ui->btn_manual_mode, SIGNAL(clicked()), this, SLOT(OnManualMode()));
	connect(ui->btn_auto_mode, SIGNAL(clicked()), this, SLOT(OnAutoMode()));
	connect(ui->btn_auto_start, SIGNAL(clicked()), this, SLOT(OnAutoStart()));

	connect(ui->btn_forward,SIGNAL(clicked()), this, SLOT(OnForward()));
	connect(ui->btn_backword, SIGNAL(clicked()), this, SLOT(OnBackWard()));
	connect(ui->btn_forward_2, SIGNAL(clicked()), this, SLOT(OnForward_2()));
	connect(ui->btn_backword_2, SIGNAL(clicked()), this, SLOT(OnBackWard_2()));
	connect(ui->btn_paoguang_rotate_start, SIGNAL(clicked()), this, SLOT(OnRotateStepStart()));
	connect(ui->btn_paoguang_rotate_stop, SIGNAL(clicked()), this, SLOT(OnRotateStepStop()));
	connect(ui->btn_wind_up, SIGNAL(clicked()), this, SLOT(OnWindUp()));
	connect(ui->btn_wind_down, SIGNAL(clicked()), this, SLOT(OnWindDown()));

	connect(ui->btn_distance_confirm, SIGNAL(clicked()), this, SLOT(OnDistanceConfirm()));
	connect(ui->btn_distance_confirm_2, SIGNAL(clicked()), this, SLOT(OnDistanceConfirm_2()));
	connect(ui->btn_time_confirm, SIGNAL(clicked()), this, SLOT(OnTimeConfirm()));

	connect(ui->btn_detect_end, SIGNAL(clicked()), this, SLOT(OnDetectEnd()));
	connect(ui->btn_result_good, SIGNAL(clicked()), this, SLOT(OnResultGood()));
	connect(ui->btn_result_bad, SIGNAL(clicked()), this, SLOT(OnResultBad()));


	QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		pPushButtons.at(i)->setProperty("status",i>5);
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
	m_pTimer->start(50);

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//设置默认查询队列模式
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	Delta_Thread::AddDefaultQueueInfo(READ_X_INPUT);	//读X00-X87
	Delta_Thread::AddDefaultQueueInfo(READ_Y_OUTPUT);	//读Y00-Y67

	//设置单次查询模式
	//Delta_Thread::setQueryMode(Delta_Thread::QueryMode::OneQuery);



	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(sendSerialData(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		thread->start();
	}
		
	Delta_Thread::AddOneQueueInfo(READ_TIME_AND_DISTANCE);	//读D200-D205 000310C80006

	ui->lcd_distance->setStyleSheet("color: white;");
	ui->lcd_distance_2->setStyleSheet("color: white;");
	ui->lcd_time->setStyleSheet("color: white;");
	//ui->lcd_time->setStyleSheet("border: 1px solid white; color: green; background: silver;");
	
	HIDDLE_DIALOG_BUTTON
}

void TestDialog::ChangeStyle()
{
	QFile qss("Resources/qss/css.qss");
	if (qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
}

void TestDialog::CloseWindow()
{
	Delta_Thread::StopRun(true);
	close();
}

void TestDialog::OnZhuaShouLeft()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_LEFT_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_LEFT_OFF);
}

void TestDialog::OnZhuaShouRight()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_RIGHT_ON);	
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_RIGHT_OFF);	
}

void TestDialog::OnZhuaShouUp()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_UP_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_UP_OFF);
}

void TestDialog::OnZhuaShouDown()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_DOWN_OFF);
}

void TestDialog::OnHandOpen()
{
	Delta_Thread::AddOneQueueInfo(HAND_OPEN_ON);
	Delta_Thread::AddOneQueueInfo(HAND_OPEN_OFF);
}

void TestDialog::OnHandClose()
{
	Delta_Thread::AddOneQueueInfo(HAND_CLOSE_ON);
	Delta_Thread::AddOneQueueInfo(HAND_CLOSE_OFF);
}

void TestDialog::OnBoShouUp()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_UP_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_UP_OFF);
}

void TestDialog::OnBoShouDown()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_DOWN_OFF);
}

void TestDialog::OnBoShouShiPingUp()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_UP_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_UP_OFF);
}

void TestDialog::OnBoShouShiPingDown()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_DOWN_OFF);
}

void TestDialog::OnProtectUp()
{
	Delta_Thread::AddOneQueueInfo(PROTECT_UP_ON);
	Delta_Thread::AddOneQueueInfo(PROTECT_UP_OFF);
}

void TestDialog::OnProtectDown()
{
	Delta_Thread::AddOneQueueInfo(PROTECT_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(PROTECT_DOWN_OFF);
}

void TestDialog::OnVisionUp()
{
	Delta_Thread::AddOneQueueInfo(VISION_UP_ON);
	Delta_Thread::AddOneQueueInfo(VISION_UP_OFF);
}

void TestDialog::OnVisionDown()
{
	Delta_Thread::AddOneQueueInfo(VISION_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(VISION_DOWN_OFF);
}

void TestDialog::OnRotateStart()	//电平信号
{
	Delta_Thread::AddOneQueueInfo(VISION_ROTATE_START_ON);
}

void TestDialog::OnRotateStop()
{
	Delta_Thread::AddOneQueueInfo(VISION_ROTATE_START_OFF);
	Delta_Thread::AddOneQueueInfo(VISION_ROTATE_STOP_ON);
	Delta_Thread::AddOneQueueInfo(VISION_ROTATE_STOP_OFF);
	
}

void TestDialog::OnPaoGuang()
{
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ON);
	Delta_Thread::AddOneQueueInfo(PAOGUANG_OFF);
}

void TestDialog::OnManualMode()	//手动模式
{
	Delta_Thread::AddOneQueueInfo(MANUAL_MODE);
	Delta_Thread::AddOneQueueInfo(AUTO_STOP);
	ui->btn_auto_start->setEnabled(false);
}

void TestDialog::OnAutoMode()	//自动模式
{
	Delta_Thread::AddOneQueueInfo(AUTO_MODE);
	ui->btn_auto_start->setEnabled(true);
}

void TestDialog::OnAutoStart()	//自动启动
{
	if (ui->btn_auto_start->text() == G2U("自动启动"))
	{
		Delta_Thread::AddOneQueueInfo(AUTO_START);
		ui->btn_auto_start->setText(G2U("停止"));	
		ui->btn_manual_mode->setEnabled(false);
	}
	else
	{
		Delta_Thread::AddOneQueueInfo(AUTO_STOP);
		ui->btn_auto_start->setText(G2U("自动启动"));
		ui->btn_manual_mode->setEnabled(true);
	}
}
 
void TestDialog::OnAutoStop()	//自动启动
{
	
}

void TestDialog::OnDistanceConfirm() //距离设置
{
	
	ushort  distance = static_cast<ushort>(ui->doubleSpinBox_distance->value() * 10);

	std::vector<ushort> data;
	ushort first = distance / 4096;
	ushort leave = distance % 4096;
	ushort second = leave / 256;
	leave = leave % 256;
	ushort third = leave / 16;
	leave = leave % 16;
	ushort forth = leave;
	data.push_back(first);
	data.push_back(second);
	data.push_back(third);
	data.push_back(forth);
	
	int length = data.size();
	QString dist;
	dist.resize(length);
	for (int index = 0; index < length; index++)
	{
		if (data[index] < 10)
			dist[index] = 0x30 + data[index];
		else
			dist[index] = 0x37 + data[index];

	}

	Delta_Thread::AddOneQueueInfo((QString(SET_DISTANCE) + dist).toStdString());

	Delta_Thread::AddOneQueueInfo(READ_TIME_AND_DISTANCE);	//读D200-D205 000310C80006
}

void TestDialog::OnDistanceConfirm_2() //距离设置
{

	ushort  distance = static_cast<ushort>(ui->doubleSpinBox_distance_2->value() * 10);

	std::vector<ushort> data;
	ushort first = distance / 4096;
	ushort leave = distance % 4096;
	ushort second = leave / 256;
	leave = leave % 256;
	ushort third = leave / 16;
	leave = leave % 16;
	ushort forth = leave;
	data.push_back(first);
	data.push_back(second);
	data.push_back(third);
	data.push_back(forth);

	int length = data.size();
	QString dist;
	dist.resize(length);
	for (int index = 0; index < length; index++)
	{
		if (data[index] < 10)
			dist[index] = 0x30 + data[index];
		else
			dist[index] = 0x37 + data[index];

	}

	Delta_Thread::AddOneQueueInfo((QString(SET_DISTANCE_2) + dist).toStdString());

	Delta_Thread::AddOneQueueInfo(READ_TIME_AND_DISTANCE);	//读D200-D205 000310C80006
}

void TestDialog::OnTimeConfirm()	//时间设置
{
	//QString Time = ui->lineEdit_time_set->text();
	//ui->doubleSpinBox_time->value();
	ushort  time = static_cast<ushort>(ui->doubleSpinBox_time->value() * 10);

	//std::string DecToHex(ushort num);
	std::vector<ushort> data;
	ushort first = time / 4096;
	ushort leave = time % 4096;
	ushort second = leave / 256;
	leave = leave % 256;
	ushort third = leave / 16;
	leave = leave % 16;
	ushort forth = leave;
	data.push_back(first);
	data.push_back(second);
	data.push_back(third);
	data.push_back(forth);

	QString ti("0000");
	for (int index = 0; index < data.size(); index++)
	{
		if (data[index] < 10)
			ti[index] = 0x30 + data[index];
		else
			ti[index] = 0x37 + data[index];

	}

	Delta_Thread::AddOneQueueInfo((QString(SET_TIME) + ti).toStdString());

	Delta_Thread::AddOneQueueInfo(READ_TIME_AND_DISTANCE);	//读D200-D205 000310C80006
}


void TestDialog::OnDetectEnd()	//检测结束
{
	Delta_Thread::AddOneQueueInfo(DETECT_END_ON);
	Delta_Thread::AddOneQueueInfo(DETECT_END_OFF);

}

void TestDialog::OnResultGood()	//检测结果合格
{
	Delta_Thread::AddOneQueueInfo(RESULT_GODD);
}

void TestDialog::OnResultBad() //检测结果不合格
{
	Delta_Thread::AddOneQueueInfo(RESULT_BAD);

}

TestDialog::~TestDialog()
{
    delete ui;
	delete m_pTimer;
}

void TestDialog::setBtnQss(QPushButton *btn,
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

void TestDialog::setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor)

{
	QStringList qss;

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

void TestDialog::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	/*QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		if (pPushButtons.at(i)->text() == G2U("拨手多位气缸缩") 
			|| pPushButtons.at(i)->text() == G2U("拨手多位气缸伸")
			|| pPushButtons.at(i)->text() == G2U("相机触发")
			|| pPushButtons.at(i)->text() == G2U("返回主界面"))
		{			
		}
		else
		{
			QPointF Center_Circle = pPushButtons.at(i)->geometry().topLeft();
			pPushButtons.at(i)->geometry().topRight();
			int width = pPushButtons.at(i)->geometry().width();
			Center_Circle.setX(Center_Circle.x() + width / 2);
			Center_Circle.setY(Center_Circle.y() - 30);

			if (pPushButtons.at(i)->property("status").toBool())
			{
				PaintCirle(painter, Center_Circle, 10, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
			}
			else
			{
				PaintCirle(painter, Center_Circle, 10, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}	
		}
	}*/

	int radius = 5;
	int distance = 15;
	QPointF x_origin(75, 550);
	if (m_X_States.size() != 0 && m_X_States.size() % 8 == 0)
	{
		for (int row = 0; row < m_X_States.size()/8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (m_X_States[row * 8 + column])
					PaintCirle(painter, QPointF(x_origin.x() + column*(radius + distance),
					x_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
				else
					PaintCirle(painter, QPointF(x_origin.x() + column*(radius + distance),
					x_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}
		}
	}
	
	QPointF y_origin(350, 550);
	if (m_Y_States.size() != 0 && m_Y_States.size()%8 == 0)
	{
		for (int row = 0; row < m_Y_States.size()/8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (m_Y_States[row * 8 + column])
					PaintCirle(painter, QPointF(y_origin.x() + column*(radius + distance),
					y_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
				else
					PaintCirle(painter, QPointF(y_origin.x() + column*(radius + distance),
					y_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}
		}
	}


	radius = 10;
	distance = 30;
	QPointF origin_status(78, 750);
	if (m_origin_States.size() != 0 && m_origin_States.size() % 12 == 0)
	{
		for (int row = 0; row < m_origin_States.size() / 12; row++)
		{
			for (int column = 0; column < 12; column++)
			{
				if (m_origin_States[row * 12 + column])
					PaintCirle(painter, QPointF(origin_status.x() + column*(radius + distance),
					origin_status.y() + row*(radius + distance)),
					radius, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
				else
					PaintCirle(painter, QPointF(origin_status.x() + column*(radius + distance),
					origin_status.y() + row*(radius + distance)),
					radius, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}
		}
	}
}

void TestDialog::PaintCirle(QPainter& painter, const QPointF& center_circle, int radius, const QPen &pen, const QBrush &brush)
{
	// 反走样
	painter.setRenderHint(QPainter::Antialiasing, true);
	// 设置画笔颜色、宽度,  边框
	painter.setPen(pen);
	// 设置画刷颜色
	painter.setBrush(brush);
	// 绘制圆
	painter.drawEllipse(center_circle, radius, radius);
}


void TestDialog::readyDataSlot(QByteArray str)
{
	if (!str.isEmpty())
	{
		std::string function_code = str.toStdString().substr(3, 2);
		if (function_code == "01")
			m_Y_States = Parse_Delta_Ascii(str.toStdString());

		if (function_code == "02")
		{
			
			m_X_States = Parse_Delta_Ascii(str.toStdString());

			//原点状态
			m_origin_States.clear();
			if (m_X_States.size() > 70)
			{
				bool flag = true;
				m_origin_States.push_back(m_X_States[8]);
				m_origin_States.push_back(m_X_States[9]);
				m_origin_States.push_back(m_X_States[29]);
				m_origin_States.push_back(m_X_States[62]);
				m_origin_States.push_back(m_X_States[31]);
				m_origin_States.push_back(m_X_States[33]);
				m_origin_States.push_back(m_X_States[40]);
				m_origin_States.push_back(m_X_States[41]);
				m_origin_States.push_back(m_X_States[57]);
				m_origin_States.push_back(m_X_States[58]);
				m_origin_States.push_back(m_X_States[61]);

				m_origin_States.push_back(m_X_States[12]);
				m_origin_States.push_back(m_X_States[13]);
				m_origin_States.push_back(m_X_States[19]);
				m_origin_States.push_back(m_X_States[21]);
				m_origin_States.push_back(m_X_States[23]);
				m_origin_States.push_back(m_X_States[25]);

				for (int index = m_origin_States.size() - 6; index < m_origin_States.size() - 1; index++)
				{
					if (!m_origin_States[index])
					{
						flag = false;
						break;
					}
					
				}
				for (int index = 0; index < 6; index++)
				{
					m_origin_States.pop_back();
				}

				m_origin_States.push_back(flag);

			}
		
		}
			
		if (function_code == "03")
		{
			m_D_Register = Parse_Delta_Ascii_03(str.toStdString());	
			if (!m_D_Register.empty() && m_D_Register.size() == 9)
			{
				ui->lcd_distance->display(m_D_Register[0] / 10.0);
				ui->lcd_time->display(m_D_Register[4] / 10.0);
				ui->lcd_distance_2->display(m_D_Register[8] / 10.0);
				ui->doubleSpinBox_distance->setValue(m_D_Register[0] / 10.0);
				ui->doubleSpinBox_time->setValue(m_D_Register[4] / 10.0);
				ui->doubleSpinBox_distance_2->setValue(m_D_Register[8] / 10.0);
			}	
		}
			
	}
	else
	{
		m_Y_States.clear();
		m_X_States.clear();
		m_D_Register.clear();
	}
}


void TestDialog::OnForward()
{
	Delta_Thread::AddOneQueueInfo(FORWARD_ON);
	Delta_Thread::AddOneQueueInfo(FORWARD_OFF);
}

void TestDialog::OnBackWard()
{
	Delta_Thread::AddOneQueueInfo(BACKWARD_ON);
	Delta_Thread::AddOneQueueInfo(BACKWARD_OFF);
}

void TestDialog::OnForward_2()
{
	Delta_Thread::AddOneQueueInfo(FORWARD_2_ON);
	Delta_Thread::AddOneQueueInfo(FORWARD_2_OFF);
}

void TestDialog::OnBackWard_2()
{
	Delta_Thread::AddOneQueueInfo(BACKWARD_2_ON);
	Delta_Thread::AddOneQueueInfo(BACKWARD_2_OFF);
}

void TestDialog::OnRotateStepStart()
{
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ROTATE_START_ON);
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ROTATE_START_OFF);
}

void TestDialog::OnRotateStepStop()
{
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ROTATE_STOP_ON);
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ROTATE_STOP_OFF);
}

void TestDialog::OnWindUp()
{
	Delta_Thread::AddOneQueueInfo(WIND_UP_ON);
	Delta_Thread::AddOneQueueInfo(WIND_UP_OFF);
}

void TestDialog::OnWindDown()
{
	Delta_Thread::AddOneQueueInfo(WIND_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(WIND_DOWN_OFF);
}
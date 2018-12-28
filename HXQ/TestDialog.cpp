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


	QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		pPushButtons.at(i)->setProperty("status",i>5);
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
	m_pTimer->start(100);

	QVariant value;
	ReadConfigure("config.ini", "MotionCard", "Ip", value);
	QString ip = value.toString();

	/****/
	m_Galil = nullptr;
	if (!m_Galil)
	{
		m_Galil = new Galil_Thread;
	}
	connect(m_Galil, SIGNAL(sendInputValue(int)), this, SLOT(receiveInputValue(int)));
	connect(m_Galil, SIGNAL(finished()), m_Galil, SLOT(deleteLater()));
	m_Galil->GcLibVersion();
	if (m_Galil->Open(ip + ""))
	{
		ui->label_status->setText(G2U("已连接！"));
		ui->label_status->setStyleSheet(QStringLiteral("color: rgb(0, 255, 0);"));
		m_Galil->start();
	}
	else
	{
		ui->label_status->setText(G2U("未连接！"));
		ui->label_status->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
		delete m_Galil;
	}
	
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
	m_Galil->stop();
	close();
}


TestDialog::~TestDialog()
{
    delete ui;
	delete m_pTimer;
	m_pTimer = nullptr;
	m_Galil = nullptr;
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

	int radius = 5;
	int distance = 15;
	QPointF x_origin(75, 250);
	if (m_Input.size() != 0 && m_Input.size() % 8 == 0)
	{
		for (int row = 0; row < m_Input.size()/8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (m_Input[row * 8 + column])
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
	
	QPointF y_origin(350, 250);
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


void TestDialog::receiveInputValue(int value)
{

	m_Input = Parse_Galil_Input(value);

}

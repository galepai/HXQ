#include "motionCardWidget.h"
#include "ui_motionCardWidget.h"
#include "Func.h"
#include "ConstParam.h"
#include <QMessageBox>

motionCardWidget::motionCardWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::motionCardWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveToXml()));
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(OnTest()));
	ReadFromXml();
	m_pGalil = nullptr;
	
}

motionCardWidget::~motionCardWidget()
{

	if (m_pGalil)
	{
		if (m_pGalil->isRunning())
		{
			m_pGalil->stop();
			m_pGalil->wait();
		}
	}
    delete ui;
}

void motionCardWidget::ReadFromXml()
{

	//QVariant Value;
	//ReadConfigure("config.ini", "MotionCard", "Ip", Value);
	//ui->lineEdit_Ip->setText(Value.toString());
	
	
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_MotionCard), QString(Node_MotionCard), xmlContent))
	{
		for each (auto each_horizontalLayout in vector_horizontalLayout)
		{
			QObject* horizontalLayout = (QObject*)each_horizontalLayout;
			horizontalLayout->deleteLater();
		}

		name.clear();
		type.clear();
		value.clear();
		vector_horizontalLayout.clear();
		vector_labels.clear();
		vector_lineEdits.clear();

		for (auto &eachContent : xmlContent)
		{
			name.push_back(eachContent.first.first);
			type.push_back(eachContent.first.second);
			value.push_back(eachContent.second);

			QHBoxLayout* horizontalLayout = new QHBoxLayout();
			horizontalLayout->setSpacing(6);

			QLabel* nameLabel = new QLabel();
			QLineEdit* text = new QLineEdit();
			nameLabel->setText(eachContent.first.first);
			text->setText(eachContent.second);
			text->setAlignment(Qt::AlignCenter);
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			text->setSizePolicy(sizePolicy);
			nameLabel->setSizePolicy(sizePolicy);
			horizontalLayout->addWidget(nameLabel);
			horizontalLayout->addWidget(text);
			ui->verticalLayout->addLayout(horizontalLayout);

			connect(horizontalLayout, &QHBoxLayout::destroyed, [=](QObject* obj)
			{
				nameLabel->deleteLater();
				text->deleteLater();
			});

			vector_horizontalLayout.push_back(horizontalLayout);
			vector_labels.push_back(nameLabel);
			vector_lineEdits.push_back(text);
		}

	}
	else
	{
		//error;
	}

}

void motionCardWidget::SaveToXml()
{
	int sucessCount = 0;
	int count = vector_horizontalLayout.size();

	for (int index = 0; index<vector_labels.size(); index++)
	{
		QLabel* label = (QLabel*)vector_labels[index];
		QLineEdit* lineEdit = (QLineEdit*)vector_lineEdits[index];

		if (UpdateXmlNodeText(QString(XML_MotionCard), QString(Node_MotionCard), label->text(), lineEdit->text()) == ChhXml::UpdateOK)
		{
			sucessCount++;
		}
	}

	if (sucessCount == count && sucessCount != 0)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("参数成功写入配置文件"));
	}

}

void motionCardWidget::OnTest()
{
	if (!m_pGalil)
	{
		m_pGalil = new Galil_Thread(this);
		connect(m_pGalil, SIGNAL(sendVarValue(QString)), m_pGalil, SLOT(OnReceiveVarValue(QString)));
		connect(m_pGalil, SIGNAL(finished()), m_pGalil, SLOT(deleteLater()));


		QString Revision, ip, varName, varValue;
		QLineEdit* pLineEdit = (QLineEdit*)vector_lineEdits[0];
		ip = pLineEdit->text();
		pLineEdit = (QLineEdit*)vector_lineEdits[1];
		varName = pLineEdit->text();

		if (m_pGalil->Open(ip))
		{
			m_pGalil->CmdT("\x12\x16", Revision);
			QMessageBox::StandardButton reply;
			reply = QMessageBox::information(this, G2U("控制卡信息"), Revision + (G2U("\r\n连接成功！")));
		}
		else
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::warning(this, G2U("信息"), G2U("控制卡连接失败"));
			delete m_pGalil;
			m_pGalil = nullptr;
			return;
		}

		m_pGalil->setVarName(varName);
		m_pGalil->start();
		ui->testButton->setText(G2U("已连接"));
	}
}

void motionCardWidget::OnReceiveVarValue(QString varValue)
{
	ui->label->setText(varValue);
}
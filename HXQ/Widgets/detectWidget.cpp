#pragma execution_character_set("utf-8")

#include "detectWidget.h"
#include "ui_detectWidget.h"
#include "ConstParam.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include <QFileDialog>
#include "Global.h"

detectWidget::detectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detectWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	ReadFromXml();

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveToXml()));
	
}

detectWidget::~detectWidget()
{
    delete ui;
}

void detectWidget::SaveToXml()
{
		//WriteConfigure("config.ini", "Model", "GouModel", ui->label_gouModel->text());
	
		//QMessageBox::StandardButton reply;
		//reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
	

	int sucessCount = 0;
	int count = vector_horizontalLayout.size();

	for (int index = 0; index<vector_labels.size(); index++)
	{
		QLabel* label = (QLabel*)vector_labels[index];
		QLineEdit* lineEdit = (QLineEdit*)vector_lineEdits[index];

		if (UpdateXmlNodeText(QString(XML_Configure), QString(Node_Hxq), label->text(), lineEdit->text()) == ChhXml::UpdateOK)
		{
			sucessCount++;
		}
	}

	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Hxq), xmlContent))
	{
		int total = sizeof(g_DetectParam) / sizeof(float);
		float* p = (float*)&g_DetectParam;
		for (int index = 0; index<total; index++)
		{
			*p = xmlContent[index].second.toFloat();
			p++;
		}
	}


	if (sucessCount == count && sucessCount != 0)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, tr("信息"), tr("参数成功写入配置文件"));
	}


}

void detectWidget::ReadFromXml()
{
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Hxq), xmlContent))
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
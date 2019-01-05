#include "camera2Widget.h"
#include "ui_camera2Widget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include "ConstParam.h"
#include <QtXml\QDomElement>
#include <QStandardItemModel>



camera2Widget::camera2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camera2Widget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveExposureToIni()));
	updateCombox();
}

camera2Widget::~camera2Widget()
{
    delete ui;
}

void camera2Widget::ReadIni(int index)
{
	QString tagName = ui->comboBox->currentText();
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Camera), tagName, xmlContent))
	{
	
	/*	if (vector_horizontalLayout.size())
		{
			for (auto each_horizontalLayout : vector_horizontalLayout)
			{

				QObject* horizontalLayout = (QObject*)each_horizontalLayout;
				horizontalLayout->deleteLater();

			}
		}*/

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

void camera2Widget::SaveExposureToIni()
{
	int sucessCount = 0;
	int count = vector_horizontalLayout.size();

	for (int index=0;index<vector_labels.size();index++)
	{
		QLabel* label = (QLabel*)vector_labels[index];
		QLineEdit* lineEdit = (QLineEdit*)vector_lineEdits[index];

		if (UpdateXmlNodeText(QString(XML_Camera), ui->comboBox->currentText(), label->text(), lineEdit->text()) == ChhXml::UpdateOK)
		{
			sucessCount++;
		}
	}

	if (sucessCount == count && sucessCount!=0)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
	}
}

void camera2Widget::updateCombox()
{
	/////////////////////////
	QDomDocument doc;
	QFile file(XML_Camera);

	if (!file.open(QFile::ReadOnly))
		return;

	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement rootElement = doc.documentElement();
	QDomNodeList list = rootElement.childNodes();
	int count = list.count();
	for (int index = 0; index < count; index++)
	{
		QString name = list.at(index).nodeName();

		QLineEdit *lineEdit = new QLineEdit();
		QFont font;
		font.setFamily(QStringLiteral("Arial Black"));
		font.setPointSize(10);
		font.setBold(true);
		font.setWeight(75);
		lineEdit->setFont(font);
		lineEdit->setText(name);
		lineEdit->setReadOnly(true);
		lineEdit->setAlignment(Qt::AlignCenter);

		ui->comboBox->setLineEdit(lineEdit);

		ui->comboBox->addItem(name);
		static_cast<QStandardItemModel*>(ui->comboBox->view()->model())->item(index)->setTextAlignment(Qt::AlignCenter);
	}
	/////////////////

	connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(ReadIni(int)));
	ui->comboBox->activated(1);
}

#include "camera2Widget.h"
#include "ui_camera2Widget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include "ConstParam.h"
#include <QtXml\QDomElement>
#include <QStandardItemModel>
#include "HalconCpp.h"



camera2Widget::camera2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camera2Widget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);
	ui->verticalLayout_select->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveToXml()));
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(OnTest()));
	updateCombox();
}

camera2Widget::~camera2Widget()
{
    delete ui;
}

void camera2Widget::ReadFromXml(int index)
{
	QString tagName = ui->comboBox->currentText();
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Camera), tagName, xmlContent))
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

void camera2Widget::SaveToXml()
{
	int sucessCount = 0;
	int count = vector_horizontalLayout.size() + 2;

	for (int index=0;index<vector_labels.size();index++)
	{
		QLabel* label = (QLabel*)vector_labels[index];
		QLineEdit* lineEdit = (QLineEdit*)vector_lineEdits[index];

		if (UpdateXmlNodeText(QString(XML_Camera), ui->comboBox->currentText(), label->text(), lineEdit->text()) == ChhXml::UpdateOK)
		{
			sucessCount++;
		}
	}

	if (UpdateXmlNodeText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Top), ui->comboBox_topCamera->currentText()) == ChhXml::UpdateOK)
	{
		sucessCount++;
	}

	if (UpdateXmlNodeText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Side), ui->comboBox_sideCamera->currentText()) == ChhXml::UpdateOK)
	{
		sucessCount++;
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

	QString topCameraName, sideCameraName, type;
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Top), type, topCameraName);
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Side), type, sideCameraName);
	int topIndex, sideIndex;

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

		ui->comboBox_topCamera->setLineEdit(lineEdit);
		ui->comboBox_topCamera->addItem(name);
		static_cast<QStandardItemModel*>(ui->comboBox_topCamera->view()->model())->item(index)->setTextAlignment(Qt::AlignCenter);

		if (name == topCameraName)
			topIndex = index;

	}

	
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

		ui->comboBox_sideCamera->setLineEdit(lineEdit);
		ui->comboBox_sideCamera->addItem(name);
		static_cast<QStandardItemModel*>(ui->comboBox_sideCamera->view()->model())->item(index)->setTextAlignment(Qt::AlignCenter);

		if (name == sideCameraName)
			sideIndex = index;


	}
	/////////////////

	connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(ReadFromXml(int)));
	ui->comboBox->activated(1);
	ui->comboBox_topCamera->setCurrentIndex(topIndex);
	ui->comboBox_sideCamera->setCurrentIndex(sideIndex);

}


void camera2Widget::OnTest()
{
	using namespace HalconCpp;
	HFramegrabber* pGrabber = new HFramegrabber;

	try {
		QLineEdit* pLineEdit = (QLineEdit*)vector_lineEdits[0];
		QString id = pLineEdit->text();
		pLineEdit = (QLineEdit*)vector_lineEdits[1];
		QString interFace = pLineEdit->text();
		pLineEdit = (QLineEdit*)vector_lineEdits[2];
		QString color = pLineEdit->text();
		
		pGrabber->OpenFramegrabber(interFace.toStdString().c_str(), 1, 1, 0, 0, 0, 0, "default", 8, \
			color.toStdString().c_str(), -1, "false", "default", \
			id.toStdString().c_str(), 0, -1);

		for (int index = 3; index < vector_lineEdits.size(); index++)
		{
			QLineEdit* pLineEdit = (QLineEdit*)vector_lineEdits[index];
			QString name1 = name[index];
			QString type1 = name[index];
			QString value1 = pLineEdit->text();

			if (type1.contains("float"))
			{
				pGrabber->SetFramegrabberParam(name1.toStdString().c_str(), value1.toFloat());
			}
			else if (type1.contains("int"))
			{
				pGrabber->SetFramegrabberParam(name1.toStdString().c_str(), value1.toInt());
			}
			else
			{
				pGrabber->SetFramegrabberParam(name1.toStdString().c_str(), value1.toStdString().c_str());
			}

		}

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("提示"), G2U("CCD连接设置正确"));
		delete pGrabber;
		pGrabber = nullptr;

	}
	catch (HException& e)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("提示"), G2U(e.ErrorMessage().Text()));
		delete pGrabber;
		pGrabber = nullptr;
	}
	
}
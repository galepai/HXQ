#include "camera2Widget.h"
#include "ui_camera2Widget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include "ConstParam.h"
#include <QtXml\QDomElement>



camera2Widget::camera2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camera2Widget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	ReadIni();

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveExposureToIni()));



	//QHBoxLayout* horizontalLayout = new QHBoxLayout();
	//horizontalLayout->setSpacing(6);
	//horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
	//QLabel* nameLabel = new QLabel(ui->groupBox);
	//QLineEdit* value = new QLineEdit(ui->groupBox);
	//nameLabel->setText("text");
	//value->setText("555");
	//QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	//value->setSizePolicy(sizePolicy);
	//nameLabel->setSizePolicy(sizePolicy);
	//horizontalLayout->addWidget(nameLabel);
	//horizontalLayout->addWidget(value);
	//ui->verticalLayout->addLayout(horizontalLayout);


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
		ui->comboBox->addItem(name);
	}

	connect(ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
		[=](int index)
	{ 
		QString tagName = ui->comboBox->currentText();
		std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
		if (ParserXmlNode(QString(XML_Camera), tagName, xmlContent))
		{
			int size = xmlContent.size();
			name.clear();
			type.clear();
			value.clear();
			int num = vector_horizontalLayout.size();
			if (num)
			{
				for (auto each : vector_horizontalLayout)
				{
					delete queue_labels.front();
					delete queue_lineEdits.front();

					ui->verticalLayout->removeItem((QHBoxLayout*)each);;
					//delete each;
					//ee->removeWidget((QWidget*)autoLabels[0]);
					queue_labels.pop();
					queue_lineEdits.pop();
				}
			}
			vector_horizontalLayout.clear();

			
			for (auto &each : xmlContent)
			{
				name.push_back(each.first.first);
				type.push_back(each.first.second);
				value.push_back(each.second);

				QHBoxLayout* horizontalLayout = new QHBoxLayout();
				horizontalLayout->setSpacing(6);
				//horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
				QLabel* nameLabel = new QLabel();
				QLineEdit* value = new QLineEdit();
				nameLabel->setText(each.first.first);
				value->setText(each.second);
				QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
				value->setSizePolicy(sizePolicy);
				nameLabel->setSizePolicy(sizePolicy);
				horizontalLayout->addWidget(nameLabel);
				horizontalLayout->addWidget(value);
				ui->verticalLayout->addLayout(horizontalLayout);

				vector_horizontalLayout.push_back(horizontalLayout);
				queue_labels.push(nameLabel);
				queue_lineEdits.push(value);
				
			}
			
			
		}
		else
		{
			//error;
		}
	}
	);

}

camera2Widget::~camera2Widget()
{
    delete ui;
}

void camera2Widget::ReadIni()
{

	QString type, value;
	READ_XML_ElEMENT(XML_Camera, Node_TopCamera, HikVision_ExposureTime, type, value);
	

	
}

void camera2Widget::SaveExposureToIni()
{

		/*if (UpdateXmlNodeText(QString(XML_Camera), QString(Node_TopCamera), QString(HikVision_ExposureTime), QString("%1").arg(ui->spinBox_top->value())) == ChhXml::UpdateOK)
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
		}
*/	


}

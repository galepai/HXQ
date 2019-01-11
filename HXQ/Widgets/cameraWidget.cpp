#include "cameraWidget.h"
#include "ui_cameraWidget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include "ConstParam.h"
#include <QtXml\QDomElement>





cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveExposureToIni()));
	
	//ui->tableView->setVisible(false);
	ui->tableView->setVerticalHeader(nullptr);


	_model = new QStandardItemModel();
	_model->setColumnCount(2);
	_model->setHeaderData(0, Qt::Horizontal, G2U("属性"));
	_model->setHeaderData(1, Qt::Horizontal, G2U("值"));
	//_model->setHeaderData(2, Qt::Horizontal, "性别");

	/*_model->setrowcount(3);
	_model->setheaderdata(0, qt::vertical, "记录一");
	_model->setheaderdata(1, qt::vertical, "记录二");
	_model->setheaderdata(2, qt::vertical, "记录三");*/

	/*_model->setItem(0, 0, new QStandardItem("张三"));
	_model->setItem(0, 1, new QStandardItem("3"));
	_model->setItem(0, 2, new QStandardItem("男"));*/

	ui->tableView->setModel(_model);


	updateCombox();
}

cameraWidget::~cameraWidget()
{
    delete ui;
}

void cameraWidget::ReadIni(int index)
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

		int count = -1;
		for (auto &eachContent : xmlContent)
		{
			name.push_back(eachContent.first.first);
			type.push_back(eachContent.first.second);
			value.push_back(eachContent.second);
			count++;
			//_model->setRowCount(xmlContent.size());
			_model->setItem(count, 0, new QStandardItem(eachContent.first.first));
			_model->setItem(count, 1, new QStandardItem(eachContent.second));

		}
	}
	else
	{
		//error;
	}

	
}

void cameraWidget::SaveExposureToIni()
{
	int sucessCount = 0;
	int count = name.size();

	for (int index=0;index<name.size();index++)
	{
	
		if (UpdateXmlNodeText(QString(XML_Camera), ui->comboBox->currentText(), ui->tableView->indexAt(QPoint(index, 0)).data().toString(),\
			ui->tableView->indexAt(QPoint(index,1)).data().toString()) == ChhXml::UpdateOK)
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

void cameraWidget::updateCombox()
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

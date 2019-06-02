#pragma execution_character_set("utf-8")

#include "saveWidget.h"
#include "ui_saveWidget.h"
#include "Func.h"
#include "ConstParam.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include "Global.h"


saveWidget::saveWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::saveWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveToXml()));
	connect(ui->pushButton_SaveTopBadPath, SIGNAL(clicked()), this, SLOT(OnSaveTopBadPath()));
	connect(ui->pushButton_SaveSideBadPath, SIGNAL(clicked()), this, SLOT(OnSaveSideBadPath()));
	connect(ui->pushButton_SaveTopAllPath, SIGNAL(clicked()), this, SLOT(OnSaveTopAllPath()));
	connect(ui->pushButton_SaveSideAllPath, SIGNAL(clicked()), this, SLOT(OnSaveSideAllPath()));

	InitWidget();
	ReadFromXml();
}

saveWidget::~saveWidget()
{

    delete ui;
}

void saveWidget::InitWidget()
{
	QStringList items;
	items << "false" << "true";
	ui->comboBox_IsSaveLog->addItems(items);
	ui->comboBox_IsSaveSideAll->addItems(items);
	ui->comboBox_IsSaveSideBad->addItems(items);
	ui->comboBox_IsSaveTopAll->addItems(items);
	ui->comboBox_IsSaveTopBad->addItems(items);

	items.clear();
	items << "tiff" << "bmp";
	ui->comboBox_SaveImageFormat->addItems(items);
}

void saveWidget::ReadFromXml()
{

	
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Save), xmlContent))
	{
		ui->spinBox_TopBadIndex->setValue(xmlContent[0].second.toInt());
		ui->spinBox_SideBadIndex->setValue(xmlContent[1].second.toInt());
			
		ui->comboBox_IsSaveTopBad->setCurrentIndex(xmlContent[2].second.toInt());
		ui->comboBox_IsSaveSideBad->setCurrentIndex(xmlContent[3].second.toInt());
		ui->comboBox_IsSaveTopAll->setCurrentIndex(xmlContent[4].second.toInt());
		ui->comboBox_IsSaveSideAll->setCurrentIndex(xmlContent[5].second.toInt());

		ui->spinBox_SaveTopBadNum->setValue(xmlContent[6].second.toInt());
		ui->spinBox_SaveSideBadNum->setValue(xmlContent[7].second.toInt());
		ui->spinBox_SaveTopAllNum->setValue(xmlContent[8].second.toInt());
		ui->spinBox_SaveSideAllNum->setValue(xmlContent[9].second.toInt());
			
		ui->textEdit_SaveTopBadPath->setText(xmlContent[10].second);
		ui->textEdit_SaveSideBadPath->setText(xmlContent[11].second);
		ui->textEdit_SaveTopAllPath->setText(xmlContent[12].second);
		ui->textEdit_SaveSideAllPath->setText(xmlContent[13].second);

		if (xmlContent[14].second == "tiff")
		{
			ui->comboBox_SaveImageFormat->setCurrentIndex(0);
		}
		else
		{
			ui->comboBox_SaveImageFormat->setCurrentIndex(1);
		}
			
		ui->comboBox_IsSaveLog->setCurrentIndex(xmlContent[15].second.toInt());

	}
	else
	{
		//error;
	}

}

void saveWidget::SaveToXml()
{
	SaveToSingleXml(QString(Save_TopBadIndex), QString("%1").arg(ui->spinBox_TopBadIndex->value()));
	SaveToSingleXml(QString(Save_SideBadIndex), QString("%1").arg(ui->spinBox_SideBadIndex->value()));

	SaveToSingleXml(QString(Save_IsSaveTopBad), QString("%1").arg(ui->comboBox_IsSaveTopBad->currentIndex()));
	SaveToSingleXml(QString(Save_IsSaveSideBad), QString("%1").arg(ui->comboBox_IsSaveSideBad->currentIndex()));
	SaveToSingleXml(QString(Save_IsSaveTopAll), QString("%1").arg(ui->comboBox_IsSaveTopAll->currentIndex()));
	SaveToSingleXml(QString(Save_IsSaveSideAll), QString("%1").arg(ui->comboBox_IsSaveSideAll ->currentIndex()));

	SaveToSingleXml(QString(Save_SaveTopBadNum), QString("%1").arg(ui->spinBox_SaveTopBadNum->value()));
	SaveToSingleXml(QString(Save_SaveSideBadNum), QString("%1").arg(ui->spinBox_SaveSideBadNum->value()));
	SaveToSingleXml(QString(Save_SaveTopAllNum), QString("%1").arg(ui->spinBox_SaveTopAllNum->value()));
	SaveToSingleXml(QString(Save_SaveSideAllNum), QString("%1").arg(ui->spinBox_SaveSideAllNum->value()));

	SaveToSingleXml(QString(Save_SaveTopBadPath), ui->textEdit_SaveTopBadPath->toPlainText());
	SaveToSingleXml(QString(Save_SaveSideBadPath), ui->textEdit_SaveSideBadPath->toPlainText());
	SaveToSingleXml(QString(Save_SaveTopAllPath), ui->textEdit_SaveTopAllPath->toPlainText());
	SaveToSingleXml(QString(Save_SaveSideAllPath), ui->textEdit_SaveSideAllPath->toPlainText());

	SaveToSingleXml(QString(Save_SaveImageFormat), QString("%1").arg(ui->comboBox_SaveImageFormat->currentText()));
	SaveToSingleXml(QString(Save_IsSaveLog), QString("%1").arg(ui->comboBox_IsSaveLog->currentIndex()));

	//update g_SaveParam;
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent2;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Save), xmlContent2))
	{
		g_SaveParam.SaveTopBadIndex = xmlContent2[0].second.toInt();
		g_SaveParam.SaveSideBadIndex = xmlContent2[1].second.toInt();

		g_SaveParam.IsSaveTopBad = xmlContent2[2].second.toInt();
		g_SaveParam.IsSaveSideBad = xmlContent2[3].second.toInt();
		g_SaveParam.IsSaveTopAll = xmlContent2[4].second.toInt();
		g_SaveParam.IsSaveSideAll = xmlContent2[5].second.toInt();

		g_SaveParam.SaveTopBadNum = xmlContent2[6].second.toInt();
		g_SaveParam.SaveSideBadNum = xmlContent2[7].second.toInt();
		g_SaveParam.SaveTopAllNum = xmlContent2[8].second.toInt();
		g_SaveParam.SaveSideAllNum = xmlContent2[9].second.toInt();


		g_SaveParam.SaveTopBadPath = xmlContent2[10].second;
		g_SaveParam.SaveSideBadPath = xmlContent2[11].second;
		g_SaveParam.SaveTopAllPath = xmlContent2[12].second;
		g_SaveParam.SaveSideAllPath = xmlContent2[13].second;

		g_SaveParam.SaveImageFormat = xmlContent2[14].second;

		g_SaveParam.IsSaveLog = xmlContent2[15].second.toInt();

		if (QFile::exists(_XML_Configure))
			QFile::remove(_XML_Configure);

		QFile::copy(XML_Configure, _XML_Configure);

		if (QFile::exists(XML_Configure_Origin))
			QFile::remove(XML_Configure_Origin);

		QFile::copy(XML_Configure, XML_Configure_Origin);
	}


	QMessageBox::StandardButton reply;
	reply = QMessageBox::information(this, tr("信息"), tr("参数成功写入配置文件"));

}

void saveWidget::SaveToSingleXml(QString& name,QString& value)
{
	if (UpdateXmlNodeText(QString(XML_Configure), QString(Node_Save), name,value) != ChhXml::UpdateOK)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, tr("信息"), tr("参数设置失败"));
	}
}

void saveWidget::OnTest()
{
	
}
void saveWidget::OnSaveTopBadPath( )
{
	QString path = QFileDialog::getExistingDirectory(this, tr("更改顶部不良品图片保存路径"));
	if(!path.isEmpty())
		ui->textEdit_SaveTopBadPath->setText(path + "/");
}

void saveWidget::OnSaveSideBadPath()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("更改侧部不良品图片保存路径"));
	if (!path.isEmpty())
		ui->textEdit_SaveSideBadPath->setText(path + "/");
}

void saveWidget::OnSaveTopAllPath()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("更改顶部图片保存路径"));
	if (!path.isEmpty())
		ui->textEdit_SaveSideBadPath->setText(path + "/");
}

void saveWidget::OnSaveSideAllPath()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("更改侧部图片保存路径"));
	if (!path.isEmpty())
		ui->textEdit_SaveSideBadPath->setText(path + "/");
}

void saveWidget::OnBad()
{
	
}

void saveWidget::OnReceiveVarValue(QString varValue)
{
	ui->label->setText(varValue);
}
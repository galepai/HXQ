#include "cameraWidget.h"
#include "ui_cameraWidget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include "ConstParam.h"

cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	ReadIni();

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveExposureToIni()));
}

cameraWidget::~cameraWidget()
{
    delete ui;
}

void cameraWidget::ReadIni()
{

	QString type, value;
	READ_XML_ElEMENT(XML_Camera, Node_TopCamera, HikVision_ExposureTime, type, value);
	ui->spinBox_top->setValue(value.toFloat());
	
	READ_XML_ElEMENT(XML_Camera, Node_TopCamera, HikVision_ExposureTime, type, value);
	ui->spinBox_side->setValue(value.toFloat());
	
}

void cameraWidget::SaveExposureToIni()
{

		if (UpdateXmlNodeText(QString(XML_Camera), QString(Node_TopCamera), QString(HikVision_ExposureTime), QString("%1").arg(ui->spinBox_top->value())) == ChhXml::UpdateOK
			&& UpdateXmlNodeText(QString(XML_Camera), QString(Node_SideCamera), QString(Dalsa_ExposureTime), QString("%1").arg(ui->spinBox_side->value())) == ChhXml::UpdateOK)
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
		}

	
}

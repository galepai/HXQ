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
	//ReadConfigure("config.ini", "TopCamera", "Exposure", Value);
	//ui->spinBox_top->setValue(Value.toInt());
	QString type, value;

	ReadXmlElementText(QString(CameraXML), QString(Node_TopCamera), QString(HikVision_ExposureTime),type,value);
	int tt = value.toFloat();
	ui->spinBox_top->setValue(value.toFloat());
	
	ReadXmlElementText(QString(CameraXML), QString(Node_SideCamera), QString(Dalsa_ExposureTime), type, value);
	ui->spinBox_side->setValue(value.toFloat());
	

}

void cameraWidget::SaveExposureToIni()
{/*
		WriteConfigure("config.ini", "TopCamera", "Exposure", QString("%1").arg(ui->spinBox_top->value()));
		WriteConfigure("config.ini", "SideCamera", "Exposure", QString("%1").arg(ui->spinBox_side->value()));

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
*/


		if (UpdateXmlNodeText(QString(CameraXML), QString(Node_TopCamera), QString(HikVision_ExposureTime), QString("%1").arg(ui->spinBox_top->value())) == ChhXml::UpdateOK
			&& UpdateXmlNodeText(QString(CameraXML), QString(Node_SideCamera), QString(Dalsa_ExposureTime), QString("%1").arg(ui->spinBox_side->value())) == ChhXml::UpdateOK)
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
		}

	
}

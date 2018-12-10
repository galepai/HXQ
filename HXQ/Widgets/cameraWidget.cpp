#include "cameraWidget.h"
#include "ui_cameraWidget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>

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

	QVariant Value;
	ReadConfigure("config.ini", "Camera_02_Clock", "Exposure", Value);
	ui->spinBox_02->setValue(Value.toInt());

	ReadConfigure("config.ini", "Camera_07_Clock", "Exposure", Value);
	ui->spinBox_07->setValue(Value.toInt());

	ReadConfigure("config.ini", "Camera_10_Clock", "Exposure", Value);
	ui->spinBox_10->setValue(Value.toInt());

	ReadConfigure("config.ini", "Camera_11_Clock", "Exposure", Value);
	ui->spinBox_11->setValue(Value.toInt());

}

void cameraWidget::SaveExposureToIni()
{
		WriteConfigure("config.ini", "Camera_02_Clock", "Exposure", QString("%1").arg(ui->spinBox_02->value()));
		WriteConfigure("config.ini", "Camera_07_Clock", "Exposure", QString("%1").arg(ui->spinBox_07->value()));
		WriteConfigure("config.ini", "Camera_10_Clock", "Exposure", QString("%1").arg(ui->spinBox_10->value()));
		WriteConfigure("config.ini", "Camera_11_Clock", "Exposure", QString("%1").arg(ui->spinBox_11->value()));

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
	
}
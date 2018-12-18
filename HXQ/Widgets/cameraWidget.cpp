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
	ReadConfigure("config.ini", "TopCamera", "Exposure", Value);
	ui->spinBox_top->setValue(Value.toInt());

	ReadConfigure("config.ini", "SideCamera", "Exposure", Value);
	ui->spinBox_side->setValue(Value.toInt());


}

void cameraWidget::SaveExposureToIni()
{
		WriteConfigure("config.ini", "TopCamera", "Exposure", QString("%1").arg(ui->spinBox_top->value()));
		WriteConfigure("config.ini", "SideCamera", "Exposure", QString("%1").arg(ui->spinBox_side->value()));

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
	
}
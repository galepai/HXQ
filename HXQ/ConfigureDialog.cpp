#include "ConfigureDialog.h"
#include "ui_ConfigureDialog.h"
#include "Func.h"
#include <QtSerialPort\QSerialPortInfo>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include "Widgets\motionCardWidget.h"
#include "Widgets\camera2Widget.h"
#include "Widgets\detectWidget.h"
#include "Widgets\saveWidget.h"

ConfigureDialog::ConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));

	ui->listWidget->setViewMode(QListView::IconMode);
	ui->listWidget->setIconSize(QSize(96, 84));
	ui->listWidget->setMovement(QListView::Static);
	//ui->listWidget->setMaximumWidth(128);
	ui->listWidget->setSpacing(12);
	

	QListWidgetItem *configButton3 = new QListWidgetItem(ui->listWidget);
	//configButton3->setIcon(QIcon(":/icon/Resources/icon/05.ico"));
	configButton3->setIcon(QIcon(":/icon03/Resources/icon03/setting_on.png"));
	configButton3->setText(G2U("尺寸设置"));
	configButton3->setTextAlignment(Qt::AlignHCenter);
	configButton3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *configButton2 = new QListWidgetItem(ui->listWidget);
	//configButton2->setIcon(QIcon(":/icon/Resources/icon/03.ico"));
	configButton2->setIcon(QIcon(":/icon03/Resources/icon03/camera_on.png"));
	configButton2->setText(G2U("相机设置"));
	configButton2->setTextAlignment(Qt::AlignHCenter);
	configButton2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *configButton = new QListWidgetItem(ui->listWidget);
	//configButton->setIcon(QIcon(":/icon/Resources/icon/01.ico"));
	configButton->setIcon(QIcon(":/icon03/Resources/icon03/motion_on.png"));
	configButton->setText(G2U("运动控制"));
	configButton->setTextAlignment(Qt::AlignHCenter);
	configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *configButton4 = new QListWidgetItem(ui->listWidget);
	//configButton3->setIcon(QIcon(":/icon/Resources/icon/05.ico"));
	configButton4->setIcon(QIcon(":/icon03/Resources/icon03/folder_on.png"));
	configButton4->setText(G2U("保存设置"));
	configButton4->setTextAlignment(Qt::AlignHCenter);
	configButton4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	ui->stackedWidget->addWidget(new detectWidget);
	ui->stackedWidget->addWidget(new camera2Widget);
	ui->stackedWidget->addWidget(new motionCardWidget);
	ui->stackedWidget->addWidget(new saveWidget);

	connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));

	HIDDLE_DIALOG_BUTTON
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::currentRowChanged(int currentRow)
{
	ui->stackedWidget->setCurrentIndex(currentRow);
}


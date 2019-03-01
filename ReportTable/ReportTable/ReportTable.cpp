#include "ReportTable.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QDateTime>
#include <QTimer>


ReportTable::ReportTable(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::ToolTip);	//隐藏标题栏，比如最小化，最大化、关闭按钮

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnShutDown()));

	if (connect_mysql(m_sqlDatabase, "localhost", 3306, "hxq", "ycgd", "ycgd"))
	{
		m_pSqlTableModel = new QSqlTableModel(nullptr, m_sqlDatabase);
		m_pSqlTableModel->setTable("total");  //指定模型要关联的表(employee)
		m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
		m_pSqlTableModel->setSort(1, Qt::DescendingOrder); //id属性，即第0列，升序排列
		m_pSqlTableModel->select();  //查询表格数据

		ui.tableView->setModel(m_pSqlTableModel);
		ui.tableView->hideColumn(0); // don't show the ID
		ui.tableView->resizeColumnsToContents();
		ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

		QTimer* timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateMySql()));
		timer->start(5000);
	}
}

void ReportTable::updateMySql()
{
		m_pSqlTableModel->select();  //查询表格数据
}

bool ReportTable::connect_mysql(QSqlDatabase& db,const QString& hostName, const int& port,const QString& dataBaseName,const QString& userName,
	const QString& passWord)
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(hostName);      //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
	db.setPort(port);                 //连接数据库端口号，与设置一致
	db.setDatabaseName(dataBaseName);      //连接数据库名，与设置一致
	db.setUserName(userName);          //数据库用户名，与设置一致
	db.setPassword(passWord);    //数据库密码，与设置一致
	db.open();
	if (!db.open())
	{
		qDebug() << "connect to mysql error" << db.lastError().text();
		return false;
	}
	else
	{
		qDebug() << "connect to mysql OK";
		return true;
	}

}

void ReportTable::OnShutDown()
{
	qApp->quit();
}
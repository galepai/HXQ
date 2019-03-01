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
	setWindowFlags(Qt::ToolTip);	//���ر�������������С������󻯡��رհ�ť

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnShutDown()));

	if (connect_mysql(m_sqlDatabase, "localhost", 3306, "hxq", "ycgd", "ycgd"))
	{
		m_pSqlTableModel = new QSqlTableModel(nullptr, m_sqlDatabase);
		m_pSqlTableModel->setTable("total");  //ָ��ģ��Ҫ�����ı�(employee)
		m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
		m_pSqlTableModel->setSort(1, Qt::DescendingOrder); //id���ԣ�����0�У���������
		m_pSqlTableModel->select();  //��ѯ�������

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
		m_pSqlTableModel->select();  //��ѯ�������
}

bool ReportTable::connect_mysql(QSqlDatabase& db,const QString& hostName, const int& port,const QString& dataBaseName,const QString& userName,
	const QString& passWord)
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(hostName);      //�������ݿ���������������Ҫע�⣨�����Ϊ��127.0.0.1�������ֲ������ӣ����Ϊlocalhost)
	db.setPort(port);                 //�������ݿ�˿ںţ�������һ��
	db.setDatabaseName(dataBaseName);      //�������ݿ�����������һ��
	db.setUserName(userName);          //���ݿ��û�����������һ��
	db.setPassword(passWord);    //���ݿ����룬������һ��
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
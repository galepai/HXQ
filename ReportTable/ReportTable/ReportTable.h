#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ReportTable.h"
#include <QSqlTableModel>

class ReportTable : public QMainWindow
{
	Q_OBJECT

public:
	ReportTable(QWidget *parent = Q_NULLPTR);

	bool connect_mysql(QSqlDatabase& db,const QString& hostName, const int& port,const QString& dataBaseName, const QString& userName,const QString& passWord);


private:
	Ui::ReportTableClass ui;
	QSqlDatabase m_sqlDatabase;
	QSqlTableModel* m_pSqlTableModel;

public slots:
	void updateMySql();
	void OnShutDown();
};

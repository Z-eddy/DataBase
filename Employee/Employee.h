#pragma once

#include <QtWidgets/QMainWindow>
#include<QSqlDatabase>
#include<QSqlTableModel>
#include<QItemSelectionModel>
#include "ui_Employee.h"

class Employee : public QMainWindow
{
	Q_OBJECT

public:
	Employee(QWidget *parent = Q_NULLPTR);

private:
	void init();
	bool initConnect();//初始化数据库链接
	void initModel();//初始化view的模型
	void setActState(bool ok);//设置使用的action状态

	Ui::EmployeeClass ui;
	QSqlTableModel *theModel;
	QItemSelectionModel *theSelectionModel;
	QString DBPath_;
	QSqlDatabase DB_;

private slots:
	void on_currentChanged();
	void on_currentRowChanged();

	void on_actOpen_triggered();//打开数据库
	void on_actExit_triggered();//退出程序
};

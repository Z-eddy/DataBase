#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include<QSqlDatabase>
#include<QSqlTableModel>
#include<QItemSelectionModel>
#include<QDataWidgetMapper>
#include "ui_Employee.h"

class Employee : public QMainWindow
{
	Q_OBJECT

public:
	Employee(QWidget *parent = Q_NULLPTR);

private:
	void init();
		bool initConnect();//初始化数据库链接
		void initSqlModel();
		void initSelectModel();//必须先初始化SqlModel,selectModel会使用
		void initMapper();//关联界面组件与数据模型的字段映射,必须在sqlModel初始化后
	void setActState(bool ok);//设置使用的action状态

	Ui::EmployeeClass ui;
	QSqlTableModel *theModel_;
	QItemSelectionModel *theSelectionModel_;
	QDataWidgetMapper *theMapper_;
	QString DBPath_;
	QSqlDatabase DB_;

private slots:
	void on_currentChanged();
	void on_currentRowChanged();

	void on_actOpen_triggered();//打开数据库
	void on_actExit_triggered();//退出程序
};

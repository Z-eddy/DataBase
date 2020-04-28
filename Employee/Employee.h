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
		void initTabView();
		bool initConnect();//初始化数据库链接
		void initSqlModel();
		void initSelectModel();//必须先初始化SqlModel,selectModel会使用
		void initMapper();//关联界面组件与数据模型的字段映射,必须在sqlModel初始化后
	void setActState(bool ok);//设置使用的action状态
	void getFieldNames();

	Ui::EmployeeClass ui;
	QSqlTableModel *theModel_;
	QItemSelectionModel *theSelectionModel_;
	QDataWidgetMapper *theMapper_;
	QString DBPath_;
	QSqlDatabase DB_;

private slots:
	void on_currentChanged(const QModelIndex&current, const QModelIndex&previous);
	void on_currentRowChanged(const QModelIndex&current, const QModelIndex&previous);

	void on_actOpen_triggered();//打开数据库
	void on_actExit_triggered();//退出程序
	void on_actAppend_triggered();//添加数据到最后一行
	void on_actInsert_triggered();//插入行
	void on_actDelete_triggered();//删除行
	void on_actSubmit_triggered();//保存
	void on_actRevert_triggered();//撤销更改
	void on_actPhoto_triggered();
	void on_actClear_triggered();
	void on_actScan_triggered();

	void on_comboField_currentIndexChanged(int col);
	void on_rBtnAscend_clicked();//调用 on_comboField_currentIndexChanged
	void on_rBtnDescend_clicked();//调用 on_comboField_currentIndexChanged
	
	void on_rBtnBoth_clicked();
	void on_rBtnMan_clicked();//调用 on_rBtnBoth_clicked
	void on_rBtnWoman_clicked();//调用 on_rBtnBoth_clicked
};

#include "MainWindow.h"
#include<iostream>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlTableModel>
#include "ui_MainWindow.h"
using std::cout;
using std::ends;
using std::endl;

static const QString DBFileName{ "test" };

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::init()
{
	if (connectDB(DBFileName)) {
		QSqlQuery query(QSqlDatabase::database("con" + DBFileName));
		const QString order{
			"CREATE TABLE student("
			"id INT PRIMARY KEY AUTO_INCREMENT,"
			"name VARCHAR(20),"
			"age INT)"
		};
		if (!query.exec(order)) {
			cout << "create table error:" << query.lastError().text().toStdString() << endl;
		}
	}
}

bool MainWindow::connectDB(const QString& dbName)
{
	//指定了名字就必须后面query实例化时指定名字
	QSqlDatabase db{ QSqlDatabase::database("con" + dbName) };
	if (!db.isValid()) {//无效则创建链接
		db = QSqlDatabase::addDatabase("QMYSQL", "con" + DBFileName);
	}
	if (db.isOpen()) {//如果已经打开则直接返回
		return true;
	}
	//未打开链接则链接
	db.setHostName("localHost");
	db.setDatabaseName(dbName);
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open()) {//打开失败时
		cout << "db open error:" << db.lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

void MainWindow::querySelect()
{
	if (connectDB(DBFileName)) {
		QSqlQuery query(QSqlDatabase::database("con" + DBFileName));
		query.exec("SELECT name,age FROM student");
		while (query.next()) {
			const auto name{ query.value(0).toString() };
			const auto age{ query.value(1).toInt() };
			cout << name.toStdString() << ends << age << endl;
		}
	}
}

void MainWindow::queryInsert()
{
	if (connectDB(DBFileName)) {
		QSqlQuery query(QSqlDatabase::database("con" + DBFileName));
		query.prepare("INSERT INTO student (name,age) VALUES (?,?)");
		QVariantList names;
		names << "nameA" << "nameB" << "nameC";
		query.addBindValue(names);
		QVariantList ages;
		ages << 12 << 28 << 29;
		query.addBindValue(ages);
		if (!query.execBatch()) {
			cout << "insert values error:" << query.lastError().text().toStdString() << endl;
		}
		query.finish();
	}
}

void MainWindow::sqlTableSelect()
{
	if (connectDB(DBFileName)) {
		QSqlTableModel model(nullptr,QSqlDatabase::database("con" + DBFileName));
		model.setTable("student");
		model.setFilter("age>20 and age<30");
		if (model.select()) {//如果执行成功
			const int size{ model.rowCount() };
			for (int i{ 0 }; i < size; ++i) {
				const QSqlRecord record{ model.record(i) };
				auto name{ record.value("name").toString() };
				auto age{ record.value("age").toInt() };
				cout << name.toStdString() << ends << age << endl;
			}
		}
	}
}

void MainWindow::sqlTableInsert()
{
	if (connectDB(DBFileName)) {
		QSqlTableModel model{ nullptr,QSqlDatabase::database("con" + DBFileName) };
		model.setTable("student");
		int row{ 0 };
		//model.removeRows(row, 1);//删除
		model.insertRows(row, 1);
		model.setData(model.index(row, 1), "Chen");//setData直接更改值
		model.setData(model.index(row, 2), 22);
		model.submitAll();//提交所有修改
	}
}

void MainWindow::on_btnWrite_clicked()
{
	sqlTableInsert();
}

void MainWindow::on_btnRead_clicked()
{
	//sqlTableSelect();
	querySelect();
}

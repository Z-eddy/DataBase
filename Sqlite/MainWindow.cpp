#include "MainWindow.h"
#include<iostream>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include "ui_MainWindow.h"
using std::cout;
using std::ends;
using std::endl;

static const QString DBFileName{ "test.db" };

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),ui(new Ui::MainWindow)
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
		QSqlQuery query(QSqlDatabase::database("con"+DBFileName));
		const QString order{
			"CREATE TABLE student("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"name VARCHAR,"
			"age INT)"
		};
		if (!query.exec(order)) {
			cout << "create table error:" << query.lastError().text().toStdString() << endl;
		}
	}
}

bool MainWindow::connectDB(const QString& dbName)
{
	QSqlDatabase db{ QSqlDatabase::addDatabase("QSQLITE","con"+dbName) };
	db.setDatabaseName(dbName);
	if (db.isOpen()) {
		return true; 
	}
	else if (!db.open()) {
		cout << "db open error:" << db.lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

void MainWindow::on_btnWrite_clicked()
{
	if (connectDB(DBFileName)) {
		QSqlQuery query(QSqlDatabase::database("con"+DBFileName));
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

void MainWindow::on_btnRead_clicked()
{
	if (connectDB(DBFileName)) {
		QSqlQuery query(QSqlDatabase::database("con"+DBFileName));
		query.exec("SELECT name,age FROM student");
		while (query.next()) {
			const auto name{ query.value(0).toString() };
			const auto age{ query.value(1).toInt() };
			cout << name.toStdString() << ends << age << endl;
		}
}
}

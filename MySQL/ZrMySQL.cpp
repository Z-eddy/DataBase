#include "ZrMySQL.h"
#include<iostream>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
using std::cout;
using std::endl;
using std::ends;

ZrMySQL::ZrMySQL(const QString&connectName, const QString&dataBaseName) :
	connectName_(connectName),\
	dataBaseName_(dataBaseName),\
	dataBase_(nullptr)
{
	init();
}

ZrMySQL::~ZrMySQL()
{
	if (dataBase_&&dataBase_->isValid()) {
		QSqlDatabase::removeDatabase(dataBaseName_);
		delete dataBase_;
		dataBase_ = nullptr;
	}
}

QSqlDatabase ZrMySQL::getDataBase()
{
	return *dataBase_;
}

bool ZrMySQL::createDB(const QString&DBName)
{
	dataBaseName_ = DBName;
	if (!dataBase_)return false;//空指针时返回
	const QString s{ "CREATE DATABASE IF NOT EXISTS " + dataBaseName_ };//如果不存在则创建数据库
	dataBase_->exec(s);
	if (dataBase_->lastError().isValid()) {
		cout << "create database error:" << dataBase_->lastError().text().toStdString() << endl;
		return false;
	}
	return initDB();
}

bool ZrMySQL::createTable(const QString&tabName)
{
	if (!dataBase_)return false;//空指针时返回
	//CREATE TABLE IF NOT EXISTS dataBaseName_.persons//如果不存在则创建表
	const QString s{ "CREATE TABLE IF NOT EXISTS " + dataBaseName_ + "." + tabName };//如果不存在则创建表
	dataBase_->exec(s);
	if (dataBase_->lastError().isValid()) {
		cout << "table create failed:" << dataBase_->lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

void ZrMySQL::init()
{
	initDBPointer();
	initDB();
}

void ZrMySQL::initDBPointer()
{
	dataBase_ = new QSqlDatabase{ QSqlDatabase::addDatabase("QMYSQL", connectName_) };
	if (!dataBase_->isValid()) {//无效则创建链接
		cout << (dataBase_ == nullptr) << endl;
		delete dataBase_;
		dataBase_ = nullptr;
		cout << "db is invalid:" << dataBase_->lastError().text().toStdString() << endl;
		return;
	}
	if (dataBase_->isOpen())return;
	dataBase_->setHostName("localHost");
	dataBase_->setUserName("root");
	dataBase_->setPassword("123456");
	if (!dataBase_->open()) {
		cout << "db open error:" << dataBase_->lastError().text().toStdString() << endl;
	}
}

bool ZrMySQL::initDB()
{
	if (!dataBase_ || dataBaseName_.isEmpty())return false;//空指针或者无数据库名时返回

	dataBase_->setDatabaseName(dataBaseName_);
	if (!dataBase_->open()) {//打开失败
		cout << "open DB error:" << dataBase_->lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

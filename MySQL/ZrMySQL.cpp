#include "ZrMySQL.h"
#include<iostream>
#include<QSqlDatabase>
#include<QSqlError>
using std::cout;
using std::endl;
using std::ends;

ZrMySQL::ZrMySQL(const QString&dataBaseName, const QString&connectName):
	dataBaseName_(dataBaseName),\
	connectName_(connectName),
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

QSqlDatabase* ZrMySQL::getDataBase()
{
	return dataBase_;
}

void ZrMySQL::init()
{
	dataBase_ = new QSqlDatabase{ QSqlDatabase::addDatabase("QMYSQL", connectName_) };
	if (!dataBase_->isValid()) {//无效则创建链接
		cout << "db is invalid:" << dataBase_->lastError().text().toStdString() << endl;
		return;
	}
	if (dataBase_->isOpen())return;
	dataBase_->setHostName("localHost");
	dataBase_->setDatabaseName(dataBaseName_);
	dataBase_->setUserName("root");
	dataBase_->setPassword("123456");
	if (!dataBase_->open()) {
		cout << "db open error:" << dataBase_->lastError().text().toStdString() << endl;
	}
}

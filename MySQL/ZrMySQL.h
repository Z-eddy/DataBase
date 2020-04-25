#pragma once
#include<QString>

class QSqlDatabase;

class ZrMySQL
{
public:
	ZrMySQL(const QString&dataBaseName, const QString&connectName="");
	~ZrMySQL();

	QSqlDatabase* getDataBase();

private:
	void init();
	
	QString dataBaseName_;//打开的数据库名
	QString connectName_;//链接的名字
	QSqlDatabase *dataBase_;
};


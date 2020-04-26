#pragma once
#include<QString>

class QSqlDatabase;

class ZrMySQL
{
public:
	ZrMySQL(const QString&connectName = "", const QString&dataBaseName = "");
	~ZrMySQL();

	QSqlDatabase getDataBase();
	//创建并打开数据库,如果已存在则直接打开
	bool createDB(const QString&DBName);
	//bool createTable(const QString&tabName);
	//执行query
	void query(const QString&s);

private:
	void init();
		void initDBPointer();//初始化指针
	bool initDB();//根据数据库名初始化,如果不存在则创建
	
	QString connectName_;//链接的名字
	QString dataBaseName_;//打开的数据库名
	QSqlDatabase *dataBase_;
};


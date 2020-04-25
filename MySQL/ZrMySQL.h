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
	
	QString dataBaseName_;//�򿪵����ݿ���
	QString connectName_;//���ӵ�����
	QSqlDatabase *dataBase_;
};


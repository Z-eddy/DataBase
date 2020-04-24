#pragma once

#include <QMainWindow>
#include<QMetaObject>
namespace Ui { class MainWindow; };

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Q_CLASSINFO("test","测试内容")

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private:
	//QSqlQuery
	void init();
	bool connectDB(const QString& dbName);
	void querySelect();
	void queryInsert();
	//QSqlTableModel
	void sqlTableSelect();
	void sqlTableInsert();//delete

	Ui::MainWindow *ui;

private slots:
	void on_btnWrite_clicked();
	void on_btnRead_clicked();
};

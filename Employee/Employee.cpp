#include "Employee.h"
#include<iostream>
#include<QFileDialog>
#include<QSqlError>
using std::cout;
using std::endl;
using std::ends;

Employee::Employee(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Employee::init()
{
	if (!initConnect())return;//无法链接则退出
	initSqlModel();
	initSelectModel();
	initMapper();
	if (theModel_->select()) {
		setActState(true);//enable action
	}
	else {
		cout << "select error:" << theModel_->lastError().text().toStdString() << endl;
	}
}

bool Employee::initConnect()
{
	DB_ = QSqlDatabase::addDatabase("QSQLITE", "theSqliteConnectName");
	if (!DB_.isValid()) {
		cout <<"invalid:"<< DB_.lastError().text().toStdString() << endl;
		return false;
	}
	DB_.setDatabaseName(DBPath_);
	if (!DB_.open()) {//成功打开
		cout <<"DB open error:"<< DB_.lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

static const QString TABLENAME{ "employee" };//table name
void Employee::initSqlModel()
{
	theModel_ = new QSqlTableModel(this, DB_);
	theModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);//更改view数据时不自动提交
	theModel_->setTable(TABLENAME);
	theModel_->setSort(0, Qt::AscendingOrder);    

	/*
	theModel_->setHeaderData(theModel_->fieldIndex("empNo"), Qt::Horizontal, "工号");
	theModel_->setHeaderData(theModel_->fieldIndex("Name"), Qt::Horizontal, "姓名");
	theModel_->setHeaderData(theModel_->fieldIndex("Gender"), Qt::Horizontal, "性别");
	theModel_->setHeaderData(theModel_->fieldIndex("Height"), Qt::Horizontal, "身高");
	theModel_->setHeaderData(theModel_->fieldIndex("Birthday"), Qt::Horizontal, "出生日期");
	theModel_->setHeaderData(theModel_->fieldIndex("Mobile"), Qt::Horizontal, "手机");
	theModel_->setHeaderData(theModel_->fieldIndex("Province"), Qt::Horizontal, "省份");
	theModel_->setHeaderData(theModel_->fieldIndex("City"), Qt::Horizontal, "城市");
	theModel_->setHeaderData(theModel_->fieldIndex("Department"), Qt::Horizontal, "部门");
	theModel_->setHeaderData(theModel_->fieldIndex("Education"), Qt::Horizontal, "学历");
	theModel_->setHeaderData(theModel_->fieldIndex("Salary"), Qt::Horizontal, "工资");

	theModel_->setHeaderData(theModel_->fieldIndex("Memo"), Qt::Horizontal, "备注"); //这两个字段不再tableView中显示
	theModel_->setHeaderData(theModel_->fieldIndex("Photo"), Qt::Horizontal, "照片");
	*/

	ui.tabView->setModel(theModel_);//早设定,否则后续设定可能无效
	ui.tabView->setColumnHidden(theModel_->fieldIndex("Memo"), true);
	ui.tabView->setColumnHidden(theModel_->fieldIndex("Photo"), true);
}

void Employee::initSelectModel()
{
	theSelectionModel_ = new QItemSelectionModel(theModel_);
	ui.tabView->setSelectionModel(theSelectionModel_);

	connect(theSelectionModel_, &QItemSelectionModel::currentChanged, this, &Employee::on_currentChanged);
	connect(theSelectionModel_, &QItemSelectionModel::currentRowChanged, this, &Employee::on_currentRowChanged);
}

void Employee::initMapper()
{

}

void Employee::setActState(bool ok)
{
	ui.actOpen->setEnabled(!ok);
	ui.actAppend->setEnabled(ok);
	ui.actInsert->setEnabled(ok);
	ui.actDelete->setEnabled(ok);
	ui.actPhoto->setEnabled(ok);
	ui.actClear->setEnabled(ok);
	ui.actScan->setEnabled(ok);
}

void Employee::on_currentChanged()
{

}

void Employee::on_currentRowChanged()
{

}

void Employee::on_actOpen_triggered()
{
	const QString tempPath{ QFileDialog::getOpenFileName(this,tr("选择数据库"),DBPath_,"DB(*.db3 *.db);;Other(*.*)") };
	if (tempPath.isEmpty())return;
	if (DBPath_ != tempPath) {//记忆路径
		DBPath_ = tempPath;
	}
	init();
}

void Employee::on_actExit_triggered()
{
	this->close();
}

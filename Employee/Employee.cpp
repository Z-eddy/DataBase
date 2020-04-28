#include "Employee.h"
#include<iostream>
#include<QFileDialog>
#include<QSqlError>
#include<QSqlRecord>
#include<QPixmap>
#include<QByteArray>
#include<QFile>
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
	initTabView();
	initSqlModel();
	initSelectModel();
	initMapper();
	if (theModel_->select()) {
		setActState(true);//enable action
		getFieldNames();
	}
	else {
		cout << "select error:" << theModel_->lastError().text().toStdString() << endl;
		return;
	}
}

void Employee::initTabView()
{
	ui.tabView->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui.tabView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tabView->setAlternatingRowColors(true);
}

bool Employee::initConnect()
{
	DB_ = QSqlDatabase::addDatabase("QMYSQL", "theMySQLConnectName");
	DB_.setHostName("localhost");
	DB_.setPort(3306);
	DB_.setUserName("root");
	DB_.setPassword("123456");
	if (!DB_.isValid()) {
		cout <<"invalid:"<< DB_.lastError().text().toStdString() << endl;
		return false;
	}
	//DB_.setDatabaseName(DBPath_);
	DB_.setDatabaseName("test");
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
	theMapper_ = new QDataWidgetMapper(this);
	theMapper_->setModel(theModel_);
	theMapper_->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

	theMapper_->addMapping(ui.spinEmpNo, theModel_->fieldIndex("EmpNo"));
	theMapper_->addMapping(ui.editName, theModel_->fieldIndex("Name"));
	theMapper_->addMapping(ui.comboSex, theModel_->fieldIndex("Gender"));
	theMapper_->addMapping(ui.dSpinHeight, theModel_->fieldIndex("Height"));
	theMapper_->addMapping(ui.dateBirth, theModel_->fieldIndex("Birthday"));
	theMapper_->addMapping(ui.editMobile, theModel_->fieldIndex("Mobile"));
	theMapper_->addMapping(ui.comboProvince, theModel_->fieldIndex("Province"));
	theMapper_->addMapping(ui.editCity, theModel_->fieldIndex("City"));
	theMapper_->addMapping(ui.comboDepartment, theModel_->fieldIndex("Department"));
	theMapper_->addMapping(ui.comboEducation, theModel_->fieldIndex("Education"));
	theMapper_->addMapping(ui.dSpinSalary, theModel_->fieldIndex("Salary"));
	theMapper_->addMapping(ui.textMemo, theModel_->fieldIndex("Memo"));
	//theMapper_->addMapping(ui.labPhoto, theModel_->fieldIndex("Photo"));//图片无法直接映射

	theMapper_->toFirst();
}

void Employee::setActState(bool ok)
{
	ui.actOpen->setEnabled(!ok);
	ui.actAppend->setEnabled(ok);
	ui.actScan->setEnabled(ok);

	ui.gBoxSort->setEnabled(ok);
	ui.gBoxFilter->setEnabled(ok);
}

void Employee::getFieldNames()
{
	const auto record{ theModel_->record() };
	const auto count{ record.count() };
	QStringList li;
	for (int i{ 0 }; i != count; ++i) {
		li.push_back(record.fieldName(i));
	}
	ui.comboField->addItems(li);
}

void Employee::on_currentChanged(const QModelIndex&current, const QModelIndex&previous)
{
	//cout <<"itemChanged:"<< "last:" << current.row() << ends << current.column() << '\t' << previous.row() << ends << previous.column() << endl;
	bool ok{theModel_->isDirty()};
	ui.actSubmit->setEnabled(ok);
	ui.actRevert->setEnabled(ok);
}

void Employee::on_currentRowChanged(const QModelIndex&current, const QModelIndex&previous)
{
	//cout << "rowChanged:" << "last:" << current.row() << ends << current.column() << '\t' << previous.row() << ends << previous.column() << endl;
	bool valid{ current.isValid() };
	if (!valid) {
		ui.labPhoto->clear();
		return;
	}
	ui.actPhoto->setEnabled(valid);
	ui.actClear->setEnabled(valid);
	ui.actInsert->setEnabled(valid);
	ui.actDelete->setEnabled(valid);

	const int row{ current.row() };
	theMapper_->setCurrentIndex(row);
	const auto record{ theModel_->record(row) };
	if (record.isNull("Photo")) {
		ui.labPhoto->clear();
		return;
	}
	else {
		const auto pixData{ record.value("Photo").toByteArray() };
		QPixmap pixmap;
		pixmap.loadFromData(pixData);
		ui.labPhoto->setPixmap(pixmap.scaledToWidth(ui.labPhoto->width()));
	}
}

void Employee::on_actOpen_triggered()
{
	/*
	const QString tempPath{ QFileDialog::getOpenFileName(this,tr("选择数据库"),DBPath_,"DB(*.db3 *.db);;Other(*.*)") };
	if (tempPath.isEmpty())return;
	if (DBPath_ != tempPath) {//记忆路径
		DBPath_ = tempPath;
	}
	*/
	init();
}

void Employee::on_actExit_triggered()
{
	this->close();
}

void Employee::on_actAppend_triggered()
{
	theModel_->insertRow(theModel_->rowCount());
	theSelectionModel_->clearSelection();

	const QModelIndex index{ theModel_->index(theModel_->rowCount() - 1,1) };
	theSelectionModel_->setCurrentIndex(index,QItemSelectionModel::Select);

	theModel_->setData(theModel_->index(index.row(), theModel_->fieldIndex("empNo")), \
		2000 + theModel_->rowCount());
	theModel_->setData(theModel_->index(index.row(), theModel_->fieldIndex("Gender")), \
		theModel_->rowCount() % 2 ? "男" : "女");
}

void Employee::on_actInsert_triggered()
{
	const QModelIndex index{ ui.tabView->currentIndex() };
	theModel_->insertRow(index.row());
	theSelectionModel_->clearSelection();
	theSelectionModel_->setCurrentIndex(index, QItemSelectionModel::Select);
}

void Employee::on_actDelete_triggered()
{
	const QModelIndex index{ ui.tabView->currentIndex() };
	theModel_->removeRow(index.row());
}

void Employee::on_actSubmit_triggered()
{
	const auto ok{ theModel_->submitAll() };
	if (ok) {
		ui.actSubmit->setEnabled(false);
		ui.actRevert->setEnabled(false);
	}
	else {
		cout << theModel_->lastError().text().toStdString() << endl;
	}
}

void Employee::on_actRevert_triggered()
{
	theModel_->revertAll();
	ui.actSubmit->setEnabled(false);
	ui.actRevert->setEnabled(false);
}

void Employee::on_actPhoto_triggered()
{
	static QString pixPath;
	const auto index{ theSelectionModel_->currentIndex() };
	if (!index.isValid())return;
	const QString tempPath{ QFileDialog::getOpenFileName(this,"打开文件",pixPath,"pixmap(*.jpg *.png);;other(*.*)") };
	if (tempPath.isEmpty())return;
	if (pixPath != tempPath) {
		pixPath = tempPath;//记忆路径
	}
	QFile file(pixPath);
	if (!file.open(QIODevice::ReadOnly)) {
		cout << "pixmap file open error:" << ends << file.errorString().toStdString();
		return;
	}
	const auto data{ file.readAll() };
	file.close();

	//直接从model设置
	//theModel_->setData(theModel_->index(index.row(),theModel_->fieldIndex("Photo")), data);

	//从recorder设置
	auto record{ theModel_->record(index.row()) };
	record.setValue("Photo", data);
	theModel_->setRecord(index.row(), record);

	QPixmap pix;
	pix.loadFromData(data);
	ui.labPhoto->setPixmap(pix.scaledToWidth(ui.labPhoto->width()));
}

void Employee::on_actClear_triggered()
{
	const int row{ theSelectionModel_->currentIndex().row() };
	QSqlRecord record{ theModel_->record(row) };
	record.setNull("Photo");
	theModel_->setRecord(row, record);
	ui.labPhoto->clear();
}

void Employee::on_actScan_triggered()
{
	const int size{ theModel_->rowCount() };
	for (int i{ 0 }; i != size; ++i) {
		QSqlRecord record{ theModel_->record(i) };
		record.setValue("Salary", record.value("Salary").toDouble()*1.1);
		theModel_->setRecord(i, record);
	}
}

void Employee::on_comboField_currentIndexChanged(int col)
{
	if (ui.rBtnAscend->isChecked()) {
		theModel_->setSort(col, Qt::AscendingOrder);
	}
	else {
		theModel_->setSort(col, Qt::DescendingOrder);
	}
	theModel_->select();
}

void Employee::on_rBtnAscend_clicked()
{
	on_comboField_currentIndexChanged(ui.comboField->currentIndex());
}

void Employee::on_rBtnDescend_clicked()
{
	on_comboField_currentIndexChanged(ui.comboField->currentIndex());
}

void Employee::on_rBtnBoth_clicked()
{
	if (ui.rBtnBoth->isChecked()) {
		theModel_->setFilter("");
	}
	else if (ui.rBtnMan->isChecked()) {
		theModel_->setFilter("Gender='男'");
	}
	else {
		theModel_->setFilter("Gender='女'");
	}
	theModel_->select();
}

void Employee::on_rBtnMan_clicked()
{
	on_rBtnBoth_clicked();
}

void Employee::on_rBtnWoman_clicked()
{
	on_rBtnBoth_clicked();
}

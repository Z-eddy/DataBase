#include "MainUI.h"
#include "ui_MainUI.h"

MainUI::MainUI(QWidget *parent)
	: QWidget(parent),mySql("test","conMySQL"),\
	theModel(nullptr)
{
	ui = new Ui::MainUI();
	ui->setupUi(this);
	init();
}

MainUI::~MainUI()
{
	delete ui;
}

void MainUI::init()
{
	theModel = new QSqlTableModel(this, *mySql.getDataBase());
	theModel->setSort(1, Qt::AscendingOrder);//index 1 name����

	ui->tableView->setModel(theModel);
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ui->tableView->setColumnHidden(0, true);//id ������
	ui->tableView->resizeColumnsToContents();//��С�����ı�
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//�޷��༭

	QHeaderView *header{ ui->tableView->horizontalHeader() };
	header->setStretchLastSection(true);//���һ������,����ʾ�հ���
}

void MainUI::on_comboDB_currentIndexChanged(int index)
{
	QString tableName;
	switch (index) {
	case 0:
		tableName = "classes";
		break;
	case 1:
		tableName = "pet";
		break;
	case 2:
		tableName = "student";
		break;
	case 3:
		tableName = "students";
		break;
	case 4:
		tableName = "user";
		break;
	}
	theModel->setTable(tableName);
	//theModel->setHeaderData(1, Qt::Horizontal, "aName");//�ֶ�ָ������
	//theModel->setHeaderData(2, Qt::Horizontal, "theAge");//Ĭ�������ݿ�ָ��������
	theModel->select();
}

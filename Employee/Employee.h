#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Employee.h"

class Employee : public QMainWindow
{
	Q_OBJECT

public:
	Employee(QWidget *parent = Q_NULLPTR);

private:
	Ui::EmployeeClass ui;
};

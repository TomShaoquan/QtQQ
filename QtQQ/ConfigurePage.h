#pragma once
#include<QDialog>

//#include"basicwindow.h"
#include "ui_ConfigurePage.h"
#include"UserLogin.h"

class ConfigurePage : public QDialog
{
	Q_OBJECT

public:
	ConfigurePage(QDialog *parent = Q_NULLPTR);
	~ConfigurePage();
public slots:
	void onOkBtnClicked();
public:
	UserLogin* login;
private:
	Ui::ConfigurePage ui;
};

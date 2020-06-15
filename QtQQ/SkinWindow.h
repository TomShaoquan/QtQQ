#pragma once

#include "basicwindow.h"
#include "ui_SkinWindow.h"

class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = Q_NULLPTR);
	~SkinWindow();
public:
	void initControl();
public slots:
	void onShowClose();
private:
	Ui::SkinWindow ui;
};

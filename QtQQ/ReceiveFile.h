#pragma once

#include "basicwindow.h"
#include "ui_ReceiveFile.h"

class ReceiveFile : public BasicWindow
{
	Q_OBJECT

public:
	ReceiveFile(QWidget *parent = Q_NULLPTR);
	~ReceiveFile();
	void setMsg(QString& msgLabel);
signals:
	void refuseFile();
private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked();

private:
	Ui::ReceiveFile ui;
};

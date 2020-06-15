#pragma once

#include "basicwindow.h"
#include "ui_SendFile.h"

class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = Q_NULLPTR);
	~SendFile();
signals://发送文件发射的信号
	void sendFileClicked(QString&, int& msgType, QString flieName);
private slots:
	void on_openBtn_clicked();
	void on_sendBtn_clicked();

private:
	Ui::SendFile ui;
	QString m_filePath;			//文件路径
};

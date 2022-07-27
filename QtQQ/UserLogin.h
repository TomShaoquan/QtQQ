#pragma once
#include<qtcpsocket.h>
#include "basicwindow.h"
#include "ui_UserLogin.h"
#include<QKeyEvent>

class MyTcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	MyTcpSocket();
	~MyTcpSocket();

private slots:
	void OnDisConnected();

};
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();
	void keyPressEvent(QKeyEvent *event);
	void setConfgure(QStringList list);

private slots:
	void onLoginBtnCicked();
	
private:
	void initControl();
	bool connectMySql();
	bool varyfyAccountCode(bool& isAccountLogin,QString& strAccount);
	

private:
	Ui::UserLogin ui;

	QString dbName;   //���ݿ���
	QString dbUser;   //���ݿ��˻�
	QString dbPasscode;   //���ݿ�����

};

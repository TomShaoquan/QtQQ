#pragma once
#include<QFile>
#include<QTextStream>
#include<QDateTime>
#include<QApplication>

class MyTcpSocket;

//#define SERVER_IP   "127.0.0.1"    //数据库服务器IP
//#define MSG_SERVER_IP   "192.168.92.128"   //业务服务器ip
extern QString dbServerIP;
extern QString dbServerPort;
extern QString MsgServerIP;
extern QString MsgServerPort;
/*
/root/projects/QtQQ_Server/bin/x64/Debug
*/



extern MyTcpSocket gTcpSocket;
extern QString gLoginEmployeeID;	// 登陆者员工号
//日志文件
#define LOG_FILE  qApp->applicationDirPath()+"/logger.txt"
void print_run_log(QString str);
QString getHeadPixPath(QString uid);

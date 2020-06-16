#pragma once
#include<QFile>
#include<QTextStream>
#include<QDateTime>
#include<QApplication>

class MyTcpSocket;
#define SERVER_IP   "114.55.145.67"
/*
/root/projects/QtQQ_Server/bin/x64/Debug
*/
const int gtcpProt = 8888;

extern MyTcpSocket gTcpSocket;
extern QString gLoginEmployeeID;	// 登陆者员工号
//日志文件
#define LOG_FILE  qApp->applicationDirPath()+"/logger.txt"
void print_run_log(QString str);
QString getHeadPixPath(QString uid);

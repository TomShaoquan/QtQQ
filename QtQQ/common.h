#pragma once
#include<QFile>
#include<QTextStream>
#include<QDateTime>
#include<QApplication>

class MyTcpSocket;

//#define SERVER_IP   "127.0.0.1"    //���ݿ������IP
//#define MSG_SERVER_IP   "192.168.92.128"   //ҵ�������ip
extern QString dbServerIP;
extern QString dbServerPort;
extern QString MsgServerIP;
extern QString MsgServerPort;
/*
/root/projects/QtQQ_Server/bin/x64/Debug
*/



extern MyTcpSocket gTcpSocket;
extern QString gLoginEmployeeID;	// ��½��Ա����
//��־�ļ�
#define LOG_FILE  qApp->applicationDirPath()+"/logger.txt"
void print_run_log(QString str);
QString getHeadPixPath(QString uid);

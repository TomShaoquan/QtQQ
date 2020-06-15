#pragma once

#include"basicwindow.h"
#include "ui_TalkWindowShell.h"
#include<qmap.h>
#include<QTcpSocket>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

enum GroupType
{
	COMPANY,
	PERSONGROUP,	//
	DEVELOPMENTGROUP,	//开发部
	MARKETGROUP,	//市场部
	PTOP		//单独聊天
};
class  TalkWindowShell: public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	//添加新的聊天窗口
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString & uid/*,GroupType grouptype*/);
	//设置当前聊天窗口
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

private:
	void initControl();
	void getEmployeesID(QStringList& employeeIDList);	//获取所有员工QQ号

	bool createJSFile(QStringList& employeesList);
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);
public slots:
	void onEmotionBtnClicked(bool);
	//客户端发送Tcp数据
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");
private slots:
	//左侧列表点击后执行的函数
	void onTalkWindowItemClicked(QListWidgetItem* item);
	void onEmotionItemClicked(int emotionNum);
	void onProcessTcpData();	//处理收到的数据
private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap; //打开的聊天窗口
	EmotionWindow* m_emotionWindow;	

private:
};

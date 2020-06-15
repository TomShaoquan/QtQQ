#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include"TalkWindowShell.h"
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid/*,GroupType groupType*/);
	~TalkWindow();
public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);
	QString getTalkId();
private slots:
	
	void onSendBtnClicded(bool);
	void onItemDoubleClicked(QTreeWidgetItem*, int);
	void onSetEmotionBtnStatus();
	void onFileOpenBtnClicked(bool);
private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;	//ÊÇ·ñÎªÈºÁÄ
	//GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;
	friend class TalkWindowShell;
private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();
	
	void initTalkWindow();
	void initPtoPTalk();
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);

};

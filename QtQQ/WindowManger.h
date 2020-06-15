#pragma once

#include <QObject>
#include"TalkWindowShell.h"

class WindowManger : public QObject
{
	Q_OBJECT

public:
	WindowManger();
	~WindowManger();
	QWidget* findWindowName(const QString& qsWindowName);
	void deleteWindowName(const QString& qsWindowName);
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);
	static WindowManger* getInstance();
	TalkWindowShell* getTalkWindowShell();
	QString getCreatingTalkId();

	void addNewTalkWindow(const QString& uid/* GroupType groupType = COMPANY,const QString& strPeople= ""*/);

private:
	TalkWindowShell* m_talkwindowShell;
	QMap<QString, QWidget*> m_windowMap;
	QString m_strCreatingTalkId="";	//正在创建的聊天窗口
};

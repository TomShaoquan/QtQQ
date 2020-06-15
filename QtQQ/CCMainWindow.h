#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"		//带ui界面

class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = Q_NULLPTR);
	~CCMainWindow();
public:
	
	void setUserName(const QString& username);
	void setLevelPixmap(int level);
	void setHeadPixmap(const QString& headPath);
	void setStatusMenuIcon(const QString& statusPath);

	//添加应用部件
	QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);

private:
	void initTimer();		//初始化计时器
	void initControl();
	QString getHeadPicturePath();
	void updateSearchStyle();		//更新搜索样式
	void initContactTree();
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const int& DepID);
private:
	void resizeEvent(QResizeEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void mousePressEvent(QMouseEvent* event);
private slots:
	void onAppIconClicked();
	void onItemClicked(QTreeWidgetItem* item, int);
	void onItemExpanded(QTreeWidgetItem* item);
	void onitemCollapsed(QTreeWidgetItem* item);
	void onitemDoubleClicked(QTreeWidgetItem* item, int);
private:
	Ui::CCMainWindowClass ui;
	bool m_isAccountLogin;
	QString m_account;	//账号 或QQ 号
	//QMap<QTreeWidgetItem*,QString> m_groupMap;		//所有的分组项
};

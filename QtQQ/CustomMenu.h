#pragma once

#include <QMenu>
#include<QMap>

/*自定义菜单*/
class CustomMenu : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent=nullptr);
	~CustomMenu();

	void addCustomMenu(const QString& text, const QString& icon, const QString& name);
	QAction* getAction(const QString& text);
private:
	QMap<QString, QAction*> m_menuActionMap;
};

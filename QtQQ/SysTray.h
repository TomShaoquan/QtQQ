#pragma once

#include <QSystemTrayIcon>
#include<qwidget.h>

/*»ŒŒÒ¿∏Õº±Í**/
class SysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();
public slots:
	void onIconActived(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray();
	void addSystemMenu();
private:
	QWidget* m_parent;
};

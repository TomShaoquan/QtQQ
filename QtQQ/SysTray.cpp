#include "SysTray.h"
#include"CustomMenu.h"
SysTray::SysTray(QWidget *parent)
	:m_parent(parent),
	 QSystemTrayIcon(parent)
{
	initSystemTray();
	show();
}

SysTray::~SysTray()
{
}

void SysTray::onIconActived(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		m_parent->show();
		m_parent->activateWindow();
	}
	else if (reason == QSystemTrayIcon::Context)
	{
		addSystemMenu();
	}
}

void SysTray::initSystemTray()
{
	setToolTip(QString::fromLocal8Bit("EveryOne"));
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActived(QSystemTrayIcon::ActivationReason)));
}

void SysTray::addSystemMenu()
{
	CustomMenu* customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico",
		QStringLiteral("ÏÔÊ¾"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png",
		QStringLiteral("ÍË³ö"));

	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)),
		m_parent, SLOT(onShowQuit(bool)));
	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}

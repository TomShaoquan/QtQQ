#include "ConfigurePage.h"

#include"common.h"

ConfigurePage::ConfigurePage(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi((BasicWindow*)this);
	//setAttribute(Qt::WA_DeleteOnClose);
	//initTitleBar();
	//setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");

	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(QString::fromLocal8Bit("ÅäÖÃ·þÎñÆ÷"));
	connect(ui.OkBtn, SIGNAL(clicked()), SLOT(onOkBtnClicked()));

}

ConfigurePage::~ConfigurePage()
{
}

void ConfigurePage::onOkBtnClicked()
{
	dbServerIP = ui.dbIP->text();
	dbServerPort = ui.dbPort->text();
	QStringList list;
	list << ui.dbName->text() << ui.dbUser->text() << ui.dbPassCode->text();

	((UserLogin*)parent())->setConfgure(list);

	MsgServerIP = ui.MessageIP->text();
	MsgServerPort = ui.MessagePort->text();
	close();

}

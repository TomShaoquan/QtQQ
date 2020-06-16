#include "UserLogin.h"
#include"CCMainWindow.h"
#include<QMessageBox>
#include<qsqldatabase.h>
#include<QSqlQuery>
#include<QSqlError>
#include<QSqlDatabase>

#include"common.h"

QString gLoginEmployeeID;	// 登陆者员工号

MyTcpSocket gTcpSocket;
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
	
}

UserLogin::~UserLogin()
{
}



void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnCicked);

	if (!connectMySql())
	{
		close();
		exit(-1); /*退出*/
	}
}

bool UserLogin::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");
	db.setHostName(SERVER_IP);
	db.setUserName("root");
	db.setPassword("qsq123");
	db.setPort(3306);

	if (db.open())
	{
		return true;
	}
	else
	{
		QSqlError error=db.lastError();
		QMessageBox::information(NULL, QString::fromLocal8Bit("连接数据库失败"),
			db.lastError().databaseText());
		
		QFile file(LOG_FILE);
		file.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream text_stream(&file);
		text_stream << "database connect error:" << errno<<" "<<error.driverText()<<"\r\n";
		QStringList drivers = db.drivers();
		for (int i = 0; i < drivers.size(); i++) {
			text_stream << "drives:" << drivers[i]<< "\r\n";
		}
		text_stream << "bool:"<<db.isOpenError()<<"driverName:" << db.driverName() << "\r\n";
		text_stream << error.nativeErrorCode();

		file.flush();
		file.close();
		return false;
	}
}

bool UserLogin::varyfyAccountCode(bool& isAccountLogin, QString& strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strPasswdInput = ui.editPassword->text();

	//QQ号登陆
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID=%1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();
	if (queryEmployeeID.first())
	{
		QString strCode = queryEmployeeID.value(0).toString();
		if (strCode == strPasswdInput)
		{
			gLoginEmployeeID = strAccountInput;
			isAccountLogin = false;
			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	//账号登录
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account='%1'").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();
		if (strCode == strPasswdInput)
		{
			gLoginEmployeeID= queryAccount.value(1).toString();
			strAccount = strAccountInput;
			isAccountLogin = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void UserLogin::onLoginBtnCicked()
{
	bool isAccountLogin;
	QString strAccount;		//账号或QQ
	if (!varyfyAccountCode(isAccountLogin,strAccount))
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("名输入的账号或密码有误,请从新输入!"));
		/*ui.editUserAccount->clear();
		ui.editPassword->clear();*/
		return;
	}
	close();

	//更改数据库登录状态为在线
	QString strSqlStatus = QString("UPDATE tab_employees SET `online` =2 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	//登录成功后
	gTcpSocket.connectToHost(SERVER_IP, gtcpProt);
	bool ok = false;
	ok= gTcpSocket.waitForConnected();
	QByteArray loginData;
	loginData[0] ='2';
	loginData.append(gLoginEmployeeID);

	gTcpSocket.write(loginData);
	gTcpSocket.waitForBytesWritten();

	CCMainWindow* mainWindow = new CCMainWindow(strAccount,isAccountLogin);
	mainWindow->show();

}


MyTcpSocket::MyTcpSocket()
{
	//QObject::connect(this, SIGNAL(disconnected()),this,SLOT(OnDisConnected()));
}

MyTcpSocket::~MyTcpSocket()
{
}

void MyTcpSocket::OnDisConnected()
{
	bool ok = false;
	do
	{
		this->connectToHost(SERVER_IP, gtcpProt);
		ok = this->waitForConnected();
	} while (!ok);

}

#include "CCMainWindow.h"
#include<qproxystyle.h>
#include<qpainter.h>
#include"SkinWindow.h"
#include<qstring.h>
#include<qtimer.h>
#include"SysTray.h"
#include<QEvent>
#include"NotifyManger.h"
#include"RootContatItem.h"
#include"ContactItem.h"
#include"TalkWindowShell.h"
#include<QMouseEvent>
#include<qapplication.h>
#include"WindowManger.h"
#include<QSqlQuery>
#include<QDir>
#include"common.h"

extern QString gLoginEmployeeID;
QString gstrLoginHeadPath;

class CustomProxyStyle :public QProxyStyle
{
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = nullptr) const
	{
		if (element == PE_FrameFocusRect)
		{
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}

};


CCMainWindow::CCMainWindow(QString account, bool isAccountLogin,QWidget *parent)
	: BasicWindow(parent)
	,m_isAccountLogin(isAccountLogin)
	,m_account(account)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");
	QDir head_imgs("./", "qtqq_images");
	if (!head_imgs.exists())
	{
		head_imgs.mkdir("qtqq_images");
	}
	QString headPath ="./qtqq_images/" + gLoginEmployeeID + ".png";
	/*QFile file_t("./cur.txt");
	file_t.open(QIODevice::WriteOnly);
	file_t.close();*/
	

	setHeadPixmap(headPath);

	print_run_log(headPath);
	print_run_log(head_imgs.currentPath());

	initControl();
	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initTimer()
{
	QTimer* timer = new QTimer(this);
	timer->setInterval(500);
	connect(timer, &QTimer::timeout, [this] {
		static int level = 0;
		if (level == 99) {
			level = 0;
		}
		level++;
		setLevelPixmap(level);
	});
	timer->start();
}

void CCMainWindow::initControl()
{
	//树获取焦点是不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);
	setLevelPixmap(0);
	//setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");
	
	QHBoxLayout* appupLayout = new QHBoxLayout;
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));

	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->addStretch();
	appupLayout->setSpacing(2);
	ui.appWidget->setLayout(appupLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();

	/*初始化树控件*/
	initContactTree();
	//个性签名
	ui.lineEdit->installEventFilter(this);
	ui.searchLineEdit->installEventFilter(this);

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
	
	connect(NotifyManger::getInstance(), &NotifyManger::signalSkinChanged, [this]() {
		updateSearchStyle();
	});
	SysTray* systray = new SysTray(this);
}

QString CCMainWindow::getHeadPicturePath()
{
	QString strPicPath;
	QSqlQuery queryPicture;

	if (!m_isAccountLogin) //QQ号登录
	{
		queryPicture.exec(QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(gLoginEmployeeID));
	}
	else  //账号登录
	{
		queryPicture.exec(QString("SELECT e.picture FROM tab_accounts a,tab_employees e WHERE \
			a.employeeID = e.employeeID AND a.account= '%1'").arg(m_account));
	}
	queryPicture.next();
	strPicPath = queryPicture.value(0).toString();
	gstrLoginHeadPath = strPicPath;
	return strPicPath;
}

void CCMainWindow::setUserName(const QString & username)
{
	ui.nameLabel->adjustSize();

	//文本过长则省略
	QString name=ui.nameLabel->fontMetrics().elidedText(username, Qt::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setText(name);
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));
	
	int unitNum = level % 10;	//个位数
	int tenNum = level / 10;	//十位数

	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());

}

void CCMainWindow::setHeadPixmap(const QString & headPath)
{
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));

}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());
	statusBtnPixmap.fill(Qt::transparent);

	QPainter painter(&statusBtnPixmap);
	painter.drawPixmap(4, 4, QPixmap(statusPath));

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString & appName)
{
	QPushButton* btn = new QPushButton(this);
	btn->setFixedSize(20, 20);

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);
	
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
		(btn->height() - appPixmap.width()) / 2, appPixmap);
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());
	btn->setObjectName(appName);
	btn->setProperty("hasborder", true);

	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
	return btn;
}

void CCMainWindow::updateSearchStyle()
{
	ui.searchLineEdit->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50); border-bottom:1px solid rgba(%1,%2,%3,30)}\
								QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png);}")
		.arg(m_colorBackGround.red())
		.arg(m_colorBackGround.green())
		.arg(m_colorBackGround.blue()));
}

void CCMainWindow::initContactTree()
{
	//展开与收缩时的信号
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), 
		this,SLOT(onItemClicked(QTreeWidgetItem*, int)));

	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
		this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
		this, SLOT(onitemCollapsed(QTreeWidgetItem*)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(onitemDoubleClicked(QTreeWidgetItem*, int)));

	//根节点
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0);

	//获取公司部门ID
	QSqlQuery queryComDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QString::fromLocal8Bit("公司群")));
	queryComDepID.exec();
	queryComDepID.first();
	int CompDepID = queryComDepID.value(0).toInt();

	//获取登陆者所在的部门ID
	QSqlQuery querySelfDepID(QString("SELECT departmentID FROM tab_employees WHERE employeeID=%1")
		.arg(gLoginEmployeeID));
	querySelfDepID.exec();
	querySelfDepID.first();
	int SelfDepID = querySelfDepID.value(0).toInt();

	//初始化公司群及所在的群
	addCompanyDeps(pRootGroupItem, CompDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);

	RootContatItem* pItemName = new RootContatItem(true, ui.treeWidget);
	QString strGroupName = QString::fromLocal8Bit("Connect EveryOne");
	pItemName->setText(strGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);

}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem * pRootGroupItem, const int& DepID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem;

	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, DepID);	//设置数据

	//获取部门头像路径
	QSqlQuery queryDepPic(QString("SELECT picture,department_name FROM tab_department WHERE departmentID=%1").arg(DepID));
	queryDepPic.exec();
	queryDepPic.first();
	QPixmap groupPix;
	groupPix.load(getHeadPixPath(QString("%1").arg(DepID)));
	print_run_log(getHeadPixPath(QString("%1").arg(DepID)));

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);
	pContactItem->setHeadPixmap(getRoundImage(groupPix,
		pix,pContactItem->getHeadLabelSize()));

	pContactItem->setUserName(queryDepPic.value(1).toString());
	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);
	//m_groupMap.insert(pChild, sDeps);
}

void CCMainWindow::resizeEvent(QResizeEvent * event)
{
	setUserName(QString::fromLocal8Bit("越分享月拥有"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event)
{
	if (ui.searchLineEdit == obj)
	{
		if (event->type() == QEvent::FocusIn)
		{
			ui.searchLineEdit->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255); border-bottom:1px solid rgba(%1,%2,%3,100)}\
			QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png);}\
			QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png);}\
			QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png);}")
				.arg(m_colorBackGround.red())
				.arg(m_colorBackGround.green())
				.arg(m_colorBackGround.blue()));
			
		}
		else if (event->type() == QEvent::FocusOut)
		{
			updateSearchStyle();
		}
	}
	return true;
}

void CCMainWindow::mousePressEvent(QMouseEvent * event)
{
	if (qApp->widgetAt(event->pos())!=ui.searchLineEdit&& ui.searchLineEdit->hasFocus())
	{
		ui.searchLineEdit->clearFocus();
	}
	else if (qApp->widgetAt(event->pos()) != ui.lineEdit&& ui.lineEdit->hasFocus())
	{
		ui.lineEdit->clearFocus();
	}
	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::onItemClicked(QTreeWidgetItem * item, int)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//未展开子项则展开子项
		item->setExpanded(!item->isExpanded());
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		RootContatItem* prootItem = dynamic_cast<RootContatItem*>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(true);
		}

	}
}

void CCMainWindow::onitemCollapsed(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		RootContatItem* prootItem = dynamic_cast<RootContatItem*>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(false);
		}

	}
}

void CCMainWindow::onitemDoubleClicked(QTreeWidgetItem * item, int)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		WindowManger::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole + 1).toString());
	}
}

void CCMainWindow::onAppIconClicked()
{
	if (sender()->objectName()== "app_skin")
	{
		SkinWindow* skinWindow = new SkinWindow;
		skinWindow->show();
	}
}

#include "basicwindow.h"
#include"CommonUtils.h"
#include"NotifyManger.h"
#include<QFile>
#include<QStyleOption>
#include<QPainter>
#include<QApplication>
#include<QMouseEvent>
#include<QDeskTopWidget>
#include<QSqlQuery>

extern QString gLoginEmployeeID;
BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);	//无边框
	setAttribute(Qt::WA_TranslucentBackground, true);	//透明
	connect(NotifyManger::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSinnalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::loadStyleSheet(const QString & sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
			.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);
	}
	file.close();
}

QPixmap BasicWindow::getRoundImage(const QPixmap & src, QPixmap & mask, QSize masksize)
{
	if (masksize == QSize(0, 0))
	{
		masksize = mask.size();
	}
	else
	{
		mask=mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	QImage resultImage(masksize, QImage::Format_A2BGR30_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//子类化部件时,需需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent *event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

void BasicWindow::mousePressEvent(QMouseEvent * event)
{
	if (event->buttons()==Qt::LeftButton)
	{
		m_mousePress = true;
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

void BasicWindow::mouseMoveEvent(QMouseEvent * event)
{
	if (m_mousePress && (event->buttons() && Qt::LeftButton))
	{
		move(event->globalPos() - m_mousePoint);
		event->accept();
	}
}

void BasicWindow::mouseReleaseEvent(QMouseEvent * event)
{
	m_mousePress = false;
}

void BasicWindow::onSinnalSkinChanged(const QColor & color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags()& Qt::Tool))
	{
		hide();
	}
	else
	{
		showMinimized();
	}

}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getResoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonMaxClicked()
{
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3,desktopRect.y() - 3,
		desktopRect.width()+6,desktopRect.height()+6);
	setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked()
{
	close();
}

void BasicWindow::initTitleBar(ButtonType buttontype)
{
	_titleBar = new TitleBar(this);		//父部件BasicWindow
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);

	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BasicWindow::setTitleBarTitle(const QString & title, const QString & icon)
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::onShowClose(bool)
{
	close();
}
void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{
	hide();
}

void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool)
{
	//更改数据库登录状态为离线
	QString strSqlStatus = QString("UPDATE tab_employees SET `online` =1 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	QApplication::quit();
}

#pragma once

#include <QDialog>
#include"titlebar.h"

class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent=nullptr);
	virtual ~BasicWindow();
public:
	void loadStyleSheet(const QString& sheetName);
	QPixmap getRoundImage(const QPixmap& src, QPixmap &mask, QSize masksize = QSize(0, 0));

private:
	void initBackGroundColor();
protected:
	void paintEvent(QPaintEvent*);

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString &icon = "");

	
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);

	void onSinnalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;	//鼠标位置
	bool m_mousePress;	//鼠标是不按下
	QColor m_colorBackGround;
	QString m_styleName;	//样式文件名
	TitleBar* _titleBar;	//标题栏
};

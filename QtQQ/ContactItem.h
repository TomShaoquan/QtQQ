#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = Q_NULLPTR);
	~ContactItem();

	void setUserName(const QString& userName);
	void setSignName(const QString& signName);
	void setHeadPixmap(const QPixmap& headPix);
	QString getUserName()const;
	QSize getHeadLabelSize() const;

private:
	void intiControl();
private:
	Ui::ContactItem ui;
};

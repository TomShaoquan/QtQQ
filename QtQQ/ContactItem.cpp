#include "ContactItem.h"

ContactItem::ContactItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	intiControl();
}

ContactItem::~ContactItem()
{
}

void ContactItem::setUserName(const QString & userName)
{
	ui.userName->setText(userName);
}

void ContactItem::setSignName(const QString & signName)
{
	ui.signName->setText(signName);
}

void ContactItem::setHeadPixmap(const QPixmap & headPix)
{
	ui.label->setPixmap(headPix);
}

QString ContactItem::getUserName() const
{
	return ui.userName->text();
}

QSize ContactItem::getHeadLabelSize() const
{
	return ui.label->size();
}

void ContactItem::intiControl()
{

}

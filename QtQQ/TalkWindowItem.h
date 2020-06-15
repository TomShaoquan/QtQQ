#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();

	void setHeadPixmap(const QPixmap& pixmap);
	void setMsgLabelContent(const QString& msg);
	QString getMsgLabelText();
private:
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);
	void resizeEvenet(QResizeEvent* event);

private:
	void initControl();
signals:
	void signalCloseClocked();
private:
	Ui::TalkWindowItem ui;
};

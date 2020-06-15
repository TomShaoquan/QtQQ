#pragma once

#include <QObject>

class NotifyManger : public QObject
{
	Q_OBJECT

public:
	NotifyManger();
	~NotifyManger();
signals:
	void signalSkinChanged(const QColor& color);
public:
	static NotifyManger* getInstance();
	void notifyOtherWindowChangeSkin(const QColor& color);

private:
	static NotifyManger* instance;
};

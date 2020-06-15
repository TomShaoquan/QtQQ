#include "NotifyManger.h"
#include"CommonUtils.h"

NotifyManger* NotifyManger::instance = nullptr;

NotifyManger::NotifyManger()
	:QObject(nullptr)
{
}

NotifyManger::~NotifyManger()
{
}

NotifyManger * NotifyManger::getInstance()
{
	if (instance == nullptr)
	{
		instance = new NotifyManger();
	}
	return instance;
}

void NotifyManger::notifyOtherWindowChangeSkin(const QColor & color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);
}

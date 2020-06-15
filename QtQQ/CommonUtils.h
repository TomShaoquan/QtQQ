#pragma once
#include<QPixmap>
#include<QSize>
#include<QProxyStyle>

class CustomProxyStyle :public QProxyStyle
{
public:
	CustomProxyStyle(QObject* parent)
	{
		setParent(parent);
	}
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter* painter, const QWidget* widget = 0) const
	{
		//去掉windows 中默认的边框或虚线框，部件获取焦点直接返回，不进行绘制
		if (PE_FrameFocusRect == element)
		{
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}

private:

};

class CommonUtils
{
public:
	CommonUtils();
public:
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize& masksize = QSize(0, 0));
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);
	static void setDefaultSkinColor(const QColor& color);
	static QColor getDefaultSkinColor();

};


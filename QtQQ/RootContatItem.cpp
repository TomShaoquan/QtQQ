#include "RootContatItem.h"
#include<qpainter.h>
RootContatItem::RootContatItem(bool hasArrow,QWidget *parent)
	: QLabel(parent)
	,m_rotation(0)
	,m_hasArrow(hasArrow)
{
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//初始化属性动画
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);
	m_animation->setEasingCurve(QEasingCurve::InQuad);
}

RootContatItem::~RootContatItem()
{
}

void RootContatItem::setText(const QString & title)
{
	m_titleText = title;
	update();
}

void RootContatItem::setExpanded(bool expand)
{
	if (expand)
	{
		m_animation->setEndValue(90);
	}
	else
	{
		m_animation->setEndValue(0);
	}
	m_animation->start();
}

int RootContatItem::rotation()
{
	return m_rotation;
}

void RootContatItem::setRotation(int rotation)
{
	m_rotation = rotation;
	update();
}

void RootContatItem::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing,true);

	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);

	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.save();
	if (m_hasArrow)
	{
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");
		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);
		
		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform);

		//坐标偏移
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);

		painter.restore();	//恢复画家设置
	}
	QLabel::paintEvent(event);
}

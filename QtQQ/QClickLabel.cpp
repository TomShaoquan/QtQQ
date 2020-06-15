#include "QClickLabel.h"
#include<QMouseEvent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QClickLabel::~QClickLabel()
{
}

void QClickLabel::mousePressEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton)
	{
		emit clicked();
	}
}

#include "SkinWindow.h"
#include<qlist.h>
#include"QClickLabel.h"
#include"NotifyManger.h"
#include<qpalette.h>
SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

SkinWindow::~SkinWindow()
{
}

void SkinWindow::initControl()
{
	QList<QColor> colorList = {
		QColor(22,154,218),QColor(40,138,221),QColor(49,166,107),QColor(218,67,68),
		QColor(177,99,158),QColor(107,81,92),QColor(89,92,160),QColor(21,156,199),
		QColor(79,169,172),QColor(155,183,154),QColor(128,77,77),QColor(240,188,189)
	};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			QClickLabel* label = new QClickLabel(this);
			label->setCursor(Qt::PointingHandCursor);
			//C++11 lambda ±í´ïÊ½
			connect(label,&QClickLabel::clicked,[i,j,colorList]() {
				NotifyManger::getInstance()->notifyOtherWindowChangeSkin(colorList.at(i * 4 + j));
			});
			label->setFixedSize(84, 84);
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(i * 4 + j));
			label->setAutoFillBackground(true);
			label->setPalette(palette);

			ui.gridLayout->addWidget(label, i, j);
		}
	}
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));
}

void SkinWindow::onShowClose()
{
	close();
}

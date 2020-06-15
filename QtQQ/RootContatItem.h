#pragma once

#include <QLabel>
#include<qpropertyanimation.h>

class RootContatItem : public QLabel
{
	Q_OBJECT

	//箭头角度
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContatItem(bool hasArrow=true,QWidget *parent=nullptr);
	~RootContatItem();
public:
	void setText(const QString& title);
	void setExpanded(bool expand);

private:
	int rotation();
	void setRotation(int rotation);
protected:
	void paintEvent(QPaintEvent* event);

private:
	QPropertyAnimation* m_animation;
	QString m_titleText;	//文本
	int m_rotation;			//角度
	bool m_hasArrow;      

};

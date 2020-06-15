#pragma once

#include "QClickLabel.h"
class QMovie;

class EmotionLabelItem : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();
	void setEmotionName(int enotionName);
private:
	void initControl();
signals:
	void emotionClicked(int emotionNum);
private:
	int m_emotionName;
	QMovie* m_apngMovie;
};

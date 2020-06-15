#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include"CommonUtils.h"

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = Q_NULLPTR);
	~EmotionWindow();
private:
	void initControl();
private slots:
	void addEmotion(int emotionNum);

signals:
	void signalEmotionWindowHide();
	//void signalEmotionClicked(int emotionNum);
	void signalEmotionItemClicked(int emotionNum);
private:
	void paintEvent(QPaintEvent* event) override;
private:
	Ui::EmotionWindow ui;
};

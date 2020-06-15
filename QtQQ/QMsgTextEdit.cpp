#include "QMsgTextEdit.h"
#include<QMovie>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deleteAllEmotionImage();
}

void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{
	QMovie* movie = qobject_cast<QMovie*>(sender());
	document()->addResource(QTextDocument::ImageResource,
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());
}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	insertHtml(QString("<img src='%1'/>").arg(flagName));
	if (m_listEmotionUrl.contains(imageName))
	{
		return;
	}
	else
	{
		m_listEmotionUrl.append(imageName);
	}
	QMovie* apngMovie = new QMovie(imageName, "apng", this);
	m_emotionMap.insert(apngMovie, flagName);
	
	//数据帧改变时发射信号
	connect(apngMovie, SIGNAL(frameChange(int)), 
		this, SLOT(onEmotionImageFrameChange(int)));
	apngMovie->start();
	updateGeometry();
}

void QMsgTextEdit::deleteAllEmotionImage()
{
	for (auto itor = m_emotionMap.constBegin();
		itor != m_emotionMap.constEnd(); itor++)
	{
		delete itor.key();
	}
	m_emotionMap.clear();
}

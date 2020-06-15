#pragma once

#include <QWebEngineView>
#include<QDomNode>
/*
	用于显示网页内容
*/

class MsgHtmlObj : public QObject
{
	Q_OBJECT
	//动态属性
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent,QString msgLPicPath="");

signals:
	void signalMsgHtml(const QString& html);
private:
	QString m_msgLHtmlTmpl;	//别人发的消息
	QString m_msgRHtmlTmpl;	//我发的消息
	QString m_msgPicPath;	//图片路径
private:
	void initHtmlTmpl();	//初始化聊天网页
	QString getMsgTmplHtml(const QString& code);
};

class MsgWebPage : public QWebEnginePage
{
public:
	MsgWebPage(QObject* parent = nullptr) :	QWebEnginePage(parent) {}
	

protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
private:

};


class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();

	void appendMsg(const QString& html,QString strObj="0");
private:
	QList<QStringList> parseHtml(const QString& html);
	//Qt 中所有dom 节点 都可以使用DOM 进行操作
	QList<QStringList> parseDocNode(const QDomNode& node);

signals:
	void signalSendMsg(QString& strData, int &msgType, QString sFile = "");
private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;	//网络通道
};

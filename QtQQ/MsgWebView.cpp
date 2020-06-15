#include "MsgWebView.h"
#include<QFile>
#include<qmessagebox.h>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include<qwebchannel.h>
#include"TalkWindowShell.h"
#include"WindowManger.h"
#include<QSqlQueryModel>

extern QString gstrLoginHeadPath;

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString msgLPicPath):QObject(parent)
{
	m_msgPicPath = msgLPicPath;
	initHtmlTmpl();
}
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	,m_channel(new QWebChannel(this))
{
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowShell* talkWindowShell = WindowManger::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, 
		talkWindowShell, &TalkWindowShell::updateSendTcpMsg);

	//当前正在聊天的窗口ID
	QString strTalkId = WindowManger::getInstance()->getCreatingTalkId();
	
	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;
	//获取公司群ID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QStringLiteral("公司群")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompangID = queryEmployeeModel.data(companyIndex).toString();
	if (strTalkId == strCompangID) {//公司群聊
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE `status`=1");
	}
	else
	{
		if (strTalkId.length() == 4) { //其他群聊
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE `status`=1 AND departmentID=%1").arg(strTalkId));
		}
		else //单独聊天
		{
			queryEmployeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE `status`=1 AND employeeID=%1").arg(strTalkId));

			QModelIndex index = queryEmployeeModel.index(0, 0);
			strPicturePath = queryEmployeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}
	if (isGroupTalk) {
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmployeeModel.rowCount();
		for (int i = 0; i < rows; i++) {
			employeeModelIndex = queryEmployeeModel.index(i, 0);
			pictureModelIndex = queryEmployeeModel.index(i, 1);

			strEmployeeID = queryEmployeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmployeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;
			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}
	this->page()->setWebChannel(m_channel);
	//初始化接收信息页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);

	int imageNum = 0;
	int msgType = 1;	//信息类型: 0表情 1 文本 2 文件
	bool isImageMsg=false;		
	QString strData;


	for (int i = 0; i < msgList.size(); i++)
	{
		if (msgList.at(i).at(0) == "img")
		{
			QString imagePath = msgList.at(i).at(1);
			QPixmap pixmap;

			//获取表情名称
			QString EmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = EmotionPath.size();
			isImageMsg = true;
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			//根据表情名称的长度进行设置报请数据
			int emotionNameL = strEmotionName.length();
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));	//去掉路径中qrc
			}
			else
			{
				pixmap.load(imagePath);
			}
			msgType = 0;	//表情信息
			imageNum++;
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text")
		{
			qsMsg += msgList.at(i).at(1);
			strData = qsMsg;
		}
	}
	msgObj.insert("MSG", qsMsg);
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	if (strObj == "0")	//发送信息
	{										//appendHtml0
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));
		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images"+strData;
		}
		emit signalSendMsg(strData, msgType);
	}
	else //来信息
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
	/*this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));

	emit signalSendMsg(strData, msgType);*/
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();	//得到所有子节点
	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);
		if (node.isElement())
		{
			//转换元素
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}

			if(element.tagName()=="span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}

			if (node.hasChildNodes())
			{
				attribute<<parseDocNode(node);
			}
		}
	}
	return attribute;
}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgPicPath);
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1",gstrLoginHeadPath);
}

QString MsgHtmlObj::getMsgTmplHtml(const QString & code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Fail to init html");
		return QString("");
	}
	file.close();

	return strData;
}

bool MsgWebPage::acceptNavigationRequest(const QUrl & url, NavigationType type, bool isMainFrame)
{
	//仅仅接收qrc :/*.html
	if (url.scheme() == QString("qrc")) return true;
	return false;
}

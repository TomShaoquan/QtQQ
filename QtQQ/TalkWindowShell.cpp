#include "TalkWindowShell.h"
#include"CommonUtils.h"
#include"EmotionWindow.h"
#include"TalkWindow.h"
#include"TalkWindowItem.h"
#include<qsqlquerymodel.h>
#include<QMessageBox>
#include<qfile.h>
#include<QSqlQuery>
#include"WindowManger.h"
#include"ReceiveFile.h"
#include"UserLogin.h"
#include"common.h"

extern QString gLoginEmployeeID;

QString gfileName;
QString gfileData;


TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size())   //文件大小为零则创建文件
	{
		QStringList employeeIDList;
		getEmployeesID(employeeIDList);
		if (!createJSFile(employeeIDList))
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("js文件写入数据失败"));
		}
	}
	
}

TalkWindowShell::~TalkWindowShell()
{
	/*delete m_emotionWindow;
	m_emotionWindow = nullptr;*/
}

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, const QString & uid)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);
	aItem->setSelected(true);

	//判断是单聊还是群聊
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(uid);
	sqlDepModel.setQuery(strSql);
	int rows = sqlDepModel.rowCount();
	if (rows == 0)	//单聊
	{
		QString sql = QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(uid);
		sqlDepModel.setQuery(sql);
	}
	QModelIndex index;
	index = sqlDepModel.index(0, 0);

	QImage img;
	img.load(sqlDepModel.data(index).toString());
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	//设置头像

	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);
	connect(talkWindowItem, &TalkWindowItem::signalCloseClocked,
		[talkWindowItem, talkWindow,aItem, this]() {
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1) close();

	});
	
}

void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("EveryOne"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		//分类器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
	connect(&gTcpSocket, &QTcpSocket::readyRead, this, &TalkWindowShell::onProcessTcpData);
}



void TalkWindowShell::getEmployeesID(QStringList& employeeIDList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE `status`=1");
	//员工总数
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++)
	{
		index = queryModel.index(i, 0);
		employeeIDList << queryModel.data(index).toString();
	}
}
bool TalkWindowShell::createJSFile(QStringList & employeesList)
{
	//读取文件数据
	QString strFileTxt("Resources/MainWindow/MsgHtml/msgtmpl.txt");
	QFile fileRead(strFileTxt);
	QString strFile;
	if (fileRead.open(QIODevice::ReadOnly))
	{
		strFile=fileRead.readAll();
		fileRead.close();
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("msgtmpl.txt读取数据失败"));
		return false;
	}
	//替换(extern0,append10用作自己发信息使用)
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if(fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		//更新空值
		QString strSourceInitNull = "var external = null;";
		//更新初始化
		QString strSourceInit = "external = channel.objects.external;";
		//更新newChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); ";
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("recvHtml.txt读取失败"));
			return false;
		}

		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;
		
		for (int i = 0; i < employeesList.length(); i++)
		{
			// 编辑 替换后的空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//编辑替换后的 newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//编辑替换后的recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
		}
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		QTextStream stream(&fileWrite);
		stream << strFile;
		return true;
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("写msgtmpl.js失败"));
		return false;
	}
}

void TalkWindowShell::handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1) { //文本
		msgTextEdit.toHtml();
	}
	else if (msgType == 0) {//表情信息处理
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;

		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * 3, emotionWidth).toInt());
		}
	}
	QString html = msgTextEdit.document()->toHtml();

	//文本HTML如果没有字体则添加字体
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("msgFont.txt 不存在"));
			return;
		}
		if (!html.contains(fontHtml))
		{
			html.replace(strMsg, fontHtml);
		}
	}

	TalkWindow* talkWindow =dynamic_cast<TalkWindow*> (ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));
}

/*发送消息槽函数*/
void TalkWindowShell::updateSendTcpMsg(QString & strData, int & msgType, QString fileName)
{
	/*文本数据包格式:
	群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型 +数据长度(5位数)+数据
	表情包数据格式:
	群聊标志+ 发信息员工QQ号 + 收信息员工QQ号(群QQ号) +信息类型 +表情个数+images+数据
	*/
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;
	QString strSend;
	if (talkId.length() == 4)
	{
		strGroupFlag = "1";
	}
	else
	{
		strGroupFlag = "0";
	}
	int nstrDataL = strData.length();
	int dataLength = QString::number(nstrDataL).length();
	//const int sourceDataLength = dataLength;
	QString strDataLength;
	if (msgType == 1)	//发送文本信息
	{
		//文本信息长度固定为5位数
		if (dataLength == 1)
		{
			strDataLength = "0000" + QString::number(nstrDataL);
		}
		else if(dataLength == 2)
		{
			strDataLength = "000" + QString::number(nstrDataL);
		}
		else if (dataLength == 3)
		{
			strDataLength = "00" + QString::number(nstrDataL);
		}
		else if (dataLength == 4)
		{
			strDataLength = "0" + QString::number(nstrDataL);
		}
		else if (dataLength == 5)
		{
			strDataLength = QString::number(nstrDataL);
		}
		else
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("数据长度不合理!"));
		}
		//群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型 +数据长度+数据
		strSend = strGroupFlag + gLoginEmployeeID + talkId+"1" + strDataLength + strData;

	}
	else if (msgType == 0) //表情信息
	{
		//群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型 + 表情个数 + images + 数据
		strSend = strGroupFlag + gLoginEmployeeID + talkId +"0"+ strData;
	}
	else if (msgType == 2) //文件
	{
		//群聊标志+发信息员工QQ号+收信息员工QQ号+信息类型+文件长度
		//+bytes+文件名+data_begin+文件内容
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());
		strSend = strGroupFlag + gLoginEmployeeID + talkId
			+ "2" + "bytes" + fileName + "data_begin" + strData;
	}
	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();
	

	gTcpSocket.write(dataBt);
}
void TalkWindowShell::onEmotionBtnClicked(bool)
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotonPoint = this->mapToGlobal(QPoint(0, 0));	//将当前控件的相对位置装换为
	emotonPoint.setX(emotonPoint.x() + 170);
	emotonPoint.setY(emotonPoint.y() + 220);
	m_emotionWindow->move(emotonPoint);
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem * item)
{
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);

}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

/*
	数据包的格式:
	群聊标志:1 位 0单聊 1群聊
	信息类型:1 位 0表情 1文本 2文件
	群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型(1) +数据长度+数据
	群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型(0) + 表情个数 + images + 数据
	群聊标志+发信息员工QQ号+收信息员工QQ号+信息类型(2)+文件长度+bytes+文件名+data_begin+文件内容
*/


void TalkWindowShell::onProcessTcpData()
{

	const static int groupFlagWidth = 1;
	const static int groupWidth = 4;
	const static int employeeWidth = 5;
	const static int msgTypeWidth = 1;
	const static int msgLengthWidth = 5;
	const static int pictureWidth = 3;

	//读取UDP数据
	QByteArray btData;
	btData = gTcpSocket.readAll();

	QString strData = btData.data();
	QString strWindowID;
	QString strSendEmployeeID, strReceieEmployeeeID;
	QString strMsg;	//数据

	int msgLen;		//数据长度
	int msgType;	//数据类型

	strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);
	if (strSendEmployeeID == gLoginEmployeeID) return;
		
	if (btData[0] == '1') //群聊
	{
		strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);
		QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];

		if (cMsgType == '1') {//文本
			msgType = 1;
			msgLen = strData.mid(groupFlagWidth + employeeWidth +
				groupWidth + msgTypeWidth ,msgLengthWidth).toInt();
			strMsg = strData.mid(groupFlagWidth + employeeWidth +
				groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
		}
		else if (cMsgType == '0')	//图片
		{
			msgType = 0;
			int posImage = strData.indexOf("images");
			strMsg = strData.right(strData.length() - posImage - QString("iamges").length());
		}
		else if (cMsgType == '2')	//文件
		{
			msgType = 2;
			//文件名称
			int bytesWidth = QString("bytes").length();
			int posBytes = strData.indexOf("bytes");
			int posData_begin = strData.indexOf("data_begin");

			QString fileName = strData.mid(posBytes + bytesWidth,
				posData_begin - bytesWidth - posBytes);
			gfileName = fileName;

			//文件内容
			int dataLengthWidth;
			int posData = posData_begin + QString("data_begin").length();
			strMsg = strData.mid(posData);
			gfileData = strMsg;

			//获取发送者姓名
			QString sender;
			int employeeID = strSendEmployeeID.toInt();
			QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
			querySenderName.exec();
				
			if (querySenderName.next()) {
				sender = querySenderName.value(0).toString();
			}
			//接收文件操作 .....
			ReceiveFile* receiveFile = new ReceiveFile(this);
			connect(receiveFile, &ReceiveFile::refuseFile, [this]() {
				return;
			});
			QString msgLebel = QString::fromLocal8Bit("收到来自") + sender +
				QString::fromLocal8Bit("发来的文件.是否接受?");
			receiveFile->setMsg(msgLebel);
			receiveFile->show();

		}

	}
	else if (btData[0] == '0') //单聊
	{
		strReceieEmployeeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
		strWindowID = strSendEmployeeID;
			
		//不是发给我的消息不做处理
		if (strReceieEmployeeeID != gLoginEmployeeID) return;

		QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
		if (cMsgType == '1')//文本信息
		{
			msgType = 1;
			msgLen = strData.mid(groupFlagWidth + employeeWidth+employeeWidth
			+msgTypeWidth,msgLengthWidth).toInt();

			strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
				+ msgTypeWidth + msgLengthWidth, msgLen);

		}
		else if (cMsgType == '0')  //表情信息
		{
			msgType = 0;
			int posImags = strData.indexOf("images");
			int imagesWidth = QString("images").length();
			strMsg = strData.mid(posImags + imagesWidth);
		}
		else if (cMsgType == '2')
		{
				
			msgType = 2;
			//文件名称
			int bytesWidth = QString("bytes").length();
			int posBytes = strData.indexOf("bytes");
			int posData_begin = strData.indexOf("data_begin");
			//11000520022bytesexit.txtdata_beginAre you ok?
			QString fileName = strData.mid(posBytes + bytesWidth,
				posData_begin - bytesWidth - posBytes);
			gfileName = fileName;
			//文件内容
			int dataLengthWidth;
			int posData = posData_begin + QString("data_begin").length();
			strMsg = strData.mid(posData);
			gfileData = strMsg;

			//获取发送者姓名
			QString sender;
			int employeeID = strSendEmployeeID.toInt();
			QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
			querySenderName.exec();

			if (querySenderName.next()) {
				sender = querySenderName.value(0).toString();
			}
			//接收文件操作 
			ReceiveFile* receiveFile = new ReceiveFile(this);
			connect(receiveFile, &ReceiveFile::refuseFile, [this]() {
				return;
			});
			QString msgLebel = QString::fromLocal8Bit("收到来自") + sender +
				QString::fromLocal8Bit("发来的文件.是否接受?");
			receiveFile->setMsg(msgLebel);
			receiveFile->show();
		}
	}

	//将聊天窗口设置为活动的
	QWidget* widget = WindowManger::getInstance()->findWindowName(strWindowID);
	if (widget)  //聊天窗口存在
	{
		this->setCurrentWidget(widget);
		//同步激活左侧聊天窗口
		QListWidgetItem* item=m_talkwindowItemMap.key(widget);
		item->setSelected(true);

	}
	else //聊天窗口未打开
	{
		return;
	}
	//文件信息另做处理
	if (msgType != 2) {
		handleReceivedMsg(strSendEmployeeID.toInt(), msgType, strMsg);
	}

}


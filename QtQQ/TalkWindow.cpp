#include "TalkWindow.h"
#include"RootContatItem.h"
#include"ContactItem.h"
#include"CommonUtils.h"
#include"WindowManger.h"
#include<QToolTip>
#include<QFile>
#include<QMessageBox>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include"SendFile.h"
#include"common.h"

TalkWindow::TalkWindow(QWidget *parent, const QString& uid)
	: QWidget(parent)
	,m_talkId(uid)
{
	ui.setupUi(this);
	WindowManger::getInstance()->addWindowName(uid,this);
	setAttribute(Qt::WA_DeleteOnClose);
	initGroupTalkStatus();
	initControl();
}

TalkWindow::~TalkWindow()
{
	WindowManger::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

QString TalkWindow::getTalkId()
{
	return m_talkId;
}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int colum)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		//QString strPeopleName = m_groupPeopleMap.value(item);
		QString strEmployeeID = item->data(0, Qt::UserRole + 1).toString();
		if (strEmployeeID == gLoginEmployeeID) return;
		WindowManger::getInstance()->addNewTalkWindow(strEmployeeID);
	}
}

void TalkWindow::onSetEmotionBtnStatus()
{
}

void TalkWindow::onFileOpenBtnClicked(bool)
{
	SendFile* sendFile = new SendFile(this);
	sendFile->show();
}

void TalkWindow::onSendBtnClicded(bool )
{
	//debug:���ͷ���ҳ�������Ϣ������ ���ܷ�û������
	if (ui.textEdit->toPlainText().isEmpty())
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)),
			QString::fromLocal8Bit("���͵���Ϣ����Ϊ��"),
			this, QRect(0, 0, 120, 100), 2000);
		return;
	}
	QString& html = ui.textEdit->document()->toHtml();
	
	//�ı�HTML���û���������������
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1",text);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("msgFont.txt ������"));
			return;
		}
		if (!html.contains(fontHtml))
		{
			html.replace(text, fontHtml);
		}
	}
	ui.textEdit->clear();
	ui.textEdit->deleteAllEmotionImage();

	ui.msgWidget->appendMsg(html);	//����Ϣ����

}

void TalkWindow::initControl()
{
	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;

	ui.bodySplitter->setSizes(rightWidgetSize);
	ui.textEdit->setFocus();

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicded(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	connect(ui.fileopenBtn, SIGNAL(clicked(bool)), this, SLOT(onFileOpenBtnClicked(bool)));
	if (m_isGroupTalk)
	{
		initTalkWindow();
	}
	else
	{
		initPtoPTalk();
	}
	
}
void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	QString sql = QString("SELECT * FROM tab_department WHERE departmentID=%1").arg(m_talkId);
	
	sqlDepModel.setQuery(sql);
	int rows = sqlDepModel.rowCount();
	if (rows == 0)	//��������
	{
		m_isGroupTalk = false;
	}
	else  //Ⱥ��
	{
		m_isGroupTalk = true;
	}
}

int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryDepID.exec();
	queryDepID.next();

	return queryDepID.value(0).toInt();
}

void TalkWindow::initTalkWindow()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);
	ui.treeWidget->setFixedHeight(646);

	//��ǰ�����Ⱥ����������
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.first())
	{
		strGroupName = queryGroupName.value(0).toString();
	}

	int nEmployeeNum = 0;
	//��ȡԱ������
	QSqlQueryModel queryEmployeeModel;
	if (getCompDepID() == m_talkId.toInt())	//��˾Ⱥ
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE `status`=1");
	}
	else
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE `status`=1 AND departmentID=%1").arg(m_talkId));
	}
	nEmployeeNum = queryEmployeeModel.rowCount();
	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName).arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		//����ӽڵ�
		int employeeID;
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		employeeID = queryEmployeeModel.data(modelIndex).toInt();
		
		addPeopInfo(pRootItem,employeeID);
	}
}

void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixSkin.size());

	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
	
}

void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem,int employeeID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();
	
	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1,employeeID);
	ContactItem *pContactItem = new ContactItem(ui.treeWidget);
	
	
	//��ȡ�� ǩ�� ͷ��·��
	QString strName, strSign, strPicPath, strQuery;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	QModelIndex nameIndex, signIndex, picIndex;
	nameIndex = queryInfoModel.index(0, 0);
	signIndex = queryInfoModel.index(0, 1);
	picIndex = queryInfoModel.index(0, 2);
	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicPath = queryInfoModel.data(picIndex).toString();
	
	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	QImage imageHead;
	imageHead.load(strPicPath);
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}

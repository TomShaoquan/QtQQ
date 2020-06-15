#include "WindowManger.h"
#include"TalkWindow.h"
#include"TalkWindowItem.h"
#include<QSqlQueryModel>

//创建全局静态对象
Q_GLOBAL_STATIC(WindowManger, theInstance)

WindowManger::WindowManger()
	:QObject(nullptr),
	m_talkwindowShell(nullptr)
{

}

WindowManger::~WindowManger()
{
}

QWidget * WindowManger::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);
	}
	else
	{
		return nullptr;
	}
}

void WindowManger::deleteWindowName(const QString & qsWindowName)
{
	m_windowMap.remove(qsWindowName);
}

void WindowManger::addWindowName(const QString & qsWindowName, QWidget * qWidget)
{
	if (!m_windowMap.contains(qsWindowName))
	{
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManger * WindowManger::getInstance()
{
	return theInstance;
}

TalkWindowShell * WindowManger::getTalkWindowShell()
{
	return m_talkwindowShell;
}

QString WindowManger::getCreatingTalkId()
{
	return m_strCreatingTalkId;
}

void WindowManger::addNewTalkWindow(const QString & uid)
{
	if (m_talkwindowShell == nullptr)
	{
		m_talkwindowShell = new TalkWindowShell;
		connect(m_talkwindowShell, &TalkWindowShell::destroyed, [this](QObject* obj)
		{
			m_talkwindowShell = nullptr;
		});

	}
	QWidget* widget = findWindowName(uid);
	if (!widget)
	{
		m_strCreatingTalkId = uid;
		TalkWindow * talkwindow = new TalkWindow(m_talkwindowShell, uid);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);
		m_strCreatingTalkId = "";

		//判断群聊还是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID=%1").arg(uid);
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();
		QString strWindowName, strMsgLabel;
		if (rows == 0)	//单聊
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID=%1").arg(uid);
			sqlDepModel.setQuery(sql);
		}
		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);
		signIndex = sqlDepModel.index(0, 1);
		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();
		
		talkwindow->setWindowName(strWindowName);
		talkwindowItem->setMsgLabelContent(strMsgLabel);	//左侧文本显示
		m_talkwindowShell->addTalkWindow(talkwindow, talkwindowItem,uid);
	}
	else  //窗口已存在
	{
		//左侧设置为选中
		QListWidgetItem* item = m_talkwindowShell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		//右侧设置为选中
		m_talkwindowShell->setCurrentWidget(widget);
	}
	m_talkwindowShell->show();
	m_talkwindowShell->activateWindow();
}

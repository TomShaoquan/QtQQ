#include "SendFile.h"
#include"TalkWindowShell.h"
#include"WindowManger.h"
#include<QFileDialog>
#include<QMessageBox>

SendFile::SendFile(QWidget *parent)
	: BasicWindow(parent)
	,m_filePath("")
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);	//资源回收
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("SendFile");
	this->move(100, 400);

	TalkWindowShell* talkWindowShell = WindowManger::getInstance()->getTalkWindowShell();

	connect(this, &SendFile::sendFileClicked, talkWindowShell, 
		&TalkWindowShell::updateSendTcpMsg);
}

SendFile::~SendFile()
{
}

void SendFile::on_openBtn_clicked()
{
	m_filePath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择文件"),
		"/",
		QString::fromLocal8Bit("发送的文件(*.txt *.doc);;所有文件(*.*);;"));
	ui.lineEdit->setText(m_filePath);

}

void SendFile::on_sendBtn_clicked()
{
	if (!m_filePath.isEmpty()) {
		QFile file(m_filePath);
		if (file.open(QIODevice::ReadOnly)) {
			int msgType = 2;
			QString str = file.readAll();

			//文件名称
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();
			emit sendFileClicked(str, msgType, fileName);
			file.close();
		}
		else
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("读取文件:%1失败!").arg(m_filePath));
			return;
		}
		m_filePath.clear();
		this->close();
	}
}

#include "ReceiveFile.h"
#include<QFileDialog>
#include<QMessageBox>

extern QString gfileName;
extern QString gfileData;
ReceiveFile::ReceiveFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);	//资源回收
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("ReceiveFile");
	this->move(100, 400);

	connect(_titleBar, &TitleBar::signalButtonCloseClicked, 
		this, &ReceiveFile::refuseFile);

}

ReceiveFile::~ReceiveFile()
{
}

void ReceiveFile::setMsg(QString & msgLabel)
{
	ui.label->setText(msgLabel);
}

void ReceiveFile::on_cancelBtn_clicked()
{
	emit refuseFile();
	this->close();
}

void ReceiveFile::on_okBtn_clicked()
{
	this->close();
	//获取问价保存路径
	QString fileDirPath = QFileDialog::getExistingDirectory(
		nullptr,
		QStringLiteral("文件保存路径"),
		"/"
	);
	QString filePath = fileDirPath + "/" + gfileName;
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("接受文件失败!"));
	}
	else
	{
		file.write(gfileData.toUtf8());
		file.close();
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("接受文件成功!"));
	}
}

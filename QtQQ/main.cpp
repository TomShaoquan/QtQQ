//#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include"UserLogin.h"
#include<QDir>

#include<QDateTime>
#include"common.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*CCMainWindow w;
	w.show();*/
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QFile file(LOG_FILE);

	file.open(QIODevice::WriteOnly|QIODevice::Append);
	a.setQuitOnLastWindowClosed(false);
	/*QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath()) +
		QDir::separator() + "plugins");

	qApp->addLibraryPath(strLibPath);*/
	QTextStream text_stream(&file);
	QStringList path_list = qApp->libraryPaths();
	for (int i = 0; i < path_list.size(); i++) {
		text_stream <<current_date_time<<"\t"<<path_list[i]<< "\r\n";
	}
	text_stream <<current_date_time << "\t" << qApp->applicationDisplayName() << "\r\n";
	text_stream << current_date_time << "\t" << qApp->applicationDirPath() << "\r\n";
	text_stream << current_date_time << "\t" << qApp->applicationFilePath() << "\r\n";
	text_stream << current_date_time << "\t" << qApp->applicationName() << "\r\n";

	file.flush();
	file.close();
	//qApp->set
	/*QStringList test;
	test=qApp->libraryPaths();*/

	UserLogin *userLogin = new UserLogin;
	userLogin->show();
	return a.exec();
}

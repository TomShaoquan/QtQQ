#include"common.h"


void print_run_log(QString str)
{
	QFile file(LOG_FILE);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << current_date_time << "\t" << str << "\r\n";
	file.flush();
	file.close();
}

QString getHeadPixPath(QString uid)
{
	QString headPath ="./qtqq_images/" +uid + ".png";
	return headPath;
}
#include "HXQ.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include <QSplashScreen>
#include <QElapsedTimer>
#include "Func.h"
#include <QTime>

#define STARTLOGO
#define OUTPUTLOG

void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();

	QString strMsg("");
	switch (type)
	{
	case QtDebugMsg:
		strMsg = QString("Debug:  ");
		break;
	case QtWarningMsg:
		strMsg = QString("Warning:  ");
		break;
	case QtCriticalMsg:
		strMsg = QString("Critical:  ");
		break;
	case QtFatalMsg:
		strMsg = QString("Fatal:  ");
		break;
	}

	// 设置输出信息格式
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString strMessage = strMsg + QString("File:%1  Line:%2  Function:%3  Message:%4  DateTime:%5\r\n")
		.arg(context.file).arg(context.line).arg(context.function).arg(localMsg.constData()).arg(strDateTime);
	
	// 加锁
	static QMutex mutex;
	mutex.lock();
	// 循环写日志
	QFile file1("log1.txt");
	QFile file2("log2.txt");
	file1.open(QIODevice::WriteOnly | QIODevice::Append);
	if (file1.size() >= 5*1024*1024)
	{
		file1.close();
		file2.remove();
		QFile::copy("log1.txt", "log2.txt");
		file1.remove();

		QFile file3("log1.txt");
		file3.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream stream(&file3);
		stream << strMessage << endl;
	}
	else
	{
		QTextStream stream(&file1);
		stream << strMessage << endl;
	}

	// 解锁
	mutex.unlock();
}

int main(int argc, char *argv[])
{
#ifdef OUTPUTLOG
	qInstallMessageHandler(MessageOutput);
#endif

	QApplication a(argc, argv);

	QFile qss("Resources/qss/css.h");
	if (qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}

	QPixmap pixmap("Resources/logo.bmp");
	QSplashScreen* screen=new QSplashScreen(pixmap);
	screen->show();
	screen->showMessage(G2U("初始化..."), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	


#ifdef STARTLOGO 
	int delayTime = 3;
	QElapsedTimer timer;
	timer.start();
	while (timer.elapsed() < (delayTime * 1000))
	{
		a.processEvents();
	}
#endif  

	hxq w;
	//w.setWindowTitle(G2U("内存"));
	//w.showMaximized();
	w.show();
	screen->finish(&w);
	
	delete screen;
	qDebug() << "Open Program!";
	return a.exec();
}

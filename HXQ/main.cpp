#pragma execution_character_set("utf-8")

#include "HXQ.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include <QSplashScreen>
#include <QElapsedTimer>
#include "Func.h"
#include <QTime>
#include <QProcess>
#include "Global.h"
#include "HalconCpp.h"

using namespace HalconCpp;

#define STARTLOGO
//#define OUTPUTLOG

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
	if (file1.size() >= 2*1024*1024)
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

void ReadGlobal()
{
	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Hxq), xmlContent))
	{
		int total = sizeof(g_DetectParam) / sizeof(float);
		float* p = (float*)&g_DetectParam;
		for (int index = 0; index<total; index++)
		{
			*p = xmlContent[index].second.toFloat();
			p++;
		}
	}

	std::vector <std::pair<std::pair<QString, QString>, QString>> xmlContent2;
	if (ParserXmlNode(QString(XML_Configure), QString(Node_Save), xmlContent2))
	{
		g_SaveParam.SaveTopBadIndex = xmlContent2[0].second.toInt();
		g_SaveParam.SaveSideBadIndex = xmlContent2[1].second.toInt();

		g_SaveParam.IsSaveTopBad = xmlContent2[2].second.toInt();
		g_SaveParam.IsSaveSideBad = xmlContent2[3].second.toInt();
		g_SaveParam.IsSaveTopAll = xmlContent2[4].second.toInt();
		g_SaveParam.IsSaveSideAll = xmlContent2[5].second.toInt();

		g_SaveParam.SaveTopBadNum = xmlContent2[6].second.toInt();
		g_SaveParam.SaveSideBadNum = xmlContent2[7].second.toInt();
		g_SaveParam.SaveTopAllNum = xmlContent2[8].second.toInt();
		g_SaveParam.SaveSideAllNum = xmlContent2[9].second.toInt();


		g_SaveParam.SaveTopBadPath = xmlContent2[10].second;
		g_SaveParam.SaveSideBadPath = xmlContent2[11].second;
		g_SaveParam.SaveTopAllPath = xmlContent2[12].second;
		g_SaveParam.SaveSideAllPath = xmlContent2[13].second;

		g_SaveParam.SaveImageFormat = xmlContent2[14].second;

		g_SaveParam.IsSaveLog = xmlContent2[15].second.toInt();

		if (QFile::exists(_XML_Configure))
			QFile::remove(_XML_Configure);

		QFile::copy(XML_Configure, _XML_Configure);

	}
	else if(ParserXmlNode(QString(_XML_Configure), QString(Node_Save), xmlContent2))
	{
		g_SaveParam.SaveTopBadIndex = xmlContent2[0].second.toInt();
		g_SaveParam.SaveSideBadIndex = xmlContent2[1].second.toInt();

		g_SaveParam.IsSaveTopBad = xmlContent2[2].second.toInt();
		g_SaveParam.IsSaveSideBad = xmlContent2[3].second.toInt();
		g_SaveParam.IsSaveTopAll = xmlContent2[4].second.toInt();
		g_SaveParam.IsSaveSideAll = xmlContent2[5].second.toInt();

		g_SaveParam.SaveTopBadNum = xmlContent2[6].second.toInt();
		g_SaveParam.SaveSideBadNum = xmlContent2[7].second.toInt();
		g_SaveParam.SaveTopAllNum = xmlContent2[8].second.toInt();
		g_SaveParam.SaveSideAllNum = xmlContent2[9].second.toInt();


		g_SaveParam.SaveTopBadPath = xmlContent2[10].second;
		g_SaveParam.SaveSideBadPath = xmlContent2[11].second;
		g_SaveParam.SaveTopAllPath = xmlContent2[12].second;
		g_SaveParam.SaveSideAllPath = xmlContent2[13].second;

		g_SaveParam.SaveImageFormat = xmlContent2[14].second;

		g_SaveParam.IsSaveLog = xmlContent2[15].second.toInt();

		if (QFile::exists(XML_Configure)) 
			QFile::remove(XML_Configure);

		QFile::copy(_XML_Configure, XML_Configure);
	}
	else if (ParserXmlNode(QString(XML_Configure_Origin), QString(Node_Save), xmlContent2))
	{
		g_SaveParam.SaveTopBadIndex = xmlContent2[0].second.toInt();
		g_SaveParam.SaveSideBadIndex = xmlContent2[1].second.toInt();

		g_SaveParam.IsSaveTopBad = xmlContent2[2].second.toInt();
		g_SaveParam.IsSaveSideBad = xmlContent2[3].second.toInt();
		g_SaveParam.IsSaveTopAll = xmlContent2[4].second.toInt();
		g_SaveParam.IsSaveSideAll = xmlContent2[5].second.toInt();

		g_SaveParam.SaveTopBadNum = xmlContent2[6].second.toInt();
		g_SaveParam.SaveSideBadNum = xmlContent2[7].second.toInt();
		g_SaveParam.SaveTopAllNum = xmlContent2[8].second.toInt();
		g_SaveParam.SaveSideAllNum = xmlContent2[9].second.toInt();


		g_SaveParam.SaveTopBadPath = xmlContent2[10].second;
		g_SaveParam.SaveSideBadPath = xmlContent2[11].second;
		g_SaveParam.SaveTopAllPath = xmlContent2[12].second;
		g_SaveParam.SaveSideAllPath = xmlContent2[13].second;

		g_SaveParam.SaveImageFormat = xmlContent2[14].second;

		g_SaveParam.IsSaveLog = xmlContent2[15].second.toInt();

		if (QFile::exists(XML_Configure))
			QFile::remove(XML_Configure);

		QFile::copy(XML_Configure_Origin, XML_Configure);

		if (QFile::exists(_XML_Configure))
			QFile::remove(_XML_Configure);

		QFile::copy(XML_Configure_Origin, _XML_Configure);
	}
}

int main(int argc, char *argv[])
{
	ResetEth("\"Basler5.8\"");

	if (g_SaveParam.IsSaveLog)
		qInstallMessageHandler(MessageOutput);


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
	screen->showMessage(QObject::tr("初始化CCD"), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	
	static int cout = 0;
	while (!isPingOk(Ip_SideCamera))
	{
		Sleep(200);
		++cout;
		if (cout > 50)
		{
			break;
		}
			
	}

	QString str = QString("第1次初始化CCD时间：%1").arg(cout*0.2);
	screen->showMessage(str, Qt::AlignTop | Qt::AlignLeft, Qt::black);

	ResetEth("\"Basler5.8\"");
	cout = 0;
	while (!isPingOk(Ip_SideCamera))
	{
		Sleep(200);
		++cout;
		if (cout > 50)
			break;
	}

	str = QString("第2次初始化CCD时间：%1").arg(cout*0.2);
	screen->showMessage(str, Qt::AlignTop | Qt::AlignLeft, Qt::black);

	ReadGlobal();
	Sleep(500);

	screen->showMessage(QObject::tr("读取配置参数"), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	Sleep(200);

	screen->showMessage(QObject::tr("初始化完成"), Qt::AlignTop | Qt::AlignLeft, Qt::black);

#ifdef STARTLOGO 
	int delayTime = 0.5;
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
	qDebug() << "\r\n\r\n\r\nOpen Program!";
	return a.exec();
}

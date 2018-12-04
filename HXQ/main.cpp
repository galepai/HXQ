#include "HXQ.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include <QSplashScreen>
#include <QElapsedTimer>
#include "Func.h"
#include <QTime>

#define STARTLOGO


bool CreaImagetDir();


int main(int argc, char *argv[])
{
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
	screen->showMessage(G2U("��ʼ��..."), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	
	/*screen->showMessage(G2U("���Ӱ���ļ����Ƿ����..."), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	if(CreateImagetDir())
		screen->showMessage(G2U("����Ӱ���ļ��гɹ�"), Qt::AlignTop | Qt::AlignLeft, Qt::black);
	else
		screen->showMessage(G2U("Ӱ���ļ��д���"), Qt::AlignTop | Qt::AlignLeft, Qt::black);*/



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
	//w.setWindowTitle(G2U("�ڴ�"));
	//w.showMaximized();
	w.show();
	screen->finish(&w);
	
	delete screen;

	return a.exec();
}

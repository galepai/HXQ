#include "GalilThread.h"
#include "Func.h"
#include <QMessageBox>
#include <QTime>
#include "ConstParam.h"


std::atomic<bool> Galil_Thread::m_bIsStop = false;


Galil_Thread::Galil_Thread(QObject *parent)
	: QThread(parent)
{
	qDebug() << "Galil_Thread construct func(),  Thread : " << QThread::currentThreadId();
	
	g = 0; 

}

bool Galil_Thread::GcLibVersion()
{
	//int buf_size = G_SMALL_BUFFER;
	//char buf[G_SMALL_BUFFER]; //traffic buffer

	try
	{
		check(GVersion(buf, sizeof(buf))); //library version
		qDebug() << "Library versions: " << buf << "\n";
		return true;
	}
	catch (GReturn gr) //for x_e() function
	{

		ExceptionInformation(gr);
		return false;
	}

}

bool Galil_Thread::Open(QString address)
{
	try
	{
		//check(GOpen("192.168.0.43 -d", &g));
		check(GOpen(address.toStdString().c_str(), &g));
		int input;
		GCmdI(g, "TI", &input);
		
		return true;

	}
	catch (GReturn gr) //for x_e() function
	{

		ExceptionInformation(gr);
		return false;
	}

}

void Galil_Thread::ExceptionInformation(GReturn gr)
{
	qDebug() << "Function returned " << gr << '\n';
	GError(gr, buf, sizeof(buf));
	qDebug() << buf << '\n';
	GSize size = sizeof(buf);

	if (g)
	{
		GUtility(g, G_UTIL_ERROR_CONTEXT, buf, &size);
		qDebug() << buf << '\n'; //further context

		GClose(g); //close g
	}

	g = 0; 
}

void Galil_Thread::run()
{
	//std::string test = ":00050520FF00D7\r\n";

	qDebug() << "Galil_Thread Run Thread : " << QThread::currentThreadId();

	while (!m_bIsStop)
	{
		try
		{
			check(GCommand(g, "MG TIME", buf, sizeof(buf), 0)); //Send MG TIME. Because response is ASCII, don't care about bytes read.
		}
		catch (GReturn gr) //for x_e() function
		{
			ExceptionInformation(gr);
		}
	}
	

	
}


Galil_Thread::~Galil_Thread()
{
	if (isRunning())
		wait();

	qDebug() << "~Galil_Thread";
}
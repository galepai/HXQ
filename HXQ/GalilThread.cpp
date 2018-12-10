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

	front = new char[G_SMALL_BUFFER];

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

bool Galil_Thread::CmdI(QString command,int* value)
{
	try
	{
		check(GCmdI(g,command.toStdString().c_str(), value));

		return true;

	}
	catch (GReturn gr) //for x_e() function
	{

		ExceptionInformation(gr);
		return false;
	}

}

bool Galil_Thread::CmdD(QString command, double* value)
{
	try
	{
		check(GCmdD(g, command.toStdString().c_str(), value));
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

bool Galil_Thread::CmdT(QString command, QString& value)
{
	try
	{
		ZeroMemory(buf, sizeof(buf));
		check(GCmdT(g, command.toStdString().c_str(),buf,sizeof(buf),&front));
		value = buf;

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
	qDebug() << "Galil_Thread Run Thread : " << QThread::currentThreadId();
	int value = 0;

	while (!m_bIsStop)
	{
		value = 0;
		CmdI("TI", &value);
		
		qDebug() << "Input : " << value;

		sleep(100);
	}
	
}


Galil_Thread::~Galil_Thread()
{
	if (isRunning())
		wait();

	delete front;
	delete[] buf;

	if (g)
	{
		GClose(g); //close g
	}
	qDebug() << "~Galil_Thread";
}
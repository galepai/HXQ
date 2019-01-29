#include "GalilThread.h"
#include "Func.h"
#include <QMessageBox>
#include <QTime>
#include "ConstParam.h"


//std::atomic<bool> Galil_Thread::m_bIsStop = false;


Galil_Thread::Galil_Thread(QObject *parent)
	: QThread(parent)
{
	qDebug() << "Galil_Thread construct func(),  Thread : " << QThread::currentThreadId();

	m_g = 0;
	m_StopThread = false;
	//m_front = new char[G_SMALL_BUFFER];

}

bool Galil_Thread::GcLibVersion()
{
	try
	{
		check(GVersion(m_buf, sizeof(m_buf))); //library version
		qDebug() << "Library Version: " << m_buf << "\n";
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
		//check(GOpen("192.168.2.7", &g));
		check(GOpen(address.toStdString().c_str(), &m_g));

		qDebug() << "Connection Galil sucess.";
		//QString Revision;
		//CmdT("\x12\x16", Revision);
		//qDebug() << "Galil Revision : " << Revision;

		return true;

	}
	catch (GReturn gr) //for x_e() function
	{

		ExceptionInformation(gr);
		return false;
	}

}

bool Galil_Thread::Cmd(QString command)
{
	try
	{
		check(GCmd(m_g, command.toStdString().c_str()));
		return true;

	}
	catch (GReturn gr) //for x_e() function
	{

		ExceptionInformation(gr);
		return false;
	}

}

bool Galil_Thread::CmdI(QString command, int* value)
{
	try
	{
		check(GCmdI(m_g, command.toStdString().c_str(), value));
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
		check(GCmdD(m_g, command.toStdString().c_str(), value));
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
		ZeroMemory(m_buf, sizeof(m_buf));
		check(GCmdT(m_g, command.toStdString().c_str(), m_buf, sizeof(m_buf), nullptr));
		value = m_buf;

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
	//qDebug() << "Function returned " << gr << '\n';
	GError(gr, m_buf, sizeof(m_buf));
	qDebug() << m_buf << '\n';
	GSize size = sizeof(m_buf);

	if (m_g)
	{
		GUtility(m_g, G_UTIL_ERROR_CONTEXT, m_buf, &size);
		qDebug() << m_buf << '\n'; //further context

		GClose(m_g); //close g
	}

	m_g = 0;

}

void Galil_Thread::run()
{
	qDebug() << "Galil_Thread Run Thread : " << QThread::currentThreadId();
	//int value = 0;
	QString varValue1,varValue2;
	while (!m_StopThread)
	{
		//value = 0;
		//CmdI("TI", &value);
		//qDebug() << "Input : " << value;
		//m_input.clear();
		//m_input = Parse_Galil_Input(value);
		//emit sendInputValue(value);
		//if (m_input[IOPoint - 1])  //发射触发相机信号
		//{
		//	emit triggerSinal();
		//}
		//Sleep(100);

		CmdT(m_varName, varValue1);
		qDebug() << m_varName + "	:" << varValue1;
		CmdT(m_varName, varValue2);
		qDebug() << m_varName + "	:" << varValue2;

		
		if (varValue1.toFloat() && varValue2.toFloat() && g_UpWaveEnable)  //发射触发相机信号
		{
			//取上升沿
			g_UpWaveEnable = false;
			emit sendVarValue(varValue1);
			emit triggerSinal();
		}

		Sleep(100);
	}

}


Galil_Thread::~Galil_Thread()
{
	if (isRunning())
		wait();

	if (m_g)
	{
		GClose(m_g); //close g
	}
	qDebug() << "~Galil_Thread";
}
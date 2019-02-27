#include "GalilThread.h"
//#include "Func.h"
#include "Global.h"
#include <QMessageBox>
#include <QTime>
#include "ConstParam.h"
#include "PylonCameraThread.h"
#include "HalconCameraThread.h"


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
	QString cameraValue, moveRightValue;
	bool IsWake = true;
	while (!m_StopThread)
	{
	//	QTime start;
	//	start.start();

		CmdT(SINAL_CAMERA, cameraValue);
		//Sleep(10);
		//CmdT(SINAL_MOVERIGHT1, moveRightValue);

		//触发相机
		static int picNum = 0;
		if (cameraValue.toFloat())
			qDebug() << "light on";

		if (cameraValue.toFloat() 
			//&&  g_UpWaveEnable 
			&& PylonCamera_Thread::ReadyWake()
			&& Halcon_Camera_Thread::ReadyWake())  //发射触发相机信号
		{
		
			IsWake = true;
			//qDebug() << QString("echo ") + SINAL_CAMERA + " :	" << cameraValue;
			//g_UpWaveEnable = false;
			PylonCamera_Thread::setReadyWake(false);
			Halcon_Camera_Thread::setReadyWake(false);
			//emit sendVarValue(varValue1);
			emit triggerSinal();
			picNum++;
			qDebug() << "emit trigger num:		"  << picNum;

		}

		//右侧触发分料
		//if (moveRightValue.toFloat() == SINAL_MOVERIGHT1_THRESHOLD
		//	&& IsWake)
		//{
		//	//qDebug() << QString("echo ") + SINAL_MOVERIGHT1 + " :	" << moveRightValue;
		//	//static int num = 0;
		//	//num++;
		//	//qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		//	//if (num % 3)
		//	//	Cmd(CLASSIFIY_BAD);		//分类良品
		//	//else
		//	//	Cmd(CLASSIFIY_GOOD);

		//		qDebug() << "enter fengliao lock !!!!		";
		//		g_mutex_Result.lock();
		//		if (g_Result_Queue.size())
		//		{
		//			qDebug() << "Enter g_Result_Queue.size =  "<<g_Result_Queue.size();
		//			if (g_Result_Queue.front())
		//			{
		//				//Cmd(CLASSIFIY_GOOD);		//分类良品
		//				qDebug() << "Galil send Good";
		//			}
		//			else
		//			{
		//				//Cmd(CLASSIFIY_BAD);		//分类不良品
		//				qDebug() << "Galil send Bad";
		//			}
		//			g_Result_Queue.pop();
		//		}
		//		g_mutex_Result.unlock();
		//		qDebug() << "quit fengliao lock!!!!		";
		//		qDebug() << "Quit g_Result_Queue.size = " << g_Result_Queue.size();
		//		IsWake = false;
		//}
	
		Sleep(80);

	//	qDebug() << "Each Galil_Loop  time:		" << start.elapsed() / 1000.0 << "s";
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
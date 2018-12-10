#include "DeltaThread.h"
#include "Func.h"
#include <QMessageBox>
#include <QTime>
#include "ConstParam.h"

QSerialPort* Delta_Thread::m_SerialPort = nullptr;
//std::atomic<std::string> Delta_Thread::m_write_string = "";
std::string Delta_Thread::m_write_string2 = "";
std::atomic<bool> Delta_Thread::m_bIsStop = false;
std::queue<std::string> Delta_Thread::m_Add_Queue;
std::queue<std::string> Delta_Thread::m_Default_Queue;
bool Delta_Thread::m_bIsOneMode = false;
int Delta_Thread::m_time_lag = 30;
Delta_Thread::QueryMode Delta_Thread::m_QueryMode = DefalutQuene;
QMutex Delta_Thread::m_mutex;
QMutex Delta_Thread::m_mutex_WriteData;
QWaitCondition Delta_Thread::m_waitWriteData;

void Delta_Thread::ClearDefaultQueue()
{
	
}


Delta_Thread::Delta_Thread(QObject *parent)
	: QThread(parent)
{
	qDebug() << "Delta_Thread construct func(),  Thread : " << QThread::currentThreadId();
}

void Delta_Thread::InitSerialPortInfo(const char* PortName, int BaudRate, QSerialPort::Parity parity, QSerialPort::DataBits databits)
{
	setPortName(PortName);
	setBaudRate(BaudRate);
	setParity(parity);
	setDataBits(databits);
}

void Delta_Thread::setWriteInfo(const char* wirteInfo)
{
	//m_write_string = Delta_Ascii_CR(wirteInfo);
	m_write_string2 = Delta_Ascii_CR(wirteInfo);

}

void Delta_Thread::setWriteInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
{
	//m_write_string = Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info);
	m_write_string2 = Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info);

}

//void Delta_Thread::AddSecondQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
//{
//	QMutexLocker locker(&m_mutex);
//	m_Add_Queue.push(Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info));
//	m_mutex_WriteData.lock();
//	m_waitWriteData.wakeAll();
//	m_mutex_WriteData.unlock();
//}

//void Delta_Thread::AddSecondQueueInfo(const std::string& data)
//{
//	QMutexLocker locker(&m_mutex);
//	m_Add_Queue.push(Delta_Ascii_CR(data));
//	m_mutex_WriteData.lock();
//	m_waitWriteData.wakeAll();
//	m_mutex_WriteData.unlock();
//}

void Delta_Thread::AddOneQueryInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
{
	Sleep(2);
	QMutexLocker locker(&m_mutex);
	m_Add_Queue.push(Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info));
	m_QueryMode = OneQuery;
	m_mutex_WriteData.lock();
	m_waitWriteData.wakeAll();
	m_mutex_WriteData.unlock();
}

void Delta_Thread::AddOneQueryInfo(const std::string& data)
{
	Sleep(2);
	QMutexLocker locker(&m_mutex);
	m_Add_Queue.push(Delta_Ascii_CR(data));
	m_mutex_WriteData.lock();
	m_QueryMode = OneQuery;
	m_waitWriteData.wakeAll();
	m_mutex_WriteData.unlock();
}

void Delta_Thread::AddOneQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
{
	Sleep(2);
	QMutexLocker locker(&m_mutex);
	m_Add_Queue.push(Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info));
	m_QueryMode = OneQueryToDefalutQuene;
	m_mutex_WriteData.lock();
	m_waitWriteData.wakeAll();
	m_mutex_WriteData.unlock();
}

void Delta_Thread::AddOneQueueInfo(const std::string& data)
{
	Sleep(2);
	QMutexLocker locker(&m_mutex);
	m_Add_Queue.push(Delta_Ascii_CR(data));
	m_mutex_WriteData.lock();
	m_QueryMode = OneQueryToDefalutQuene;
	//m_QueryMode = OneQuery;
	m_waitWriteData.wakeAll();
	m_mutex_WriteData.unlock();
}


//void Delta_Thread::AddDefaultQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
//{
//	QMutexLocker locker(&m_mutex);
//	m_Default_Queue.push(Delta_Ascii_CR(Slave + Function_Code + Start_Address + Other_Info));
//	m_mutex_WriteData.lock();
//	m_waitWriteData.wakeAll();
//	m_mutex_WriteData.unlock();
//}

void Delta_Thread::AddDefaultQueueInfo(const std::string& data)
{
	QMutexLocker locker(&m_mutex);
	m_Default_Queue.push(Delta_Ascii_CR(data));

}

void Delta_Thread::setPortName(const char* PortName)
{
	m_PortName = PortName;
}

void Delta_Thread::setBaudRate(int BaudRate)
{
	m_BaudRate = BaudRate;
}

void Delta_Thread::setParity(QSerialPort::Parity parity)
{
	m_parity = parity;
}

void Delta_Thread::setDataBits(QSerialPort::DataBits databits)
{
	m_databits = databits;
}

void Delta_Thread::CreateSerialPort()
{
	m_SerialPort = new QSerialPort;
	m_SerialPort->setBaudRate(m_BaudRate);
	m_SerialPort->setPortName(m_PortName.c_str());
	m_SerialPort->setParity(m_parity);
	m_SerialPort->setDataBits(m_databits);
	m_SerialPort->setStopBits(QSerialPort::OneStop);
	m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);
	
}

void Delta_Thread::run()
{
	//std::string test = ":00050520FF00D7\r\n";

	qDebug() << "Delta_Thread Run Thread : " << QThread::currentThreadId();

	//setWriteInfo("00050520FF00");

	bool is_Clear_m_write_string;

	CreateSerialPort();

	if (!m_SerialPort->open(QSerialPort::ReadWrite))
	{
		qDebug() << "Com open error,please check Com setting! ";
		emit error(G2U("未能正确打开PLC的串口,请检测串口设置!"));
		emit bool_error(false);
		delete m_SerialPort;
		m_SerialPort = nullptr;
		return;
	}
	
	m_bIsStop = false;
	while (!m_bIsStop)
	{
		/*static int tt = 0;
		QTime time;
		time.start();*/

		QMutexLocker locker(&m_mutex);
		switch (m_QueryMode)
		{
			case LoopOneQueue:

				if (!m_Add_Queue.empty())
				{
					//m_write_string = m_Add_Queue.front();
					m_write_string2 = m_Add_Queue.front();
					m_Add_Queue.pop();
				}
				break;

			case DefalutQuene:

				if (m_Default_Queue.size() == 0) 
				{
					delete m_SerialPort;
					m_SerialPort = nullptr;
					return;
				}
				
				//m_write_string = m_Default_Queue.front();
				m_write_string2 = m_Default_Queue.front();
				//m_Default_Queue.push(m_write_string._My_val);
				m_Default_Queue.push(m_write_string2);
				m_Default_Queue.pop();
				
				break;

			case OneQueryToDefalutQuene:
				if (m_Add_Queue.size() == 0)
				{
					qDebug() << " m_Add_Queue.size == 0" ;
				}
				else
				{
					//m_write_string = m_Add_Queue.front();
					m_write_string2 = m_Add_Queue.front();
					m_Add_Queue.pop();
				}
				
				break;

			case OneQuery:
				if (!m_Add_Queue.empty())
				{
					//m_write_string = m_Add_Queue.front();
					m_write_string2 = m_Add_Queue.front();
					m_Add_Queue.pop();
				}
				else
				{
					//m_write_string = "";
					m_write_string2 = "";
				}
				break;

			default:
				break;
		}

		//if (!(m_write_string._My_val == ""))
		if (!(m_write_string2 == ""))
		{
			//writeData(m_write_string._My_val.c_str(), m_write_string._My_val.length());
			writeData(m_write_string2.c_str(), m_write_string2.length());
			m_SerialPort->waitForReadyRead(500);
			receiveData();
			m_SerialPort->clear();		
		}
		else
		{
			locker.unlock();
			qDebug() << "waiting data write ......... ";
			m_mutex_WriteData.lock();
			m_waitWriteData.wait(&m_mutex_WriteData);
			m_mutex_WriteData.unlock();
		}
		
		if (m_bIsOneMode)
			break;

		//qDebug() << time.elapsed() << "ms";
	}

	delete m_SerialPort;
	m_SerialPort = nullptr;


}

int Delta_Thread::writeData(const char *data, int size)
{
	//qDebug() << "lastest writeData: " << data;
	
	int len = 0;
	forever
	{
		int n = m_SerialPort->write(&data[len], size - len);
		if (n == -1)
		{
			return -1;
		}
		else
		{
			len += n;
			if (size == len)
				break;
		}
	}
	//qDebug() << "Send to Plc data:	 " << data;
	return len;

}

bool Delta_Thread::receiveData()
{

	bool is_continue_recive = true;
	//bool is_Clear_m_write_string = false;


	while (is_continue_recive)
	{
		m_SerialPort->waitForReadyRead(m_time_lag);
		requestData = m_SerialPort->readAll();
		
		if (requestData.isEmpty())
		{
			is_continue_recive = false;
			m_SerialPort->clear();

			
			if (m_total_data[0] == ':' && m_total_data[m_total_data.size() - 1] == '\n')
			{
				//qDebug() << "lastest receiveData: "<<m_total_data;
				if (m_QueryMode == OneQueryToDefalutQuene)
				{
					if (m_Add_Queue.size() > 0)
						qDebug() << "Add_Queue > 0................";
					else
					{
						//qDebug() << "Change DefalutQuene................";
						m_QueryMode = DefalutQuene;
						//qDebug() << m_total_data;
					}

				}

				emit sendSerialData(m_total_data);
				//std::string function_code = m_total_data.toStdString().substr(3, 2);
				//if (function_code == "01")
				//{
				//	std::vector<bool> m_Y_States = Parse_Delta_Ascii(m_total_data.toStdString());
				//	if (m_Y_States.size() == 8)
				//	{
				//		bool isStartGrab = m_Y_States[1];	//读取Y61的状态
				//		//qDebug() << "pan duan Y61. ";
				//		if (isStartGrab)
				//		{
				//			qDebug() << "data:	 " << m_total_data;
				//			qDebug() << "ENTER IF. ";
				//			static int num = 0;
				//			m_Add_Queue.push(Delta_Ascii_CR(RESET_Y61));

				//			m_QueryMode = OneQueryToDefalutQuene;

				//			mutex_Camera.lock();
				//			condition_Camera.wakeAll();
				//			mutex_Camera.unlock();

				//			qDebug() << "receive Y61: " << ++num;
				//		}

				//	}
				//}
			}
			//Parse_Delta_Ascii(m_total_data.toStdString());
			//qDebug() << "recivedata : " << m_total_data;
			m_total_data.clear();
		}
		else
		{
			m_total_data.append(requestData);
		}
	}

	/*while (is_continue_recive)
	{
		
		while (m_SerialPort->bytesAvailable() < 17)
		{
			m_SerialPort->waitForReadyRead(m_time_lag);
		}
		requestData = m_SerialPort->readAll();

		if (m_QueryMode == OneQueryToDefalutQuene)
			m_QueryMode = DefalutQuene;

		emit sendSerialData(requestData);
		qDebug() << "recivedata : " << requestData;
		m_total_data.clear();	
		is_continue_recive = false;
	}
	*/
	return is_continue_recive;
}

void Delta_Thread::setOneMode(bool status)
{
	m_bIsOneMode = status;
}

void Delta_Thread::StopRun(bool status)
{
	m_bIsStop = status;
	m_mutex_WriteData.lock();
	m_waitWriteData.wakeAll();
	m_mutex_WriteData.unlock();
}

QSerialPort* Delta_Thread::GetSerialPort()
{
	return m_SerialPort;
}

void Delta_Thread::SetTimeLag(int ms)
{
	m_time_lag = ms;
}

void Delta_Thread::setQueryMode(QueryMode query_mode)
{
	m_QueryMode = query_mode;
}

Delta_Thread::~Delta_Thread()
{
	if (isRunning())
		wait();

	m_bIsStop = false;
	std::queue<std::string> empty;
	swap(empty, m_Add_Queue);

	std::queue<std::string> empty2;
	swap(empty, m_Default_Queue);

	//m_write_string._My_val.clear();
	m_write_string2.clear();
	m_bIsOneMode = false;

	qDebug() << "~Delta_Thread";
}
/**
*	���ܣ�̨��DVP_PLC�̡߳�
*	���ߣ�chenhui
*/

#ifndef DELTA_THREAD_H
#define DELTA_THREAD_H

#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <atomic>
#include <queue>
#include <QMutex>



class Delta_Thread : public QThread
{
	Q_OBJECT

public:
	enum QueryMode
	{
		LoopOneQueue,
		DefalutQuene,	//Ĭ�϶��з���ѭ����ѯ
		OneQueryToDefalutQuene,	//��ѯһ�κ�,�л���Ĭ�϶��в�ѯ
		OneQuery,	//��ѯһ�Σ������´��������ٲ�ѯ
	};

	explicit Delta_Thread(QObject *parent = 0);
	~Delta_Thread();
	
	int writeData(const char *data, int size);
	bool receiveData();
	void setPortName(const char* PortName);
	void setBaudRate(int BaudRate);
	void setParity(QSerialPort::Parity parity);
	void setDataBits(QSerialPort::DataBits databits);

	void InitSerialPortInfo(const char* PortName, int BaudRate, QSerialPort::Parity parity, QSerialPort::DataBits databits);
	static void setWriteInfo(const char* wirteInfo);
	static void setWriteInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	
	//static void AddSecondQueueInfo(const std::string& data);
	//static void AddSecondQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);
	static void AddOneQueryInfo(const std::string& data);	//���β�ѯ
	static void AddOneQueryInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//���β�ѯ
	static void AddOneQueueInfo(const std::string& data);	//��Ӳ�ѯ�ֶε���ѯһ�ζ���
	static void AddOneQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//��Ӳ�ѯ�ֶε���ѯһ�ζ���
	static void AddDefaultQueueInfo(const std::string& data);	//��Ӳ�ѯ�ֶε�Ĭ�϶���
	//static void AddDefaultQueueInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//��Ӳ�ѯ�ֶε�Ĭ�϶���
	static void setOneMode(bool status = false);
	static void StopRun(bool status = false);	//ֹͣ��ѯ���˳��߳�
	static void SetTimeLag(int ms);	//���ò�ѯ���ʱ��
	static QSerialPort* GetSerialPort();	//�ж��Ƿ�Ϊ��
	static void setQueryMode(QueryMode query_mode);	//�л���ѯģʽ
	static void ClearDefaultQueue();	//���Ĭ�϶���
	static void ClearOneQueue();	//��ղ�ѯ����
	
	

protected:
	virtual void run() Q_DECL_OVERRIDE;

	void CreateSerialPort();
	QByteArray requestData;
	QByteArray m_total_data;
	std::string m_PortName;
	int m_BaudRate;
	QSerialPort::Parity m_parity;
	QSerialPort::DataBits m_databits;
	
	static QSerialPort* m_SerialPort;
	//static std::string str;
	//static std::atomic<std::string> m_write_string;
	static std::string m_write_string2;
	static std::atomic<bool> m_bIsStop;
	static std::queue<std::string> m_Add_Queue;
	static std::queue<std::string> m_Default_Queue;
	static bool m_bIsOneMode;
	static int m_time_lag;
	static QueryMode m_QueryMode;
	static QMutex m_mutex;
	static QMutex m_mutex_WriteData;
	static QWaitCondition m_waitWriteData;

signals:
	void sendSerialData(QByteArray receiveData);
	void error(QString error);
	void bool_error(bool error);
};

#endif
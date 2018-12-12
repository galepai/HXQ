#pragma once
/**
*	���ܣ�̨��DVP_PLC�̡߳�
*	���ߣ�chenhui
*/

#ifndef GALIL_THREAD_H
#define GALIL_THREAD_H

#include <QThread>
#include <QDebug>
#include <atomic>
#include <queue>
#include <vector>
#include <QMutex>

#include "gclib.h"
#include "gclibo.h"


class Galil_Thread : public QThread
{
	Q_OBJECT
	Q_PROPERTY(QString ip READ ip WRITE setIp);

public:
	enum QueryMode
	{
		LoopOneQueue,
		DefalutQuene,	//Ĭ�϶��з���ѭ����ѯ
		OneQueryToDefalutQuene,	//��ѯһ�κ�,�л���Ĭ�϶��в�ѯ
		OneQuery,	//��ѯһ�Σ������´��������ٲ�ѯ
	};

	explicit Galil_Thread(QObject *parent = 0);
	~Galil_Thread();

	inline void check(GReturn rc)
	{
		if (rc != G_NO_ERROR)
			throw rc;
	}

	bool GcLibVersion();
	bool Open(QString address);
	bool Cmd(QString command);
	bool CmdI(QString command, int* value);
	bool CmdD(QString command, double* value);
	bool CmdT(QString command, QString& value);

	QString ip() const { return m_ip; }
	void setIp(QString ip) { m_ip = ip; }

	void stop() { m_StopThread = true; }

	//QString ip;
protected:
	virtual void run() Q_DECL_OVERRIDE;
	void ExceptionInformation(GReturn gr);
	

	
	std::vector<bool> m_input;


private:
	GCon m_g; //var used to refer to a unique connection. A valid connection is nonzero.
	int m_buf_size = G_SMALL_BUFFER;
	char m_buf[G_SMALL_BUFFER]; //traffic buffer
	//char* m_front;
	QString m_ip;
	bool m_StopThread;

signals:
	void sendInputValue(int value);
	void triggerSinal();

};

#endif
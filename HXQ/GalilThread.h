#pragma once
/**
*	���ܣ�̨��DVP_PLC�̡߳�
*	���ߣ�chenhui
*/

#ifndef GALIL_THREAD_H
#define GALIL_THREAD_H

#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <atomic>
#include <queue>
#include <QMutex>

#include "gclib.h"
#include "gclibo.h"


class Galil_Thread : public QThread
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

	explicit Galil_Thread(QObject *parent = 0);
	~Galil_Thread();

	inline void check(GReturn rc)
	{
		if (rc != G_NO_ERROR)
			throw rc;
	}

	bool GcLibVersion();
	bool Open(QString address);

protected:
	virtual void run() Q_DECL_OVERRIDE;
	void ExceptionInformation(GReturn gr);
	

	static std::atomic<bool> m_bIsStop;


private:
	GCon g; //var used to refer to a unique connection. A valid connection is nonzero.
	int buf_size = G_SMALL_BUFFER;
	char buf[G_SMALL_BUFFER]; //traffic buffer

signals:
	void sendSerialData(QByteArray receiveData);

};

#endif
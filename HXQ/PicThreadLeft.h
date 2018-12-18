#ifndef PIC_THREAD_LEFT_H
#define PIC_THREAD_LEFT_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadLeft : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadLeft(QObject *parent = 0)
		: QThread(parent), m_CameraId(0)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
		
	}

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

	void OnHandle(HTuple WindowHandle);

	void setCameraId(int id) { m_CameraId = id; }

protected:
	virtual void run() Q_DECL_OVERRIDE;

private:
	int m_CameraId;

signals:
	void resultReady(int Result,int CameraItem);



};

#endif
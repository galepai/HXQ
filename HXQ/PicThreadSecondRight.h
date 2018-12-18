#ifndef PIC_THREAD_SECONDRIGHT_H
#define PIC_THREAD_SECONDRIGHT_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadSecondRight : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadSecondRight(QObject *parent = 0)
		: QThread(parent)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}

	void OnHandle(HTuple WindowHandle);
	void setCameraId(int id) { m_CameraId = id; }


	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;

private:
	int m_CameraId;

signals:
	void resultReady(int Result, int CameraItem);



};

#endif
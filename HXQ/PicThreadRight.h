#ifndef PIC_THREAD_RIGHT_H
#define PIC_THREAD_RIGHT_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadRight : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadRight(QObject *parent = 0)
		: QThread(parent)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}

	void OnHandle(HTuple WindowHandle);
	void setModel(QString path){ m_ModelPath = path; }
	void setCameraId(int id) { m_CameraId = id; }

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;
	QString m_ModelPath;

private:
	int m_CameraId;

signals:
	void resultReady(int Result, int CameraItem);



};

#endif
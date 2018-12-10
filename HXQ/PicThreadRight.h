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

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;
	QString m_ModelPath;

signals:
	void resultReady(int bIsBad);



};

#endif
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
		: QThread(parent), m_CameraId(0),m_MaxNum(50),m_bIsSaveImage(false)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
		
	}

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

	void OnHandle(HObject& image, const HTuple& WindowHandle,HTuple* Result);
	void QueueSaveImage(const HObject& Image, int maxnum);
	void setCameraId(int id) { m_CameraId = id; }

	void setSaveImageDirName(const QString& path) { m_SaveImageDirName = path; }

	//Do Image save?
	bool IsSaveImage() { return m_bIsSaveImage; }
	void setSaveImage(bool enable = true) { m_bIsSaveImage = enable; }

	//save image nums
	void setSaveImageNum(int count = 50) { m_MaxNum = count; }
	int SaveImageNum() { return m_MaxNum; }

protected:
	virtual void run() Q_DECL_OVERRIDE;

private:
	int m_CameraId,m_MaxNum;
	bool m_bIsSaveImage;
	QString m_SaveImageDirName;

signals:
	void resultReady(int Result,int CameraItem);



};

#endif
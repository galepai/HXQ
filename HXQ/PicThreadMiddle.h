#ifndef PIC_THREAD_MIDDLE_H
#define PIC_THREAD_MIDDLE_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadMiddle : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadMiddle(QObject *parent = 0)
		: QThread(parent)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}

	void OnHandle(HTuple WindowHandle);
	void QueueSaveImage(const HObject& Image, int maxnum);


	void setCameraId(int id) { m_CameraId = id; }

	void setSaveImageDirName(const QString& path) { m_SaveImageDirName = path; }

	//Do Image save?
	bool IsSaveImage() { return m_bIsSaveImage; }
	void setSaveImage(bool enable = true) { m_bIsSaveImage = enable; }

	//save image nums
	void setSaveImageNum(int num = 50) { m_MaxNum = num; }
	int SaveImageNum() { return m_MaxNum; }

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;

private:
	int m_CameraId,m_MaxNum;
	bool m_bIsSaveImage;
	QString m_SaveImageDirName;

signals:
	void resultReady(int Result, int CameraItem);




};

#endif
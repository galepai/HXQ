/**
*	功能：Halcon使用QThread调用线程类
*	作者：chenhui
*/

#ifndef HALCON_CAMERA_THREAD_H
#define HALCON_CAMERA_THREAD_H

#include <QThread>
//#include <QDebug>
#include <QWaitCondition>
#include <atomic>
#include <QMutex>
#include "HalconCpp.h"

using namespace HalconCpp;
class Halcon_Camera_Thread : public QThread
{
	Q_OBJECT
	
public:
	enum ConnectionType
	{
		DirectShow,		//
		GigEVision2,	//网线连接
		USB3Vision		//
	};

	enum Code
	{
		XmlError,		
		ParamError,	
		Good
	};

	explicit Halcon_Camera_Thread(QString CameraId, QObject *parent = 0);
	~Halcon_Camera_Thread();
	
	QString CameraId() const;
	HFramegrabber* Camera() { return m_pGrabber; }

	void setSaveImageDirName(const QString& path);
	void setAcquisitionMode(QString acquisitionMode){m_AcquisitionMode = acquisitionMode;}

	void setTriggerMode(QString triggerMode){m_TriggerMode = triggerMode;}

	void setExposureTime(float exposureTime){m_exposureTime = exposureTime;}

	void setAcquisitionLineRate(float acquisitionLineRate){m_acquisitionLineRate = acquisitionLineRate;}

	void setHeight(int height){m_height = height;}

	bool ReadyWake(){return m_WaitWake;}

	//Do Image save?
	bool IsSaveImage() { return m_bIsSaveImage; }
	void setSaveImage(bool enable = true){ m_bIsSaveImage = enable;}

	//Do thread stop? 
	void setStopStatus(bool enable = true) { m_bIsStop = enable; }
	bool StopStatus() { return m_bIsStop; }

	//save image nums
	void setSaveImageNum(int num = 50) { m_MaxNum = num; }
	int SaveImageNum() { return m_MaxNum; }

	//manual control grab
	void setMutexTrigger(bool enable = false) { m_bIsMutexTrigger = enable; }

	static QStringList m_CameraIdlist;
	static bool IsExistCameraId(QString cameraId);

	HTuple m_WindowHandle;
	
protected:
	virtual void run() Q_DECL_OVERRIDE;
	virtual bool OpenCamera(HalconCpp::HFramegrabber* pGrabber);

	static QMutex m_mutex;
	static QMutex m_mutex_WriteData;
	static QWaitCondition m_waitWriteData;
	void QueueSaveImage(const HObject& Image, int maxnum);
	

signals:
	void ReadyOk(int num);
	void sendImage(void* pimage);
	void CameraErrorInformation(QString error);
	void CameraErrorInformation(bool error);
	void grab_correct_image(int num);
	void OpenCameraSinal(void** pGrabber,QString nodeCameraName,int* returnCode);

private:
	QString m_nodeCameraName,m_CameraId;
	ConnectionType m_connectionType;
	HFramegrabber* m_pGrabber;
	HImage m_Image;

	bool m_bIsStop, m_bIsMutexTrigger;
	QString m_SaveDatePath, m_SaveImageDirName;
	int m_image_index, m_MaxNum;
	bool m_WaitWake;
	bool m_bIsSaveImage;

	float m_exposureTime;
	int m_height;
	float m_acquisitionLineRate;
	QString m_AcquisitionMode;
	QString m_TriggerMode;
	QString m_TriggerSource;
};

#endif
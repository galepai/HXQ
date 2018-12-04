/**
*	功能：台达DVP_PLC线程。
*	作者：chenhui
*/

#ifndef PylonCamera_Thread_H
#define PylonCamera_Thread_H

#include <QThread>
#include <QDebug>
#include <QWaitCondition>
#include <atomic>
#include <QMutex>
#include <HalconCpp.h>
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
using namespace Pylon;
using namespace HalconCpp;


class PylonCamera_Thread : public QThread
{
	Q_OBJECT
	
public:
	enum ConnectionType
	{
		DirectShow,	//USB连接
		GigEVision,	//网线连接
	};

	explicit PylonCamera_Thread(ConnectionType connection_type, QString CameraId, QObject *parent = 0);
	~PylonCamera_Thread();
	
	QString CameraId() const;
	void setSaveDatePath(const QString& path);
	void setSaveImageNum(int MaxNum = 50){ m_MaxNum = MaxNum; };
	void setSaveImageDirName(const QString& path);
	void SetExposureTime(int exposureTime)
	{
		m_exposureTime = exposureTime;
	}
	bool ReadyWake()
	{
		return m_WaitWake;
	}

	void stop();

	static QStringList m_CameraIdlist;
	static bool IsExistCameraId(QString cameraId);

	HTuple m_WindowHandle;
	
protected:
	virtual void run() Q_DECL_OVERRIDE;

	bool OpenCamera();
	void setConnection_Type(ConnectionType connection_type);
	static QMutex m_mutex;
	static QMutex m_mutex_WriteData;
	static QWaitCondition m_waitWriteData;
	void QueueSaveImage(const HObject& Image, int maxnum);
	

signals:
	void ReadyOk(int num);
	void signal_image(void* pimage);
	void signal_error(QString error);
	void grab_correct_image(int num);

private:
	QString m_CameraId;
	ConnectionType m_connectionType;
	CBaslerGigEInstantCamera m_camera;
	bool m_bIsStop;
	QString m_SaveDatePath, m_SaveImageDirName;
	int m_image_index, m_MaxNum, m_exposureTime;
	bool m_WaitWake;
};

#endif
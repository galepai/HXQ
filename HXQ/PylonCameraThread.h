/**
*	���ܣ�̨��DVP_PLC�̡߳�
*	���ߣ�chenhui
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
		DirectShow,	//USB����
		GigEVision,	//��������
	};

	//explicit PylonCamera_Thread(ConnectionType connection_type, QString CameraId, QObject *parent = 0);
	explicit  PylonCamera_Thread::PylonCamera_Thread(QString nodeCameraName, QObject *parent);
	~PylonCamera_Thread();
	
	QString CameraId() const;
	void setSaveDatePath(const QString& path);
	void setSaveImageNum(int MaxNum = 50){ m_MaxNum = MaxNum; };
	void setSaveImageDirName(const QString& path);
	void SetExposureTime(int exposureTime)
	{
		m_exposureTime = exposureTime;
	}


	//Do Image save?
	bool IsSaveImage() { return m_bIsSaveImage; }
	void setSaveImage(bool enable = true) { m_bIsSaveImage = enable; }

	//manual control grab
	void setMutexTrigger(bool enable = false) { m_bIsMutexTrigger = enable; }

	//Do thread stop? 
	void setStopStatus(bool enable = true) { m_bIsStop = enable; }
	bool StopStatus() { return m_bIsStop; }

	void stop();

	static QStringList m_CameraIdlist;
	static bool IsExistCameraId(QString cameraId);

	static bool m_WaitWake;
	static bool ReadyWake(){return m_WaitWake;}
	static void setReadyWake(bool enable) { m_WaitWake = enable; }

	HTuple m_WindowHandle;
	
protected:
	virtual void run() Q_DECL_OVERRIDE;

	bool OpenCamera();
	void setConnection_Type(ConnectionType connection_type);
	static QMutex m_mutex;
	static QMutex m_mutex_WriteData;
	static QWaitCondition m_waitWriteData;
	void QueueSaveImage(const HObject& Image, int maxnum);
	void ParserCamParamAndSetCamera(CBaslerGigEInstantCamera& pCamera, std::vector <std::pair<std::pair<QString, QString>, QString>>& CamParam);

signals:
	void ReadyOk(int num);
	void signal_image(void* pimage);
	void signal_error(QString error);
	void grab_correct_image(int num);
	void CameraErrorInformation(QString error);
	void CameraErrorInformation(bool error);

private:
	QString m_nodeCameraName,m_CameraId;
	ConnectionType m_connectionType;
	CBaslerGigEInstantCamera m_camera;
	//CInstantCamera m_camera;
	bool m_bIsStop;
	QString m_SaveDatePath, m_SaveImageDirName;
	int m_image_index, m_MaxNum, m_exposureTime;
	bool  m_bIsSaveImage, m_bIsMutexTrigger;
	

};

#endif
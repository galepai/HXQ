
#ifndef HikVision_Camera_Thread_H
#define HikVision_Camera_Thread_H

#include "HalconCameraThread.h"
#include <QDebug>
//#include <QWaitCondition>
//#include <atomic>
//#include <QMutex>

//using namespace HalconCpp;
class HikVision_Camera_Thread : public Halcon_Camera_Thread
{
	Q_OBJECT
	
public:
	explicit HikVision_Camera_Thread(QString CameraId, QObject *parent = 0);
	~HikVision_Camera_Thread();
	
protected:
	virtual bool OpenCamera(HalconCpp::HFramegrabber* pGrabber);

};

#endif
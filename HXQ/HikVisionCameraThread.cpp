#pragma execution_character_set("utf-8")

#include "HikVisionCameraThread.h"
//#include <QMessageBox>
//#include <QTime>
#include "Func.h"
#include "ConstParam.h"



HikVision_Camera_Thread::HikVision_Camera_Thread(QString CameraId, QObject *parent):Halcon_Camera_Thread(CameraId, parent)
{

}

HikVision_Camera_Thread::~HikVision_Camera_Thread()
{
	
}


bool HikVision_Camera_Thread::OpenCamera(HalconCpp::HFramegrabber* pGrabber)
{
	pGrabber = new HalconCpp::HFramegrabber;
	try{
		
		pGrabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", \
			CameraId().toStdString().c_str(), 0, -1);
		pGrabber->SetFramegrabberParam("AcquisitionMode", "Continuous");
		pGrabber->SetFramegrabberParam("TriggerSelector", "FrameBurstStart");
		pGrabber->SetFramegrabberParam("TriggerMode", "Off");
		pGrabber->SetFramegrabberParam("ExposureTime", 400000.0);
		pGrabber->SetFramegrabberParam("grab_timeout", 5000);

		emit CameraErrorInformation(true);
		return true;
	}
	catch (HException& e)
	{
		//emit signal_error(e.ErrorMessage().Text());

		QString eror = e.ErrorMessage().Text();
		emit CameraErrorInformation(tr("不能获取相机，请检测相机ID是否正确"));
		emit CameraErrorInformation(false);
		//m_mutex.unlock();
		return false;
	}
	
}


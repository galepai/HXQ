#pragma execution_character_set("utf-8")

#include "HalconCameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"
#include "Global.h"
#include "DeltaThread.h"
#include "ConstParam.h"


QMutex Halcon_Camera_Thread::m_mutex;
//QMutex Halcon_Camera_Thread::m_mutex_WriteData;
//QWaitCondition Halcon_Camera_Thread::m_waitWriteData;
QStringList Halcon_Camera_Thread::m_CameraIdlist;
bool Halcon_Camera_Thread::m_WaitWake;


Halcon_Camera_Thread::Halcon_Camera_Thread(QString nodeCameraName, QObject *parent)
	:m_nodeCameraName(nodeCameraName),QThread(parent)
{
	m_image_index = 1;
	m_exposureTime = 30.0;
	m_acquisitionLineRate = 10000.0;
	m_height = 10000;
	
	m_pGrabber = nullptr;
	m_WaitWake = false;
	m_AcquisitionMode = "SingleFrame";
	m_TriggerMode = "Off";

	QString type, cameraName;
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), nodeCameraName, type, cameraName);
	ReadXmlElementText(QString(XML_Camera), cameraName, QString("id"), type, m_CameraId);
	m_CameraIdlist.append(m_CameraId);
}

void Halcon_Camera_Thread::run()
{
	int isCorretOpen = Code::Ok;

	//if (!OpenCamera(m_pGrabber))
	//	return;
		
	//qDebug() << "HalconThread: "<<QThread::currentThreadId();

	emit OpenCameraSinal((void**)&m_pGrabber, m_nodeCameraName, &isCorretOpen);

	switch (isCorretOpen)
	{
	case Code::Ok:
		emit CameraErrorInformation(true);
		
		break;

	case Code::XmlError:
		emit CameraErrorInformation(tr("无法连接相机:\r\n相机ID：") + CameraId() + tr("\r\n\r\n原因：\r\n1.读取配置文件错误"));
		emit CameraErrorInformation(false);
		return;
		break;

	case Code::ParamError:
		emit CameraErrorInformation(tr("无法连接相机:\r\n相机ID：") + CameraId() + tr("\r\n\r\n原因：\r\n1.相机线缆未正确连接。\r\n2.相机ID错误。"));
		emit CameraErrorInformation(false);
		return;
		break;
	}
	

	m_bIsStop = false;
	

	bool first = true;
	HImage Image;

	m_pGrabber->GrabImageStart(-1);
	while (!m_bIsStop)
	{
		try{
			
			qDebug() << m_CameraId << " ready cap.... ";

			
			if (m_bIsMutexTrigger)
			{
				g_mutex_Camera.lock();
				//if (first) {
				//	//emit ReadyOk(1);
				//	first = false;
				//}

				Sleep(10);
				m_WaitWake = true;
				g_condition_Camera.wait(&g_mutex_Camera);
				g_mutex_Camera.unlock();
				m_WaitWake = false;
				
			}
				

			if (m_bIsStop)
				break;
			
			//HImage* pImage = new HImage();
			QTime timedebuge;//声明一个时钟对象
			timedebuge.start();//开始计时

			Image = m_pGrabber->GrabImage();
			qDebug() << "Camera：" << timedebuge.elapsed() / 1000.0 << "s";//输出计时
			//Image = m_pGrabber->GrabImageAsync(-1);
			
			emit grab_correct_image(1);
			emit sendImage(&Image);
			//DispColor(Image, m_WindowHandle);
			if (m_bIsSaveImage)
			{
				
				QueueSaveImage(Image, m_MaxNum);
				
			}	
		
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			if (!m_bIsStop)
			{
				qDebug() << m_CameraId << ": " << error << "\n";
				qDebug() << "Camere enter exception error" << ": " << error << "\n";

				{

					Image = m_pGrabber->GrabImage();

					emit grab_correct_image(1);
					emit sendImage(&Image);
				
					if (m_bIsSaveImage)
					{
						QueueSaveImage(Image, m_MaxNum);
					}
				}

				continue;

			}		
			else
				break;
		}	
	}
	m_pGrabber->Clear();
	
}

Halcon_Camera_Thread::~Halcon_Camera_Thread()
{
	m_CameraIdlist.removeAll(m_CameraId);
	delete m_pGrabber;
	m_pGrabber = nullptr;
}

bool Halcon_Camera_Thread::IsExistCameraId(QString cameraId)
{
	return m_CameraIdlist.contains(cameraId);
}


QString Halcon_Camera_Thread::CameraId() const
{
	return m_CameraId;
}

bool Halcon_Camera_Thread::OpenCamera(HalconCpp::HFramegrabber* pGrabber)
{
	pGrabber = new HalconCpp::HFramegrabber;

	try{

		pGrabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", \
			m_CameraId.toStdString().c_str(), 0, -1);

		/*pGrabber->SetFramegrabberParam("AcquisitionMode", m_AcquisitionMode.toStdString().c_str());
		pGrabber->SetFramegrabberParam("TriggerSelector", "FrameBurstStart");
		pGrabber->SetFramegrabberParam("TriggerMode", m_TriggerMode.toStdString().c_str());
		pGrabber->SetFramegrabberParam("ExposureTime", m_exposureTime);
		pGrabber->SetFramegrabberParam("grab_timeout", 5000);*/

		emit CameraErrorInformation(true);
		return true;
	}
	catch (HException& e)
	{
		//emit signal_error(e.ErrorMessage().Text());

		QString eror = e.ErrorMessage().Text();
		//emit CameraErrorInformation(QTextCodec::codecForName("GBK")->toUnicode("不能获取相机，请检测相机ID是否正确"));
		emit CameraErrorInformation(tr("不能获取相机，请检测相机ID是否正确"));
		emit CameraErrorInformation(false);
		//m_mutex.unlock();
		return false;
	}
	

}

void Halcon_Camera_Thread::QueueSaveImage(const HObject& Image,int maxnum)
{
	g_mutex_SaveImage.lock();
	if (m_image_index <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(m_image_index, 4, 10, QChar('0'));
		QTime start;
		start.start();
		
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		
		qDebug() << "SaveImage：" << start.elapsed() / 1000.0 << "s";//输出计时
		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(m_image_index, 4, 10, QChar('0'));
		QTime start;
		start.start();
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		qDebug() << "SaveImage：" << start.elapsed() / 1000.0 << "s";//输出计时
		m_image_index++;
	}

	g_mutex_SaveImage.unlock();
}

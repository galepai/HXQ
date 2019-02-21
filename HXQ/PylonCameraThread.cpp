#include "PylonCameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"
#include "Global.h"
#include "ConstParam.h"
#include "pylon/PylonGUIIncludes.h"
#include "DeltaThread.h"

QMutex PylonCamera_Thread::m_mutex;
QStringList PylonCamera_Thread::m_CameraIdlist;
bool PylonCamera_Thread::m_WaitWake;

class CHardwareTriggerConfiguration : public Pylon::CConfigurationEventHandler
{
private:
	int m_ExposureTime;
public:
	CHardwareTriggerConfiguration(int ExposureTime){ m_ExposureTime = ExposureTime; }

	void OnOpened(Pylon::CInstantCamera& camera)
	{
		try
		{
			CBaslerGigEInstantCamera* _pCamera = static_cast<CBaslerGigEInstantCamera*>(&camera);

			qDebug() << endl << "==========================================";
			qDebug() << _pCamera->DeviceVendorName.ToString();
			qDebug() << _pCamera->DeviceModelName.ToString();
			qDebug() << _pCamera->DeviceFirmwareVersion.ToString();
			qDebug() << _pCamera->DeviceVendorName.ToString();
			qDebug() << _pCamera->DeviceID.ToString();

			_pCamera->Width.SetValue(2048);
			_pCamera->Height.SetValue(8000);
			_pCamera->ExposureTimeRaw.SetValue(m_ExposureTime);
			_pCamera->AcquisitionLineRateAbs.SetValue(16000.0);
			_pCamera->TriggerSelector.FromString("FrameStart");
#ifdef HARDTRIGGER
			_pCamera->TriggerMode.FromString("On");
			_pCamera->TriggerSource.FromString("Line1");
			_pCamera->TriggerActivation.FromString("LevelHigh");
			_pCamera->LineDebouncerTimeAbs.SetValue(1.0);
#else
			_pCamera->TriggerMode.FromString("Off");
#endif
		
			qDebug() << _pCamera->Width.GetValue();
			qDebug() << _pCamera->Height.GetValue();
			qDebug() << _pCamera->ExposureTimeRaw.GetValue();
			qDebug() << _pCamera->AcquisitionLineRateAbs.GetValue();
			qDebug() << _pCamera->TriggerSelector.ToString();
			qDebug() << _pCamera->TriggerMode.ToString();
			qDebug() << _pCamera->TriggerSource.ToString();
			qDebug() << _pCamera->TriggerActivation.ToString();
			qDebug() << "==========================================" << endl;
			qDebug() << "CHardwareTriggerConfiguration Run Thread : " << QThread::currentThreadId();
			
		}
		catch (const GenericException& e)
		{
			throw RUNTIME_EXCEPTION("Could not apply configuration. const GenericException caught in OnOpened method msg=%hs", e.what());
		}
	}
};

//PylonCamera_Thread::PylonCamera_Thread(ConnectionType connection_type,QString CameraId, QObject *parent)
//	: m_connectionType(connection_type),m_CameraId(CameraId),QThread(parent)

PylonCamera_Thread::PylonCamera_Thread(QString nodeCameraName, QObject *parent)
	: m_nodeCameraName(nodeCameraName), QThread(parent)
{
	m_image_index = 1;
	m_exposureTime = 200;
	//m_CameraIdlist.append(CameraId);
	m_WaitWake = false;

	QString type, cameraName;
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), nodeCameraName, type, cameraName);
	ReadXmlElementText(QString(XML_Camera), cameraName, QString("id"), type, m_CameraId);
	m_CameraIdlist.append(m_CameraId);
}

void PylonCamera_Thread::run()
{
	qDebug() << "PylonCamera_Thread Run Thread : " << QThread::currentThreadId();
	
	if (!OpenCamera())
	{
		emit CameraErrorInformation(G2U("无法连接相机:\r\n相机ID：") + CameraId() );
		emit CameraErrorInformation(false);
		return;
	}

	emit CameraErrorInformation(true);


	m_bIsStop = false;
	HImage Image;
	QTime time;

	bool first = true;
	//m_camera.StartGrabbing();  //配合WaitForFrameTriggerReady版本
	while (!m_bIsStop)
	{
		try{
			//time.start();
			qDebug() << endl << m_CameraId << " ready cap.... ";
			
			CGrabResultPtr ptrGrabResult;

			
			//if (m_camera.IsGrabbing())
			//{
			//	m_camera.WaitForFrameTriggerReady(5000);
			//	m_camera.ExecuteSoftwareTrigger();
			//	time.start();
			//	m_camera.RetrieveResult(5000, ptrGrabResult);
			//	qDebug() << m_CameraId << " all time: " << time.elapsed() / 1000.0;
			//	if (ptrGrabResult->GrabSucceeded())
			//	{
			//		// Access the image data.
			//		qDebug() << endl << "Grab Image Info";
			//		qDebug() << "=========================";
			//		qDebug() << "SizeX: " << ptrGrabResult->GetWidth();
			//		qDebug() << "SizeY: " << ptrGrabResult->GetHeight();thread condition
			//		const uint8_t* pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
			//		qDebug() << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0];
			//		qDebug() << "=========================" << endl;
			//		m_mutex.lock();
			//		GenImage1(&Image, "byte", (int)ptrGrabResult->GetWidth(), (int)ptrGrabResult->GetHeight(), (Hlong)pImageBuffer);
			//		m_mutex.unlock();
			//		signal_image(&Image);
			//		QueueSaveImage(Image, m_MaxNum);
			//		//qDebug() << m_CameraId << " all time: " << time.elapsed() / 1000.0;
			//	}
			//	else
			//	{
			//		qDebug() << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
			//	}
			//}
		
			if (m_bIsMutexTrigger)
			{
				g_mutex_Camera.lock();
				if (first) {
					emit ReadyOk(1);
					first = false;
				}

				Sleep(10);
				m_WaitWake = true;
				g_condition_Camera.wait(&g_mutex_Camera);
				g_mutex_Camera.unlock();

				m_WaitWake = false;
			}

			if (m_bIsStop)
				break;
			

			//m_camera.ExposureTimeRaw.SetValue(m_exposureTime);
			////
			QTime timedebuge;//声明一个时钟对象
			timedebuge.start();//开始计时
			if (m_camera.GrabOne(5000, ptrGrabResult))
			{
				if (ptrGrabResult->GrabSucceeded())
				{
					qDebug() << "Camera Pylon：" << timedebuge.elapsed() / 1000.0 << "s";//输出计时
					// Access the image data.
					/*qDebug() << endl << "Grab Image Info :" << m_CameraId;
					qDebug() << "=========================" ;
					qDebug() << "SizeX: " << ptrGrabResult->GetWidth();
					qDebug() << "SizeY: " << ptrGrabResult->GetHeight();*/
					const uint8_t* pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
					//qDebug() << m_CameraId << "	:	" << pImageBuffer;
					
					/*qDebug() << "========================="  << endl;*/

					//m_mutex.lock();
					GenImage1(&Image, "byte", (int)ptrGrabResult->GetWidth(), (int)ptrGrabResult->GetHeight(), (Hlong)pImageBuffer);
					//m_mutex.unlock();

					//if (isCorrectImage(Image, 0.0))
					//{
						
						emit grab_correct_image(1);
						emit signal_image(&Image);

						if (m_bIsSaveImage)
						{
							QueueSaveImage(Image, m_MaxNum);
						}

						//Sleep(20);
					//}
					
					//qDebug() << m_CameraId << " all time: " << time.elapsed() / 1000.0;
					//Sleep(300);

				}
				else
				{
					qDebug() << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
				}
			}
			
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			if (!m_bIsStop)
			{
				qDebug() << m_CameraId << error ;
				continue;

			}		
			else
				break;
		}
		catch (const GenericException &e)
		{
			// Error handling.
			qDebug() << "An exception occurred in run:" << endl
				<< e.GetDescription() << endl;
			emit signal_error(e.GetDescription());
		}
	}
	m_camera.Close();
	m_camera.DetachDevice();

}

PylonCamera_Thread::~PylonCamera_Thread()
{
	m_CameraIdlist.removeAll(m_CameraId);
	// Releases all pylon resources. 
	PylonTerminate();
}

bool PylonCamera_Thread::IsExistCameraId(QString cameraId)
{
	return m_CameraIdlist.contains(cameraId);
}


QString PylonCamera_Thread::CameraId() const
{
	return m_CameraId;
}

bool PylonCamera_Thread::OpenCamera()
{

	try{
				// Before using any pylon methods, the pylon runtime must be initialized. 
			PylonInitialize();

			try
			{
				//m_camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());
				m_camera.Attach(CTlFactory::GetInstance().CreateDevice(m_CameraId.toStdString().c_str()));
				//m_camera.RegisterConfiguration(new CHardwareTriggerConfiguration(m_exposureTime), RegistrationMode_ReplaceAll, Cleanup_Delete);
				//CInstantCamera m_camera(CTlFactory::GetInstance().CreateFirstDevice());

				m_camera.MaxNumBuffer = 3;
				m_camera.Open();

				QString  sideCameraName,type;
				ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), m_nodeCameraName, type, sideCameraName);
				
				std::vector <std::pair<std::pair<QString, QString>, QString>> Param;
				if (!ParserXmlNode(QString(XML_Camera), sideCameraName, Param))
				{
					return false;
				}
				ParserCamParamAndSetCamera(m_camera, Param);

			//	using namespace GenApi;
			//	INodeMap& nodemap = m_camera.GetNodeMap();
			//	 //Get camera device information.
			//	qDebug() << endl << "Camera Device Information" << endl
			//		<< "=========================";
			////	qDebug() << "Vendor					: "
			////		<< CStringPtr(nodemap.GetNode("DeviceVendorName"))->GetValue();
			//	qDebug() << "Model					: "
			//		<< CStringPtr(nodemap.GetNode("DeviceModelName"))->GetValue();
			//	qDebug() << "Firmware version		: "
			//		<< CStringPtr(nodemap.GetNode("DeviceFirmwareVersion"))->GetValue();
			//	qDebug() << "Using Device FullInfo	: " 
			//		<< m_camera.GetDeviceInfo().GetFullName();
			//	// Camera settings.
			//	qDebug() << endl << "Camera Device Settings" << endl
			//		<< "======================" << endl;
			//	CIntegerPtr Width(nodemap.GetNode("Width"));
			//	CIntegerPtr Height(nodemap.GetNode("Height"));
			//	CIntegerPtr ExposureTimeRaw(nodemap.GetNode("ExposureTimeRaw"));
			//	CFloatPtr	AcquisitionLineRateAbs(nodemap.GetNode("AcquisitionLineRateAbs"));
			//	CEnumerationPtr TriggerSelector(nodemap.GetNode("TriggerSelector"));
			//	CEnumerationPtr TriggerMode(nodemap.GetNode("TriggerMode"));
			//	CEnumerationPtr	TriggerSource(nodemap.GetNode("TriggerSource"));
			//	CEnumerationPtr TriggerActivation(nodemap.GetNode("TriggerActivation"));	
			//	Width->SetValue(2048);
			//	Height->SetValue(8000);
			//	ExposureTimeRaw->SetValue(350);
			//	AcquisitionLineRateAbs->SetValue(16000.0);
			//	TriggerSelector->FromString("FrameStart");
			//	TriggerMode->FromString("Off");
			//	TriggerSource->FromString("Line1");
			//	TriggerActivation->FromString("RisingEdge");
			//	/*qDebug() << "Width					: " << Width->GetValue();
			//	qDebug() << "Height					: " << Height->GetValue();
			//	qDebug() << "ExposureTimeRaw			: " << ExposureTimeRaw->GetValue();
			//	qDebug() << "AcquisitionLineRateAbs	: " << AcquisitionLineRateAbs->GetValue();
			//	qDebug() << "TriggerSelector			: " << TriggerSelector->ToString();
			//	qDebug() << "TriggerMode				: " << TriggerMode->ToString();
			//	qDebug() << "TriggerSource			: " << TriggerSource->ToString();
			//	qDebug() << "TriggerActivation		: " << TriggerActivation->ToString();
			//	qDebug() << "======================" << endl;*/
			//	// The parameter MaxNumBuffer can be used to control the count of buffers
			//	// allocated for grabbing. The default value of this parameter is 10.


			}
			catch (const GenericException &e)
			{
					// Error handling.
					qDebug() << "An exception occurred." << endl
						<< e.GetDescription() << endl;
					emit signal_error(e.GetDescription());
					m_camera.DetachDevice();
					m_camera.Close();
					return false;
			}
	
		return true;
	}
	catch (GenICam::AccessException& e)
	{
		QString  error = e.GetDescription();
		emit signal_error(error);
		return false;
	}

}


void PylonCamera_Thread::stop()
{
	m_bIsStop = true;
}

void PylonCamera_Thread::setSaveDatePath(const QString& path)
{
	m_SaveDatePath = path;
}

void PylonCamera_Thread::setSaveImageDirName(const QString& ImageDirName)
{
	m_SaveImageDirName = ImageDirName;
}

void PylonCamera_Thread::QueueSaveImage(const HObject& Image,int maxnum)
{
	g_mutex_SaveImage.lock();
	if (m_image_index <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		m_image_index++;
	}
	g_mutex_SaveImage.unlock();
}

void PylonCamera_Thread::ParserCamParamAndSetCamera(CBaslerGigEInstantCamera& Camera, std::vector <std::pair<std::pair<QString, QString>, QString>>& CamParam)
{
	using namespace GenApi;
	INodeMap& nodemap = Camera.GetNodeMap();
	//Get camera device information.
	//qDebug() << endl << "Camera Device Information" << endl
	//	<< "=========================";
	//qDebug() << "Vendor					: "
	//	<< CStringPtr(nodemap.GetNode("DeviceVendorName"))->GetValue();
	//qDebug() << "Model					: "
	//	<< CStringPtr(nodemap.GetNode("DeviceModelName"))->GetValue();
	//qDebug() << "Firmware version		: "
	//	<< CStringPtr(nodemap.GetNode("DeviceFirmwareVersion"))->GetValue();
	//qDebug() << "Using Device FullInfo	: "
	//	<< m_camera.GetDeviceInfo().GetFullName();
	//// Camera settings.
	//qDebug() << endl << "Camera Device Settings" << endl
	//	<< "======================" << endl;

	for (int index = 1; index < CamParam.size(); index++)
	{
		QString name = CamParam[index].first.first;
		QString type = CamParam[index].first.second;
		QString value = CamParam[index].second;
		//qDebug() << name+"\r\n" << endl;
		if (type.contains("float"))
		{
			CFloatPtr	floatPtr(nodemap.GetNode(name.toStdString().c_str()));
			floatPtr->SetValue(value.toFloat());
		}
		else if (type.contains("int"))
		{
			CIntegerPtr integerPtr(nodemap.GetNode(name.toStdString().c_str()));
			integerPtr->SetValue(value.toInt());
		}
		else
		{
			CEnumerationPtr enumerationPtr(nodemap.GetNode(name.toStdString().c_str()));
			enumerationPtr->FromString(value.toStdString().c_str());
		}
	}
}


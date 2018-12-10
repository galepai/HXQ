#include "PylonCameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"
#include "ConstParam.h"
#include "pylon/PylonGUIIncludes.h"
#include "DeltaThread.h"

QMutex PylonCamera_Thread::m_mutex;
QStringList PylonCamera_Thread::m_CameraIdlist;


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
			_pCamera->Height.SetValue(10000);
			_pCamera->ExposureTimeRaw.SetValue(m_ExposureTime);
			_pCamera->AcquisitionLineRateAbs.SetValue(10000.0);
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

PylonCamera_Thread::PylonCamera_Thread(ConnectionType connection_type,QString CameraId, QObject *parent)
	: m_connectionType(connection_type),m_CameraId(CameraId),QThread(parent)
{
	m_image_index = 1;
	m_exposureTime = 200;
	m_CameraIdlist.append(CameraId);
	m_WaitWake = false;
}

void PylonCamera_Thread::run()
{
	qDebug() << "PylonCamera_Thread Run Thread : " << QThread::currentThreadId();
	
	if (!OpenCamera())
		return;

	m_bIsStop = false;
	HImage Image;
	QTime time;

	bool first = true;
	//m_camera.StartGrabbing();  //ÅäºÏWaitForFrameTriggerReady°æ±¾
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
		
			mutex_Camera.lock();
			if (first){
				emit ReadyOk(1);
				first = false;
			}

			Sleep(10);
			m_WaitWake = true;
			condition_Camera.wait(&mutex_Camera);
			mutex_Camera.unlock();

			m_WaitWake = false;

			if (m_bIsStop)
				break;
			

			m_camera.ExposureTimeRaw.SetValue(m_exposureTime);
			
			if (m_camera.GrabOne(5000, ptrGrabResult))
			{
				if (ptrGrabResult->GrabSucceeded())
				{
					// Access the image data.
					/*qDebug() << endl << "Grab Image Info :" << m_CameraId;
					qDebug() << "=========================" ;
					qDebug() << "SizeX: " << ptrGrabResult->GetWidth();
					qDebug() << "SizeY: " << ptrGrabResult->GetHeight();*/
					const uint8_t* pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
					//qDebug() << m_CameraId << "	:	" << pImageBuffer;
					
					/*qDebug() << "========================="  << endl;*/

					m_mutex.lock();
					GenImage1(&Image, "byte", (int)ptrGrabResult->GetWidth(), (int)ptrGrabResult->GetHeight(), (Hlong)pImageBuffer);
					m_mutex.unlock();

					//if (isCorrectImage(Image, 0.0))
					//{
						
						emit grab_correct_image(1);
						emit signal_image(&Image);
						QueueSaveImage(Image, m_MaxNum);

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
		switch (m_connectionType)
		{
		case PylonCamera_Thread::DirectShow:
			
			return false;
			break;

		case PylonCamera_Thread::GigEVision:
	
			// Before using any pylon methods, the pylon runtime must be initialized. 
			PylonInitialize();

			try
			{
				//m_camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());
				m_camera.Attach(CTlFactory::GetInstance().CreateDevice(m_CameraId.toStdString().c_str()));
				m_camera.RegisterConfiguration(new CHardwareTriggerConfiguration(m_exposureTime), RegistrationMode_ReplaceAll, Cleanup_Delete);

				//using namespace GenApi;
				//INodeMap& nodemap = m_camera.GetNodeMap();
				// //Get camera device information.
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
				//CIntegerPtr Width(nodemap.GetNode("Width"));
				//CIntegerPtr Height(nodemap.GetNode("Height"));
				//CIntegerPtr ExposureTimeRaw(nodemap.GetNode("ExposureTimeRaw"));
				//CFloatPtr	AcquisitionLineRateAbs(nodemap.GetNode("AcquisitionLineRateAbs"));
				//CEnumerationPtr TriggerSelector(nodemap.GetNode("TriggerSelector"));
				//CEnumerationPtr TriggerMode(nodemap.GetNode("TriggerMode"));
				//CEnumerationPtr	TriggerSource(nodemap.GetNode("TriggerSource"));
				//CEnumerationPtr TriggerActivation(nodemap.GetNode("TriggerActivation"));	
				//Width->SetValue(2048);
				//Height->SetValue(10000);
				//ExposureTimeRaw->SetValue(760);
				//AcquisitionLineRateAbs->SetValue(10000.0);
				//TriggerSelector->FromString("FrameStart");
				//TriggerMode->FromString("Off");
				//TriggerSource->FromString("Line1");
				//TriggerActivation->FromString("RisingEdge");
				//qDebug() << "Width					: " << Width->GetValue();
				//qDebug() << "Height					: " << Height->GetValue();
				//qDebug() << "ExposureTimeRaw			: " << ExposureTimeRaw->GetValue();
				//qDebug() << "AcquisitionLineRateAbs	: " << AcquisitionLineRateAbs->GetValue();
				//qDebug() << "TriggerSelector			: " << TriggerSelector->ToString();
				//qDebug() << "TriggerMode				: " << TriggerMode->ToString();
				//qDebug() << "TriggerSource			: " << TriggerSource->ToString();
				//qDebug() << "TriggerActivation		: " << TriggerActivation->ToString();
				//qDebug() << "======================" << endl;
				// The parameter MaxNumBuffer can be used to control the count of buffers
				// allocated for grabbing. The default value of this parameter is 10.

				m_camera.MaxNumBuffer = 3;
				m_camera.Open();
			

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

			break;

		default:

			break;
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

	if (m_image_index <= maxnum)
	{
		QString saveImagePath = QString("images/" + m_SaveImageDirName + "/" + m_SaveImageDirName + "_%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		QString saveImagePath = QString("images/" + m_SaveImageDirName + "/"+ m_SaveImageDirName +"_%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		m_image_index++;
	}
}


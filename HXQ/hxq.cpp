#include "hxq.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QThread>
#include "CHH.h"
#include "aboutdialog.h"
#include "ComDialog.h"
#include "DeltaThread.h"
#include "ConstParam.h"
#include "ConfigureDialog.h"
#include "PicThreadMiddle.h"
#include "PicThreadLeft.h"
#include "PicThreadRight.h"
#include "PicThreadSecondRight.h"




hxq::hxq(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_LeftWindowHandle = m_MiddleWindowHandle = m_RightWindowHandle = m_SecondRightWindowHandle = 0;
	// �˵�����Ӧ����
	connect(ui.About, &QAction::triggered, this, &hxq::OnAbout);
	connect(ui.OnLRC, &QAction::triggered, this, &hxq::OnLRC);
	connect(ui.OnDeltaDebug, &QAction::triggered, this, &hxq::DebugDialog);
	//��������Ӧ����
	connect(ui.Open, &QAction::triggered, this, &hxq::OnOpen);
	connect(ui.OnRun, &QAction::triggered, this, &hxq::OnOneHandle_AllPic);
	connect(ui.OnLineRun, SIGNAL(triggered()), this, SLOT(OnStart()));
	connect(ui.OnStop, SIGNAL(triggered()), this, SLOT(OnStop()));
	connect(ui.Configure, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnTest()));
	connect(ui.AreaCamera, SIGNAL(triggered()), this, SLOT(OnOpen()));
	
	
	ui.OnStop->setEnabled(false);

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //��֧������д�������ļ�
	
	//HIDDLE_DIALOG_BUTTON
	FullScreenShow();	//ȫ����ʾ

	m_camera_thread_7_Clock = nullptr;
	m_camera_thread_11_Clock = nullptr;
	m_Pylon_camera_thread_2_Clock = nullptr;
	m_Pylon_camera_thread_10_Clock = nullptr;

	m_Galil = nullptr;

	m_AllResult = 0;
	m_good = m_bad = m_total = 0;

	m_peviousProductDectectEnd = true;
	
	ReadExposure();
	


	std::vector<bool> num = Parse_Galil_Input(7);


	/****/
	if (!m_Galil)
	{
		m_Galil = new Galil_Thread(this);
	}
	connect(m_Galil, SIGNAL(finished()), m_Galil, SLOT(deleteLater())); 
	connect(m_Galil, SIGNAL(triggerSinal()), m_Galil, SLOT(OnWakeCamera()));
	m_Galil->GcLibVersion();
	if (m_Galil->Open(QString(GalilIp) + ""))
	{
		m_Galil->start();
		if (m_Galil->Cmd("SB0"))
		{
			qDebug() << "SB0 Sucess!";
		}
	}
	else
	{
		delete m_Galil;
	}
	/****/


}

//ȫ����ʾ
void hxq::FullScreenShow()
{
	QRect rect = qApp->desktop()->geometry();
	setGeometry(qApp->desktop()->geometry());
	setContentsMargins(0, 0, 0, 0);
}


hxq::~hxq()
{

}

void hxq::OnAbout()
{
	AboutDialog Dlg(this);
	Dlg.exec();
}

void hxq::OnLRC()
{
	ComDialog Dlg(this);
	Dlg.exec();
}

void hxq::OnOpen()
{

	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files(*.jpg *.tif *.bmp)"));

	if (path.contains("camera1")
		|| path.contains("camera2")
		//|| path.contains("camera3")
		|| path.contains("camera4"))
	{
		int i = path.lastIndexOf('/');
		QString ImageName = path.toStdString().substr(i, path.length() - 1).c_str(); 
		QString tempPath = path.remove(ImageName);
		i = tempPath.lastIndexOf('/');
		QString upDir = tempPath.toStdString().substr(0, i + 1).c_str();
		
		i = ImageName.lastIndexOf('_');
		ImageName = ImageName.toStdString().substr(i+1, ImageName.length() - 1).c_str();


		QString ImagePath = upDir + "camera1/Camera1_" + ImageName;
		ReadImage(&m_LeftImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_LeftImage, LeftView);

		ImagePath = upDir + "camera2/Camera2_" + ImageName;
		ReadImage(&m_MiddleImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_MiddleImage, MiddleView);

		/*ImagePath = upDir + "camera3/Camera3_" + ImageName;
		ReadImage(&m_SecondRightImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_SecondRightImage, SecondRightView);*/

		/*ImagePath = upDir + "camera4/Camera4_" + ImageName;
		ReadImage(&m_RightImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_RightImage, RightView);*/

		statusBar()->showMessage(QString(G2U("���μ��ͼ��: ")) + ImageName);
	}
	else
	{
		if (path.length() != 0 &&
			(path.contains(".jpg") || path.contains(".tif")))
		{
			ReadImage(&m_Image, path.toLocal8Bit().constData());
			DispPic(m_Image, LeftView);
			statusBar()->showMessage(path);
		}
	}

	
}

void hxq::OnDisplayAreaCamera()
{
	QVariant ExposureValue;
	/**	�����������	*/
	m_camera_thread_11_Clock = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, LineCameraId_Dalsa_11_Clock, this);
	m_camera_thread_11_Clock->setSaveImageDirName("Camera1");
	m_camera_thread_11_Clock->setSaveImageNum(999);
	ReadConfigure("config.ini", "Camera_11_Clock", "Exposure", ExposureValue);
	m_camera_thread_11_Clock->SetExposureTime(ExposureValue.toFloat());
	//m_camera_thread_11_Clock->SetAcquisitionLineRate(10000.0);
	//m_camera_thread_11_Clock->SetHeight(10000);
	connect(m_camera_thread_11_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_camera_thread_11_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	connect(m_camera_thread_11_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_camera_thread_11_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));	//����ͼ��ʾ
	connect(m_camera_thread_11_Clock, SIGNAL(finished()), m_camera_thread_11_Clock, SLOT(deleteLater()));
	m_camera_thread_11_Clock->start();
}

//OffLine����ͼ��
void hxq::OnOneHandle_AllPic()
{
	readAllModelFromIni();

	if (m_LeftImage.Key() != 0)
	{
		DispPic(m_LeftImage, LeftView);
		OnHandleImageThread(m_LeftImage, LeftView);
	}
		

	if (m_MiddleImage.Key() != 0)
	{
		DispPic(m_MiddleImage, MiddleView);
		OnHandleImageThread(m_MiddleImage, MiddleView);
	}
		

	/*if (m_SecondRightImage.Key() != 0)
	{
		DispPic(m_SecondRightImage, SecondRightView);
		OnHandleImageThread(m_SecondRightImage, SecondRightView);
	}*/
		

	/*if (m_RightImage.Key() != 0)
	{
		DispPic(m_RightImage, RightView);
		OnHandleImageThread(m_RightImage, RightView);
	}*/
		
}

void hxq::OnShutDown()
{
	if (!ui.OnStop->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, G2U("ϵͳ"), G2U("ȷ���˳�ϵͳ"));
		if (reply == QMessageBox::StandardButton::Yes)
		{
			//OnClearCameraThread();
			qApp->quit();
		}
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("��ʾ"), G2U("����ֹͣϵͳ,��������'ֹͣ''��ť"));

	}
	
}

void hxq::OnClearCameraThread()
{
	if (m_camera_thread_7_Clock)
		m_camera_thread_7_Clock->stop();

	if (m_camera_thread_11_Clock)
		m_camera_thread_11_Clock->stop();

	if (m_Pylon_camera_thread_2_Clock)
		m_Pylon_camera_thread_2_Clock->stop();

	/*if (m_Pylon_camera_thread_10_Clock)
		m_Pylon_camera_thread_10_Clock->stop();*/

	Sleep(20);
	condition_Camera.wakeAll();

	if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa_7_Clock))
	{
		if (m_camera_thread_7_Clock->isRunning())
		{
			m_camera_thread_7_Clock->stop();
			m_camera_thread_7_Clock->wait();
		}
	}

	if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa_11_Clock))
	{
		if (m_camera_thread_11_Clock->isRunning())
		{
			m_camera_thread_11_Clock->stop();
			m_camera_thread_11_Clock->wait();
		}
	}

	if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler_2_Clock))
	{
		if (m_Pylon_camera_thread_2_Clock->isRunning())
		{
			m_Pylon_camera_thread_2_Clock->stop();
			m_Pylon_camera_thread_2_Clock->wait();
		}
	}

	/*if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler_10_Clock))
	{
		if (m_Pylon_camera_thread_10_Clock->isRunning())
		{
			m_Pylon_camera_thread_10_Clock->stop();
			m_Pylon_camera_thread_10_Clock->wait();
		}
	}*/
}

void hxq::OnConfigure()
{
	ConfigureDialog Dlg(this);
	Dlg.exec();
}

void hxq::OnLineRun()
{
	OnTest();
}

//��Ӧ���ڵĶ�ȡ�ۺ���
void hxq::receiveSerialData(QByteArray str)
{	
	//qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str,500);	
	static int num = 0;
	if (!str.isEmpty())
	{
		std::string function_code = str.toStdString().substr(3, 2);
		if (function_code == "01")
		{
			std::vector<bool> m_Y_States = Parse_Delta_Ascii(str.toStdString());
			if (m_Y_States.size() == 8)
			{
				bool isStartGrab = m_Y_States[1];	//��ȡY61��״̬,On��ʾ�Ӿ���λ
				if (isStartGrab 
					&& m_peviousProductDectectEnd
					//&& m_Pylon_camera_thread_10_Clock->ReadyWake()
					&& m_Pylon_camera_thread_2_Clock->ReadyWake()
					&& m_camera_thread_7_Clock->ReadyWake()
					&& m_camera_thread_11_Clock->ReadyWake())
				{
					
						Delta_Thread::AddOneQueueInfo(RESET_Y61);
						Sleep(20);
						m_peviousProductDectectEnd = false;
						OnWakeCamera();
						qDebug() << "receive Y61: " << ++num;
					
				}		
			}
		}
			
	}
	
}


void hxq::OnWakeCamera()
{
	mutex_Camera.lock();
	condition_Camera.wakeAll();
	mutex_Camera.unlock();
	Sleep(10);
	mutex_Camera.lock();
	condition_Camera.wakeAll();
	mutex_Camera.unlock();
}


void hxq::DispPic(HImage& Image, LocationView location)
{
	int width = Image.Width();
	int height = Image.Height();
	HTuple* pWindowHandle = nullptr;
	switch (location)
	{
		case RightView:
			pWindowHandle = &m_RightWindowHandle;;
			break;

		case SecondRightView:
			pWindowHandle = &m_SecondRightWindowHandle;;
			break;

		case LeftView:
			pWindowHandle = &m_LeftWindowHandle;
			break;

		case MiddleView:
			pWindowHandle = &m_MiddleWindowHandle;
			break;

	}
	
	if (*pWindowHandle == 0)
	{
		
		SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);

	}
	else
	{
		//ClearWindow(*pWindowHandle);
		//CloseWindow(*pWindowHandle);

		//SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);
	}

}

//ָ����ʾ���ڵľ��
void hxq::SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location)
{
	switch (location)
	{
	/*case RightView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.RightPicView->winId(), pWindowHandle);
		break;*/

	/*case SecondRightView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.SecondRightPicView->winId(), pWindowHandle);
		break;*/

	case LeftView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.LeftPicView->winId(), pWindowHandle);
		break;

	case MiddleView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.MiddlePicView->winId(), pWindowHandle);
		break;
	}
}

//ָ����ͼ�Ĺ���������
void hxq::SetPicViewScroll(int width, int height, LocationView location)
{
	float scaleX = width / 1024.0;
	float scaleY = height / 2000.0;
	switch (location)
	{
	/*case SecondRightView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_SecondRightPic(2050);
			VSCROLL_WIDTH_SecondRightPic(410);
		}
		break;*/

	/*case RightView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_RightPic(2050);
			VSCROLL_WIDTH_RightPic(410);
		}
		else
		{
			HSCROLL_HEIGHT_RightPic(500);
		}
		break;*/

	case LeftView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_LeftPic(2050);
			VSCROLL_WIDTH_LeftPic(410);
		}
		else
		{
			HSCROLL_HEIGHT_LeftPic(500);
		}
		break;

	case MiddleView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_MiddlePic(2050);
			VSCROLL_WIDTH_MiddlePic(410);
		}
		else
		{
			HSCROLL_HEIGHT_MiddlePic(500);
		}
		break;
	}
}


const HalconCpp::HTuple hxq::GetViewWindowHandle(LocationView location)
{
	if (location == RightView)
	{
		return m_RightWindowHandle;
	}
	else if (location == MiddleView)
	{
		return m_MiddleWindowHandle;
	}
	else if (location == SecondRightView)
	{
		return m_SecondRightWindowHandle;
	}
	else
	{
		return m_LeftWindowHandle;
	}
}

/***********�Ҷ˵��Թ���************/
//�򿪵��Խ���
void hxq::DebugDialog()
{
	TestDialog dlg(this);
	dlg.exec();
}

//����
void hxq::OnStart()
{
	QVariant ModeValue;
	ReadConfigure("config.ini", "Mode", "Connection", ModeValue);
	if (ModeValue.toString() == "OnLine")
	{
		m_bIsOnLine = true;
	}
	else
	{
		m_bIsOnLine = false;
	}


	if (m_bIsOnLine)
	{
		if (OpenSerial())
		{
			OnOpenCameras();
		}
	}
	else
	{
		OnOpenCameras();
	}

	readAllModelFromIni();

}

void hxq::OnOpenCameras()
{
	QVariant ExposureValue;
	/*-----Halcon Version------------*/
	/**	�����������	*/
	m_camera_thread_11_Clock = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, LineCameraId_Dalsa_11_Clock, this);
	m_camera_thread_11_Clock->setSaveImageDirName("Camera1");
	m_camera_thread_11_Clock->setSaveImageNum(999);
	ReadConfigure("config.ini", "Camera_11_Clock", "Exposure", ExposureValue);
	m_camera_thread_11_Clock->SetExposureTime(ExposureValue.toFloat());
	//m_camera_thread_11_Clock->SetAcquisitionLineRate(10000.0);
	//m_camera_thread_11_Clock->SetHeight(10000);
	connect(m_camera_thread_11_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_camera_thread_11_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	connect(m_camera_thread_11_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_camera_thread_11_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));	//����ͼ��ʾ
	connect(m_camera_thread_11_Clock, SIGNAL(finished()), m_camera_thread_11_Clock, SLOT(deleteLater()));
	m_camera_thread_11_Clock->start();

	/**	7�㷽��DALSA��ɨ	*/
	//m_camera_thread_7_Clock = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, LineCameraId_Dalsa_7_Clock, this);
	//m_camera_thread_7_Clock->setSaveImageDirName("Camera2");
	//m_camera_thread_7_Clock->setSaveImageNum(999);
	//ReadConfigure("config.ini", "Camera_07_Clock", "Exposure", ExposureValue);
	//m_camera_thread_7_Clock->SetExposureTime(ExposureValue.toFloat());
	//m_camera_thread_7_Clock->SetAcquisitionLineRate(10000.0);
	//m_camera_thread_7_Clock->SetHeight(10000);
	//connect(m_camera_thread_7_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_camera_thread_7_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	//connect(m_camera_thread_7_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	//connect(m_camera_thread_7_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImage(void*)));	//����ͼ��ʾ
	//connect(m_camera_thread_7_Clock, SIGNAL(finished()), m_camera_thread_7_Clock, SLOT(deleteLater()));
	//m_camera_thread_7_Clock->start();


	///*-----Pylon Version------------*/
	/**	Basler��ɨ	*/
	m_Pylon_camera_thread_10_Clock = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler_10_Clock, this);
	m_Pylon_camera_thread_10_Clock->setSaveImageDirName("Camera3");
	m_Pylon_camera_thread_10_Clock->setSaveImageNum(100);
	ReadConfigure("config.ini", "Camera_10_Clock", "Exposure", ExposureValue);
	m_Pylon_camera_thread_10_Clock->SetExposureTime(ExposureValue.toInt());
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveSecondRightImage(void*)));	//�Ҷ���ͼ��ʾ
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(finished()), m_Pylon_camera_thread_10_Clock, SLOT(deleteLater()));
	m_Pylon_camera_thread_10_Clock->start();

	/**	2�㷽��Basler��ɨ	*/
	//m_Pylon_camera_thread_2_Clock = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler_2_Clock, this);
	//m_Pylon_camera_thread_2_Clock->setSaveImageDirName("Camera4");
	//m_Pylon_camera_thread_2_Clock->setSaveImageNum(999);
	//ReadConfigure("config.ini", "Camera_02_Clock", "Exposure", ExposureValue);
	//m_Pylon_camera_thread_2_Clock->SetExposureTime(ExposureValue.toInt());
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveRightImage(void*)));	//������ͼ��ʾ
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(finished()), m_Pylon_camera_thread_2_Clock, SLOT(deleteLater()));
	//m_Pylon_camera_thread_2_Clock->start();

	ui.OnLineRun->setEnabled(false);
}

//׼���������,�������������豸�ȴ�PLC�Ĵ����źŻ����ֶ�����
void hxq::OnReadyOk(int num)
{
	static int total = 0;
	total += num;
	if (total == 3)
	{
		QMessageBox::StandardButton reply;
		if (m_bIsOnLine)
		{
			reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("�ɼ��豸�Ѿ�λ,�ȴ�����Ʒ...\r\n		���'OK'"));
		}
		else
		{
			reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("�ֶ�ģʽ,���ֶ��������...\r\n		���'OK'"));
		}
		
		
		total = 0;
		/* Button State */
		ui.OnStop->setEnabled(true);
	}
}

//ֹͣ
void hxq::OnStop()
{
	Delta_Thread::StopRun(true);

	OnClearCameraThread();

	m_Result_AllQueue;
	ui.OnStop->setEnabled(false);
	ui.OnLineRun->setEnabled(true);

	
	
}

//�������ź�
void hxq::OnDetectEnd()
{
	if (Delta_Thread::GetSerialPort())
	{
		Delta_Thread::AddOneQueueInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueueInfo(DETECT_END_OFF);
	}		
}


//******��Ӧ��ͼ��ʾͼƬ
void hxq::receiveLeftImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, LeftView);	
}

void hxq::receiveMiddleImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, MiddleView);
}

void hxq::receiveSecondRightImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, SecondRightView);
	
}

void hxq::receiveRightImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, RightView);
}
/********/

//******��Ӧ��ͼ��ʾͼƬ,������ͼƬ
void hxq::receiveLeftImageAndHandle(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, LeftView);
	OnHandleImageThread(ima, LeftView);
}

void hxq::receiveMiddleImageAndHandle(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, MiddleView);
	OnHandleImageThread(ima, MiddleView);
}

void hxq::receiveSecondRightImageAndHandle(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, SecondRightView);
	OnHandleImageThread(ima, SecondRightView);

}

void hxq::receiveRightImageAndHandle(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, RightView);
	OnHandleImageThread(ima, RightView);

}
/********/

//�ַ���ͼ�����߳�
void hxq::OnHandleImageThread(HImage& ima, LocationView view)
{
	switch (view)
	{
	case LeftView:
	{
		PicThreadLeft* pPicThread = new PicThreadLeft(this);
		pPicThread->m_Image = ima;
		pPicThread->m_WindowHandle = GetViewWindowHandle(view);
		pPicThread->setModel(gbModel());
		connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
		connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
		pPicThread->start();
	}

	break;

	case MiddleView:
	{
		PicThreadMiddle* pPicThread = new PicThreadMiddle(this);
		pPicThread->m_Image = ima;
		pPicThread->m_WindowHandle = GetViewWindowHandle(view);
		connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
		connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
		pPicThread->start();
	}
	break;

	//case SecondRightView:
	//{
	//	PicThreadSecondRight* pPicThread = new PicThreadSecondRight(this);
	//	pPicThread->m_Image = ima;
	//	pPicThread->m_WindowHandle = GetViewWindowHandle(view);
	//	connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
	//	connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
	//	pPicThread->start();
	//}
	//break;

	case RightView:
	{
		PicThreadRight* pPicThread = new PicThreadRight(this);
		pPicThread->m_Image = ima;
		//pPicThread->setModel(bottomModel());
		pPicThread->m_WindowHandle = GetViewWindowHandle(view);
		connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
		connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
		pPicThread->start();
	}
	break;

	/*default:
	break;*/
	}
}


//���ɴ���Ի���ʾ��
void hxq::genErrorDialog(QString error)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::warning(this, G2U("����"), error);
}




//�յ���ȷͼƬ��,������
void hxq::receiveCorrectImage(int value)
{
	static int imageNum = 0;
	imageNum += value;

	if (imageNum == 3)  //�յ�1��ͼƬ
	{
		m_total++;
		if (m_bIsOnLine)
		{
			//OnDetectEnd();
		}
		imageNum = 0;
		qDebug() << "Send M177:				" << m_total;

		ui.lcdNumber_total->display(m_total);
	}

}


void hxq::OnTest()
{
	if (m_bIsOnLine)
	{
		OnDetectEnd();
	}
	else
	{
		if (m_peviousProductDectectEnd)
		{
			OnWakeCamera();
		}
		else
		{
			qDebug() << "m_peviousProductDectectEnd = false.";
		}
		
	}
	
}

bool hxq::OpenSerial()
{
	bool isRet = true;

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	Delta_Thread::AddDefaultQueueInfo(READ_Y61_OUTPUT);	//��Y60-Y67

	//���õ��β�ѯģʽ
	//Delta_Thread::setQueryMode(Delta_Thread::QueryMode::OneQuery);

	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(sendSerialData(QByteArray)), this, SLOT(receiveSerialData(QByteArray)));
		connect(thread, SIGNAL(error(QString)), this, SLOT(genErrorDialog(QString)));
		connect(thread, &Delta_Thread::bool_error, [&](bool is){ isRet = is; });
		thread->start();
		Sleep(100);  //�ȴ�������������,����isRet;
	}

	return isRet;

}

//�õ��̵߳��ź�,���жϼ����,���͸�PLC
void hxq::handleResults(int singleResult)
{

	m_AllResult += singleResult;

	//if (m_AllResult >= LeftGood + MiddleGood + SecondRightGood + RightGood)
	if (m_AllResult >= LeftGood + MiddleGood + RightGood)
	{
		
		qDebug() << "Image num:				" << m_total;

		//if (m_AllResult == LeftGood + MiddleGood + SecondRightGood + RightGood)
		if (m_AllResult == LeftGood + MiddleGood + RightGood)
		{
			Sleep(30);
			m_good++;
			if (m_bIsOnLine)
			{
				
				Delta_Thread::AddOneQueueInfo(RESULT_GODD);	
				OnDetectEnd();
				m_Result_Queue.push(1);
				m_Result_AllQueue.push(1);
			}	

			qDebug() << "Send Good!!!	";
			ui.lcdNumber_good->display(m_good);
		}
		else
		{
			Sleep(30);
			m_bad++;
			if (m_bIsOnLine)
			{
				
				Delta_Thread::AddOneQueueInfo(RESULT_BAD);
				OnDetectEnd();
				m_Result_Queue.push(0);
				m_Result_AllQueue.push(0);
			}
				

			qDebug() << "Send Bad!!!	";
			ui.lcdNumber_bad->display(m_bad);
		}

		qDebug() << "All Detect Result:	" << m_AllResult;
		if (m_Result_Queue.size() >= 3)
		{
			int result = m_Result_Queue.front();
			if (result)
			{
				Delta_Thread::AddOneQueueInfo(FENGLIAO_GOOD);
				m_Result_Queue.pop();
				//qDebug() << "POP GOOD ";
				//qDebug() << "m_Result_Queue:  "<<m_Result_Queue.size();
			}
			else
			{
				Delta_Thread::AddOneQueueInfo(FENGLIAO_BAD);
				m_Result_Queue.pop();
				//qDebug() << "POP BAD ";
				//qDebug() << "m_Result_Queue:  " << m_Result_Queue.size();
			}
		}
		m_AllResult = 0;
		m_peviousProductDectectEnd = true;

		return;
	}

	qDebug() << "Detect is_bad:	" << singleResult;
	qDebug() << "Detect Result:	" << m_AllResult;
}

//
void hxq::ReadExposure()
{

	

}

//ʵʱ�����ع�
void hxq::OnSetExposure()
{


	
}

//read lastest model for picHandle.
void hxq::readAllModelFromIni()
{
	QVariant value;
	ReadConfigure("config.ini", "Model", "BottomModel", value);
	setBottomModel(value.toString());

	ReadConfigure("config.ini", "Model", "GouModel", value);
	setGbModel(value.toString());

}
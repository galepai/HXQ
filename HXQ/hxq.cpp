#include "hxq.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QThread>
#include "CHH.h"
#include "aboutdialog.h"
#include "EngineerDialog.h"
#include "DeltaThread.h"
#include "ConstParam.h"
#include "ConfigureDialog.h"
#include "PicThreadMiddle.h"
#include "PicThreadLeft.h"
#include "PicThreadRight.h"
#include "PicThreadSecondRight.h"
#include <QtXml\QDomDocument>
#include <map>





hxq::hxq(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//HIDDLE_DIALOG_BUTTON
	FullScreenShow();	//全屏显示

	//信号槽连接
	menuConnect();
	toolConnect();

	//其他初始化
	m_LeftWindowHandle = m_MiddleWindowHandle = m_RightWindowHandle = m_SecondRightWindowHandle = 0;

	//初始工程师状态
	ui.OnOperator->setChecked(false);
	ui.OnEngineer->setChecked(true);
	OnEngineerStatus();

	//初始操作员状态
	//ui.OnOperator->setChecked(true);
	//ui.OnEngineer->setChecked(false);
	//OnOperatorStatus();

	//写入最后一次打开时间
	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //不支持中文写入配置文件

	m_bIsDisOneImage = false;

	//相机线程
	m_camera_thread_7_Clock = nullptr;
	m_camera_thread_top = nullptr;
	m_Pylon_camera_thread_2_Clock = nullptr;
	m_Pylon_camera_thread_10_Clock = nullptr;
	m_HikVisionCameraThread = nullptr;

	m_Galil = nullptr;

	m_AllResult = 0;
	m_good = m_bad = m_gou = m_cao = m_liantong = m_total = 0;

	m_peviousProductDectectEnd = true;

	ZeroMemory(&m_detectResult, sizeof(m_detectResult));
	
	process = new QProcess(this);

}


// 菜单栏对应功能
void hxq::menuConnect()
{
	connect(ui.OnEngineer, &QAction::triggered, this, &hxq::OnEngineer);
	connect(ui.OnOperator, &QAction::triggered, this, &hxq::OnOperator);
	connect(ui.About, &QAction::triggered, this, &hxq::OnAbout);
}

//工具栏对应功能
void hxq::toolConnect()
{
	connect(ui.Open, &QAction::triggered, this, &hxq::OnOpen);
	connect(ui.OnRun, &QAction::triggered, this, &hxq::OnOneHandle_AllPic);
	connect(ui.OnLineRun, SIGNAL(triggered()), this, SLOT(OnStart()));
	connect(ui.OnStop, SIGNAL(triggered()), this, SLOT(OnStop()));
	connect(ui.Configure, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnTest()));
	connect(ui.AreaCamera, SIGNAL(triggered()), this, SLOT(OnDisplayAreaCamera()));

}

//全屏显示
void hxq::FullScreenShow()
{
	QRect rect = qApp->desktop()->geometry();
	setGeometry(qApp->desktop()->geometry());
	setContentsMargins(0, 0, 0, 0);
}


hxq::~hxq()
{

}

void hxq::OnEngineer()
{
	EngineerDialog Dlg(this);
	Dlg.exec();
	if (Dlg.correct())
	{
		ui.OnOperator->setChecked(false);
		ui.OnEngineer->setChecked(true);
		OnEngineerStatus();
	}
	else
	{
		ui.OnOperator->setChecked(true);
		ui.OnEngineer->setChecked(false);
		OnOperatorStatus();
	}

	
}

void hxq::OnOperator()
{
	ui.OnOperator->setChecked(true);
	ui.OnEngineer->setChecked(false);
	OnOperatorStatus();

	//process->start("d:/PanDownload/PanDownload.exe", QStringList("d:/PanDownload/PanDownload.exe"));
	//process->startDetached("d:/PanDownload/PanDownload.exe", QStringList("d:/PanDownload/PanDownload.exe"));
	
	//QProcess::execute("d:/PanDownload/PanDownload.exe", QStringList("d:/PanDownload/PanDownload.exe"));
}

void hxq::OnEngineerStatus()
{
	ui.Configure->setEnabled(true);
}

void hxq::OnOperatorStatus()
{
	ui.Configure->setEnabled(false);
}

void hxq::OnAbout()
{
	AboutDialog Dlg(this);
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
		ImageName = ImageName.toStdString().substr(i + 1, ImageName.length() - 1).c_str();


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

		statusBar()->showMessage(QString(G2U("单次检测图像: ")) + ImageName);
	}
	else
	{
		if (path.length() != 0 &&
			(path.contains(".jpg") || path.contains(".tiff") || path.contains(".bmp")))
		{
			ReadImage(&m_Image, path.toLocal8Bit().constData());
			m_bIsDisOneImage = true;
			DispPic(m_Image, LeftView);
			statusBar()->showMessage(path);
		}
	}


}

void hxq::OnDisplayAreaCamera()
{
	QVariant ExposureValue,AreaCameraId;
	/**	海康面阵相机	*/
	//ReadConfigure("config.ini", "TopCamera", "Id", AreaCameraId);
	//m_camera_thread_top = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, AreaCameraId.toString(), this);
	//m_camera_thread_top->setSaveImageDirName("Area");
	//m_camera_thread_top->setSaveImageNum(50);
	//ReadConfigure("config.ini", "TopCamera", "Exposure", ExposureValue);
	//m_camera_thread_top->setExposureTime(ExposureValue.toFloat());
	//m_camera_thread_top->setAcquisitionMode("Continuous");
	//m_camera_thread_top->setTriggerMode("Off");
	//connect(m_camera_thread_top, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_camera_thread_top, SIGNAL(signal_bool(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	//connect(m_camera_thread_top, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	////connect(m_camera_thread_top, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	//connect(m_camera_thread_top, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImageAndHandle(void*)));	//左视图显示
	//connect(m_camera_thread_top, SIGNAL(finished()), m_camera_thread_top, SLOT(deleteLater()));
	//m_camera_thread_top->start();
	
	ReadConfigure("config.ini", "TopCamera", "Id", AreaCameraId);
	//m_HikVisionCameraThread = new Halcon_Camera_Thread(AreaCameraId.toString(), this);
	m_HikVisionCameraThread = new Halcon_Camera_Thread("[0] USB3_CMOS_8.8M(1)", this);
	m_HikVisionCameraThread->setSaveImageDirName("Images/Top/Good/");
	m_HikVisionCameraThread->setSaveImageNum(10);
	m_HikVisionCameraThread->setSaveImage(false);
	m_HikVisionCameraThread->setMutexTrigger(false);
	//emit OpenCameraSinal(m_pGrabber, &isCorretOpen);
	connect(m_HikVisionCameraThread, SIGNAL(OpenCameraSinal(void**,int*)), this, SLOT(OpenTestCamera(void**, int*)),Qt::DirectConnection);
	connect(m_HikVisionCameraThread, SIGNAL(CameraErrorInformation(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_HikVisionCameraThread, SIGNAL(CameraErrorInformation(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	//connect(m_camera_top, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	//connect(m_camera_thread_top, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_HikVisionCameraThread, SIGNAL(sendImage(void*)), this, SLOT(receiveLeftImage(void*)));	//左视图显示
	connect(m_HikVisionCameraThread, SIGNAL(finished()), m_HikVisionCameraThread, SLOT(deleteLater()));
	m_HikVisionCameraThread->start();



}

void hxq::OpenCamera(void** pGrabber, int* isCorrectOpen,QString& xmlPath,QString& nodeName)
{

	HFramegrabber* Grabber = new HFramegrabber;

	try {

		std::vector <std::pair<std::pair<QString, QString>, QString>> Param;
		if (!ParserXmlNode(xmlPath, nodeName, Param))
		{
			*isCorrectOpen = Halcon_Camera_Thread::Code::XmlError;
			*pGrabber = Grabber;
			return;
		}

		Grabber->OpenFramegrabber(Param[1].second.toStdString().c_str(), 1, 1, 0, 0, 0, 0, "default", 8, \
			Param[2].second.toStdString().c_str(), -1, "false", "default", \
			Param[0].second.toStdString().c_str(), 0, -1);

		ParserCamParamAndSetFramerabber(Grabber, Param);

		*isCorrectOpen = Halcon_Camera_Thread::Code::Good;
		*pGrabber = Grabber;
	}
	catch (HException& e)
	{
		*isCorrectOpen = Halcon_Camera_Thread::Code::ParamError;
		*pGrabber = Grabber;
	}

}

//顶部相机设置
void hxq::OpenTestCamera(void** pGrabber, int* isCorrectOpen)
{
	OpenCamera(pGrabber, isCorrectOpen, QString(CameraXML), QString("DoThink"));

}

//顶部相机设置
void hxq::OpenTopCamera(void** pGrabber, bool* isCorrectOpen)
{
	
	HFramegrabber* Grabber = new HFramegrabber;
	try {

		std::vector <std::pair<std::pair<QString, QString>, QString>> Param;
		if (!ParserXmlNode(QString(CameraXML), QString(Node_TopCamera), Param))
		{
			*isCorrectOpen = false;
			*pGrabber = Grabber;
			return;
		}
	
		Grabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", \
			Param[0].second.toStdString().c_str(), 0, -1);

		ParserCamParamAndSetFramerabber(Grabber, Param);

	/*	Grabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", \
			HikVision[0].first.toStdString().c_str(), 0, -1);
		Grabber->SetFramegrabberParam("AcquisitionMode", "Continuous");
		Grabber->SetFramegrabberParam("TriggerSelector", "FrameBurstStart");
		Grabber->SetFramegrabberParam("TriggerMode", "Off");
		Grabber->SetFramegrabberParam("ExposureTime", 400000.0);
		Grabber->SetFramegrabberParam("grab_timeout", 5000);*/

		*isCorrectOpen = true;
		*pGrabber = Grabber;
	}
	catch (HException& e)
	{
		*isCorrectOpen = false;
		*pGrabber = Grabber;
	}

}

//侧部相机设置
void hxq::OpenSideCamera(void** pGrabber, bool* isCorrectOpen)
{

	HFramegrabber* Grabber = new HFramegrabber;
	try {

		std::vector <std::pair<std::pair<QString, QString>, QString>> Param;
		if (!ParserXmlNode(QString(CameraXML), QString(Node_SideCamera), Param))
		{
			*isCorrectOpen = false;
			*pGrabber = Grabber;
			return;

		}
		Grabber->OpenFramegrabber("GigEVision2", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
			Param[0].second.toStdString().c_str(), 0, -1);
		
		/*Grabber->SetFramegrabberParam("AcquisitionLineRate", 8000);
		Grabber->SetFramegrabberParam("ExposureTime", 350);
		Grabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
		Grabber->SetFramegrabberParam("TriggerMode", "Off");
		Grabber->SetFramegrabberParam("Height", 8000);
		Grabber->SetFramegrabberParam("grab_timeout", 5000);*/

		*isCorrectOpen = true;
		*pGrabber = Grabber;
	}
	catch (HException& e)
	{
		*isCorrectOpen = false;
		*pGrabber = Grabber;
	}

}

void hxq::OnOpenCameraIsCorrect(bool enable)
{
	if (enable)
	{
		ui.OnStop->setEnabled(true);
		ui.AreaCamera->setEnabled(false);
	}
}

//OffLine处理图像
void hxq::OnOneHandle_AllPic()
{
	//readAllModelFromIni();

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
		reply = QMessageBox::question(this, G2U("系统"), G2U("确认退出系统"));
		if (reply == QMessageBox::StandardButton::Yes)
		{
			//OnClearCameraThread();
			qApp->quit();
		}
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("提示"), G2U("请先停止系统,请点击左侧的'停止''按钮"));

	}

}

void hxq::OnClearCameraThread()
{
	//if (m_camera_thread_7_Clock)
	//	m_camera_thread_7_Clock->stop();

	//if (m_camera_thread_top)
	//	m_camera_thread_top->stop();

	//if (m_Pylon_camera_thread_2_Clock)
	//	m_Pylon_camera_thread_2_Clock->stop();

	//if (m_Pylon_camera_thread_10_Clock)
	//	m_Pylon_camera_thread_10_Clock->stop();

	if (m_HikVisionCameraThread)
		m_HikVisionCameraThread->setStopStatus(true);

	Sleep(20);
	condition_Camera.wakeAll();

	if (Halcon_Camera_Thread::IsExistCameraId(TopAreaCamera))
	{
		if (m_HikVisionCameraThread->isRunning())
		{
			m_HikVisionCameraThread->setStopStatus(true);
			m_HikVisionCameraThread->wait();
		}
	}

	//if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa_7_Clock))
	//{
	//	if (m_camera_thread_7_Clock->isRunning())
	//	{
	//		m_camera_thread_7_Clock->stop();
	//		m_camera_thread_7_Clock->wait();
	//	}
	//}

	//if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa_11_Clock))
	//{
	//	if (m_camera_thread_top->isRunning())
	//	{
	//		m_camera_thread_top->stop();
	//		m_camera_thread_top->wait();
	//	}
	//}

	//if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler_2_Clock))
	//{
	//	if (m_Pylon_camera_thread_2_Clock->isRunning())
	//	{
	//		m_Pylon_camera_thread_2_Clock->stop();
	//		m_Pylon_camera_thread_2_Clock->wait();
	//	}
	//}

	//if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler_10_Clock))
	//{
	//	if (m_Pylon_camera_thread_10_Clock->isRunning())
	//	{
	//		m_Pylon_camera_thread_10_Clock->stop();
	//		m_Pylon_camera_thread_10_Clock->wait();
	//	}
	//}
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

//对应串口的读取槽函数
void hxq::receiveTriggerSinal(QByteArray str)
{
	//qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str, 500);
	static int num = 0;
	if (!str.isEmpty())
	{
		std::string function_code = str.toStdString().substr(3, 2);
		if (function_code == "01")
		{
			std::vector<bool> m_Y_States = Parse_Delta_Ascii(str.toStdString());
			if (m_Y_States.size() == 8)
			{
				bool isStartGrab = m_Y_States[1];	//读取Y61的状态,On表示视觉到位
				if (isStartGrab
					&& m_peviousProductDectectEnd
					//&& m_Pylon_camera_thread_10_Clock->ReadyWake()
					&& m_Pylon_camera_thread_2_Clock->ReadyWake()
					&& m_camera_thread_7_Clock->ReadyWake()
					&& m_camera_thread_top->ReadyWake())
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
		m_leftPicViewgeometry = ui.LeftPicView->geometry();

		SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);	

	}
	else
	{
		if (m_bIsDisOneImage)
		{
			HSCROLL_HEIGHT_LeftPic(500);
			ui.LeftPicView->setGeometry(m_leftPicViewgeometry);
			ClearWindow(*pWindowHandle);
			CloseWindow(*pWindowHandle);
			SetOpenWindowHandle(Image, pWindowHandle, location);
			m_bIsDisOneImage = false;
		}
		

		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);
	}

}

//指定显示窗口的句柄
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
	{
		int pic_width = Image.Width().I();
		int pic_height = Image.Height().I();
		int width = ui.LeftPicView->geometry().width();
		int height = ui.LeftPicView->geometry().height();

		float width_scale = (float)width / (float)pic_width;
		float height_scale = (float)height / (float)pic_height;

		if(width_scale<1 && height_scale<1)
		{ 
			if (width_scale < height_scale)
			{
				int dis_pic_height = width_scale*pic_height;
				int offsetHeight = (height - dis_pic_height) / 2;
				//CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.LeftPicView->winId(), pWindowHandle);
				CHH::dev_open_window_fit_image(Image, offsetHeight, 0, width, 2000, (long)ui.LeftPicView->winId(), pWindowHandle);
			}
			else
			{
				int dis_pic_width = height_scale*pic_width;
				int offsetWidth = (height - dis_pic_width) / 2;
				//CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.LeftPicView->winId(), pWindowHandle);
				CHH::dev_open_window_fit_image(Image, 0, offsetWidth, 500, height, (long)ui.LeftPicView->winId(), pWindowHandle);
			}
		}
		else
		{
			
			int offsetWidth = (width - pic_width) / 2;
			int offsetHeight = (height - pic_height) / 2;
				//CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.LeftPicView->winId(), pWindowHandle);
				CHH::dev_open_window_fit_image(Image, offsetHeight, offsetWidth, width, height, (long)ui.LeftPicView->winId(), pWindowHandle);		
		}
		
	}
		
		break;

	case MiddleView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 2000, (long)ui.MiddlePicView->winId(), pWindowHandle);
		break;
	}
}

//指定视图的滚动条长度
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

//启动
void hxq::OnStart()
{
	QVariant ModeValue, Ip;
	ReadConfigure("config.ini", "Mode", "Status", ModeValue);
	ReadConfigure("config.ini", "MotionCard", "Ip", Ip);
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

		///****/
		if (!m_Galil)
		{
			m_Galil = new Galil_Thread(this);
		}
		connect(m_Galil, SIGNAL(finished()), m_Galil, SLOT(deleteLater()));
		//connect(m_Galil, SIGNAL(triggerSinal()), m_Galil, SLOT(OnWakeCamera()));
		m_Galil->GcLibVersion();
		if (m_Galil->Open(Ip.toString() + ""))
		{
			m_Galil->start();
		}
		else
		{
			genErrorDialog(G2U("控制卡连接错误！"));
			delete m_Galil;
			m_Galil = nullptr;
			return;
		}
	}
	else
	{
		//OnOpenCameras();
	}

	//readAllModelFromIni();
	ui.OnLineRun->setEnabled(false);
	ui.OnStop->setEnabled(true);

}

void hxq::OnOpenCameras()
{
	QVariant ExposureValue;
	/*-----Halcon Version------------*/
	/**	海康面阵相机	*/
	m_camera_thread_top = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, LineCameraId_Dalsa_11_Clock, this);
	m_camera_thread_top->setSaveImageDirName("Images/Camera1/Good/");
	m_camera_thread_top->setSaveImageNum(999);
	ReadConfigure("config.ini", "Camera_11_Clock", "Exposure", ExposureValue);
	m_camera_thread_top->setExposureTime(ExposureValue.toFloat());
	//m_camera_thread_top->SetAcquisitionLineRate(10000.0);
	//m_camera_thread_top->SetHeight(10000);
	connect(m_camera_thread_top, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_camera_thread_top, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	connect(m_camera_thread_top, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_camera_thread_top, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));	//左视图显示
	connect(m_camera_thread_top, SIGNAL(finished()), m_camera_thread_top, SLOT(deleteLater()));
	m_camera_thread_top->start();

	/**	7点方向DALSA线扫	*/
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
	//connect(m_camera_thread_7_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImage(void*)));	//中视图显示
	//connect(m_camera_thread_7_Clock, SIGNAL(finished()), m_camera_thread_7_Clock, SLOT(deleteLater()));
	//m_camera_thread_7_Clock->start();


	///*-----Pylon Version------------*/
	/**	Basler线扫	*/
	m_Pylon_camera_thread_10_Clock = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler_10_Clock, this);
	m_Pylon_camera_thread_10_Clock->setSaveImageDirName("Camera3");
	m_Pylon_camera_thread_10_Clock->setSaveImageNum(100);
	ReadConfigure("config.ini", "Camera_10_Clock", "Exposure", ExposureValue);
	m_Pylon_camera_thread_10_Clock->SetExposureTime(ExposureValue.toInt());
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveSecondRightImage(void*)));	//右二视图显示
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(finished()), m_Pylon_camera_thread_10_Clock, SLOT(deleteLater()));
	m_Pylon_camera_thread_10_Clock->start();

	/**	2点方向Basler线扫	*/
	//m_Pylon_camera_thread_2_Clock = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler_2_Clock, this);
	//m_Pylon_camera_thread_2_Clock->setSaveImageDirName("Camera4");
	//m_Pylon_camera_thread_2_Clock->setSaveImageNum(999);
	//ReadConfigure("config.ini", "Camera_02_Clock", "Exposure", ExposureValue);
	//m_Pylon_camera_thread_2_Clock->SetExposureTime(ExposureValue.toInt());
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(signal_error(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(ReadyOk(int)), this, SLOT(OnReadyOk(int)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveRightImage(void*)));	//最右视图显示
	//connect(m_Pylon_camera_thread_2_Clock, SIGNAL(finished()), m_Pylon_camera_thread_2_Clock, SLOT(deleteLater()));
	//m_Pylon_camera_thread_2_Clock->start();

	ui.OnLineRun->setEnabled(false);

}

//准备工作完成,可以启动电气设备等待PLC的触发信号或者手动触发
void hxq::OnReadyOk(int num)
{
	static int total = 0;
	total += num;
	if (total == 3)
	{
		QMessageBox::StandardButton reply;
		if (m_bIsOnLine)
		{
			reply = QMessageBox::information(this, G2U("信息"), G2U("采集设备已就位,等待检测产品...\r\n		点击'OK'"));
		}
		else
		{
			reply = QMessageBox::information(this, G2U("信息"), G2U("手动模式,请手动唤醒相机...\r\n		点击'OK'"));
		}


		total = 0;
		/* Button State */
		ui.OnStop->setEnabled(true);
	}
}

//停止
void hxq::OnStop()
{
	if (m_Galil)
	{
		m_Galil->stop();
	}

	OnClearCameraThread();

	m_Result_AllQueue;
	ui.OnStop->setEnabled(false);
	ui.OnLineRun->setEnabled(true);
	ui.AreaCamera->setEnabled(true);


}

//检测完成信号
void hxq::OnDetectEnd()
{
	if (Delta_Thread::GetSerialPort())
	{
		Delta_Thread::AddOneQueueInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueueInfo(DETECT_END_OFF);
	}
}


//******对应视图显示图片
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

//******对应视图显示图片,并处理图片
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

//分发至图像处理线程
void hxq::OnHandleImageThread(HImage& ima, LocationView view)
{
	switch (view)
	{
	case LeftView:
	{
		PicThreadLeft* pPicThread = new PicThreadLeft(this);
		pPicThread->m_Image = ima;
		pPicThread->setCameraId(TopCamera);
		pPicThread->m_WindowHandle = GetViewWindowHandle(view);
		//pPicThread->setModel(gbModel());
		connect(pPicThread, SIGNAL(resultReady(int,int)), this, SLOT(OnHandleResults(int,int)));
		connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
		pPicThread->start();
	}

	break;

	case MiddleView:
	{
		PicThreadMiddle* pPicThread = new PicThreadMiddle(this);
		pPicThread->m_Image = ima;
		pPicThread->setCameraId(SideCamera);
		pPicThread->m_WindowHandle = GetViewWindowHandle(view);
		connect(pPicThread, SIGNAL(resultReady(int,int)), this, SLOT(OnHandleResults(int,int)));
		connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
		pPicThread->start();
	}
	break;

	//case SecondRightView:
	//{
	//	PicThreadSecondRight* pPicThread = new PicThreadSecondRight(this);
	//	pPicThread->m_Image = ima;
	//	pPicThread->m_WindowHandle = GetViewWindowHandle(view);
	//	connect(pPicThread, SIGNAL(resultReady(int,int)), this, SLOT(OnHandleResults(int,int)));
	//	connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
	//	pPicThread->start();
	//}
	//break;

	//case RightView:
	//{
	//	PicThreadRight* pPicThread = new PicThreadRight(this);
	//	pPicThread->m_Image = ima;
	//	//pPicThread->setModel(bottomModel());
	//	pPicThread->m_WindowHandle = GetViewWindowHandle(view);
	//	connect(pPicThread, SIGNAL(resultReady(int,int)), this, SLOT(OnHandleResults(int,int)));
	//	connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
	//	pPicThread->start();
	//}
	//break;

	/*default:
	break;*/
	}
}


//生成错误对话提示框
void hxq::genErrorDialog(QString error)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::warning(this, G2U("错误"), error);
}




//收到正确图片数,检测完成
void hxq::receiveCorrectImage(int value)
{
	static int imageNum = 0;
	imageNum += value;

	if (imageNum == 3)  //收到1张图片
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
	//if (m_bIsOnLine)
	//{
	//	OnDetectEnd();
	//}
	//else
	//{
	//	if (m_peviousProductDectectEnd)
	//	{
	//		OnWakeCamera();
	//	}
	//	else
	//	{
	//		qDebug() << "m_peviousProductDectectEnd = false.";
	//	}

	//}

	OnWakeCamera();
}


//得到线程的信号,并判断检测结果,发送给控制卡
void hxq::OnHandleResults(int singleResult, int cameraId)
{
	static int sum = 0;
	sum++;

	m_Pic_Set.insert(cameraId);
	qDebug() << "receive singleResult:   " << singleResult;
/*
	if (cameraId == TopCamera)
	{
		m_detectResult.current_area = m_detectResult.next_area;
		m_detectResult.next_area = singleResult;
	}
	else if (cameraId == SideCamera)
	{
		m_detectResult.current_line = singleResult;
	}


	if ((m_Pic_Set.count(TopCamera)) 
		&& (m_Pic_Set.count(SideCamera)))
	{
		qDebug() << "acquice 2 pic....";
		m_Pic_Set.clear();

		qDebug() << "Image num:	" << ++m_total;

		if (!(m_detectResult.current_area + m_detectResult.current_line))
		{


			m_good++;
			if (m_bIsOnLine)
			{
				if (m_Galil)
					m_Galil->Cmd("SB0");

			}
			qDebug() << "Send Good!!!	";
			ui.lcdNumber_good->display(m_good);

		}
		else
		{
			//Sleep(30);
			if (m_bIsOnLine)
			{
				if (m_Galil)
					m_Galil->Cmd("SB1");
			}

			qDebug() << "Send Bad!!!	";
			if (m_detectResult.current_area == Gou || m_detectResult.current_line == Gou)
			{
				ui.lcdNumber_cao->display(++m_gou);
			}
			else if (m_detectResult.current_area == Cao || m_detectResult.current_line == Cao)
			{
				ui.lcdNumber_cao->display(++m_cao);
			}
			else if (m_detectResult.current_area == Liantong || m_detectResult.current_area == Liantong)
			{
				ui.lcdNumber_liantong->display(++m_liantong);
			}

		}
		
		m_peviousProductDectectEnd = true;

		return;
	}
	*/
}


//实时设置曝光
void hxq::OnSetExposure()
{



}




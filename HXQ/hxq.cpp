#include "hxq.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QThread>
#include "CHH.h"
#include "aboutdialog.h"
#include "EngineerDialog.h"
#include "DeltaThread.h"
#include "Global.h"
#include "ConfigureDialog.h"
#include "PicThreadMiddle.h"
#include "PicThreadLeft.h"
#include "PicThreadRight.h"
#include "PicThreadSecondRight.h"
#include <QtXml\QDomDocument>
#include <QtSql/QSqlDatabase>
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
#ifdef ENGINEER
	ui.OnOperator->setChecked(false);
	ui.OnEngineer->setChecked(true);
	OnEngineerStatus();
#endif // ENGINEER

#ifdef OPERATOR
	//初始操作员状态
	//ui.OnOperator->setChecked(true);
	//ui.OnEngineer->setChecked(false);
	//OnOperatorStatus();
#endif // OPERATOR



	m_bIsDisOneImage = false;

	//相机线程
	m_camera_thread_7_Clock = nullptr;
	m_camera_thread_top = nullptr;
	m_Pylon_camera_thread_2_Clock = nullptr;
	m_Pylon_camera_thread_10_Clock = nullptr;
	m_TopCameraThread = nullptr;

	m_Galil = nullptr;

	m_AllResult = 0;
	m_good = m_bad = m_gou = m_cao = m_liantong = m_total = 0;

	//m_peviousProductDectectEnd = true;

	ZeroMemory(&m_detectResult, sizeof(m_detectResult));
	
	process = new QProcess(this);

	m_bOneDetect = false;

	m_pTimer = nullptr;
}


// 安装定时器插入新行，并更新数据库
void hxq::installTimerToUpdateMySql()
{
	m_startTime = MySql_Now();
	if (MySql_Connect(m_sqlDatabase, "localhost", 3306, "hxq", "ycgd", "ycgd"))
	{
		MySql_Query(m_sqlDatabase, QString("INSERT INTO total (startTime) VALUES ('%1')").arg(m_startTime));
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateMySql()));
	m_pTimer->start(5000);

	
}

// 更新数据库
void hxq::updateMySql()
{
	QTime start;
	start.start();
	MySql_Query(m_sqlDatabase, QString("UPDATE total SET endTime='%1',gou=%2,cao=%3,maoci=%4,other=%5,good=%6,total=%7 WHERE startTime='%8'").
		arg(MySql_Now()).
		arg(ui.lcdNumber_gou->value()).
		arg(ui.lcdNumber_cao->value()).
		arg(ui.lcdNumber_liantong->value()).
		arg(ui.lcdNumber_bad->value()).
		arg(ui.lcdNumber_good->value()).
		arg(ui.lcdNumber_total->value()).
		arg(m_startTime));
	//qDebug() << "update new data time:	" << start.elapsed() / 1000.0 << "s";
}


// 菜单栏对应功能
void hxq::menuConnect()
{
	connect(ui.OnEngineer, &QAction::triggered, this, &hxq::OnEngineer);
	connect(ui.OnOperator, &QAction::triggered, this, &hxq::OnOperator);
	connect(ui.OnAbout, &QAction::triggered, this, &hxq::OnAbout);
}

//工具栏对应功能
void hxq::toolConnect()
{
	connect(ui.OnOpen, &QAction::triggered, this, &hxq::OnOpen);
	connect(ui.OnRun, &QAction::triggered, this, &hxq::OnOneHandle_AllPic);
	connect(ui.OnLineRun, SIGNAL(triggered()), this, SLOT(OnStart()));
	connect(ui.OnStop, SIGNAL(triggered()), this, SLOT(OnStop()));
	connect(ui.OnConfigure, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	connect(ui.OnShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.OnMotionCard, SIGNAL(triggered()), this, SLOT(OnMotionCardDebug()));
	connect(ui.OnAreaCamera, SIGNAL(triggered()), this, SLOT(OnDisplayAreaCamera()));

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
	UpdateXmlNodeText(QString(XML_Configure), QString(Node_Save), QString(Save_TopBadIndex), QString("%1").arg(g_SaveParam.SaveTopBadIndex));
	UpdateXmlNodeText(QString(XML_Configure), QString(Node_Save), QString(Save_SideBadIndex), QString("%1").arg(g_SaveParam.SaveSideBadIndex));
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
}

void hxq::OnEngineerStatus()
{
	ui.OnConfigure->setEnabled(true);
}

void hxq::OnOperatorStatus()
{
	ui.OnConfigure->setEnabled(false);
}

void hxq::OnAbout()
{
	AboutDialog Dlg(this);
	Dlg.exec();
}

void hxq::OnOpen()
{

	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files(*.jpg *.tif *.bmp)"));

	if(path.contains(g_SaveParam.SaveSideBadPath)
		||  path.contains(g_SaveParam.SaveTopBadPath))
	{
		m_bOneDetect = true;
		int i = path.lastIndexOf('/');
		QString ImageName = path.toStdString().substr(i+1, path.length() - 1).c_str();

		QString ImagePath = g_SaveParam.SaveTopBadPath + ImageName;
		if (isFileExist(ImagePath))
		{
			ReadImage(&m_LeftImage, ImagePath.toLocal8Bit().constData());
			DispPic(m_LeftImage, LeftView);
		}
		else
		{
			m_LeftImage.Clear();
		}

		ImagePath = g_SaveParam.SaveSideBadPath + ImageName;
		if (isFileExist(ImagePath))
		{
			ReadImage(&m_MiddleImage, ImagePath.toLocal8Bit().constData());
			DispPic(m_MiddleImage, MiddleView);
		}
		else
		{
			m_MiddleImage.Clear();
		}
			

		statusBar()->showMessage(QString(G2U("单次检测图像: ")) + path);

		
	}
	else if (path.contains(g_SaveParam.SaveSideAllPath)
		|| path.contains(g_SaveParam.SaveTopAllPath))
	{
		m_bOneDetect = true;
		int i = path.lastIndexOf('/');
		QString ImageName = path.toStdString().substr(i, path.length() - 1).c_str();

		QString ImagePath = g_SaveParam.SaveTopAllPath + ImageName;
		ReadImage(&m_LeftImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_LeftImage, LeftView);

		ImagePath = g_SaveParam.SaveSideAllPath + ImageName;
		ReadImage(&m_MiddleImage, ImagePath.toLocal8Bit().constData());
		DispPic(m_MiddleImage, MiddleView);

		statusBar()->showMessage(QString(G2U("单次检测图像: ")) + path);

		
	}
	else
	{
		if (path.length() != 0 &&
			(path.contains(".jpg") || path.contains(".tif") || path.contains(".bmp")))
		{
			ReadImage(&m_LeftImage, path.toLocal8Bit().constData());
			m_bIsDisOneImage = true;
			DispPic(m_LeftImage, LeftView);
			statusBar()->showMessage(path);

		}
	}


}

void hxq::OnDisplayAreaCamera()
{

	m_TopCameraThread = new Halcon_Camera_Thread(QString(Camera_Top), this);
	m_TopCameraThread->setSaveImage(false);
	m_TopCameraThread->setMutexTrigger(false);
	connect(m_TopCameraThread, SIGNAL(OpenCameraSinal(void**,QString,int*)), this, SLOT(OpenPreCamera(void**, QString,int*)),Qt::DirectConnection);
	connect(m_TopCameraThread, SIGNAL(CameraErrorInformation(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_TopCameraThread, SIGNAL(CameraErrorInformation(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	connect(m_TopCameraThread, SIGNAL(sendImage(void*)), this, SLOT(receiveLeftImage(void*)));	//左视图显示
	connect(m_TopCameraThread, SIGNAL(finished()), m_TopCameraThread, SLOT(deleteLater()));
	m_TopCameraThread->start();

	m_Pylon_camera_thread_10_Clock = new PylonCamera_Thread(QString(Camera_Side), this);
	m_Pylon_camera_thread_10_Clock->setSaveImage(false);
	m_Pylon_camera_thread_10_Clock->setMutexTrigger(false);
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(CameraErrorInformation(QString)), this, SLOT(genErrorDialog(QString)));
	//connect(m_Pylon_camera_thread_10_Clock, SIGNAL(CameraErrorInformation(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	//connect(m_Pylon_camera_thread_10_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImage(void*)));	//中视图显示
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(finished()), m_Pylon_camera_thread_10_Clock, SLOT(deleteLater()));
	m_Pylon_camera_thread_10_Clock->start();


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

		*isCorrectOpen = Halcon_Camera_Thread::Code::Ok;
		*pGrabber = Grabber;
	}
	catch (HException& e)
	{
		*isCorrectOpen = Halcon_Camera_Thread::Code::ParamError;
		*pGrabber = Grabber;
	}

}

//相机设置
void hxq::OpenPreCamera(void** pGrabber, QString nodeCameraName,int* isCorrectOpen)
{
	QString type, topCameraName;
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), nodeCameraName, type, topCameraName);
	OpenCamera(pGrabber, isCorrectOpen, QString(XML_Camera), topCameraName);

}

void hxq::OnOpenCameraIsCorrect(bool enable)
{
	static std::vector<bool> CameraStatus;
	CameraStatus.push_back(enable);

	if (CameraStatus.size() == 2)
	{
		if (CameraStatus[0] && CameraStatus[1])
		{
			autoStartButtonStatus();
			genErrorDialog(G2U("相机已正确打开！"));

			{
				QString type, Ip, varName1, varName2;

				ReadXmlElementText(QString(XML_MotionCard), QString(Node_MotionCard), QString(MotionCard_ip), type, Ip);
				//ReadXmlElementText(QString(XML_MotionCard), QString(Node_MotionCard), QString("varName1"), type, varName1);
				//ReadXmlElementText(QString(XML_MotionCard), QString(Node_MotionCard), QString("varName2"), type, varName2);

				///****/
				if (!m_Galil)
				{
					m_Galil = new Galil_Thread(this);
				}

				connect(m_Galil, SIGNAL(triggerSinal()), this, SLOT(OnWakeCamera()), Qt::DirectConnection);
				connect(m_Galil, SIGNAL(finished()), m_Galil, SLOT(deleteLater()));

				if (m_Galil->Open(Ip + ""))
				{
					genErrorDialog(G2U("控制卡连接成功！"));
					m_Galil->Cmd(LIGHT_CLOSE);
					Sleep(50);
					m_Galil->Cmd(AUTOSTOP);
					Sleep(100);
					m_Galil->Cmd(AUTOSTART);

					/*std::queue<bool> empty;
					swap(empty, g_Result_Queue);
					g_Result_Queue.push(true);*/

					m_Galil->Cmd(CLASSIFIY_BAD);

					m_Galil->start();

					OnLcdDispalyReset();

					installTimerToUpdateMySql();

					PicThreadLeft::num = 0;
					PicThreadMiddle::num = 0;
				
				}
				else
				{
					genErrorDialog(G2U("控制卡连接错误！"));
					delete m_Galil;
					m_Galil = nullptr;
					OnStop();
					CameraStatus.clear();
					return;
				}

				AllButtonFalse();
				ui.OnStop->setEnabled(true);
			}

		}
		else
		{
			//genErrorDialog(G2U("相机未正确打开！"));
			OnStop();
			//normalButtonStatus();
			
		}	
		CameraStatus.clear();
	}
}

//OffLine处理图像
void hxq::OnOneHandle_AllPic()
{
	//readAllModelFromIni();

	if (m_LeftImage.Key() != 0)
	{
		//DispPic(m_LeftImage, LeftView);
		OnHandleImageThread(m_LeftImage, LeftView);
	}


	if (m_MiddleImage.Key() != 0)
	{
		//DispPic(m_MiddleImage, MiddleView);
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
	QString topCameraName, sideCameraName, topCameraId, sideCameraId, type;
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Top), type, topCameraName);
	ReadXmlElementText(QString(XML_Configure), QString(Node_Camera), QString(Camera_Side), type, sideCameraName);

	ReadXmlElementText(QString(XML_Camera), topCameraName, QString("id"), type, topCameraId);
	ReadXmlElementText(QString(XML_Camera), sideCameraName, QString("id"), type, sideCameraId);

	if (m_TopCameraThread)
		m_TopCameraThread->setStopStatus(true);

	//if (m_SideCameraThread)
	//	m_SideCameraThread->setStopStatus(true);

	if (m_Pylon_camera_thread_10_Clock)
		m_Pylon_camera_thread_10_Clock->setStopStatus(true);

	Sleep(20);
	g_condition_Camera.wakeAll();

	if (Halcon_Camera_Thread::IsExistCameraId(topCameraId))
	{
		if (m_TopCameraThread->isRunning())
		{
			m_TopCameraThread->setStopStatus(true);
			m_TopCameraThread->wait();
		}
	}

	//if (Camera_Thread::IsExistCameraId(sideCameraId))
	//{
	//	if (m_SideCameraThread->isRunning())
	//	{
	//		m_SideCameraThread->setStopStatus(true);
	//		m_SideCameraThread->wait();
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

	if (PylonCamera_Thread::IsExistCameraId(sideCameraId))
	{
		if (m_Pylon_camera_thread_10_Clock->isRunning())
		{
			m_Pylon_camera_thread_10_Clock->setStopStatus(true);
			m_Pylon_camera_thread_10_Clock->wait();
		}
	}
}

void hxq::OnConfigure()
{
	ConfigureDialog Dlg(this);
	Dlg.exec();
}

void hxq::OnLineRun()
{
	
}

//对应串口的读取槽函数
void hxq::receiveTriggerSinal(QByteArray str)
{
	////qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	//statusBar()->showMessage(str, 500);
	//static int num = 0;
	//if (!str.isEmpty())
	//{
	//	std::string function_code = str.toStdString().substr(3, 2);
	//	if (function_code == "01")
	//	{
	//		std::vector<bool> m_Y_States = Parse_Delta_Ascii(str.toStdString());
	//		if (m_Y_States.size() == 8)
	//		{
	//			bool isStartGrab = m_Y_States[1];	//读取Y61的状态,On表示视觉到位
	//			if (isStartGrab
	//				&& m_peviousProductDectectEnd
	//				//&& m_Pylon_camera_thread_10_Clock->ReadyWake()
	//				&& m_Pylon_camera_thread_2_Clock->ReadyWake()
	//				&& m_camera_thread_7_Clock->ReadyWake()
	//				&& m_camera_thread_top->ReadyWake())
	//			{

	//				//Delta_Thread::AddOneQueueInfo(RESET_Y61);
	//				Sleep(20);
	//				m_peviousProductDectectEnd = false;
	//				OnWakeCamera();
	//				qDebug() << "receive Y61: " << ++num;

	//			}
	//		}
	//	}

	//}

}


void hxq::OnWakeCamera()
{
	Sleep(10);
	g_mutex_Camera.lock();
	g_condition_Camera.wakeAll();
	g_mutex_Camera.unlock();
	
	//Sleep(10);
	//g_mutex_Camera.lock();
	//g_condition_Camera.wakeAll();
	//g_mutex_Camera.unlock();
}

void hxq::OnModToRight()
{
	if (m_Galil)
	{
		m_Galil->Cmd("SB4");
	}
		
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
		//ClearWindow(*pWindowHandle);
		//CloseWindow(*pWindowHandle);
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

		if(width_scale<1 || height_scale<1)
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
	m_bOneDetect = false;
	AllButtonFalse();
	OnOpenCameras();
	//OnOpenCameraIsCorrect()   打开控制卡线程
}

void hxq:: AllButtonFalse()
{
	ui.OnOpen->setEnabled(false);
	ui.OnRun->setEnabled(false);

	ui.OnAreaCamera->setEnabled(false);
	ui.OnMotionCard->setEnabled(false);
	ui.OnConfigure->setEnabled(false);

	ui.OnLineRun->setEnabled(false);
	ui.OnStop->setEnabled(false);

	ui.OnShutDown->setEnabled(false);
}

void hxq::autoStartButtonStatus()
{
	ui.OnOpen->setEnabled(false);
	ui.OnRun->setEnabled(false);

	ui.OnAreaCamera->setEnabled(false);
	ui.OnMotionCard->setEnabled(false);
	ui.OnConfigure->setEnabled(false);

	ui.OnLineRun->setEnabled(false); 
	ui.OnStop->setEnabled(true);

	ui.OnShutDown->setEnabled(false);
}
void hxq::normalButtonStatus()
{
	ui.OnOpen->setEnabled(true);
	ui.OnRun->setEnabled(true);

	ui.OnAreaCamera->setEnabled(true);
	ui.OnMotionCard->setEnabled(true);
	ui.OnConfigure->setEnabled(true);

	ui.OnLineRun->setEnabled(true);
	ui.OnStop->setEnabled(false);

	ui.OnShutDown->setEnabled(true);
}

void hxq::OnOpenCameras()
{
	/****************************/
	m_TopCameraThread = new Halcon_Camera_Thread(QString(Camera_Top), this);
	m_TopCameraThread->setSaveImageDirName(g_SaveParam.SaveTopAllPath);
	m_TopCameraThread->setSaveImageNum(g_SaveParam.SaveTopAllNum);
	m_TopCameraThread->setSaveImage(g_SaveParam.IsSaveTopAll);
	m_TopCameraThread->setMutexTrigger(true);
	connect(m_TopCameraThread, SIGNAL(OpenCameraSinal(void**, QString,int*)), this, SLOT(OpenPreCamera(void**, QString,int*)), Qt::DirectConnection);
	connect(m_TopCameraThread, SIGNAL(CameraErrorInformation(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_TopCameraThread, SIGNAL(CameraErrorInformation(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	connect(m_TopCameraThread, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_TopCameraThread, SIGNAL(sendImage(void*)), this, SLOT(receiveLeftImageAndHandle(void*)));	//左视图显示
	connect(m_TopCameraThread, SIGNAL(finished()), m_TopCameraThread, SLOT(deleteLater()));
	m_TopCameraThread->start();


	m_Pylon_camera_thread_10_Clock = new PylonCamera_Thread(QString(Camera_Side), this);
	m_Pylon_camera_thread_10_Clock->setSaveImageDirName(g_SaveParam.SaveSideAllPath);
	m_Pylon_camera_thread_10_Clock->setSaveImageNum(g_SaveParam.SaveSideAllNum);
	m_Pylon_camera_thread_10_Clock->setSaveImage(g_SaveParam.IsSaveSideAll);
	m_Pylon_camera_thread_10_Clock->setMutexTrigger(true);
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(CameraErrorInformation(QString)), this, SLOT(genErrorDialog(QString)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(CameraErrorInformation(bool)), this, SLOT(OnOpenCameraIsCorrect(bool)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImageAndHandle(void*)));	//中视图显示
	connect(m_Pylon_camera_thread_10_Clock, SIGNAL(finished()), m_Pylon_camera_thread_10_Clock, SLOT(deleteLater()));
	m_Pylon_camera_thread_10_Clock->start();

}

//停止
void hxq::OnStop()
{
	if (m_Galil && m_Galil->isRunning())
	{
		m_Galil->Cmd(AUTOSTOP);
		m_Galil->stop();
		m_Galil->wait();
		m_Galil = nullptr;
	}

	OnClearCameraThread();

	//m_Result_AllQueue;
	normalButtonStatus();

	UpdateXmlNodeText(QString(XML_Configure), QString(Node_Save), QString(Save_TopBadIndex), QString("%1").arg(g_SaveParam.SaveTopBadIndex));
	UpdateXmlNodeText(QString(XML_Configure), QString(Node_Save), QString(Save_SideBadIndex), QString("%1").arg(g_SaveParam.SaveSideBadIndex));

	if (m_pTimer && m_pTimer->isActive())
	{
		m_pTimer->stop();
		m_pTimer->deleteLater();
	}

	updateMySql();
}

//检测完成信号
void hxq::OnDetectEnd()
{
	if (Delta_Thread::GetSerialPort())
	{
		/*Delta_Thread::AddOneQueueInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueueInfo(DETECT_END_OFF);*/
	}
}


//******对应视图显示图片
void hxq::receiveLeftImage(void* image)
{
	/*HImage* ima = (HImage*)image;
	DispPic(*ima, LeftView);
	delete ima;*/

	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, LeftView);
	
}

void hxq::receiveMiddleImage(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, MiddleView);
}

void hxq::receiveSecondRightImage(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, SecondRightView);

}

void hxq::receiveRightImage(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, RightView);
}
/********/

//******对应视图显示图片,并处理图片
void hxq::receiveLeftImageAndHandle(void* image)
{
	//HImage* ima = (HImage*)image;
	//DispPic(*ima, LeftView);
	//OnHandleImageThread(*ima, LeftView);
	//delete ima;

	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, LeftView);
	OnHandleImageThread(ima, LeftView);

	
	/*m_LeftImage = *(HImage*)image;
	DispPic(m_LeftImage, LeftView);
	OnHandleImageThread(m_LeftImage, LeftView);*/

}

void hxq::receiveMiddleImageAndHandle(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, MiddleView);
	OnHandleImageThread(ima, MiddleView);

	/*m_MiddleImage = *(HImage*)image;
	DispPic(m_MiddleImage, MiddleView);
	OnHandleImageThread(m_MiddleImage, MiddleView);*/
}

void hxq::receiveSecondRightImageAndHandle(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
	DispPic(ima, SecondRightView);
	OnHandleImageThread(ima, SecondRightView);

}

void hxq::receiveRightImageAndHandle(void* image)
{
	HImage ima = *static_cast<HImage*>(image);
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


		if (!m_bOneDetect)
		{
			pPicThread->setSaveImage(true);
			pPicThread->setSaveImageDirName(g_SaveParam.SaveTopBadPath);
			pPicThread->setSaveImageNum(g_SaveParam.SaveTopBadNum);
			connect(pPicThread, SIGNAL(resultReady(int, int)), this, SLOT(OnHandleResults(int, int)));
		}
		
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

		if (!m_bOneDetect)
		{
			pPicThread->setSaveImage(true);
			pPicThread->setSaveImageDirName(g_SaveParam.SaveSideBadPath);
			pPicThread->setSaveImageNum(g_SaveParam.SaveSideBadNum);
			connect(pPicThread, SIGNAL(resultReady(int, int)), this, SLOT(OnHandleResults(int, int)));
		}
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

	if (imageNum == 2)  //收到1张图片
	{
		//m_total++;

		g_mutex_Result.lock();
		if (m_Galil)
		{
			m_Galil->Cmd(LIGHT_CLOSE);
		}
		g_mutex_Result.unlock();
		imageNum = 0;
		qDebug() << "	grabed 2 pic !" ;

		//上升沿使能
		g_UpWaveEnable = true;

		//ui.lcdNumber_total->display(m_total);
	}

}


void hxq::OnMotionCardDebug()
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
	//process->start("C:/Program Files/Galil/GalilTools/bin/GalilTools.exe", QStringList("C:/Program Files/Galil/GalilTools/bin/GalilTools.exe"));

	//process->startDetached("d:/PanDownload/PanDownload.exe", QStringList("d:/PanDownload/PanDownload.exe"));
	//QProcess::execute("d:/PanDownload/PanDownload.exe", QStringList("d:/PanDownload/PanDownload.exe"));
	
	//OnWakeCamera();


	QString type, Ip;

	///****/
	ReadXmlElementText(QString(XML_MotionCard), QString(Node_MotionCard), QString(MotionCard_ip), type, Ip);
	Galil_Thread gali;
	if(gali.Open(Ip))
		gali.Cmd(LIGHT_CLOSE);

	
		
}


//得到线程的信号,并判断检测结果,发送给控制卡
void hxq::OnHandleResults(int singleResult, int cameraId)
{
	static int sum = 0;
	sum++;

	m_Pic_Set.insert(cameraId);
	qDebug() << "receive singleResult:   " << singleResult;

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
		//qDebug() << "acquice 2 pic....";
		m_Pic_Set.clear();

		qDebug() << "Image total num:	" << ++m_total;

		if (!(m_detectResult.current_area + m_detectResult.current_line))
		{
			m_good++;

			/*g_mutex_Result.lock();
			g_Result_Queue.push(true);
			g_mutex_Result.unlock();*/

			qDebug() << "Send Good!!!	";
			m_Galil->Cmd(CLASSIFIY_GOOD);
			ui.lcdNumber_good->display(m_good);

		}
		else
		{

				/*g_mutex_Result.lock();
				g_Result_Queue.push(false);
				g_mutex_Result.unlock();*/

			qDebug() << "Send Bad!!!	";
			m_Galil->Cmd(CLASSIFIY_BAD);
			if (m_detectResult.current_area == Gou || m_detectResult.current_line == Gou)
			{
				ui.lcdNumber_gou->display(++m_gou);
				qDebug() << "Send Gou Bad!!!	";
			}
			else if (m_detectResult.current_area == Cao || m_detectResult.current_line == Cao)
			{
				ui.lcdNumber_cao->display(++m_cao);
				qDebug() << "Send Cao Bad!!!	";
			}
			else if (m_detectResult.current_area == Liantong || m_detectResult.current_line == Liantong)
			{
				ui.lcdNumber_liantong->display(++m_liantong);
				qDebug() << "Send Liangtong Bad!!!	";
			}
			else if(m_detectResult.current_line == Gou || m_detectResult.current_area == Gou)
			{
				ui.lcdNumber_gou->display(++m_gou);
				qDebug() << "Send Gou Bad!!!	";
			}
			else if (m_detectResult.current_line == Cao || m_detectResult.current_area == Cao)
			{
				ui.lcdNumber_cao->display(++m_cao);
				qDebug() << "Send Cao Bad!!!	";
			}
			else if (m_detectResult.current_line == Liantong || m_detectResult.current_area == Liantong)
			{
				ui.lcdNumber_liantong->display(++m_liantong);
				qDebug() << "Send Liangtong Bad!!!	";
			}
			else
			{
				ui.lcdNumber_bad->display(++m_bad);
				qDebug() << "Send area bad:		" << m_detectResult.current_area<< "Send line bad:	"<<m_detectResult.current_line;
			}
		}

		ui.lcdNumber_total->display(ui.lcdNumber_bad->intValue() + ui.lcdNumber_gou->intValue() + ui.lcdNumber_cao->intValue() + ui.lcdNumber_liantong->intValue() + ui.lcdNumber_good->intValue());
		
		//updateMySql();
		//m_peviousProductDectectEnd = true;

		return;
	}
	
}


//实时设置曝光
void hxq::OnSetExposure()
{



}

//统计数字复位清零
void hxq::OnLcdDispalyReset()
{
	ui.lcdNumber_bad->display(0);
	ui.lcdNumber_gou->display(0);
	ui.lcdNumber_cao->display(0);
	ui.lcdNumber_liantong->display(0);
	ui.lcdNumber_good->display(0);
	ui.lcdNumber_total->display(0);
}



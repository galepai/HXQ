#ifndef hxq_H
#define hxq_H

#include <QtWidgets/QMainWindow>
#include "ui_hxq.h"
#include "HalconCpp.h"
#include "HalconCpp.h"
#include "Func.h"
#include "TestDialog.h"
#include "CameraThread.h"
#include "PylonCameraThread.h"
#include <queue>


using namespace HalconCpp;

class hxq : public QMainWindow
{
	Q_OBJECT

public:
	hxq(QWidget *parent = 0);
	~hxq();

	//bool getIsBadStatu(){ return m_AllResult; };
	bool OpenSerial();

	void OnWakeCamera();
	void ReadExposure();
	void setBottomModel(QString path){ m_BottomModel = path; }
	const QString& bottomModel(){return m_BottomModel;}
	void setGbModel(QString path){ m_GBModel = path; }
	const QString& gbModel(){ return m_GBModel; }
	void readAllModelFromIni();
	void addResultQueue();

	//*********图像处理线程**************/
	void OnHandleImageThread(HImage& ima, LocationView view);

public slots:

	/***********菜单栏功能************/
	void OnLRC();
	void OnAbout();

	/***********工具栏功能************/
	void OnOneHandle_AllPic();
	void OnOpen();
	void OnLineRun();
	void OnConfigure();
	void OnShutDown();	//关闭系统
	void OnTest();

	/***********右端电气功能************/
	void DebugDialog();
	void OnStart();
	void OnStop();
	void OnDetectEnd();

	//
	void handleResults(int is_bad);
	void receiveSerialData(QByteArray str);
	void receiveLeftImage(void* image);
	void receiveMiddleImage(void* image);
	void receiveSecondRightImage(void* image);
	void receiveRightImage(void* image);
	//生成错误对话提示框
	void genErrorDialog(QString error);
	void receiveCorrectImage(int value);
	void OnReadyOk(int num);

	//
	void OnSetExposure();

private:
	void OnClearCameraThread();	//清理相机线程
	void FullScreenShow();	//全屏显示
	void DispPic(HImage& path, LocationView location);	//显示图片在指定位置视图
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//返回指定视图的窗口Handle（Halcon显示图片用）
	void SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location);	//指定显示窗口的句柄
	void SetPicViewScroll(int width, int height, LocationView location); //指定视图的滚动条长度
	void OnOpenCameras();	//打开相机


private:
	Ui::hxqClass ui;
	HalconCpp::HTuple m_LeftWindowHandle, m_MiddleWindowHandle, m_SecondRightWindowHandle, m_RightWindowHandle;	//左、中、右二、右视图的窗口Handle（Halcon显示图片用）
	QString m_Title;
	HImage m_Image, m_LeftImage, m_MiddleImage, m_SecondRightImage, m_RightImage;
	int m_AllResult;
	int m_good, m_bad, m_total;
	bool m_peviousProductDectectEnd;
	bool m_bIsOnLine; //打开相机时,选择是否与PLC联机
	QString m_BottomModel;
	QString m_GBModel;
	std::queue<int> m_Result_Queue;
	std::queue<int> m_Result_AllQueue;

signals:
	void ReadyLoop();

public:
	Camera_Thread* m_camera_thread_7_Clock;
	Camera_Thread* m_camera_thread_11_Clock;
	
	PylonCamera_Thread* m_Pylon_camera_thread_2_Clock;
	PylonCamera_Thread* m_Pylon_camera_thread_10_Clock;

};

#endif // hxq_H

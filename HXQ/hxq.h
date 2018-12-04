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

	//*********ͼ�����߳�**************/
	void OnHandleImageThread(HImage& ima, LocationView view);

public slots:

	/***********�˵�������************/
	void OnLRC();
	void OnAbout();

	/***********����������************/
	void OnOneHandle_AllPic();
	void OnOpen();
	void OnLineRun();
	void OnConfigure();
	void OnShutDown();	//�ر�ϵͳ
	void OnTest();

	/***********�Ҷ˵�������************/
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
	//���ɴ���Ի���ʾ��
	void genErrorDialog(QString error);
	void receiveCorrectImage(int value);
	void OnReadyOk(int num);

	//
	void OnSetExposure();

private:
	void OnClearCameraThread();	//��������߳�
	void FullScreenShow();	//ȫ����ʾ
	void DispPic(HImage& path, LocationView location);	//��ʾͼƬ��ָ��λ����ͼ
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//����ָ����ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�
	void SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location);	//ָ����ʾ���ڵľ��
	void SetPicViewScroll(int width, int height, LocationView location); //ָ����ͼ�Ĺ���������
	void OnOpenCameras();	//�����


private:
	Ui::hxqClass ui;
	HalconCpp::HTuple m_LeftWindowHandle, m_MiddleWindowHandle, m_SecondRightWindowHandle, m_RightWindowHandle;	//���С��Ҷ�������ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�
	QString m_Title;
	HImage m_Image, m_LeftImage, m_MiddleImage, m_SecondRightImage, m_RightImage;
	int m_AllResult;
	int m_good, m_bad, m_total;
	bool m_peviousProductDectectEnd;
	bool m_bIsOnLine; //�����ʱ,ѡ���Ƿ���PLC����
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

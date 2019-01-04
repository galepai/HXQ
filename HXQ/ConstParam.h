
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const char* AreaCamera880Id = "[0] USB3_CMOS_8.8M(2)";
static const char* AreaCameraId = "[0] USB3_CMOS_8.8M(1)";
static const char* AreaCameraId2 = "[0] USB3_CMOS_1.3M_M(2)";
static const char* LineCameraId_Basler_10_Clock= "0030531d42b0_Basler_raL204848gm";				//192.168.2.7
static const char* LineCameraId_Basler_2_Clock = "003053255252_Basler_raL204848gm";				//192.168.3.7
static const char* LineCameraId_Dalsa_7_Clock = "00010dc2ad7a_TeledyneDALSA_LineaM20487um";		//192.168.4.7
static const char* LineCameraId_Dalsa_11_Clock = "00010dc2ae11_TeledyneDALSA_LineaM20487um";	//192.168.5.7  


static const char* LineCameraId_Pylon_Basler_10_Clock = "Basler raL2048-48gm#0030531D42B0#192.168.2.7:3956";	//192.168.2.7
static const char* LineCameraId_Pylon_Basler_2_Clock = "Basler raL2048-48gm#003053255252#192.168.3.7:3956";		//192.168.3.7


static const int IOPoint = 4;  //Galil控制卡的触发相机的input点

/*******************    CameraParam    **************************/
static const char* XML_Camera		= "Xml/Camera.xml";					//"Xml/Camera.xml"
static const char* Node_TopCamera	= "HikVision_Top";					//"HikVision_Top"
static const char* Node_SideCamera	= "Dalsa_Side";						//"Dalsa_Side"

static const char* HikVision_AcquisitionMode	= "AcquisitionMode";	// "AcquisitionMode"
static const char* HikVision_TriggerSelector	= "TriggerSelector";	// "ExposureTime"
static const char* HikVision_TriggerMode		= "ExposureTime";		// "ExposureTime"
static const char* HikVision_ExposureTime		= "ExposureTime";		// "ExposureTime"
static const char* HikVision_grab_timeout		= "grab_timeout";		// "grab_timeout"

static const char* Dalsa_AcquisitionMode	= "AcquisitionMode";		// "AcquisitionMode"
static const char* Dalsa_TriggerSelector	= "TriggerSelector";		// "ExposureTime"
static const char* Dalsa_TriggerMode		= "ExposureTime";			// "ExposureTime"
static const char* Dalsa_ExposureTime		= "ExposureTime";			// "ExposureTime"
static const char* Dalsa_grab_timeout		= "grab_timeout";			// "grab_timeout"


/*******************    MotionCardParam    **************************/
static const char* XML_MotionCard			= "Xml/MotionCard.xml";		//"Xml/MotionCard.xml"
static const char* Node_MotionCard			= "MotionCard";				//"MotionCard"
static const char* MotionCard_ip			= "ip";						//"MotionCard"



/****************************************************/
static const int TopCamera	= 0x00;
static const int SideCamera = 0x01;


static const int Good				= 0x00;
static const int Bad				= 0x01;
static const int Gou				= 0x02;
static const int Cao				= 0x03;
static const int Liantong			= 0x04;

static const int MiddleGood			= 0x00;
static const int MiddleBad			= 0x01;
static const int SecondRightGood	= 0x30;
static const int SecondRightBad		= 0x31;
static const int RightGood			= 0x40;
static const int RightBad			= 0x41;
static const int AllGood			= 0xA0;



#endif

/**
*	功能：常量文件
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const int IOPoint = 4;  //Galil控制卡的触发相机的input点

/*******************    XML-CameraParam    **************************/
static const char* XML_Camera		= "Xml/Camera.xml";					//"Xml/Camera.xml"


/*******************    XML-MotionCardParam    **************************/
static const char* XML_MotionCard			= "Xml/MotionCard.xml";		//"Xml/MotionCard.xml"
static const char* Node_MotionCard			= "MotionCard";				//"MotionCard"
static const char* MotionCard_ip			= "ip";						//"MotionCard"


/*******************    XML-ConfigureParam    **************************/
static const char* XML_Configure		= "Xml/Configure.xml";			//"Xml/Configure.xml"

static const char* Node_Camera			= "Camera";						//"Camera"
static const char* Camera_Top			= "Camera_Top";					// "Camera_Top"
static const char* Camera_Side			= "Camera_Side";				// "Camera_Side"

static const char* Node_Hxq				= "Hxq";						//"Hxq"


/********************    Classisfy		****************/
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
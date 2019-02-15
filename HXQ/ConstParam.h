
/**
*	功能：常量文件
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

#define LIGHT_OPEN			"SB3"					//"SB3"
#define LIGHT_CLOSE			"CB3"					//"CB3"
#define CLASSIFIY_BAD		"SB4"					//"SB4"
#define CLASSIFIY_GOOD		"CB4"					//"CB4"

#define SINAL_CAMERA					"MG @OUT[3]"		//DO3
#define SINAL_MOVERIGHT1				"MG _RPA"			//<=3000000
#define SINAL_MOVERIGHT2				"MG _BGA"			//==1
#define SINAL_MOVERIGHT1_THRESHOLD		-300000				//移动右端RPA的阀值

#define AUTOSTART		"XQ#ZERO"					//自动启动
#define AUTOSTOP		"ST"						//自动停止

/*******************    XML-CameraParam    **************************/
#define XML_Camera				 "Xml/Camera.xml"			//"Xml/Camera.xml"


/*******************    XML-MotionCardParam    **************************/
#define XML_MotionCard			"Xml/MotionCard.xml"		//"Xml/MotionCard.xml"
#define Node_MotionCard			"MotionCard"				//"MotionCard"
#define MotionCard_ip			"ip"						//"MotionCard"


/*******************    XML-ConfigureParam    **************************/
#define XML_Configure		"Xml/Configure.xml"			//"Xml/Configure.xml"

#define Node_Camera			"Camera"					//"Camera"
#define Camera_Top			"Camera_Top"				// "Camera_Top"
#define Camera_Side			"Camera_Side"				// "Camera_Side"

#define Node_Hxq			"Hxq"						//"Hxq"

#define Node_Save			"Save"						//Save

/********************    Classisfy		****************/
#define TopCamera			0x00
#define SideCamera			0x01

#define Good				0x00
#define Bad					0x01
#define Gou					0x02
#define Cao					0x03
#define Liantong			0x04

#define MiddleGood			0x00
#define MiddleBad			0x01
#define SecondRightGood		0x30
#define SecondRightBad		0x31
#define RightGood			0x40
#define RightBad			0x41
#define AllGood				0xA0

/********************    Classisfy		****************/
//static const int TopCamera	= 0x00;
//static const int SideCamera = 0x01;
//
//static const int Good				= 0x00;
//static const int Bad				= 0x01;
//static const int Gou				= 0x02;
//static const int Cao				= 0x03;
//static const int Liantong			= 0x04;
//
//static const int MiddleGood			= 0x00;
//static const int MiddleBad			= 0x01;
//static const int SecondRightGood	= 0x30;
//static const int SecondRightBad		= 0x31;
//static const int RightGood			= 0x40;
//static const int RightBad			= 0x41;
//static const int AllGood			= 0xA0;


#define LineCameraId_Pylon_Basler_Side  "Basler raL2048-48gm#0030531D42B0#192.168.5.7:3956"	//Basler Camera

//#define SaveTopImageDir					"Images/Top/All/"		//	Images/Top/All/
//#define SaveSideImageDir				"Images/Side/All/"		//	Images/Side/All/

//#define SaveTopBadImageDir					"Images/Top/Bad/"		//	Images/Top/Bad/
//#define SaveTopBadImageDir					"Images/Side/Bad/"		//	Images/Top/Bad/


#define SaveTopImageDir					"e:/Images/Top/All/"		//	Images/Top/All/
#define SaveSideImageDir				"e:/Images/Side/All/"		//	Images/Side/All/

#define SaveTopBadImageDir					"e:/Images/Top/Bad/"		//	Images/Top/All/
#define SaveSideBadImageDir					"e:/Images/Side/Bad/"		//	Images/Top/All/


#endif 
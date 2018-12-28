
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const char* AreaCamera880Id = "[0] USB3_CMOS_8.8M(2)";
static const char* AreaCameraId = "[0] USB3_CMOS_8.8M(1)";
static const char* AreaCameraId2 = "[0] USB3_CMOS_1.3M_M(2)";
static const char* LineCameraId_Basler_10_Clock= "0030531d42b0_Basler_raL204848gm";	//192.168.2.7
static const char* LineCameraId_Basler_2_Clock = "003053255252_Basler_raL204848gm";	//192.168.3.7
static const char* LineCameraId_Dalsa_7_Clock = "00010dc2ad7a_TeledyneDALSA_LineaM20487um";	//192.168.4.7
static const char* LineCameraId_Dalsa_11_Clock = "00010dc2ae11_TeledyneDALSA_LineaM20487um";	//192.168.5.7  ui.OnStop->setEnabled(true);


static const char* LineCameraId_Pylon_Basler_10_Clock = "Basler raL2048-48gm#0030531D42B0#192.168.2.7:3956";	//192.168.2.7
static const char* LineCameraId_Pylon_Basler_2_Clock = "Basler raL2048-48gm#003053255252#192.168.3.7:3956";	//192.168.3.7
//m_camera.Attach(CTlFactory::GetInstance().CreateDevice("Basler raL2048-48gm#0030531D42B0#192.168.2.7:3956"));
//"Basler raL2048-48gm#0030531D42B0#192.168.2.7:3956"
//"Basler raL2048-48gm#003053255252#192.168.3.7:3956"

static const char* TopAreaCamera = "c42f90f0ff29_Hikvision_MVCE10031GM";	//

static const char* GalilIp = "192.168.2.7";	//Galil控制卡的网口通信地址
static const int IOPoint = 4;  //Galil控制卡的触发相机的input点

/*******************    CameraParam    **************************/
static const char* CameraXML		= "camera.xml";		//"camera.xml"
static const char* Node_TopCamera	= "HikVision_Top";	//"HikVision_Top"
static const char* Node_SideCamera	= "Dalsa_Side";	//"Dalsa_Side"

static const char* HikVision_AcquisitionMode	= "AcquisitionMode";	// "AcquisitionMode"
static const char* HikVision_TriggerSelector	= "TriggerSelector";	// "ExposureTime"
static const char* HikVision_TriggerMode		= "ExposureTime";			// "ExposureTime"
static const char* HikVision_ExposureTime		= "ExposureTime";			// "ExposureTime"
static const char* HikVision_grab_timeout		= "grab_timeout";		// "grab_timeout"

static const char* Dalsa_AcquisitionMode	= "AcquisitionMode";	// "AcquisitionMode"
static const char* Dalsa_TriggerSelector	= "TriggerSelector";	// "ExposureTime"
static const char* Dalsa_TriggerMode		= "ExposureTime";			// "ExposureTime"
static const char* Dalsa_ExposureTime		= "ExposureTime";			// "ExposureTime"
static const char* Dalsa_grab_timeout		= "grab_timeout";		// "grab_timeout"



/****************************************************/
static const int TopCamera = 0x00;
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



//	抓手水平左移
static const char* ZHUASHOU_LEFT_ON = "000508E2FF00";  //M226 SET 抓手水平左移
static const char* ZHUASHOU_LEFT_OFF = "000508E20000";  //M226 RST 抓手水平左移

//	抓手水平右移
static const char* ZHUASHOU_RIGHT_ON = "000508E3FF00";  //M227 SET 抓手水平右移
static const char* ZHUASHOU_RIGHT_OFF = "000508E30000";  //M227 RST 抓手水平右移

//	抓手垂直下移
static const char* ZHUASHOU_DOWN_ON = "000508E4FF00";  //M228 SET 抓手水平下移
static const char* ZHUASHOU_DOWN_OFF = "000508E40000";  //M228 RST 抓手水平下移

//	抓手垂直上移
static const char* ZHUASHOU_UP_ON = "000508E5FF00";  //M229 SET 抓手水平上移
static const char* ZHUASHOU_UP_OFF = "000508E50000";  //M229 RST 抓手水平上移

//	手指开
static const char* HAND_OPEN_ON = "000508E6FF00";  //M230 SET 手指开
static const char* HAND_OPEN_OFF = "000508E60000";  //M230 RST 手指开

//	手指关
static const char* HAND_CLOSE_ON = "000508E7FF00";  //M231 SET 手指关
static const char* HAND_CLOSE_OFF = "000508E70000";  //M231 RST 手指关

//	拨手伸
static const char* BOSHOU_UP_ON = "000508ECFF00";  //M236 SET 拨手伸
static const char* BOSHOU_UP_OFF = "000508EC0000";  //M236 RST 拨手伸

//	拨手缩
static const char* BOSHOU_DOWN_ON = "000508EDFF00";  //M237 SET 拨手缩
static const char* BOSHOU_DOWN_OFF = "000508ED0000";  //M237 RST 拨手缩

//	拨手水平伸
static const char* BOSHOU_SHUIPING_UP_ON = "000508EAFF00";  //M234 SET 拨手水平伸
static const char* BOSHOU_SHUIPING_UP_OFF = "000508EA0000";  //M234 RST 拨手水平伸

//	拨手水平缩
static const char* BOSHOU_SHUIPING_DOWN_ON = "000508EBFF00";  //M235 SET 拨手水平缩
static const char* BOSHOU_SHUIPING_DOWN_OFF = "000508EB0000";  //M235 RST 拨手水平缩

//	吹气气缸上升
static const char* WIND_UP_ON = "000508EEFF00";  //M238 SET 吹气气缸上升
static const char* WIND_UP_OFF = "000508EE0000";  //M238 RST 吹气气缸上升

//	吹气气缸下降
static const char* WIND_DOWN_ON = "000508EFFF00";  //M239 SET 吹气气缸下降
static const char* WIND_DOWN_OFF = "000508EF0000";  //M239 RST 吹气气缸下降



/*******************COM1 从站00*******************/
/************************************************/
/***********************************************/
//static const char* READ_X_INPUT = "000204000048";  //读X00-X87
//static const char* READ_Y_OUTPUT = "000105000038";  //读Y00-Y67
//
////	直线模组前进(自动),自动定时抛光
//static const char* PAOGUANG_ON = "000508A9FF00";  //M169 SET 直线模组前进,自动定时抛光
//static const char* PAOGUANG_OFF = "000508A90000";  //M169 RST 直线模组前进,自动定时抛光
//
////	视觉升
//static const char* VISION_UP_ON = "000508A5FF00";  //M165 SET 视觉升
//static const char* VISION_UP_OFF = "000508A50000";  //M165 RST 视觉升
//
////	视觉降
//static const char* VISION_DOWN_ON = "000508A6FF00";  //M166 SET 视觉降
//static const char* VISION_DOWN_OFF = "000508A60000";  //M166 RST 视觉降
//
////	视觉开始旋转，电平信号
//static const char* VISION_ROTATE_START_ON = "000508A7FF00";  //M167 SET 视觉开始旋转
//
////	视觉停止旋转
//static const char* VISION_ROTATE_STOP_ON = "000508A8FF00";  //M168 SET 视觉停止旋转
//static const char* VISION_ROTATE_STOP_OFF = "000508A80000";  //M168 RST 视觉停止旋转
//static const char* VISION_ROTATE_START_OFF = "000508A70000";  //M167 RST 视觉开始旋转
//
////	直线模组前进
//static const char* FORWARD_ON = "000508ADFF00";  //M173 SET 直线模组前进
//static const char* FORWARD_OFF = "000508AD0000";  //M173 RST 直线模组前进
//
////	直线模组后退
//static const char* BACKWARD_ON = "000508AEFF00";  //M174 SET 直线模组后退
//static const char* BACKWARD_OFF = "000508AE0000";  //M174 RST 直线模组后退
//
////	抛光电机旋转
//static const char* PAOGUANG_ROTATE_START_ON = "000508AFFF00";  //M175 SET 抛光电机旋转
//static const char* PAOGUANG_ROTATE_START_OFF = "000508AF0000";  //M175 RST 抛光电机旋转
//
////	抛光电机停止
//static const char* PAOGUANG_ROTATE_STOP_ON = "000508B0FF00";  //M176 SET 抛光电机停止
//static const char* PAOGUANG_ROTATE_STOP_OFF = "000508B00000";  //M176 RST 抛光电机停止
//
////	读D200-D205,  
////	返回的D200的16位数据代表直线模组的距离.	(单位:0.1mm),
////	返回的D202的16位数据代表抛光时间.	(单位:100ms)
//static const char* READ_TIME_AND_DISTANCE = "000310C80006";  //  读D200-D206,  *返回的D200(10C8)的32位数据代表直线模组的距离.(单位:0.1mm)		*返回的D204的16位数据代表抛光时间.(单位:100ms)
//
////	自动模式(电平信号),M115置1为自动
//static const char* AUTO_MODE = "00050873FF00";  //自动模式(电平信号),M115置1为自动
//
////	手动模式(电平信号),M115置0为手动
//static const char* MANUAL_MODE = "000508730000";  //手动模式(电平信号),M115置0为手动
//
////	自动模式启动(电平信号),M116置为1S
//static const char* AUTO_START = "00050874FF00";  //自动模式启动(电平信号),M116置为1
//
////	自动模式停止(电平信号),M116置为0
//static const char* AUTO_STOP = "000508740000";  //自动模式停止(电平信号),M116置为0
//
////	距离设置,D200(10C8) (单位:0.1mm)
//static const char* SET_DISTANCE = "000610C8";  //距离设置,D200(10C8) (单位:0.1mm)
//
////	时间设置,D204(10CC) 单位:100ms
//static const char* SET_TIME = "000610CC";  //时间设置,D204(10CC) 单位:100ms
//
//
////	检测完成信号,M177
//static const char* DETECT_END_ON = "000508B1FF00";  //M177 SET 检测完成信号
//static const char* DETECT_END_OFF = "000508B10000";  //M177 RST 检测完成信号
//
////	检测结果GOOD,D240(10F0) 
//static const char* RESULT_GODD = "000610F00010";  //时间设置,D240(10F0) 1合格
//
////	检测结果BAD,D240(10F0)
//static const char* RESULT_BAD = "000610F00020";  //时间设置,D240(10F0) 0不合格
//


/*********************COM2 站号01*****************/
/************************************************/
/***********************************************/

static const char* READ_X_INPUT = "010204000048";  //读X00-X87
static const char* READ_Y_OUTPUT = "010105000038";  //读Y00-Y67  
static const char* READ_Y61_OUTPUT = "010105300008";  //读Y60-Y67  
static const char* RESET_Y61 = "010505310000";  //置Y61为0  
static const char* RESET_Y60 = "010505300000";  //置Y60为0 (触发相机)

// 分料气缸
static const char* FENGLIAO_BAD = "010508FDFF00";  //置M253为1  
static const char* FENGLIAO_GOOD = "010508FD0000";  //置M253为0 

//	直线模组前进(自动),自动定时抛光
static const char* PAOGUANG_ON = "010508A9FF00";  //M169 SET 直线模组前进,自动定时抛光
static const char* PAOGUANG_OFF = "010508A90000";  //M169 RST 直线模组前进,自动定时抛光

//	视觉升
static const char* VISION_UP_ON = "010508A5FF00";  //M165 SET 视觉升
static const char* VISION_UP_OFF = "010508A50000";  //M165 RST 视觉升

//	视觉降
static const char* VISION_DOWN_ON = "010508A6FF00";  //M166 SET 视觉降
static const char* VISION_DOWN_OFF = "010508A60000";  //M166 RST 视觉降

//	视觉开始旋转，电平信号
static const char* VISION_ROTATE_START_ON = "010508A7FF00";  //M167 SET 视觉开始旋转

//	视觉停止旋转
static const char* VISION_ROTATE_STOP_ON = "010508A8FF00";  //M168 SET 视觉停止旋转
static const char* VISION_ROTATE_STOP_OFF = "010508A80000";  //M168 RST 视觉停止旋转
static const char* VISION_ROTATE_START_OFF = "010508A70000";  //M167 RST 视觉开始旋转

//	直线模组前进
static const char* FORWARD_ON = "010508ADFF00";  //M173 SET 直线模组前进
static const char* FORWARD_OFF = "010508AD0000";  //M173 RST 直线模组前进

//	直线模组后退
static const char* BACKWARD_ON = "010508AEFF00";  //M174 SET 直线模组后退
static const char* BACKWARD_OFF = "010508AE0000";  //M174 RST 直线模组后退

//	直线模组2前进
static const char* FORWARD_2_ON = "010508ABFF00";  //M171 SET 直线模组前进
static const char* FORWARD_2_OFF = "010508AB0000";  //M171 RST 直线模组前进

//	直线模组2后退
static const char* BACKWARD_2_ON = "010508ACFF00";  //M172 SET 直线模组后退
static const char* BACKWARD_2_OFF = "010508AC0000";  //M172 RST 直线模组后退

//	抛光电机旋转
static const char* PAOGUANG_ROTATE_START_ON = "010508AFFF00";  //M175 SET 抛光电机旋转
static const char* PAOGUANG_ROTATE_START_OFF = "010508AF0000";  //M175 RST 抛光电机旋转

//	抛光电机停止
static const char* PAOGUANG_ROTATE_STOP_ON = "010508B0FF00";  //M176 SET 抛光电机停止
static const char* PAOGUANG_ROTATE_STOP_OFF = "010508B00000";  //M176 RST 抛光电机停止

//	读D200-D208,  
//	返回的D200的16位数据代表直线模组的距离.	(单位:0.1mm),
//	返回的D202的16位数据代表抛光时间.	(单位:100ms)
//	返回的D208的16位数据代表直线模组的距离.	(单位:0.1mm)
static const char* READ_TIME_AND_DISTANCE = "010310C80009";  //  读D200-D208,  *返回的D200(10C8)的16位数据代表直线模组的距离.(单位:0.1mm)		*返回的D204的16位数据代表抛光时间.(单位:100ms)	   *返回的D208(10d0)的16位数据代表直线模组2的距离.(单位:0.1mm)	

//	距离设置,D200(10C8) (单位:0.1mm)
static const char* SET_DISTANCE = "010610C8";  //距离设置,D200(10C8) (单位:0.1mm)

//	时间设置,D204(10CC) 单位:100ms
static const char* SET_TIME = "010610CC";  //时间设置,D204(10CC) 单位:100ms

//	距离设置,D208(10D0) (单位:0.1mm)
static const char* SET_DISTANCE_2 = "010610D0";  //距离设置,D208(10D) (单位:0.1mm)

//	自动模式(电平信号),M115置1为自动
static const char* AUTO_MODE = "01050873FF00";  //自动模式(电平信号),M115置1为自动

//	手动模式(电平信号),M115置0为手动
static const char* MANUAL_MODE = "010508730000";  //手动模式(电平信号),M115置0为手动

//	自动模式启动(电平信号),M116置为1S
static const char* AUTO_START = "01050874FF00";  //自动模式启动(电平信号),M116置为1

//	自动模式停止(电平信号),M116置为0
static const char* AUTO_STOP = "010508740000";  //自动模式停止(电平信号),M116置为0


//	检测完成信号,M177
static const char* DETECT_END_ON = "010508B1FF00";  //M77 SET 检测完成信号
static const char* DETECT_END_OFF = "010508B10000";  //M77 RST 检测完成信号


//	检测结果GOOD,D240(10F0) 
static const char* RESULT_GODD = "010610F00010";  //时间设置,D240(10F0) 1合格

//	检测结果BAD,D240(10F0)
static const char* RESULT_BAD = "010610F00020";  //时间设置,D240(10F0) 0不合格



//	换向器保护伸
static const char* PROTECT_UP_ON = "01050896FF00";  //M150 SET 换向器保护伸
static const char* PROTECT_UP_OFF = "010508960000";  //M150 RST 换向器保护伸

//	换向器保护缩
static const char* PROTECT_DOWN_ON = "01050897FF00";  //M151 SET 换向器保护缩
static const char* PROTECT_DOWN_OFF = "010508970000";  //M151 RST 换向器保护缩

#endif
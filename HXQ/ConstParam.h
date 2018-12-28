
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
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

static const char* GalilIp = "192.168.2.7";	//Galil���ƿ�������ͨ�ŵ�ַ
static const int IOPoint = 4;  //Galil���ƿ��Ĵ��������input��

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



//	ץ��ˮƽ����
static const char* ZHUASHOU_LEFT_ON = "000508E2FF00";  //M226 SET ץ��ˮƽ����
static const char* ZHUASHOU_LEFT_OFF = "000508E20000";  //M226 RST ץ��ˮƽ����

//	ץ��ˮƽ����
static const char* ZHUASHOU_RIGHT_ON = "000508E3FF00";  //M227 SET ץ��ˮƽ����
static const char* ZHUASHOU_RIGHT_OFF = "000508E30000";  //M227 RST ץ��ˮƽ����

//	ץ�ִ�ֱ����
static const char* ZHUASHOU_DOWN_ON = "000508E4FF00";  //M228 SET ץ��ˮƽ����
static const char* ZHUASHOU_DOWN_OFF = "000508E40000";  //M228 RST ץ��ˮƽ����

//	ץ�ִ�ֱ����
static const char* ZHUASHOU_UP_ON = "000508E5FF00";  //M229 SET ץ��ˮƽ����
static const char* ZHUASHOU_UP_OFF = "000508E50000";  //M229 RST ץ��ˮƽ����

//	��ָ��
static const char* HAND_OPEN_ON = "000508E6FF00";  //M230 SET ��ָ��
static const char* HAND_OPEN_OFF = "000508E60000";  //M230 RST ��ָ��

//	��ָ��
static const char* HAND_CLOSE_ON = "000508E7FF00";  //M231 SET ��ָ��
static const char* HAND_CLOSE_OFF = "000508E70000";  //M231 RST ��ָ��

//	������
static const char* BOSHOU_UP_ON = "000508ECFF00";  //M236 SET ������
static const char* BOSHOU_UP_OFF = "000508EC0000";  //M236 RST ������

//	������
static const char* BOSHOU_DOWN_ON = "000508EDFF00";  //M237 SET ������
static const char* BOSHOU_DOWN_OFF = "000508ED0000";  //M237 RST ������

//	����ˮƽ��
static const char* BOSHOU_SHUIPING_UP_ON = "000508EAFF00";  //M234 SET ����ˮƽ��
static const char* BOSHOU_SHUIPING_UP_OFF = "000508EA0000";  //M234 RST ����ˮƽ��

//	����ˮƽ��
static const char* BOSHOU_SHUIPING_DOWN_ON = "000508EBFF00";  //M235 SET ����ˮƽ��
static const char* BOSHOU_SHUIPING_DOWN_OFF = "000508EB0000";  //M235 RST ����ˮƽ��

//	������������
static const char* WIND_UP_ON = "000508EEFF00";  //M238 SET ������������
static const char* WIND_UP_OFF = "000508EE0000";  //M238 RST ������������

//	���������½�
static const char* WIND_DOWN_ON = "000508EFFF00";  //M239 SET ���������½�
static const char* WIND_DOWN_OFF = "000508EF0000";  //M239 RST ���������½�



/*******************COM1 ��վ00*******************/
/************************************************/
/***********************************************/
//static const char* READ_X_INPUT = "000204000048";  //��X00-X87
//static const char* READ_Y_OUTPUT = "000105000038";  //��Y00-Y67
//
////	ֱ��ģ��ǰ��(�Զ�),�Զ���ʱ�׹�
//static const char* PAOGUANG_ON = "000508A9FF00";  //M169 SET ֱ��ģ��ǰ��,�Զ���ʱ�׹�
//static const char* PAOGUANG_OFF = "000508A90000";  //M169 RST ֱ��ģ��ǰ��,�Զ���ʱ�׹�
//
////	�Ӿ���
//static const char* VISION_UP_ON = "000508A5FF00";  //M165 SET �Ӿ���
//static const char* VISION_UP_OFF = "000508A50000";  //M165 RST �Ӿ���
//
////	�Ӿ���
//static const char* VISION_DOWN_ON = "000508A6FF00";  //M166 SET �Ӿ���
//static const char* VISION_DOWN_OFF = "000508A60000";  //M166 RST �Ӿ���
//
////	�Ӿ���ʼ��ת����ƽ�ź�
//static const char* VISION_ROTATE_START_ON = "000508A7FF00";  //M167 SET �Ӿ���ʼ��ת
//
////	�Ӿ�ֹͣ��ת
//static const char* VISION_ROTATE_STOP_ON = "000508A8FF00";  //M168 SET �Ӿ�ֹͣ��ת
//static const char* VISION_ROTATE_STOP_OFF = "000508A80000";  //M168 RST �Ӿ�ֹͣ��ת
//static const char* VISION_ROTATE_START_OFF = "000508A70000";  //M167 RST �Ӿ���ʼ��ת
//
////	ֱ��ģ��ǰ��
//static const char* FORWARD_ON = "000508ADFF00";  //M173 SET ֱ��ģ��ǰ��
//static const char* FORWARD_OFF = "000508AD0000";  //M173 RST ֱ��ģ��ǰ��
//
////	ֱ��ģ�����
//static const char* BACKWARD_ON = "000508AEFF00";  //M174 SET ֱ��ģ�����
//static const char* BACKWARD_OFF = "000508AE0000";  //M174 RST ֱ��ģ�����
//
////	�׹�����ת
//static const char* PAOGUANG_ROTATE_START_ON = "000508AFFF00";  //M175 SET �׹�����ת
//static const char* PAOGUANG_ROTATE_START_OFF = "000508AF0000";  //M175 RST �׹�����ת
//
////	�׹���ֹͣ
//static const char* PAOGUANG_ROTATE_STOP_ON = "000508B0FF00";  //M176 SET �׹���ֹͣ
//static const char* PAOGUANG_ROTATE_STOP_OFF = "000508B00000";  //M176 RST �׹���ֹͣ
//
////	��D200-D205,  
////	���ص�D200��16λ���ݴ���ֱ��ģ��ľ���.	(��λ:0.1mm),
////	���ص�D202��16λ���ݴ����׹�ʱ��.	(��λ:100ms)
//static const char* READ_TIME_AND_DISTANCE = "000310C80006";  //  ��D200-D206,  *���ص�D200(10C8)��32λ���ݴ���ֱ��ģ��ľ���.(��λ:0.1mm)		*���ص�D204��16λ���ݴ����׹�ʱ��.(��λ:100ms)
//
////	�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�
//static const char* AUTO_MODE = "00050873FF00";  //�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�
//
////	�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�
//static const char* MANUAL_MODE = "000508730000";  //�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�
//
////	�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1S
//static const char* AUTO_START = "00050874FF00";  //�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1
//
////	�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0
//static const char* AUTO_STOP = "000508740000";  //�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0
//
////	��������,D200(10C8) (��λ:0.1mm)
//static const char* SET_DISTANCE = "000610C8";  //��������,D200(10C8) (��λ:0.1mm)
//
////	ʱ������,D204(10CC) ��λ:100ms
//static const char* SET_TIME = "000610CC";  //ʱ������,D204(10CC) ��λ:100ms
//
//
////	�������ź�,M177
//static const char* DETECT_END_ON = "000508B1FF00";  //M177 SET �������ź�
//static const char* DETECT_END_OFF = "000508B10000";  //M177 RST �������ź�
//
////	�����GOOD,D240(10F0) 
//static const char* RESULT_GODD = "000610F00010";  //ʱ������,D240(10F0) 1�ϸ�
//
////	�����BAD,D240(10F0)
//static const char* RESULT_BAD = "000610F00020";  //ʱ������,D240(10F0) 0���ϸ�
//


/*********************COM2 վ��01*****************/
/************************************************/
/***********************************************/

static const char* READ_X_INPUT = "010204000048";  //��X00-X87
static const char* READ_Y_OUTPUT = "010105000038";  //��Y00-Y67  
static const char* READ_Y61_OUTPUT = "010105300008";  //��Y60-Y67  
static const char* RESET_Y61 = "010505310000";  //��Y61Ϊ0  
static const char* RESET_Y60 = "010505300000";  //��Y60Ϊ0 (�������)

// ��������
static const char* FENGLIAO_BAD = "010508FDFF00";  //��M253Ϊ1  
static const char* FENGLIAO_GOOD = "010508FD0000";  //��M253Ϊ0 

//	ֱ��ģ��ǰ��(�Զ�),�Զ���ʱ�׹�
static const char* PAOGUANG_ON = "010508A9FF00";  //M169 SET ֱ��ģ��ǰ��,�Զ���ʱ�׹�
static const char* PAOGUANG_OFF = "010508A90000";  //M169 RST ֱ��ģ��ǰ��,�Զ���ʱ�׹�

//	�Ӿ���
static const char* VISION_UP_ON = "010508A5FF00";  //M165 SET �Ӿ���
static const char* VISION_UP_OFF = "010508A50000";  //M165 RST �Ӿ���

//	�Ӿ���
static const char* VISION_DOWN_ON = "010508A6FF00";  //M166 SET �Ӿ���
static const char* VISION_DOWN_OFF = "010508A60000";  //M166 RST �Ӿ���

//	�Ӿ���ʼ��ת����ƽ�ź�
static const char* VISION_ROTATE_START_ON = "010508A7FF00";  //M167 SET �Ӿ���ʼ��ת

//	�Ӿ�ֹͣ��ת
static const char* VISION_ROTATE_STOP_ON = "010508A8FF00";  //M168 SET �Ӿ�ֹͣ��ת
static const char* VISION_ROTATE_STOP_OFF = "010508A80000";  //M168 RST �Ӿ�ֹͣ��ת
static const char* VISION_ROTATE_START_OFF = "010508A70000";  //M167 RST �Ӿ���ʼ��ת

//	ֱ��ģ��ǰ��
static const char* FORWARD_ON = "010508ADFF00";  //M173 SET ֱ��ģ��ǰ��
static const char* FORWARD_OFF = "010508AD0000";  //M173 RST ֱ��ģ��ǰ��

//	ֱ��ģ�����
static const char* BACKWARD_ON = "010508AEFF00";  //M174 SET ֱ��ģ�����
static const char* BACKWARD_OFF = "010508AE0000";  //M174 RST ֱ��ģ�����

//	ֱ��ģ��2ǰ��
static const char* FORWARD_2_ON = "010508ABFF00";  //M171 SET ֱ��ģ��ǰ��
static const char* FORWARD_2_OFF = "010508AB0000";  //M171 RST ֱ��ģ��ǰ��

//	ֱ��ģ��2����
static const char* BACKWARD_2_ON = "010508ACFF00";  //M172 SET ֱ��ģ�����
static const char* BACKWARD_2_OFF = "010508AC0000";  //M172 RST ֱ��ģ�����

//	�׹�����ת
static const char* PAOGUANG_ROTATE_START_ON = "010508AFFF00";  //M175 SET �׹�����ת
static const char* PAOGUANG_ROTATE_START_OFF = "010508AF0000";  //M175 RST �׹�����ת

//	�׹���ֹͣ
static const char* PAOGUANG_ROTATE_STOP_ON = "010508B0FF00";  //M176 SET �׹���ֹͣ
static const char* PAOGUANG_ROTATE_STOP_OFF = "010508B00000";  //M176 RST �׹���ֹͣ

//	��D200-D208,  
//	���ص�D200��16λ���ݴ���ֱ��ģ��ľ���.	(��λ:0.1mm),
//	���ص�D202��16λ���ݴ����׹�ʱ��.	(��λ:100ms)
//	���ص�D208��16λ���ݴ���ֱ��ģ��ľ���.	(��λ:0.1mm)
static const char* READ_TIME_AND_DISTANCE = "010310C80009";  //  ��D200-D208,  *���ص�D200(10C8)��16λ���ݴ���ֱ��ģ��ľ���.(��λ:0.1mm)		*���ص�D204��16λ���ݴ����׹�ʱ��.(��λ:100ms)	   *���ص�D208(10d0)��16λ���ݴ���ֱ��ģ��2�ľ���.(��λ:0.1mm)	

//	��������,D200(10C8) (��λ:0.1mm)
static const char* SET_DISTANCE = "010610C8";  //��������,D200(10C8) (��λ:0.1mm)

//	ʱ������,D204(10CC) ��λ:100ms
static const char* SET_TIME = "010610CC";  //ʱ������,D204(10CC) ��λ:100ms

//	��������,D208(10D0) (��λ:0.1mm)
static const char* SET_DISTANCE_2 = "010610D0";  //��������,D208(10D) (��λ:0.1mm)

//	�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�
static const char* AUTO_MODE = "01050873FF00";  //�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�

//	�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�
static const char* MANUAL_MODE = "010508730000";  //�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�

//	�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1S
static const char* AUTO_START = "01050874FF00";  //�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1

//	�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0
static const char* AUTO_STOP = "010508740000";  //�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0


//	�������ź�,M177
static const char* DETECT_END_ON = "010508B1FF00";  //M77 SET �������ź�
static const char* DETECT_END_OFF = "010508B10000";  //M77 RST �������ź�


//	�����GOOD,D240(10F0) 
static const char* RESULT_GODD = "010610F00010";  //ʱ������,D240(10F0) 1�ϸ�

//	�����BAD,D240(10F0)
static const char* RESULT_BAD = "010610F00020";  //ʱ������,D240(10F0) 0���ϸ�



//	������������
static const char* PROTECT_UP_ON = "01050896FF00";  //M150 SET ������������
static const char* PROTECT_UP_OFF = "010508960000";  //M150 RST ������������

//	������������
static const char* PROTECT_DOWN_ON = "01050897FF00";  //M151 SET ������������
static const char* PROTECT_DOWN_OFF = "010508970000";  //M151 RST ������������

#endif
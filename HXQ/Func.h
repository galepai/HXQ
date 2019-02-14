
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QWaitCondition>
#include <HalconCpp.h>
#include <queue>

//#define HARDTRIGGER

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT����ʾ���ģ�ʹ�÷��� G2U("����")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_SecondRightPic(height) ui.scrollAreaWidgetContentsSecondRight->setMinimumHeight(height)	//��ʾ�Ҷ�����߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//��ʾ�ұ�����߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//��ʾ�������߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_MiddlePic(height) ui.scrollAreaWidgetContentsMiddle->setMinimumHeight(height)	//��ʾ�м�����߶ȣ�������ʾ��������

#define VSCROLL_WIDTH_SecondRightPic(width) ui.scrollAreaWidgetContentsSecondRight->setMinimumWidth(width)	//��ʾ�Ҷ������ȣ�������ʾ��������
#define VSCROLL_WIDTH_RightPic(width) ui.scrollAreaWidgetContentsRight->setMinimumWidth(width)	//��ʾ�ұ������ȣ�������ʾ��������
#define VSCROLL_WIDTH_LeftPic(width) ui.scrollAreaWidgetContentsLeft->setMinimumWidth(width)	//��ʾ��������ȣ�������ʾ��������
#define VSCROLL_WIDTH_MiddlePic(width) ui.scrollAreaWidgetContentsMiddle->setMinimumWidth(width)	//��ʾ�м������ȣ�������ʾ��������

//#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//���ر�������������С������󻯡��رհ�ť
#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::SplashScreen);	//���ر�������������С������󻯡��رհ�ť


#define READ_XML_ElEMENT(path,element,nodename,out_type,out_value) ReadXmlElementText(QString(path), QString(element), QString(nodename), out_type, out_value);

void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//�����ļ���д�뵱ǰʱ�� Use QSetting,slower than QFile 
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//�����ļ���д��������Ϣ
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	//�����ļ��ж�ȡ��Ϣ


/* Example
for (int i = 1; i < 101; i++)
{
	QStringList writeinfo_list;
	writeinfo_list.append(QString("images/camera1/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera2/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera3/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera4/%1.bmp").arg(i, 4, 10, QChar('0')));
	QtWriteFile(QString("0423/%1.ini").arg(i, 4, 10, QChar('0')), writeinfo_list);
}
*/
void QtWriteFile(const QString& path_filename, const QStringList& writeinfo_list);

/* Example
QStringList readinfo_list;
QtReadFile(QString("0423/%1.ini").arg(15, 4, 10, QChar('0')), readinfo_list);
*/
void QtReadFile(const QString& path_filename,  QStringList& readinfo_list);

std::string Gen_Delta_Ascii_CR(const std::string& data);	//����̨��ASCIIģʽ�µ�LRC
std::string Delta_Ascii_CR(const std::string& data);	//����̨��ASCIIģʽ�µ�����data,����:+data+LRC+\r\n
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//����̨��ASCIIģʽ�µ�����data,����:+data+LRC+\r\n
std::vector<bool> Parse_Delta_Ascii(const std::string& data);	//Delta������Ϣ����������
std::vector<ushort> Parse_Delta_Ascii_03(const std::string& data);	//Delta������Ϣ����������


std::vector<bool> Parse_Galil_Input(int value); //����Galil��Input��Ϣ

//����Xml
bool ReadXmlElementText(QString& path, QString& childNodeName, QString& childrenNodeName, QString& OutType, QString& OutValue);
bool ParserXmlNode(QString& path, QString& childNodeName, std::vector <std::pair<std::pair<QString, QString>, QString>>& xmlContent);
void ParserCamParamAndSetFramerabber(HalconCpp::HFramegrabber* pGrabber, std::vector <std::pair<std::pair<QString, QString>, QString>>& CamParam);
int UpdateXmlNodeText(QString& path, QString& childNodeName, QString& childrenNodeName, QString& changeValue);

enum ChhXml
{
	UpdateOK,
	UpdateFail,
};


bool CreateImagetDir();
bool isCorrectImage(HalconCpp::HImage& image, double threshold_gray);


enum LocationView
{
	LeftView,
	MiddleView,
	SecondRightView,
	RightView,
};


struct Detect_Result
{
	bool isLeftEnd;
	bool isLeftBad;
	bool isMiddleEnd;
	bool isMiddleBad;
	bool isRightEnd;
	bool isRightBad;
	bool isBad;
};


struct DetectResultInfo
{
	int current_line;
	int current_area;
	int next_area;
};



//extern QMutex mutex_Camera;
//extern QMutex mutex_Result;
//extern QWaitCondition condition_Camera;
//extern bool g_UpWaveEnable;
//extern std::queue<bool> g_Result_Queue;
//extern int g_SaveTopBadIndex,g_SaveSideBadIndex;

#endif // FUNC_H

/**
*	功能：通用函数定义
*	作者：chenhui
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

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT中显示中文，使用方法 G2U("中文")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_SecondRightPic(height) ui.scrollAreaWidgetContentsSecondRight->setMinimumHeight(height)	//显示右二区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//显示右边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//显示左边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_MiddlePic(height) ui.scrollAreaWidgetContentsMiddle->setMinimumHeight(height)	//显示中间区域高度，用于显示出滚动条

#define VSCROLL_WIDTH_SecondRightPic(width) ui.scrollAreaWidgetContentsSecondRight->setMinimumWidth(width)	//显示右二区域宽度，用于显示出滚动条
#define VSCROLL_WIDTH_RightPic(width) ui.scrollAreaWidgetContentsRight->setMinimumWidth(width)	//显示右边区域宽度，用于显示出滚动条
#define VSCROLL_WIDTH_LeftPic(width) ui.scrollAreaWidgetContentsLeft->setMinimumWidth(width)	//显示左边区域宽度，用于显示出滚动条
#define VSCROLL_WIDTH_MiddlePic(width) ui.scrollAreaWidgetContentsMiddle->setMinimumWidth(width)	//显示中间区域宽度，用于显示出滚动条

//#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//隐藏标题栏，比如最小化，最大化、关闭按钮
#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::SplashScreen);	//隐藏标题栏，比如最小化，最大化、关闭按钮


#define READ_XML_ElEMENT(path,element,nodename,out_type,out_value) ReadXmlElementText(QString(path), QString(element), QString(nodename), out_type, out_value);

void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//配置文件中写入当前时间 Use QSetting,slower than QFile 
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//配置文件中写入设置信息
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	//配置文件中读取信息


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

std::string Gen_Delta_Ascii_CR(const std::string& data);	//生成台达ASCII模式下的LRC
std::string Delta_Ascii_CR(const std::string& data);	//生成台达ASCII模式下的完整data,包括:+data+LRC+\r\n
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//生成台达ASCII模式下的完整data,包括:+data+LRC+\r\n
std::vector<bool> Parse_Delta_Ascii(const std::string& data);	//Delta接收信息，解析功能
std::vector<ushort> Parse_Delta_Ascii_03(const std::string& data);	//Delta接收信息，解析功能


std::vector<bool> Parse_Galil_Input(int value); //解析Galil的Input信息

//解析Xml
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
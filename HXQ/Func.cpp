
#include "Func.h"
#include <QTime>
#include <QSettings>
#include <QDir>
#include <vector>
#include <bitset>
#include <qdebug.h>
#include <QtXml\QDomElement>

//QMutex g_mutex_Camera;
//QMutex g_mutex_Result;
//QWaitCondition g_condition_Camera;
//bool g_UpWaveEnable = true;
//std::queue<bool> g_Result_Queue;
//int g_SaveTopBadIndex, g_SaveSideBadIndex;

//配置文件中写入当前时间 Use QSetting slower than QFile 
void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName)
{
	QDateTime datetime;
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	setting.setValue(SetValueName, QVariant(datetime.currentDateTime().toString(Qt::ISODateWithMs)));
	setting.endGroup();
}

void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param)
{
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	setting.setValue(SetValueName, Param);
	setting.endGroup();
}

void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value)
{
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	Value = setting.value(GetValueName);
	setting.endGroup();
}

//生成台达Ascii的检验码及生成完整数据（: + data + CR + \r\n）； 注意 (A-F)必须传入大写
std::string Delta_Ascii_CR(const std::string& data)
{
	// "01020304FF05"  ~( 0X(01+02+03+04+EF+05) ) + 0x01;	两两字符用高低字节表示1个字节再相加，比如EF，用1个字节表示，E是1110,F是1111，组成字节是11101111。

	//unsigned char lr = 0x0;
	//
	//for (int index = 0; index < data.length() / 2; index++)
	//{
	//	unsigned char high = data[index * 2];
	//	unsigned char low = data[index * 2 + 1];
	//	if (high<0x3a)
	//		high = (high - 0x30) << 4;
	//	else
	//		high = (high - 0x37) << 4;

	//	if (low<0x3a)
	//		low = low - 0x30;
	//	else
	//		low = low - 0x37;

	//	lr = high + low + lr;
	//}
	//lr = ~lr + 0x01;

	//std::string cr = "ff";
	//unsigned char high = lr >> 4;
	//unsigned char low = lr & 0x0f;
	//if (high < 0xa)		// 小于10的Ascii码对应的Hex
	//{
	//	cr[0] = high + 0x30;	// 对应的数值(0-9)的Ascii码 ＝ 该数对应的Hex + 0x30(0对应的ASCII码的Hex)； 例如: 2对应的ASCII ＝ 0x02+0x30；
	//}
	//else          // 其他的Ascii码对应的Hex
	//{
	//	cr[0] = high + 0x37;	// 对应的字母（A-F）的Ascii码 = 该字母对应的Hex  + 0x37； 例如: d对应的ASCII ＝ 0xd+0x37；
	//}

	//if (low < 0xa)
	//{
	//	cr[1] = low + 0x30;
	//}
	//else
	//{
	//	cr[1] = low + 0x37;
	//}

	std::string all_data(":");
	all_data += data + Gen_Delta_Ascii_CR(data) + "\r\n";

	return all_data;
}

//生成台达ASCII模式下的LRC
std::string Gen_Delta_Ascii_CR(const std::string& data)
{
	// "01020304FF05"  ~( 0X(01+02+03+04+EF+05) ) + 0x01;	两两字符用高低字节表示1个字节再相加，比如EF，用1个字节表示，E是1110,F是1111，组成字节是11101111。
	unsigned char lr = 0x0;

	for (int index = 0; index < data.length() / 2; index++)
	{
		unsigned char high = data[index * 2];
		unsigned char low = data[index * 2 + 1];
		if (high < 0x3a)
			high = (high - 0x30) << 4;
		else
			high = (high - 0x37) << 4;

		if (low < 0x3a)
			low = low - 0x30;
		else
			low = low - 0x37;

		lr = high + low + lr;
	}
	lr = ~lr + 0x01;

	std::string cr = "ff";
	unsigned char high = lr >> 4;
	unsigned char low = lr & 0x0f;
	if (high < 0xa)		// 小于10的Ascii码对应的Hex
	{
		cr[0] = high + 0x30;	// 对应的数值(0-9)的Ascii码 ＝ 该数对应的Hex + 0x30(0对应的ASCII码的Hex)； 例如: 2对应的ASCII ＝ 0x02+0x30；
	}
	else          // 其他的Ascii码对应的Hex
	{
		cr[0] = high + 0x37;	// 对应的字母（A-F）的Ascii码 = 该字母对应的Hex  + 0x37； 例如: d对应的ASCII ＝ 0xd+0x37；
	}

	if (low < 0xa)
	{
		cr[1] = low + 0x30;
	}
	else
	{
		cr[1] = low + 0x37;
	}

	return cr;
}

std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info)
{
	return Delta_Ascii_CR(Slave + Function_Code + Function_Code + Start_Address + Other_Info);
}

//解析功能，还未完善...
std::vector<bool> Parse_Delta_Ascii(const std::string& data)
{
	//data = ':00050520FF00D7\r\n';
	//":00010500000A6E0082\r\n"		//读取Y00-Y47返回的数据
	//":000205042AA8A2057C\r\n"		//读取X00-X47返回的数据

	std::string temp;
	int len = data.length() / 2;
	std::vector<bool> nums;
	if (data[0] == ':' && data[data.length() - 1] == '\n')
	{
		temp = data.substr(1, data.length() - 5);
		std::string function_code = temp.substr(2, 2);
		if (function_code == "01")	//读取Y00 - Y47返回的数据
		{
			std::string byte_count = temp.substr(4, 2);
			int count = atoi(byte_count.c_str());
			temp = data.substr(7, count * 2);
		}
		else if (function_code == "02")	//读取X00 - X47返回的数据
		{
			std::string byte_count = temp.substr(4, 2);
			int count = atoi(byte_count.c_str());
			temp = data.substr(7, count * 2);
		}

		for (int index = 0; index < temp.length() / 2; index++)
		{
			unsigned char high = temp[index * 2];
			unsigned char low = temp[index * 2 + 1];
			if (high < 0x3a)
				high = (high - 0x30) << 4;
			else
				high = (high - 0x37) << 4;

			if (low < 0x3a)
				low = low - 0x30;
			else
				low = low - 0x37;

			unsigned char byte = high + low;
			/*unsigned char bit = 0x01;
			for (int i = 0; i < 8; i++)
			{
				if ((byte&bit) == bit)
				{
					nums.push_back(true);
					bit <<= 1;
				}
				else
				{
					nums.push_back(false);
					bit <<= 1;
				}
			}*/

			std::bitset<8> bar(byte);
			for (int i = 0; i < 8; i++)
			{
				if (bar[i] == 1)
				{
					nums.push_back(true);
				}
				else
				{
					nums.push_back(false);
				}
			}
		}
	}
	else
	{
		// 数据不完整或者数据有错
	}

	return nums;
}

std::vector<ushort> Parse_Delta_Ascii_03(const std::string& data)
{
	//data = ':000310C8000223\r\n'; 写入PLC的数据,读取D200-D201
	//":000304006E00008B\r\n"	//读取D200-D201返回的数据 04代表字节数,006E代表D200的32数据,0000代表D201的32数据

	std::string temp;
	int len = data.length() / 2;
	std::vector<ushort> nums;
	if (data[0] == ':' && data[data.length() - 1] == '\n')
	{
		temp = data.substr(1, data.length() - 5);
		std::string function_code = temp.substr(2, 2);
		std::string byte_count = temp.substr(4, 2);
		unsigned char high = byte_count[0];
		unsigned char low = byte_count[1];
		if (high < 0x3a)
			high = (high - 0x30) << 4;
		else
			high = (high - 0x37) << 4;

		if (low < 0x3a)
			low = low - 0x30;
		else
			low = low - 0x37;

		unsigned char byte = high + low;
		int count = byte;
		temp = data.substr(7, count * 2);

		std::vector<unsigned char> total_char;
		for (int index = 0; index < temp.length() / 2; index++)
		{
			unsigned char high = temp[index * 2];
			unsigned char low = temp[index * 2 + 1];
			if (high < 0x3a)
				high = (high - 0x30) << 4;
			else
				high = (high - 0x37) << 4;

			if (low < 0x3a)
				low = low - 0x30;
			else
				low = low - 0x37;

			unsigned char byte = high + low;
			total_char.push_back(byte);
		}


		int DataNum = total_char.size() / 2;

		for (int index = 0; index < DataNum; index++)
		{
			ushort Hi = total_char[index * 2];
			Hi = Hi * 0x100;
			ushort Lo = total_char[index * 2 + 1];
			ushort Data = Hi + Lo;
			nums.push_back(Data);
		}
	}
	else
	{
		// 数据不完整或者数据有错
	}

	return nums;
}

std::vector<bool> Parse_Galil_Input(int value)
{
	std::vector<bool> vec;
	quint8 i = value;
	std::bitset<8> bar(i);
	for (int i = 0; i < 8; i++)
	{
		if (bar[i] == 1)
		{
			vec.push_back(true);
		}
		else
		{
			vec.push_back(false);
		}
	}

	return vec;
}



//	Use QFile faster than QSetting
void QtWriteFile(const QString& path_filename, const QStringList& writeinfo_list)
{

	QFile file(path_filename);

	if (file.open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream out(&file);
		foreach(QString str, writeinfo_list)
		{
			out << str << "\r\n";
		}
	}
	else
	{

	}
	file.close();

}

void QtReadFile(const QString& path_filename, QStringList& readinfo_list)
{
	QFile file(path_filename);

	if (file.open(QFile::ReadOnly))
	{
		QTextStream in(&file);
		QByteArray str = file.readLine();
		while (!str.isEmpty())
		{
			readinfo_list.append(str);
			str = file.readLine();
		}

	}
	else
	{

	}
	file.close();
}

//生成保存图像文件夹，以日期做为文件夹（内含子文件夹），例如在当前路径“images/180424”
bool CreateImagetDir()
{
	QString parent_dir_path = "images";
	QDate date;
	QString currentDate = date.currentDate().toString("yyMMdd");
	currentDate = parent_dir_path + "/" + currentDate;
	QDir dir(currentDate);
	if (!dir.exists())
	{
		QDir dir1;
		QStringList path_list = { currentDate,
			currentDate + "/camera1",
			currentDate + "/camera2",
			currentDate + "/camera3",
			currentDate + "/camera4" };

		int index = 0;
		for (auto path : path_list)
		{
			index++;
			dir1.mkpath(path);
			WriteConfigure("config.ini", "Config", QString("ImagePath%1").arg(index), path);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool isCorrectImage(HalconCpp::HImage& image, double threshold_gray)
{
	HalconCpp::HTuple mean_gray;
	HalconCpp::GrayFeatures(image, image, "mean", &mean_gray);
	qDebug() << "mean gray:		" << mean_gray.D();
	//WriteImage(image, "bmp", 0, "c:/lastest.bmp");
	return mean_gray > threshold_gray;
}


bool ParserXmlNode(QString& path, QString& childNodeName, std::vector <std::pair<std::pair<QString, QString>, QString>>& xmlContent)
{
	QDomDocument doc;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();
	
	QDomElement childElement = root.firstChildElement(childNodeName);

	if (!childElement.isNull())
	{
		QDomNodeList list = childElement.childNodes();
		for (int index = 0; index < list.count(); index++)
		{
			QString name = childElement.childNodes().at(index).toElement().nodeName();
			QString type = childElement.childNodes().at(index).toElement().attribute("type");
			QString value = childElement.childNodes().at(index).toElement().text();
			//xmlContent.push_back(std::pair<QString, QString>(name, value));
			xmlContent.push_back(std::pair<std::pair<QString, QString>, QString>(std::pair<QString, QString>(name, type), value));
		}
	}
	else
	{
		return false;
	}

	//QDomNode child = root.firstChild();  //fivision
	//while (!child.isNull())
	//{
	//	if (child.toElement().nodeName() == childNodeName)
	//	{	
	//		QDomNodeList list = child.toElement().childNodes();
	//		int num = list.count();
	//		for (int index = 0; index < list.count(); index++)
	//		{
	//			QString name = child.toElement().childNodes().at(index).toElement().nodeName();
	//			QString type = child.toElement().childNodes().at(index).toElement().attribute("type");
	//			QString value = child.toElement().childNodes().at(index).toElement().text();			
	//			//xmlContent.push_back(std::pair<QString, QString>(name, value));
	//			xmlContent.push_back(std::pair<std::pair<QString, QString>,QString>(std::pair<QString, QString>(name, type), value));
	//		}
	//	}
	//	child = child.nextSibling();
	//}

	return true;
}

int UpdateXmlNodeText(QString& path, QString& childNodeName, QString& childrenNodeName, QString& changeValue)
{
	int retValue;
	QDomDocument doc;
	QFile file(path);

	if (!file.open(QFile::ReadOnly))
		return ChhXml::UpdateFail;

	if (!doc.setContent(&file))
	{
		file.close();
		return ChhXml::UpdateFail;
	}
	file.close();
	
	QDomElement rootElement = doc.documentElement();

	QDomElement childElement = rootElement.firstChildElement(childNodeName);
	if (!childElement.isNull())
	{
			QDomElement childrenElement = childElement.firstChildElement(childrenNodeName);
			if (!childrenElement.isNull())
			{
				QString oldText = childrenElement.text();
				childrenElement.firstChild().setNodeValue(changeValue);
				
				QFile file1(path);
				if (!file1.open(QFile::WriteOnly | QFile::Truncate))
				{
					file1.close();
					return ChhXml::UpdateFail;
				}

				QTextStream outStrem(&file1);
				doc.save(outStrem, 4);
				file1.close();

				return ChhXml::UpdateOK;;
			}
			else
			{
				return ChhXml::UpdateFail;
			}
	}

	return ChhXml::UpdateFail;

	//while (!child.isNull())
	//{
	//	if (child.nodeName() == childNodeName)
	//	{
	//		QDomElement childrenElement = child.firstChildElement(childrenNodeName);
	//		if (!childrenElement.isNull())
	//		{
	//			childrenElement.firstChild().setNodeValue(changeValue);
	//			retValue = ChhXml::UpdateOK;
	//			break;
	//		}
	//		
	//		/*QDomNodeList list = child.childNodes();
	//		for (int index = 0; index < list.count(); index++)
	//		{
	//			QString name = child.childNodes().at(index).toElement().nodeName();
	//			QString value = child.childNodes().at(index).toElement().text();
	//			if (name == childrenNodeName)
	//			{
	//				QString pre = value.left(value.indexOf("_")+1);
	//				child.childNodes().at(index).firstChild().setNodeValue(pre + changeValue);
	//				retValue = ChhXml::UpdateOK;
	//				break;
	//			}
	//		}*/
	//	}
	//	child = child.nextSibling();
	//}
	
}

bool ReadXmlElementText(QString& path, QString& childNodeName, QString& childrenNodeName,QString& OutType, QString& OutValue)
{
	int retValue;
	QDomDocument doc;
	QFile file(path);

	if (!file.open(QFile::ReadOnly))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement rootElement = doc.documentElement();

	QDomElement childElement = rootElement.firstChildElement(childNodeName);
	if (!childElement.isNull())
	{
		QDomElement childrenElement = childElement.firstChildElement(childrenNodeName);
		if (!childrenElement.isNull())
		{
			OutType = childrenElement.attribute("type");
			OutValue = childrenElement.text();
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;


}


void ParserCamParamAndSetFramerabber(HalconCpp::HFramegrabber* pGrabber, std::vector <std::pair<std::pair<QString, QString>, QString>>& CamParam)
{
	for (int index = 3; index < CamParam.size(); index++)
	{
		QString name = CamParam[index].first.first;
		QString type = CamParam[index].first.second;
		QString value = CamParam[index].second;

		if (type.contains("float"))
		{
			pGrabber->SetFramegrabberParam(name.toStdString().c_str(), value.toFloat());
		}
		else if(type.contains("int"))
		{
			pGrabber->SetFramegrabberParam(name.toStdString().c_str(), value.toInt());
		}
		else
		{
			pGrabber->SetFramegrabberParam(name.toStdString().c_str(), value.toStdString().c_str());
		}


	}
}

bool isFileExist(QString fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	if (fileInfo.isFile())
	{
		return true;
	}
	return false;
}
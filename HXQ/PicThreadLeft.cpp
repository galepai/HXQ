#include "PicThreadLeft.h"
#include <QTime>
#include "CHH.h"
#include "CHH2.h"
#include <QDebug>
//#include "Func.h"
#include "Global.h"
//#include "../Detect/Detect/Detect.h"
#include "Detect.h"


int PicThreadLeft::num = 0;
HImage PicThreadLeft::m_Image;

void PicThreadLeft::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		try
		{
			HTuple hv_IsBad=0;

			OnHandle(m_Image,m_WindowHandle,&hv_IsBad);
			int result = hv_IsBad.I();

			//int result = 0;
			switch (result)
			{
			case Good:

				emit resultReady(Good, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("良品 "), "image", 120, 12, "black", "true");
				break;

			case Bad:

				emit resultReady(Bad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("不良品 "), "image", 120, 12, "black", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Gou:

				emit resultReady(Gou, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("钩不良 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Cao:

				emit resultReady(Cao, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("槽不良 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Liantong:

				emit resultReady(Liantong, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("连铜 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			default:

				emit resultReady(Bad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("无正确分类号"), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;
			}

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");
			

			//qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			////if (hv_IsBad.I()==0)
			//if (num % 3)
			//{
			//	emit resultReady(Good,m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("良品 "), "image", 120, 12, "black", "true");
			//}
			//else if (num % 5)
			//{
			//	emit resultReady(Gou,m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("钩不良 "), "image", 120, 12, "red", "true");

			//	if(IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}
			//else if (num % 7)
			//{
			//	emit resultReady(Cao, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("槽不良 "), "image", 120, 12, "red", "true");

			//	if (IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}
			//else if (num % 11)
			//{
			//	emit resultReady(Liantong, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("槽内粘铜 "), "image", 120, 12, "red", "true");

			//	if (IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}

		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			DispText(m_WindowHandle, "LeftThread handle Error.", "image", 120, 12, "red", HTuple(), HTuple());
			emit resultReady(Bad, m_CameraId);
			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");
			CHH::disp_message(m_WindowHandle, HTuple("无正确分类号"), "image", 120, 12, "red", "true");
			qDebug() << "ThreadLeft error:  " << error;
			if (IsSaveImage())
				QueueSaveImage(m_Image, SaveImageNum());
		}

		
	}
	
}

void PicThreadLeft::OnHandle(HObject& image,const HTuple& WindowHandle,HTuple* Result)
{
	//static int step = 1;
	//step++;

	//HObject circle;
	//int row = 100 + 50 * step;
	//if (row < 2000)
	//	GenCircle(&circle, 100 + 50 * step, 300, 100);
	//else
	//{
	//	GenCircle(&circle, 100 + 50 * step, 300, 100);
	//	step = 1;
	//}

	//DispObj(circle, WindowHandle);

	////结果
	//*Result = 1;

	//引用动态库
	HTuple output_ExceptionInformtion("");
	DetectModule::detectTop(image, WindowHandle,
		g_DetectParam.diameterUp, 
		g_DetectParam.diameterDown,
		g_DetectParam.gouWidthUP, 
		g_DetectParam.gouWidthDown,
		Result,
		&output_ExceptionInformtion);

	qDebug() <<  "Top Detect Information :	"<< output_ExceptionInformtion.ToString().Text();
}

void PicThreadLeft::QueueSaveImage(const HObject& Image, int maxnum)
{
	g_mutex_SaveImage.lock();
	if (g_SaveParam.SaveTopBadIndex <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveParam.SaveTopBadIndex, 4, 10, QChar('0'));

		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		
		//QTime start;
		//start.start();

		//for(int i=0;i<100;++i)	
		//	WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		//qDebug() << "SaveImage1：" << start.elapsed() / 100000.0 << "s";//输出计时
		////
		//QTime start1;
		//start1.start();

		//for (int i = 0; i<100; ++i)
		//	WriteImage(Image, "jpg", 0, saveImagePath.toStdString().c_str());
		//qDebug() << "SaveImage2：" << start1.elapsed() / 100000.0 << "s";//输出计时


		g_SaveParam.SaveTopBadIndex++;
	}
	else
	{
		g_SaveParam.SaveTopBadIndex = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveParam.SaveTopBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		g_SaveParam.SaveTopBadIndex++;
	}
	g_mutex_SaveImage.unlock();
}
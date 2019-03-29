#include "PicThreadMiddle.h"
#include <QTime>
//#include <Func.h>
#include "Global.h"
#include <qdebug.h>
#include "CHH.h"
#include "CHH2.h"
#include "Detect.h"

int PicThreadMiddle::num = 0;
HImage PicThreadMiddle::m_Image;

void PicThreadMiddle::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();


	if (m_Image.Key() != 0)
	{
		try
		{
			
			HTuple  hv_IsBad=0;
			OnHandle(m_Image, m_WindowHandle, &hv_IsBad);
			int result = hv_IsBad.I();

			//int result = 0;
			switch (result)
			{
			case Good:

				emit resultReady(Good, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("良品 "), "image", 120, 12, "black", "true");
				break;

			case Bad:

				emit resultReady(Bad, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("不良品 "), "image", 120, 12, "black", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Gou:

				emit resultReady(Gou, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("钩不良 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Cao:

				emit resultReady(Cao, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("槽不良 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Liantong:

				emit resultReady(Liantong, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("连铜 "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			default:

				emit resultReady(Bad, m_CameraId);
				//CHH::disp_message(m_WindowHandle, HTuple("无正确分类号"), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			}

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");

			//qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (num % 3)
			////if (!hv_IsBad.I())
			//{
			//	emit resultReady(MiddleGood, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "black", "true");
			//}
			//else
			//{
			//	emit resultReady(MiddleBad, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("Bad "), "image", 120, 12, "red", "true");

			//	if (IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}
			//上升沿使能
			//g_UpWaveEnable = true;
	
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			DispText(m_WindowHandle, "MiddleThread handle Error.", "image", 120, 12, "red", HTuple(), HTuple());
			//g_UpWaveEnable = true;
			emit resultReady(Bad, m_CameraId);
			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");
			CHH::disp_message(m_WindowHandle, HTuple("代码处理异常"), "image", 120, 12, "red", "true");
			if (IsSaveImage())
				QueueSaveImage(m_Image, SaveImageNum());

		}
	}
		
}

void PicThreadMiddle::OnHandle(HObject& image, const HTuple& WindowHandle, HTuple* Result)
{
	HTuple  output_ExceptionInformation("");
	//CHH::PingJie(m_Image, &m_Image, 1000, 30, 3, 10, &hv_DownRow);
	//DispObj(m_Image, WindowHandle);

	DetectModule::detectSide(image, WindowHandle,
		g_DetectParam.slotWidthUP, 
		g_DetectParam.slotWidthDown,
		g_DetectParam.maociWidth,
		g_DetectParam.maociHeight,
		g_DetectParam.scale_5,
		g_DetectParam.scale_6,
		g_DetectParam.scale_7,
		g_DetectParam.scale_8,
		Result, 
		&output_ExceptionInformation);

	qDebug() << "Side Detect Information :	"<<output_ExceptionInformation.ToString().Text();
}

void PicThreadMiddle::QueueSaveImage(const HObject& Image, int maxnum)
{
	g_mutex_SaveImage.lock();
	if (g_SaveParam.SaveSideBadIndex <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveParam.SaveSideBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		g_SaveParam.SaveSideBadIndex++;
	}
	else
	{
		g_SaveParam.SaveSideBadIndex = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveParam.SaveSideBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, g_SaveParam.SaveImageFormat.toStdString().c_str(), 0, saveImagePath.toStdString().c_str());
		g_SaveParam.SaveSideBadIndex++;
	}
	g_mutex_SaveImage.unlock();
}
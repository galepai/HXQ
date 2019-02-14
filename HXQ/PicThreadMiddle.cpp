#include "PicThreadMiddle.h"
#include <QTime>
//#include <Func.h>
#include "Global.h"
#include <qdebug.h>
#include "CHH.h"
#include "CHH2.h"
#include "Detect.h"

int PicThreadMiddle::num = 0;
void PicThreadMiddle::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();


	if (m_Image.Key() != 0)
	{
		try
		{
			
			HTuple hv_DownRow, hv_IsBad;
			HObject TileImage, ImageEmphasize;
		//	CHH::PingJie(m_Image, &m_Image, 700, 30, 3, 80, &hv_DownRow);
		//	DispObj(m_Image, m_WindowHandle);
			//Emphasize(m_Image, &ImageEmphasize, 3, 102, 1);
		//	SetColor(m_WindowHandle, "red");
		//	SetDraw(m_WindowHandle,"margin");
			
			//QTime ti;
			//ti.start();

		//	CHH2::PengShang_Camera2(ImageEmphasize, m_WindowHandle, &hv_IsBad);

			//qDebug() << "middle: "<<ti.elapsed() << "ms";

			OnHandle(m_Image, m_WindowHandle, &hv_IsBad);
			int result = hv_IsBad.I();

			switch (result)
			{
			case Good:

				emit resultReady(Good, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("良品 "), "image", 120, 12, "black", "true");
				break;

			case Bad:

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

				break;

			default:
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
			if (IsSaveImage())
				QueueSaveImage(m_Image, SaveImageNum());

		}
	}
		
}

void PicThreadMiddle::OnHandle(HObject& image, const HTuple& WindowHandle, HTuple* Result)
{
	HTuple hv_DownRow, output_ExceptionInformation;
	CHH::PingJie(m_Image, &m_Image, 1000, 30, 3, 10, &hv_DownRow);
	DispObj(m_Image, WindowHandle);

	DetectModule::detectSide(image, WindowHandle, 0.65, 0.55, 0.15, 0.15, Result, &output_ExceptionInformation);
}

void PicThreadMiddle::QueueSaveImage(const HObject& Image, int maxnum)
{

	if (g_SaveTopBadIndex <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveSideBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		g_SaveSideBadIndex++;
	}
	else
	{
		g_SaveTopBadIndex = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveSideBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		g_SaveSideBadIndex++;
	}
}
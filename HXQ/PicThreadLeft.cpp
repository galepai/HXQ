#include "PicThreadLeft.h"
#include <QTime>
#include "CHH.h"
#include "CHH2.h"
#include <QDebug>
//#include "Func.h"
#include "Global.h"
#include "../Detect/Detect/Detect.h"

int PicThreadLeft::num = 0;

void PicThreadLeft::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		try
		{
			HTuple hv_DownRow, hv_IsBad, hv_ModelHandle;
			HObject TileImage, ImageEmphasize;
			//CHH::PingJie(m_Image, &m_Image, 500, 40, 3, 40, &hv_DownRow);
			//DispObj(m_Image, m_WindowHandle);
			//Emphasize(m_Image, &ImageEmphasize, 3, 102, 1);

			//OnHandle(m_Image,m_WindowHandle,&hv_IsBad);

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");
			

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (hv_IsBad.I()==0)
			if (num % 3)
			{
				emit resultReady(Good,m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("良品 "), "image", 120, 12, "black", "true");
			}
			else if (num % 5)
			{
				emit resultReady(Gou,m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("钩不良 "), "image", 120, 12, "red", "true");

				if(IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
			}
			else if (num % 7)
			{
				emit resultReady(Cao, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("槽不良 "), "image", 120, 12, "red", "true");

				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
			}
			else if (num % 11)
			{
				emit resultReady(Liantong, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("槽内粘铜 "), "image", 120, 12, "red", "true");

				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
			}

		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			qDebug() << "ThreadLeft error:  " << error;
			QueueSaveImage(m_Image, 5);
		}

		
	}
	
}

void PicThreadLeft::OnHandle(HObject& image,const HTuple& WindowHandle,HTuple* Result)
{
	static int step = 1;
	step++;

	HObject circle;
	int row = 100 + 50 * step;
	if (row < 2000)
		GenCircle(&circle, 100 + 50 * step, 300, 100);
	else
	{
		GenCircle(&circle, 100 + 50 * step, 300, 100);
		step = 1;
	}

	DispObj(circle, WindowHandle);

	//结果
	*Result = 1;

	//引用动态库
	HTuple tmp_Result = 0;
	HTuple ExceptionInformation("");
	//DetectModule::detectTop(image, WindowHandle, &tmp_Result, &ExceptionInformation);

	qDebug() << ExceptionInformation.ToString().Text();
}

void PicThreadLeft::QueueSaveImage(const HObject& Image, int maxnum)
{

	if (g_SaveTopBadIndex <= maxnum)
	{
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveTopBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		g_SaveTopBadIndex++;
	}
	else
	{
		g_SaveTopBadIndex = 1;
		QString saveImagePath = QString(m_SaveImageDirName + "%1").arg(g_SaveTopBadIndex, 4, 10, QChar('0'));
		WriteImage(Image, "tiff", 0, saveImagePath.toStdString().c_str());
		g_SaveTopBadIndex++;
	}
}
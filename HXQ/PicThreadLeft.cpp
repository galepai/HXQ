#include "PicThreadLeft.h"
#include <QTime>
#include "CHH.h"
#include "CHH2.h"
#include <QDebug>

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
			DispObj(m_Image, m_WindowHandle);
			//Emphasize(m_Image, &ImageEmphasize, 3, 102, 1);

			//OnHandle(m_WindowHandle);
			//ReadClassMlp(getModel().toStdString().c_str(), &hv_ModelHandle);
			//CHH2::GB_Camera1(ImageEmphasize, m_Image, hv_ModelHandle, m_WindowHandle, &hv_IsBad);

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");
			

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (hv_IsBad.I()==0)
			if (num % 3)
			{
				emit resultReady(Good,m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Á¼Æ· "), "image", 120, 12, "black", "true");
			}
			else if (num % 5)
			{
				emit resultReady(Gou,m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("¹³²»Á¼ "), "image", 120, 12, "red", "true");
			}
			else if (num % 7)
			{
				emit resultReady(Cao, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("²Û²»Á¼ "), "image", 120, 12, "red", "true");
			}
			else if (num % 11)
			{
				emit resultReady(Liantong, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("²ÛÄÚÕ³Í­ "), "image", 120, 12, "red", "true");
			}
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			qDebug() << "ThreadLeft error:  " << error;
		}
	}

}

void PicThreadLeft::OnHandle(HTuple WindowHandle)
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
}
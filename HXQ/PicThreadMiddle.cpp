#include "PicThreadMiddle.h"
#include <QTime>
#include <Func.h>
#include "CHH.h"
#include "CHH2.h"

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
			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (num % 3)
			if (!hv_IsBad.I())
			{
				emit resultReady(MiddleGood, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "black", "true");
			}
			else
			{
				emit resultReady(MiddleBad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Bad "), "image", 120, 12, "red", "true");
			}
			//ÉÏÉýÑØÊ¹ÄÜ
			g_UpWaveEnable = true;
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			DispText(m_WindowHandle, "MiddleThread handle Error.", "image", 120, 12, "red", HTuple(), HTuple());
		}
	}
		
}

void PicThreadMiddle::OnHandle(HTuple WindowHandle)
{
	HTuple hv_DownRow;
	CHH::PingJie(m_Image, &m_Image, 1000, 30, 3, 10, &hv_DownRow);
	DispObj(m_Image, WindowHandle);
}
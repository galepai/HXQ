#include "PicThreadRight.h"
#include <QTime>
#include "CHH.h"
#include "CHH2.h"
#include <QDebug>


int PicThreadRight::num = 0;


void PicThreadRight::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
	
	if (m_Image.Key() != 0)
	{
		try
		{
			
			HTuple hv_DownRow, hv_IsBad;
			HObject TileImage,ImageEmphasize;
			CHH::PingJie(m_Image, &m_Image, 700, 30, 3, 40, &hv_DownRow);
			DispObj(m_Image, m_WindowHandle);
			SetDraw(m_WindowHandle, "margin");
			Emphasize(m_Image, &ImageEmphasize, 7, 101, 1);

		//	CHH2::BottomAndTop_Camera4(ImageEmphasize, m_Image, m_WindowHandle, m_ModelPath.toStdString().c_str(), &hv_IsBad);

			//QTime ti;
			//ti.start();

			CHH2::PengShang_Camera4(ImageEmphasize, m_Image, m_WindowHandle, &hv_IsBad);

			//qDebug() << "right: " << ti.elapsed() << "ms";
			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");


			//qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (num % 3)
			if (hv_IsBad.I()==0)
			{
				emit resultReady(RightGood, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "black", "true");
			}
			else
			{
				emit resultReady(RightBad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Bad "), "image", 120, 12, "red", "true");
				//QString saveImagePath = QString(QString("images/badImage/camera4/Camera4_") + "%1").arg(num, 4, 10, QChar('0'));
				//WriteImage(m_Image, "tiff", 0, saveImagePath.toStdString().c_str());
			}
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			qDebug() << "ThreadRight error:  " << error;
		}	
	}
		
}

void PicThreadRight::OnHandle(HTuple WindowHandle)
{
	HTuple hv_DownRow;
	CHH::PingJie(m_Image, &m_Image, 540, 30, 3, 50, &hv_DownRow);
	DispObj(m_Image, WindowHandle);

}
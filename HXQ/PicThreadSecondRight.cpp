#include "PicThreadSecondRight.h"
#include <QTime>
#include "CHH.h"
#include "CHH2.h"
#include <QDebug>

int PicThreadSecondRight::num = 0;
void PicThreadSecondRight::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
	
	if (m_Image.Key() != 0)
	{
		try
		{
			HTuple hv_DownRow, hv_MlpHandle, hv_IsBad;
			HObject ImageEmphasize;
			CHH::PingJie(m_Image, &m_Image, 1080, 30, 3, 40, &hv_DownRow);
			DispObj(m_Image, m_WindowHandle);
			Emphasize(m_Image, &ImageEmphasize, 7, 101, 1);

			CHH2::PengShang_Camera3(ImageEmphasize, m_Image, m_WindowHandle, &hv_IsBad);
			//ReadClassMlp("d:/github/CommutatorDetect/code/Model/classify_camera3_tiff.gmc", &hv_MlpHandle);
			//CHH2::PengShang2_Camera3(ImageEmphasize, m_Image, hv_MlpHandle, m_WindowHandle, &hv_IsBad);
			//ClearClassMlp(hv_MlpHandle);

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			//if (num % 3)
			if (!hv_IsBad.I())
			{
				emit resultReady(SecondRightGood,m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 420, "black", "true");
			}
			else
			{
				emit resultReady(SecondRightBad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("Blad "), "image", 120, 420, "red", "true");
				//QString saveImagePath = QString(QString("images/badImage/camera3/Camera3_") + "%1").arg(num, 4, 10, QChar('0'));
				//WriteImage(m_Image, "tiff", 0, saveImagePath.toStdString().c_str());
			}
				
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
			qDebug() << "ThreadSecondRight error:  " << error;
		}	
	}
		
}

void PicThreadSecondRight::OnHandle(HTuple WindowHandle)
{
	HTuple hv_DownRow, hv_IsBad;
	HObject ImageEmphasize;
	CHH::PingJie(m_Image, &m_Image, 1080, 30, 3, 40, &hv_DownRow);
	DispObj(m_Image, WindowHandle);
	Emphasize(m_Image, &ImageEmphasize, 7, 101, 1);
	CHH2::PengShang_Camera3(ImageEmphasize, m_Image, WindowHandle, &hv_IsBad);
}
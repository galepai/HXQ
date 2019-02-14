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

void PicThreadLeft::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		try
		{
			HTuple hv_IsBad;

			OnHandle(m_Image,m_WindowHandle,&hv_IsBad);
			int result = hv_IsBad.I();

			//int result = 1;
			switch (result)
			{
			case Good:

				emit resultReady(Good, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("��Ʒ "), "image", 120, 12, "black", "true");
				break;

			case Bad:

				emit resultReady(Bad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("����Ʒ "), "image", 120, 12, "black", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Gou:

				emit resultReady(Gou, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("������ "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Cao:

				emit resultReady(Cao, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("�۲��� "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			case Liantong:

				emit resultReady(Liantong, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("��ͭ "), "image", 120, 12, "red", "true");
				if (IsSaveImage())
					QueueSaveImage(m_Image, SaveImageNum());
				break;

			default:

				emit resultReady(Bad, m_CameraId);
				CHH::disp_message(m_WindowHandle, HTuple("����ȷ�����"), "image", 120, 12, "red", "true");
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
			//	CHH::disp_message(m_WindowHandle, HTuple("��Ʒ "), "image", 120, 12, "black", "true");
			//}
			//else if (num % 5)
			//{
			//	emit resultReady(Gou,m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("������ "), "image", 120, 12, "red", "true");

			//	if(IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}
			//else if (num % 7)
			//{
			//	emit resultReady(Cao, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("�۲��� "), "image", 120, 12, "red", "true");

			//	if (IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}
			//else if (num % 11)
			//{
			//	emit resultReady(Liantong, m_CameraId);
			//	CHH::disp_message(m_WindowHandle, HTuple("����ճͭ "), "image", 120, 12, "red", "true");

			//	if (IsSaveImage())
			//		QueueSaveImage(m_Image, SaveImageNum());
			//}

		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			DispText(m_WindowHandle, "LeftThread handle Error.", "image", 120, 12, "red", HTuple(), HTuple());
			emit resultReady(Bad, m_CameraId);
			CHH::disp_message(m_WindowHandle, HTuple("����ȷ�����"), "image", 120, 12, "red", "true");
			qDebug() << "ThreadLeft error:  " << error;
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

	////���
	//*Result = 1;

	//���ö�̬��
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
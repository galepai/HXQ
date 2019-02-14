#pragma once
#include "HalconCpp.h"

using namespace HalconCpp;

namespace DetectModule {
	/**			---------����˵��------------		

		input_Image:											����ͼ��
		input_WindowHandle:								���ھ��
		input_Param_BaoluoDiameterUp:			������ֱ������
		input_Param_BaoluoDiameterDown:		������ֱ������
		input_Param_GouWidthUp:						��������
		input_Param_GouWidthDown:				��������

		input_Param_CaoWidthUp:						�ۿ�����
		input_Param_CaoWidthDown:				�ۿ�����
		input_Param_MaociWidth:						ë�̿�
		input_Param_MaociHeight:						ë�̸�
		output_Result��										������
		output_ExceptionInformtion��				�쳣��Ϣ�����쳣��Ϊ��

		output_Result ���������壺
		static const int Good				= 0x00;
		static const int Bad					= 0x01;
		static const int Gou					= 0x02;
		static const int Cao					= 0x03;
		static const int Liantong			= 0x04;

	**/

	__declspec(dllexport) void detectTop(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle,
											const HTuple& input_Param_BaoluoDiameterUp, const HTuple& input_Param_BaoluoDiameterDown,
											const HTuple& input_Param_GouWidthUp, const HTuple& input_Param_GouWidthDown,
											HTuple* output_Result, HTuple* output_ExceptionInformtion);

	__declspec(dllexport) void detectSide(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle, 
											const HTuple& input_Param_CaoWidthUp, const HTuple& input_Param_CaoWidthDown,
											const HTuple& input_Param_MaociWidth, const HTuple& input_Param_GouWidthHeight,
											HTuple* output_Result, HTuple* output_ExceptionInformtion);

}

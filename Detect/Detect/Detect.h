#pragma once
#include "HalconCpp.h"

using namespace HalconCpp;

namespace DetectModule {
	/**			---------����˵��------------		

		input_Image:				����ͼ��
		input_Image:				���ھ��
		output_Result��				������
		output_ExceptionInformtion���쳣��Ϣ�����쳣��Ϊ��

	**/

	__declspec(dllexport) void detectTop(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_Result, HalconCpp::HString& output_ExceptionInformtion);

	__declspec(dllexport) void detectSide(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_Result, HalconCpp::HString& output_ExceptionInformtion);

}

#pragma once
#include "HalconCpp.h"

using namespace HalconCpp;

namespace DetectModule {
	/**			---------参数说明------------		

		input_Image:				输入图像
		input_Image:				窗口句柄
		output_Result：				输出结果
		output_ExceptionInformtion：异常信息，无异常是为空

	**/

	__declspec(dllexport) void detectTop(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_Result, HalconCpp::HString& output_ExceptionInformtion);

	__declspec(dllexport) void detectSide(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_Result, HalconCpp::HString& output_ExceptionInformtion);

}

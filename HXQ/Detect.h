#pragma once
#include "HalconCpp.h"

using namespace HalconCpp;

namespace DetectModule {
	/**			---------参数说明------------		

		input_Image:											输入图像
		input_WindowHandle:								窗口句柄
		input_Param_BaoluoDiameterUp:			包络线直径上限
		input_Param_BaoluoDiameterDown:		包络线直径下限
		input_Param_GouWidthUp:						钩宽上限
		input_Param_GouWidthDown:				钩宽下限

		input_Param_CaoWidthUp:						槽宽上限
		input_Param_CaoWidthDown:				槽宽下限
		input_Param_MaociWidth:						毛刺宽
		input_Param_MaociHeight:						毛刺高
		output_Result：										输出结果
		output_ExceptionInformtion：				异常信息，无异常是为空

		output_Result 输出结果定义：
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

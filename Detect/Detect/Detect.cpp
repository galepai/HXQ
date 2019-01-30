#include "Detect.h"

void DetectModule::detectTop(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle,
	const HTuple& input_Param_BaoluoDiameterUp, const HTuple& input_Param_BaoluoDiameterDown,
	const HTuple& input_Param_GouWidthUp, const HTuple& input_Param_GouWidthDown,
	HTuple* output_Result, HTuple* output_ExceptionInformtion)
{
	// √Ê’ÛÕº∆¨ºÏ≤‚
	HString errorInformation("");
	try
	{
		//
		HObject circle;
		GenCircle(&circle, 300, 300, 100);
		SetColor(input_WindowHandle, "green");
		DispObj(circle, input_WindowHandle);
		
		//
		*output_ExceptionInformtion = errorInformation;
	}
	catch (const HException& e)
	{
		*output_ExceptionInformtion = e.ErrorMessage();
	}
}


void DetectModule::detectSide(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle,
	const HTuple& input_Param_CaoWidthUp, const HTuple& input_Param_CaoWidthDown,
	const HTuple& input_Param_MaociWidth, const HTuple& input_Param_GouWidthHeight,
	HTuple* output_Result, HTuple* output_ExceptionInformtion)
{
	// œﬂ’ÛÕº∆¨ºÏ≤‚
	HString errorInformation("");
	try
	{
		//

		//
		*output_ExceptionInformtion = errorInformation;
	}
	catch (const HException& e)
	{
		*output_ExceptionInformtion = e.ErrorMessage();
	}
}
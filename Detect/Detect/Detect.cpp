#include "Detect.h"

void DetectModule::detectTop(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle, HTuple* output_Result, HTuple* output_ExceptionInformtion)
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


void DetectModule::detectSide(HalconCpp::HObject& input_Image, const HTuple& input_WindowHandle, HTuple* output_result, HalconCpp::HTuple* output_ExceptionInformtion)
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
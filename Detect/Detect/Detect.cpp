#include "Detect.h"

void DetectModule::detectTop(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_Result, HalconCpp::HString& output_ExceptionInformtion)
{
	// √Ê’ÛÕº∆¨ºÏ≤‚
	HString errorInformation("");
	try
	{
		HObject circle;
		GenCircle(&circle, 300, 300, 100);
		SetColor(input_WindowHandle, "green");
		DispObj(circle, input_WindowHandle);
		
	}
	catch (const HException& e)
	{
		errorInformation = e.ErrorMessage();
	}
}


void DetectModule::detectSide(HalconCpp::HObject& input_Image, const HalconCpp::HTuple& input_WindowHandle, int& output_result, HalconCpp::HString& output_ExceptionInformtion)
{
	// œﬂ’ÛÕº∆¨ºÏ≤‚
	try
	{

	}
	catch (const HException& e)
	{

	}
}
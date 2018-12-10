#ifndef CHH2_H
#define CHH2_H


#include "HalconCpp.h"
#include "CHH.h"

using namespace HalconCpp;

namespace CHH2
{	
	/*Camera1*/
	void GB_Camera1(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_MLPHandle,
		HTuple hv_WindowHandle, HTuple *hv_IsBad);
	void Gen_features_V1(HObject ho_Image, HTuple *hv_FeatureVector);
	void Gen_features_V2(HObject ho_Image, HTuple *hv_FeatureVector);
	void Gen_Sobel_features(HObject ho_Image, HTuple hv_Features, HTuple *hv_FeaturesExtended);

	/*Camera2*/
	void PengShang_Camera2(HObject ho_ImageEmphasize, HTuple hv_WindowHandle, HTuple *hv_IsBad);

	/*Camera3*/
	void PengShang2_Camera3(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_ModelHandle,
		HTuple hv_WindowHandle, HTuple *hv_IsBad); 

	void PengShang_Camera3(const HObject& ho_ImageEmphasize, const HObject& ho_TileImage, HTuple hv_WindowHandle,
		HTuple *hv_IsBad);

	/*Camera4*/
	void BottomAndTop_Camera4(HObject ho_ImageEmphasize, HObject ho_TileImage,
		HTuple hv_WindowHandle, HTuple hv_Model, HTuple *hv_IsBad);
	void PengShang_Camera4(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_WindowHandle,
		HTuple *hv_IsBad);
	
	
	
	
	

	

}

#endif // CHH2_H


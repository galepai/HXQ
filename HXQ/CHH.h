#ifndef CHH_H
#define CHH_H


#include "HalconCpp.h"

using namespace HalconCpp;

namespace CHH
{	//dev_open_window_fit_image(m_Image,0,0,-1,-1,0,&m_WindowHandle);
	void dev_open_window_fit_image (HObject ho_Image, HTuple hv_Row, HTuple hv_Column, 
		HTuple hv_WidthLimit, HTuple hv_HeightLimit,  const HTuple& FatherWindow,HTuple *hv_WindowHandle);
	void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, 
		HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
	void CreateRectangleROI(HalconCpp::HImage& Image,HalconCpp::HObject* Rectangle,const HalconCpp::HTuple& WindowHandle);
	void CreateCircleROI(HalconCpp::HImage& Image,HalconCpp::HObject* Circle,const HalconCpp::HTuple& WindowHandle);
	void gen_arrow_contour_xld (HObject *ho_Arrow, HTuple hv_Row1, HTuple hv_Column1, HTuple hv_Row2, HTuple hv_Column2,\
		HTuple hv_HeadLength, HTuple hv_HeadWidth);

	//图像拼接函数
	BOOL PingJie (HObject ho_Image, HObject *ho_TileImage, HTuple hv_MearsureColCenter, 
		HTuple hv_EdgeOffset, HTuple hv_Offset, HTuple hv_HorProjectionGrayVal, HTuple *hv_DownRow);
	//钩部特征
	void GB_Feature (HObject ho_GrayImage, HObject *ho_GBRegionUnion, HObject *ho_ErrorGBTopRegions, 
		HTuple hv_GBTopCutColumn, HTuple *hv_ErrorGBRows, HTuple *hv_GB_Num, HTuple *hv_RowGBTopCenters, 
		HTuple *hv_ColumnGBTopCenters, HTuple *hv_RowGBBottomCenters, HTuple *hv_ColumnGBBottomCenters);
	// 片底部坐标和片区域
	void PiecesAndBottomPoint (HObject ho_GrayImage, HObject *ho_SelectedRegionsPieces, 
		HTuple hv_Bottom_Column, HTuple *hv_RowBottomsCenter, HTuple *hv_ColumnBottomsCenter);
	//面阵圆的中心
	void AreaCenter (HObject ho_GrayImage, HObject ho_ROI, HObject *ho_ContCircle, 
		HTuple *hv_RowCenter, HTuple *hv_ColumnCenter, HTuple *hv_Radius);
	//检测顶面包络线
	void DetectDiameter (HObject ho_Image, HObject *ho_InnerTorenceCircle, HObject *ho_OuterTorenceCircle, 
		HObject *ho_Lines, HTuple hv_RowCenter, HTuple hv_ColumnCenter, HTuple hv_PixelLengthMM, 
		HTuple hv_StandDiameterMM, HTuple hv_TorenceMM, HTuple hv_WindowHandle, HTuple *hv_Number, 
		HTuple *hv_ShortNum, HTuple *hv_pcDistanceMaxs, HTuple *hv_IsBadProduce,HTuple *hv_ErrorNum);

	void DeteterDiameter2 (HObject ho_Image, HObject *ho_TransRegions, HObject *ho_TransPoints, 
		HObject *ho_BadLines, HObject *ho_GoodLines, HObject *ho_RegionCircle, HTuple hv_PixelLengthMM, 
		HTuple hv_OffsetMM, HTuple hv_StandDiameter, HTuple hv_Torence, HTuple hv_WindowsHandle, 
		HTuple *hv_IsHavePIcError, HTuple *hv_GBIsTooShortError, HTuple *hv_GBRowCeters, 
		HTuple *hv_GBColumnCenters, HTuple *hv_DiameterLens);

	//检测顶面气孔
	void AreaHoles (HObject ho_GrayImage, HObject *ho_XYTransRegionError, HTuple hv_RowCenter, 
		HTuple hv_ColumnCenter, HTuple hv_OutRadius, HTuple hv_InnerRadius, HTuple hv_ErrorWidthMM, 
		HTuple hv_ErrorHeightMM, HTuple hv_WindowHandle, HTuple hv_PixelLength, HTuple dynDarkValue,HTuple *hv_Number, 
		HTuple *hv_WidthValue, HTuple *hv_HeightValue, HTuple *hv_AreaValue);
	//检测顶面外沿气孔
	void AreaOutHole (HObject ho_Image, HObject *ho_BadTransRegions, HTuple hv_RowCenter, 
		HTuple hv_ColumnCenter, HTuple hv_WindowHandle, HTuple *hv_GBIsOutHoles);

	//检测槽边缘检测
	void Gap_EdgesAndRegressLines (HObject ho_EmphaImage, HObject ho_ObjectSelected, 
		HObject *ho_SelectedEdges, HObject *ho_RegressContour0, HObject *ho_RegressContour1, 
		HObject *ho_GapWidthContours, HObject *ho_Arrow, HObject *ho_RegionError, HObject *ho_ErrorEdge, 
		HTuple *hv_Gap_LRCLengths, HTuple *hv_RowRegressCenter, HTuple *hv_ColRegressCenter);
	//检测槽特征（要引用Gap_EdgesAndRegressLines函数）
	void GapFeature (HObject ho_EmphaImage, HObject *ho_RegionErrorOthers, HObject *ho_ErrorEdges, 
		HObject *ho_RegionErrors, HObject *ho_Edges, HObject *ho_Arrows, HObject *ho_RegressContours0, 
		HObject *ho_RegressContours1, HObject *ho_HorArrowlines, HObject *ho_GapToGapArrows, 
		HObject *ho_GapToGapContours, HTuple hv_side_column, HTuple hv_GapCutColumn, 
		HTuple hv_WindowHandle, HTuple hv_LengthPerColPiexl, HTuple hv_DgreePerPiexl, 
		HTuple *hv_RowRegressCenters, HTuple *hv_ColRegressCenters, HTuple *hv_GapCenterWidths, 
		HTuple *hv_GapCenterWidthsMM, HTuple *hv_GapsDegree);
	//检测片高
	void PieceHeight (HObject ho_EmphaImage, HObject *ho_RegionLines, HObject *ho_BadMaxLines, 
		HObject *ho_BadMinLines, HTuple hv_GBTopCutCol, HTuple hv_PieceBottomCutCol, 
		HTuple hv_LengthPerRowPiexl, HTuple hv_StandHeight, HTuple hv_Torence, HTuple hv_GBPhiThreshold, 
		HTuple hv_WindowHandle, HTuple *hv_PieceHeightIsError, HTuple *hv_GBIsError, 
		HTuple *hv_ErrorMaxNum, HTuple *hv_ErrorMinNum, HTuple *hv_HeightMM, HTuple *hv_RowGBTopCenters, 
		HTuple *hv_ColGBTopCenters, HTuple *hv_RowPieceBottomCenters, HTuple *hv_ColPieceBottomCenters);

	void PieceHeight222 (HObject ho_EmphaImage, HObject *ho_RegionLines, HObject *ho_RegionGBHeightLines, 
		HObject *ho_BadMaxLines, HObject *ho_BadMinLines, HTuple hv_GBTopCutCol, HTuple hv_PieceBottomCutCol, 
		HTuple hv_LengthPerRowPiexl, HTuple hv_StandPieceHeight, HTuple hv_PieceHeightTorence, 
		HTuple hv_StandGBHeight, HTuple hv_GBHeightTorence, HTuple hv_GBPhiThreshold, 
		HTuple hv_WindowHandle, HTuple *hv_GBTopIsNo18Error, HTuple *hv_GBBottomIsNo18Error, 
		HTuple *hv_PieceBottomIsNo18Error, HTuple *hv_PieceHeightIsError, HTuple *hv_GBHeightIsError, 
		HTuple *hv_GBIsOffsetError, HTuple *hv_PieceHeights, HTuple *hv_GBHeights, HTuple *hv_Phi, 
		HTuple *hv_RowGBTopCenters, HTuple *hv_ColGBTopCenters, HTuple *hv_RowPieceBottomCenters, 
		HTuple *hv_ColPieceBottomCenters);

	//面阵图片第1钩的解译，展开图需使用其相关参数
	void FirstPhi (HObject ho_GrayImage, HObject *ho_MearsureCircle, HObject *ho_StartCircles, 
		HObject *ho_CenterCircles, HObject *ho_EndCircles, HTuple hv_ArcRowCenter_Measure, 
		HTuple hv_ArcColumnCenter_Measure, HTuple hv_ArcRadius_Measure, HTuple *hv_AngleStart, 
		HTuple *hv_AngleEnd, HTuple *hv_Dgree_Center, HTuple *hv_Dgree_Start, HTuple *hv_Dgree_End);
	//检测面阵图中钩部的缺陷，需配合FirstPhi使用
	void DM_Detect_GB (HObject ho_GrayImage, HObject *ho_XYTransRegionError, HTuple hv_RowCenter, 
		HTuple hv_ColumnCenter, HTuple hv_Mearsure_Radius, HTuple hv_RadiusStart, HTuple hv_RadiusEnd, HTuple Dist_Deviation,
		HTuple *hv_ErrorNum);
//测量槽宽和槽宽边缘异常、槽分度
	void NewGapWidthAndBadGap  (HObject ho_EmphaImage, HObject *ho_GapUnFilterRegions, 
		HObject *ho_GapFilterRegions, HObject *ho_GapBadRegions, HObject *ho_MaxRegions, 
		HObject *ho_MixRegions, HObject *ho_Arrows, HTuple hv_GapBottomCutColumn, HTuple hv_GapTopCutColumn, 
		HTuple hv_threshold_min, HTuple hv_threshold_max, HTuple hv_GapBadHeight, HTuple hv_GapBadWidth, 
		HTuple hv_LengthPerColPiexl, HTuple hv_LengthPerRowPiexl, HTuple hv_StandGapWidth, 
		HTuple hv_GapWidthTorence, HTuple hv_StandGapAngle, HTuple hv_GapAngleTorence, 
		HTuple hv_WindowHandle, HTuple *hv_GapWidth_GapAngleIsError, HTuple *hv_GapIsConvex_Que, 
		HTuple *hv_GapWidthIsError, HTuple *hv_GapAngleIsError, HTuple *hv_GapLenths, 
		HTuple *hv_GapAngles, HTuple *hv_RowGapCenters, HTuple *hv_ColGapCenters);

	void NewGapWidthAndBadGap222 (HObject ho_EmphaImage, HObject *ho_GapUnFilterRegions, 
		HObject *ho_GapFilterRegions, HTuple hv_GapBottomCutColumn, HTuple hv_GapTopCutColumn, 
		HTuple hv_threshold_min, HTuple hv_threshold_max, HTuple hv_GapBadHeight, HTuple hv_GapBadWidth, 
		HTuple hv_LengthPerColPiexl, HTuple hv_LengthPerRowPiexl, HTuple hv_StandGapWidth, 
		HTuple hv_GapWidthTorence, HTuple hv_StandGapAngle, HTuple hv_GapAngleTorence, 
		HTuple hv_WindowHandle, HTuple *hv_GapRegionNoEqual19, HTuple *hv_GapIsConvex_Que, 
		HTuple *hv_GapWidthIsError, HTuple *hv_GapAngleIsError, HTuple *hv_GapLenths, 
		HTuple *hv_GapAngles, HTuple *hv_RowGapCenters, HTuple *hv_ColGapCenters);

	//面阵，标定圆心和像素大小
	void Area_CalibraCenter_mm (HObject ho_CalibraImage, HObject *ho_ContCircle, 
		HObject *ho_CrossCenter, HTuple hv_WindowHandle, HTuple hv_StandDiameter, HTuple *hv_RowCenter, 
		HTuple *hv_ColumnCenter, HTuple *hv_LenPixelToMM);

	//自动搜寻电机中心坐标
	BOOL ProduceCenter(HObject& Image,	HObject* ContCircle,	HTuple* RowCenter,	HTuple* ColumnCenter);

	//自动搜寻电机中心坐标
	void AutoCirclePoint (HObject ho_Image, HObject *ho_ContCircle, HTuple hv_DeadRowCenter, 
		HTuple hv_DeadColumnCenter, HTuple hv_WindowHandle, HTuple *hv_DMIsNoCenterPoint, 
		HTuple *hv_RowCenter, HTuple *hv_ColumnCenter);
}

#endif // CHH_H


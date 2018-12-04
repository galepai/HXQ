
#include "CHH2.h"
#include "qdebug.h"


/****************Camera1******************/
void CHH2::Gen_features_V1(HObject ho_Image, HTuple *hv_FeatureVector)
{

	// Local iconic variables
	HObject  ho_Zoomed1, ho_Zoomed2;

	// Local control variables
	HTuple  hv_Width, hv_Height;

	(*hv_FeatureVector) = HTuple();
	GetImageSize(ho_Image, &hv_Width, &hv_Height);

	CHH2::Gen_Sobel_features(ho_Image, (*hv_FeatureVector), &(*hv_FeatureVector));

	ZoomImageFactor(ho_Image, &ho_Zoomed1, 0.5, 0.5, "constant");
	CHH2::Gen_Sobel_features(ho_Zoomed1, (*hv_FeatureVector), &(*hv_FeatureVector));

	ZoomImageFactor(ho_Zoomed1, &ho_Zoomed2, 0.5, 0.5, "constant");

	(*hv_FeatureVector) = ((*hv_FeatureVector).TupleConcat(hv_Width)).TupleConcat(hv_Height);
	(*hv_FeatureVector) = (*hv_FeatureVector).TupleReal();
	return;

}

void CHH2::Gen_features_V2(HObject ho_Image, HTuple *hv_FeatureVector)
{

	// Local iconic variables
	HObject  ho_Zoomed1, ho_Zoomed2;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_HorProjection;
	HTuple  hv_VertProjection;

	(*hv_FeatureVector) = HTuple();
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	CHH2::Gen_Sobel_features(ho_Image, (*hv_FeatureVector), &(*hv_FeatureVector));
	ZoomImageFactor(ho_Image, &ho_Zoomed1, 0.5, 0.5, "constant");
	CHH2::Gen_Sobel_features(ho_Zoomed1, (*hv_FeatureVector), &(*hv_FeatureVector));
	ZoomImageFactor(ho_Zoomed1, &ho_Zoomed2, 0.5, 0.5, "constant");
	GrayProjections(ho_Image, ho_Zoomed2, "simple", &hv_HorProjection, &hv_VertProjection);

	(*hv_FeatureVector) = (((*hv_FeatureVector).TupleConcat(hv_Width)).TupleConcat(hv_Height)).TupleConcat(hv_VertProjection);
	(*hv_FeatureVector) = (*hv_FeatureVector).TupleReal();
	return;

}

void CHH2::Gen_Sobel_features(HObject ho_Image, HTuple hv_Features, HTuple *hv_FeaturesExtended)
{

	// Local iconic variables
	HObject  ho_EdgeAmplitude;

	// Local control variables
	HTuple  hv_Energy, hv_Correlation, hv_Homogeneity;
	HTuple  hv_Contrast, hv_AbsoluteHistoEdgeAmplitude;

	//Coocurrence matrix for 0 deg and 90 deg:
	/*CoocFeatureImage(ho_Image, ho_Image, 6, 0, &hv_Energy, &hv_Correlation, &hv_Homogeneity,
	&hv_Contrast);*/
	CoocFeatureImage(ho_Image, ho_Image, 6, 90, &hv_Energy, &hv_Correlation, &hv_Homogeneity,
		&hv_Contrast);
	//Absolute histogram of edge amplitudes:
	SobelAmp(ho_Image, &ho_EdgeAmplitude, "sum_abs", 3);
	GrayHistoAbs(ho_EdgeAmplitude, ho_EdgeAmplitude, 8, &hv_AbsoluteHistoEdgeAmplitude);

	(*hv_FeaturesExtended).Clear();
	(*hv_FeaturesExtended).Append(hv_Features);
	(*hv_FeaturesExtended).Append(hv_Energy);
	(*hv_FeaturesExtended).Append(hv_Correlation);
	(*hv_FeaturesExtended).Append(hv_Homogeneity);
	(*hv_FeaturesExtended).Append(hv_Contrast);
	(*hv_FeaturesExtended) = (*hv_FeaturesExtended).TupleConcat(hv_AbsoluteHistoEdgeAmplitude);


	return;
}

void CHH2::GB_Camera1(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_MLPHandle,
		HTuple hv_WindowHandle, HTuple *hv_IsBad)
	{
		try{
			HObject  ho_Region, ho_RegionOpening, ho_RegionClosing;
			HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionTrans;
			HObject  ho_BadObjects, ho_ObjectSelected, ho_ImagePart;
			HObject  ho_Rectangle;

			// Local control variables
			HTuple  hv_NumInput, hv_NumHidden, hv_NumOutput;
			HTuple  hv_OutputFunction, hv_Preprocessing, hv_NumComponents;
			HTuple  hv_GBNum, hv_Index, hv_Value, hv_FeatureVector;
			HTuple  hv_Class, hv_Confidence, hv_BadNum;

			(*hv_IsBad) = 0;
			GetParamsClassMlp(hv_MLPHandle, &hv_NumInput, &hv_NumHidden, &hv_NumOutput, &hv_OutputFunction,
				&hv_Preprocessing, &hv_NumComponents);

			Threshold(ho_ImageEmphasize, &ho_Region, 200, 255);
			OpeningCircle(ho_Region, &ho_RegionOpening, 25.5);
			ClosingCircle(ho_RegionOpening, &ho_RegionClosing, 5.5);
			Connection(ho_RegionClosing, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 350, 1000);
			CountObj(ho_SelectedRegions, &hv_GBNum);
			ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "rectangle1");

			GenEmptyObj(&ho_BadObjects);
			if (0 != (hv_GBNum == 18))
			{
				{
					HTuple end_val13 = hv_GBNum;
					HTuple step_val13 = 1;
					for (hv_Index = 1; hv_Index.Continue(end_val13, step_val13); hv_Index += step_val13)
					{
						SelectObj(ho_RegionTrans, &ho_ObjectSelected, hv_Index);
						RegionFeatures(ho_ObjectSelected, ((HTuple("row1").Append("column2")).Append("height")),
							&hv_Value);
						CropPart(ho_TileImage, &ho_ImagePart, HTuple(hv_Value[0]), HTuple(hv_Value[1]) - 400,
							800, HTuple(hv_Value[2]));
						GenRectangle1(&ho_Rectangle, HTuple(hv_Value[0]), HTuple(hv_Value[1]) - 400, HTuple(hv_Value[0]) + HTuple(hv_Value[2]),
							(HTuple(hv_Value[1]) - 400) + 800);
						if (0 != (hv_NumInput == 274))
						{
							CHH2::Gen_features_V2(ho_ImagePart, &hv_FeatureVector);
						}
						else
						{
							CHH2::Gen_features_V1(ho_ImagePart, &hv_FeatureVector);
						}

						ClassifyClassMlp(hv_MLPHandle, hv_FeatureVector, 1, &hv_Class, &hv_Confidence);
						if (0 != (hv_Class == 1))
						{
							ConcatObj(ho_BadObjects, ho_Rectangle, &ho_BadObjects);
						}
					}
				}

				//**********display BadObject
				CountObj(ho_BadObjects, &hv_BadNum);
				if (HDevWindowStack::IsOpen())
					DispObj(ho_TileImage, hv_WindowHandle);
				if (0 != (hv_BadNum>0))
				{
					(*hv_IsBad) = 1;
					SetLineWidth(hv_WindowHandle, 2);
					SetDraw(hv_WindowHandle, "margin");
					SetColor(hv_WindowHandle, "red");
					if (HDevWindowStack::IsOpen())
						DispObj(ho_BadObjects, hv_WindowHandle);
				}
				
			}
			else
			{
				//***********钩部区域不足18
				(*hv_IsBad) = 1;
				if (HDevWindowStack::IsOpen())
					DispText(hv_WindowHandle, "not enough 18 pieces.", "image", 200,
					150, "black", HTuple(), HTuple());
			}
			return;
		}
		catch (HException& except)
		{
			qDebug() << "error: " << except.ErrorMessage().Text();
			(*hv_IsBad) = 1;
			CHH::disp_message(hv_WindowHandle, "GB_Camera1 Code Error.", "image", 400, 200, "red", "true");
			return;
		}
	}

/****************Camera1****************************************************/







/****************Camera2******************/
void CHH2::PengShang_Camera2(HObject ho_ImageEmphasize, HTuple hv_WindowHandle, HTuple *hv_IsBad)
{
	try{
	
		
		// Local iconic variables
		HObject  ho_ImageOpening, ho_Region, ho_RegionOpening;
		HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionTrans;
		HObject  ho_RegionFillUp, ho_Rois, ho_SelectedObject, ho_Rectangle;
		HObject  ho_IntersectionRoi, ho_SelectedROI, ho_RegionLine;
		HObject  ho_IntersectionLine, ho_UpRegion, ho_DownRegion;
		HObject  ho_PieceGBTopRoi, ho_ImageReduced1, ho_Regions;
		HObject  ho_RegionClosing, ho_ConnectedRegions2, ho_GouTop;
		HObject  ho_ROI, ho_ImageReduced, ho_Region1, ho_ConnectedRegions1;
		HObject  ho_MaybeBadRegions, ho_BadObjects, ho_ObjectSelected;
		HObject  ho_Up, ho_Down, ho_RegionDilation;

		// Local control variables
		HTuple  hv_WIDTH, hv_ImageWidth, hv_ImageHeight;
		HTuple  hv_Number, hv_Col1, hv_offset_GbDistances, hv_GbToPiece_Threshold;
		HTuple  hv_offset, hv_offsetWidth, hv_offsetHeight, hv_Index;
		HTuple  hv_Col2, hv_row, hv_Area2, hv_PieceGBTopRoiRow;
		HTuple  hv_Column2, hv_Area1, hv_GouTopRow, hv_Column1;
		HTuple  hv_distance_offset, hv_Num, hv_index, hv_Ra, hv_Rb;
		HTuple  hv_Area, hv_GrayMean, hv_BadNumber, hv_paichu, hv_Index1;
		HTuple  hv_PHI, hv_Row, hv_Column, hv_length, hv_UpGrayMean;
		HTuple  hv_DownGrayMean;

		(*hv_IsBad) = 0;

		GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 5, 5);
		hv_WIDTH = 870;
		GetImageSize(ho_ImageOpening, &hv_ImageWidth, &hv_ImageHeight);
		Threshold(ho_ImageEmphasize, &ho_Region, 120, 255);
		OpeningCircle(ho_Region, &ho_RegionOpening, 8.5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("height")),
			"and", (HTuple(150000).Append(200)), (HTuple(9999999).Append(800)));
		CountObj(ho_SelectedRegions, &hv_Number);
		if (0 != (hv_Number == 18))
		{
			ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "convex");
			FillUp(ho_RegionTrans, &ho_RegionFillUp);
			SortRegion(ho_RegionFillUp, &ho_RegionFillUp, "upper_left", "true", "row");
			//region_features (RegionFillUp, 'column1', Col1)
			//tuple_mean (Col1, Col1)
			//union1 (RegionFillUp, RegionUnion)

			GenEmptyObj(&ho_Rois);
			hv_offset_GbDistances = HTuple();
			//*钩中心的偏移阀值
			hv_GbToPiece_Threshold = 54;
			//*选取钩上下两区域
			hv_offset = 5;
			hv_offsetWidth = 90;
			hv_offsetHeight = 90;

			for (hv_Index = 1; hv_Index <= 18; hv_Index += 1)
			{
				SelectObj(ho_RegionFillUp, &ho_SelectedObject, hv_Index);
				RegionFeatures(ho_SelectedObject, "column1", &hv_Col1);
				GenRectangle1(&ho_Rectangle, 0, hv_Col1 + 10, hv_ImageHeight, hv_Col1 + hv_WIDTH);
				Intersection(ho_Rectangle, ho_SelectedObject, &ho_IntersectionRoi);
				ErosionCircle(ho_IntersectionRoi, &ho_SelectedROI, 8.5);

				RegionFeatures(ho_SelectedROI, "column2", &hv_Col2);
				GenRegionLine(&ho_RegionLine, 0, hv_Col2, hv_ImageHeight, hv_Col2);
				Intersection(ho_RegionLine, ho_SelectedROI, &ho_IntersectionLine);
				RegionFeatures(ho_IntersectionLine, (HTuple("row1").Append("row2")), &hv_row);
				GenRectangle1(&ho_UpRegion, HTuple(hv_row[0]) + hv_offset, hv_Col2, (HTuple(hv_row[0]) + hv_offset) + hv_offsetHeight,
					hv_Col2 + hv_offsetWidth);
				GenRectangle1(&ho_DownRegion, (HTuple(hv_row[1]) - hv_offset) - hv_offsetHeight,
					hv_Col2, HTuple(hv_row[1]) - hv_offset, hv_Col2 + hv_offsetWidth);
				Union2(ho_SelectedROI, ho_UpRegion, &ho_SelectedROI);
				Union2(ho_SelectedROI, ho_DownRegion, &ho_SelectedROI);
				ConcatObj(ho_Rois, ho_SelectedROI, &ho_Rois);

				//*******************钩部偏移设置
				GenRectangle1(&ho_PieceGBTopRoi, HTuple(hv_row[0]) + hv_offset, hv_Col2, HTuple(hv_row[1]) - hv_offset,
					hv_Col2 + hv_offsetWidth);
				AreaCenter(ho_PieceGBTopRoi, &hv_Area2, &hv_PieceGBTopRoiRow, &hv_Column2);
				ReduceDomain(ho_ImageOpening, ho_PieceGBTopRoi, &ho_ImageReduced1);

				Threshold(ho_ImageReduced1, &ho_Regions, 0, 120);
				ClosingCircle(ho_Regions, &ho_RegionClosing, 3.5);
				Connection(ho_RegionClosing, &ho_ConnectedRegions2);
				SelectShape(ho_ConnectedRegions2, &ho_GouTop, "height", "and", 150, 500);
				AreaCenter(ho_GouTop, &hv_Area1, &hv_GouTopRow, &hv_Column1);

				TupleAbs(hv_GouTopRow - hv_PieceGBTopRoiRow, &hv_distance_offset);
				if (0 != (hv_distance_offset >= hv_GbToPiece_Threshold))
				{
					if (HDevWindowStack::IsOpen())
						DispObj(ho_PieceGBTopRoi, hv_WindowHandle);
					(*hv_IsBad) = 1;
				}
				hv_offset_GbDistances = hv_offset_GbDistances.TupleConcat(hv_distance_offset);
				//**************************
			}


			Union1(ho_Rois, &ho_ROI);

			//gen_rectangle1 (Rectangle, 0, Col1+20, ImageHeight, Col1+WIDTH)
			//intersection (Rectangle, RegionUnion, RoiUnion)
			//erosion_circle (RoiUnion, ROI, 8.5)
			ReduceDomain(ho_ImageOpening, ho_ROI, &ho_ImageReduced);

			Threshold(ho_ImageReduced, &ho_Region1, 0, 150);
			ClosingCircle(ho_Region1, &ho_Region1, 3.5);
			Connection(ho_Region1, &ho_ConnectedRegions1);
			SelectShape(ho_ConnectedRegions1, &ho_MaybeBadRegions, "area", "and", 120, 99999);
			CountObj(ho_MaybeBadRegions, &hv_Num);
			GenEmptyObj(&ho_BadObjects);
			{
				HTuple end_val77 = hv_Num;
				HTuple step_val77 = 1;
				for (hv_index = 1; hv_index.Continue(end_val77, step_val77); hv_index += step_val77)
				{
					SelectObj(ho_MaybeBadRegions, &ho_ObjectSelected, hv_index);
					RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
					RegionFeatures(ho_ObjectSelected, "rb", &hv_Rb);
					RegionFeatures(ho_ObjectSelected, "area", &hv_Area);
					GrayFeatures(ho_ObjectSelected, ho_ImageOpening, "mean", &hv_GrayMean);
					if (0 != ((hv_Ra / hv_Rb)>7.5))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (hv_Area>3000))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(HTuple(hv_GrayMean<60).TupleAnd((hv_Ra / hv_Rb)>4.5)).TupleAnd(hv_Ra>30)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(hv_Area>1500).TupleAnd(hv_GrayMean<80)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}

				}
			}
			CountObj(ho_BadObjects, &hv_BadNumber);
			hv_paichu = HTuple();
			{
				HTuple end_val96 = hv_BadNumber;
				HTuple step_val96 = 1;
				for (hv_Index1 = 1; hv_Index1.Continue(end_val96, step_val96); hv_Index1 += step_val96)
				{
					SelectObj(ho_BadObjects, &ho_ObjectSelected, hv_Index1);
					RegionFeatures(ho_ObjectSelected, "phi", &hv_PHI);
					GrayFeatures(ho_ObjectSelected, ho_ImageOpening, "mean", &hv_GrayMean);
					if (0 != (hv_PHI<0))
					{
						hv_PHI = -hv_PHI;
					}
					if (0 != (HTuple(hv_PHI<(HTuple(10).TupleRad())).TupleAnd(hv_GrayMean<80.0)))
					{
						RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
						RegionFeatures(ho_ObjectSelected, (HTuple("row1").Append("row2")), &hv_Row);
						RegionFeatures(ho_ObjectSelected, (HTuple("column1").Append("column2")),
							&hv_Column);
						//tuple_mean (Row, RowMean)
						//tuple_mean (Column, ColumnMean)
						hv_length = HTuple(hv_Column[1]) - HTuple(hv_Column[0]);
						GenRectangle1(&ho_Up, HTuple(hv_Row[0]) - 20, HTuple(hv_Column[0]), HTuple(hv_Row[0]) - 10,
							HTuple(hv_Column[1]));
						GrayFeatures(ho_Up, ho_ImageOpening, "mean", &hv_UpGrayMean);
						GenRectangle1(&ho_Down, HTuple(hv_Row[1]) + 10, HTuple(hv_Column[0]), HTuple(hv_Row[1]) + 20,
							HTuple(hv_Column[1]));
						GrayFeatures(ho_Down, ho_ImageOpening, "mean", &hv_DownGrayMean);
						if (0 != (HTuple(HTuple(hv_UpGrayMean<3.0).TupleAnd(hv_DownGrayMean>100.0)).TupleOr(HTuple(hv_UpGrayMean>100.0).TupleAnd(hv_DownGrayMean<100.0))))
						{
							hv_paichu = hv_paichu.TupleConcat(hv_Index1);
						}
					}
				}
			}
			RemoveObj(ho_BadObjects, &ho_BadObjects, hv_paichu);
			CountObj(ho_BadObjects, &hv_BadNumber);

			//***********
			hv_paichu = HTuple();
			{
				HTuple end_val124 = hv_BadNumber;
				HTuple step_val124 = 1;
				for (hv_Index1 = 1; hv_Index1.Continue(end_val124, step_val124); hv_Index1 += step_val124)
				{
					SelectObj(ho_BadObjects, &ho_ObjectSelected, hv_Index1);
					RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
					if (0 != (hv_Ra<50))
					{
						hv_paichu = hv_paichu.TupleConcat(hv_Index1);
					}
				}
			}
			RemoveObj(ho_BadObjects, &ho_BadObjects, hv_paichu);
			CountObj(ho_BadObjects, &hv_BadNumber);
			//*******************

			if (0 != (hv_BadNumber>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects, &ho_RegionDilation, 5.5);
				if (HDevWindowStack::IsOpen())
					DispObj(ho_RegionDilation, hv_WindowHandle);
			}


		}
		else
		{
			(*hv_IsBad) = 1;
			if (HDevWindowStack::IsOpen())
				DispText(hv_WindowHandle, "片不等于18", "window", "top", "left",
				"black", HTuple(), HTuple());
		}

		return;

	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "PengShang_Camera2 Code Error.", "image", 400, 200, "red", "true");
		return;
	}
}
/****************Camera2****************************************************/







/****************Camera3******************/
void CHH2::PengShang2_Camera3(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_ModelHandle,
	HTuple hv_WindowHandle, HTuple *hv_IsBad)
{
	try{
		// Local iconic variables
		HObject  ho_ImageOpening, ho_ImageClosing, ho_Regions;
		HObject  ho_RegionOpening, ho_ConnectedRegions, ho_SelectedRegions;
		HObject  ho_RegionClosing, ho_RegionFillUp, ho_Rectangle;
		HObject  ho_RegionUnion, ho_RegionIntersection, ho_RegionErosion;
		HObject  ho_ImageReduced, ho_Region, ho_ConnectedRegions1;
		HObject  ho_BadRegions1, ho_RegionUnion1, ho_RegionFillUp1;
		HObject  ho_ConnectedRegions2, ho_RegionTrans, ho_BadObjects;
		HObject  ho_ObjectSelected, ho_CropImage;

		// Local control variables
		HTuple  hv_WIDTH, hv_SaveImagePath, hv_Number;
		HTuple  hv_Col2s, hv_Col2, hv_Col1, hv_Index, hv_Value;
		HTuple  hv_FeatureVector, hv_FoundClassIDs, hv_Confidence;
		HTuple  hv_Num;

		//****************************
		//**
		//****************************

		(*hv_IsBad) = 0;
		hv_WIDTH = 870;
		hv_SaveImagePath = "f:/mlp/auto3/";

		GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 7, 7);
		GrayClosingRect(ho_ImageOpening, &ho_ImageClosing, 5, 5);

		Threshold(ho_ImageClosing, &ho_Regions, 20, 255);
		OpeningCircle(ho_Regions, &ho_RegionOpening, 15.5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("height").Append("column2")),
			"and", (HTuple(400).Append(0)), (HTuple(800).Append(1800)));

		CountObj(ho_SelectedRegions, &hv_Number);
		ClosingCircle(ho_SelectedRegions, &ho_RegionClosing, 10.5);
		FillUp(ho_RegionClosing, &ho_RegionFillUp);
		RegionFeatures(ho_RegionFillUp, "column2", &hv_Col2s);
		TupleMean(hv_Col2s, &hv_Col2);
		hv_Col1 = hv_Col2 - hv_WIDTH;
		GenRectangle1(&ho_Rectangle, 0, hv_Col1, 9999, hv_Col2);
		Union1(ho_RegionFillUp, &ho_RegionUnion);
		Intersection(ho_Rectangle, ho_RegionUnion, &ho_RegionIntersection);

		ErosionCircle(ho_RegionIntersection, &ho_RegionErosion, 6.5);

		ReduceDomain(ho_ImageClosing, ho_RegionErosion, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Region, 0, 200);
		ClosingCircle(ho_Region, &ho_RegionClosing, 5.5);
		Connection(ho_RegionClosing, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_BadRegions1, "area", "and", 700, 99999);
		Union1(ho_BadRegions1, &ho_RegionUnion1);
		FillUp(ho_RegionUnion1, &ho_RegionFillUp1);
		Connection(ho_RegionFillUp1, &ho_ConnectedRegions2);
		ShapeTrans(ho_ConnectedRegions2, &ho_RegionTrans, "rectangle1");
		CountObj(ho_RegionTrans, &hv_Number);
		GenEmptyObj(&ho_BadObjects);
		{
			HTuple end_val39 = hv_Number;
			HTuple step_val39 = 1;
			for (hv_Index = 1; hv_Index.Continue(end_val39, step_val39); hv_Index += step_val39)
			{
				SelectObj(ho_RegionTrans, &ho_ObjectSelected, hv_Index);
				RegionFeatures(ho_ObjectSelected, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
					&hv_Value);

				CropPart(ho_TileImage, &ho_CropImage, HTuple(hv_Value[0]), HTuple(hv_Value[1]),
					HTuple(hv_Value[2]), HTuple(hv_Value[3]));
				//write_image (CropImage, 'tiff', 0, SaveImagePath+Index)

				CHH2::Gen_features_V1(ho_CropImage, &hv_FeatureVector);
				ClassifyClassMlp(hv_ModelHandle, hv_FeatureVector, 1, &hv_FoundClassIDs, &hv_Confidence);
				if (0 != (hv_FoundClassIDs == 1))
				{
					//WriteImage(ho_CropImage, "tiff", 0, hv_SaveImagePath + hv_Index);
					ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
				}

			}
		}

		//if (HDevWindowStack::IsOpen())
		DispObj(ho_TileImage, hv_WindowHandle);
		//if (HDevWindowStack::IsOpen())
		SetDraw(hv_WindowHandle, "margin");
		SetColor(hv_WindowHandle, "red");
		//if (HDevWindowStack::IsOpen())
		DispObj(ho_BadObjects, hv_WindowHandle);
		CountObj(ho_BadObjects, &hv_Num);
		if (0 != (hv_Num>0))
		{
			(*hv_IsBad) = 1;
		}


		return;
	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "PengShang2_Camera3 Code Error.", "image", 400, 200, "red", "true");
		return;
	}

}

void CHH2::PengShang_Camera3(const HObject& ho_ImageEmphasize, const HObject& ho_TileImage, HTuple hv_WindowHandle,
	HTuple *hv_IsBad)
{
	try{


		// Local iconic variables
		HObject  ho_ImageOpening, ho_ImageClosing, ho_Regions;
		HObject  ho_RegionOpening, ho_ConnectedRegions, ho_SelectedRegions;
		HObject  ho_Rectangle, ho_RegionIntersection, ho_ConnectedRegions4;
		HObject  ho_RegionFillUp, ho_RegionTran, ho_Roi, ho_RoiUnion;
		HObject  ho_ImageReduced, ho_Bad, ho_RegionOpening1, ho_RegionClosing1;
		HObject  ho_ConnectedRegions1, ho_BadRegions1, ho_RegionUnion1;
		HObject  ho_RegionFillUp1, ho_ConnectedRegions2, ho_BlackRegion;
		HObject  ho_RegionClosing, ho_ConnectedRegions3, ho_SelectedRegions1;
		HObject  ho_BadObjects, ho_ObjectSelected, ho_Up, ho_Down;
		HObject  ho_RegionDilation;

		// Local control variables
		HTuple  hv_WIDTH, hv_Number, hv_Col2s, hv_Col2;
		HTuple  hv_Col1, hv_Number1, hv_Index, hv_Ra, hv_Rb, hv_Area;
		HTuple  hv_GrayMean, hv_BadNumber, hv_paichu, hv_Index1;
		HTuple  hv_PHI, hv_Row, hv_Column, hv_RowMean, hv_ColumnMean;
		HTuple  hv_length, hv_UpGrayMean, hv_DownGrayMean;

		//****************************
		//**
		//****************************

		(*hv_IsBad) = 0;
		hv_WIDTH = 870;

		GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 7, 7);
		SmoothImage(ho_ImageOpening, &ho_ImageOpening, "gauss", 0.5);
		GrayClosingRect(ho_ImageOpening, &ho_ImageClosing, 7, 7);

		Threshold(ho_ImageClosing, &ho_Regions, 40, 255);
		OpeningCircle(ho_Regions, &ho_RegionOpening, 15.5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 400, 800);

		CountObj(ho_SelectedRegions, &hv_Number);
		if (0 != (hv_Number == 18))
		{
			RegionFeatures(ho_SelectedRegions, "column2", &hv_Col2s);
			TupleMean(hv_Col2s, &hv_Col2);
			hv_Col1 = hv_Col2 - hv_WIDTH;
			GenRectangle1(&ho_Rectangle, 0, hv_Col1, 9999, hv_Col2);
			Intersection(ho_Rectangle, ho_SelectedRegions, &ho_RegionIntersection);
			Connection(ho_RegionIntersection, &ho_ConnectedRegions4);
			FillUp(ho_ConnectedRegions4, &ho_RegionFillUp);
			ShapeTrans(ho_RegionFillUp, &ho_RegionTran, "rectangle2");
			ErosionCircle(ho_RegionTran, &ho_Roi, 10.5);
			Union1(ho_Roi, &ho_RoiUnion);

			//shape_trans (SelectedRegions, RegionTrans, 'rectangle2')
			//union1 (RegionTrans, RegionUnion)
			//fill_up (RegionUnion, RegionFillUp)
			//erosion_circle (RegionFillUp, RegionErosion, 10.5)
			//intersection (Rectangle, RegionErosion, RegionIntersection)

			ReduceDomain(ho_ImageOpening, ho_RoiUnion, &ho_ImageReduced);
			Threshold(ho_ImageReduced, &ho_Bad, 0, 80);
			OpeningCircle(ho_Bad, &ho_RegionOpening1, 2.5);
			ClosingCircle(ho_RegionOpening1, &ho_RegionClosing1, 3.5);
			Connection(ho_RegionClosing1, &ho_ConnectedRegions1);
			SelectShape(ho_ConnectedRegions1, &ho_BadRegions1, ((HTuple("area").Append("convexity")).Append("column1")),
				"and", ((HTuple(300).Append(0.5)).Append(0)), (HTuple(99999).Append(1.0)).TupleConcat(hv_Col2 - 50));
			Union1(ho_BadRegions1, &ho_RegionUnion1);
			FillUp(ho_RegionUnion1, &ho_RegionFillUp1);
			Connection(ho_RegionFillUp1, &ho_ConnectedRegions2);
			//***
			Threshold(ho_ImageReduced, &ho_BlackRegion, 0, 20);
			ClosingCircle(ho_BlackRegion, &ho_RegionClosing, 8.5);
			Connection(ho_RegionClosing, &ho_ConnectedRegions3);
			SelectShape(ho_ConnectedRegions3, &ho_SelectedRegions1, (HTuple("area").Append("rb")),
				"and", (HTuple(250).Append(8)), (HTuple(99999).Append(999999)));
			//*****************
			ConcatObj(ho_ConnectedRegions2, ho_SelectedRegions1, &ho_ConnectedRegions2);
			CountObj(ho_ConnectedRegions2, &hv_Number1);
			GenEmptyObj(&ho_BadObjects);
			{
				HTuple end_val53 = hv_Number1;
				HTuple step_val53 = 1;
				for (hv_Index = 1; hv_Index.Continue(end_val53, step_val53); hv_Index += step_val53)
				{

					SelectObj(ho_ConnectedRegions2, &ho_ObjectSelected, hv_Index);
					RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
					RegionFeatures(ho_ObjectSelected, "rb", &hv_Rb);
					RegionFeatures(ho_ObjectSelected, "area", &hv_Area);
					GrayFeatures(ho_ObjectSelected, ho_ImageOpening, "mean", &hv_GrayMean);
					if (0 != ((hv_Ra / hv_Rb)>7.5))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (hv_Area>3000))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(HTuple(hv_GrayMean<40).TupleAnd((hv_Ra / hv_Rb)>4.5)).TupleAnd(hv_Ra>30)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(hv_Area>1500).TupleAnd(hv_GrayMean<30)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
				}
			}

			CountObj(ho_BadObjects, &hv_BadNumber);
			hv_paichu = HTuple();
			{
				HTuple end_val73 = hv_BadNumber;
				HTuple step_val73 = 1;
				for (hv_Index1 = 1; hv_Index1.Continue(end_val73, step_val73); hv_Index1 += step_val73)
				{
					SelectObj(ho_BadObjects, &ho_ObjectSelected, hv_Index1);
					RegionFeatures(ho_ObjectSelected, "phi", &hv_PHI);
					GrayFeatures(ho_ObjectSelected, ho_ImageOpening, "mean", &hv_GrayMean);
					if (0 != (hv_PHI<0))
					{
						hv_PHI = -hv_PHI;
					}
					if (0 != (HTuple(hv_PHI<(HTuple(10).TupleRad())).TupleAnd(hv_GrayMean<20.0)))
					{
						RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
						RegionFeatures(ho_ObjectSelected, (HTuple("row1").Append("row2")), &hv_Row);
						RegionFeatures(ho_ObjectSelected, (HTuple("column1").Append("column2")),
							&hv_Column);
						TupleMean(hv_Row, &hv_RowMean);
						TupleMean(hv_Column, &hv_ColumnMean);
						hv_length = HTuple(hv_Column[1]) - HTuple(hv_Column[0]);
						GenRectangle1(&ho_Up, HTuple(hv_Row[0]) - 20, HTuple(hv_Column[0]), HTuple(hv_Row[0]),
							HTuple(hv_Column[1]));
						GrayFeatures(ho_Up, ho_ImageOpening, "mean", &hv_UpGrayMean);
						GenRectangle1(&ho_Down, HTuple(hv_Row[1]), HTuple(hv_Column[0]), HTuple(hv_Row[1]) + 20,
							HTuple(hv_Column[1]));
						GrayFeatures(ho_Down, ho_ImageOpening, "mean", &hv_DownGrayMean);
						if (0 != (HTuple(HTuple(hv_UpGrayMean<1.0).TupleAnd(hv_DownGrayMean>100.0)).TupleOr(HTuple(hv_UpGrayMean>100.0).TupleAnd(hv_DownGrayMean<100.0))))
						{
							hv_paichu = hv_paichu.TupleConcat(hv_Index1);
						}
					}
				}
			}
			RemoveObj(ho_BadObjects, &ho_BadObjects, hv_paichu);
			CountObj(ho_BadObjects, &hv_BadNumber);
			if (HDevWindowStack::IsOpen())
				DispObj(ho_TileImage, hv_WindowHandle);
			if (HDevWindowStack::IsOpen())
				SetDraw(hv_WindowHandle, "margin");
			if (HDevWindowStack::IsOpen())
				SetColor(hv_WindowHandle, "red");
			if (HDevWindowStack::IsOpen())
				SetLineWidth(hv_WindowHandle, 2);

			if (0 != (hv_BadNumber>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects, &ho_RegionDilation, 8.5);
				if (HDevWindowStack::IsOpen())
					DispObj(ho_RegionDilation, hv_WindowHandle);
			}

			//****片底
			//union1 (SelectedRegions1, RegionUnion1)
			//closing_circle (RegionUnion1, RegionClosing, 5.5)
			//connection (RegionClosing, ConnectedRegions2)
			//select_shape (ConnectedRegions2, SelectedRegions1, ['area','column2'], 'and', [1200,Col2-100], [999999,Col2+50])
			//gray_features (SelectedRegions1, TileImage, 'mean', GrayMeans)
			//tuple_sgn (GrayMeans-130, Sgn)
			//tuple_find (Sgn, -1, Indices)
			//badNum := |Indices|
			//gen_empty_obj (Bad2Objects)
			//if (badNum!=0 and Indices!=-1)
			//for Index := 0 to badNum-1 by 1
			//select_obj (SelectedRegions1, ObjectSelected, Indices[Index]+1)
			//concat_obj (Bad2Objects, ObjectSelected, Bad2Objects)
			//endfor
			//endif

			//count_obj (Bad2Objects, Number2)
			//if (Number2>0)
			//IsBad := true
			//dilation_circle (Bad2Objects, RegionDilation2, 8.5)
			//dev_display (RegionDilation2)
			//endif
		}
		else
		{
			(*hv_IsBad) = 1;
			if (HDevWindowStack::IsOpen())
				DispText(hv_WindowHandle, "片数不等于18", "image", 300, 220, "red",
				HTuple(), HTuple());
		}
		return;
	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "PengShang_Camera3 Code Error.", "image", 400, 200, "red", "true");
		return;
	}
}
/****************Camera3****************************************************/







/****************Camera4******************/
void CHH2::BottomAndTop_Camera4(HObject ho_ImageEmphasize, HObject ho_TileImage,
	HTuple hv_WindowHandle, HTuple hv_Model, HTuple *hv_IsBad)
{
	try{
		// Local iconic variables
		HObject  ho_Regions, ho_RegionOpening, ho_ConnectedRegions;
		HObject  ho_SelectedRegions, ho_RegionTrans1, ho_Rectangle;
		HObject  ho_ImageReduced, ho_ImageClosing, ho_Regions1, ho_RegionFillUp1;
		HObject  ho_RegionOpening1, ho_RegionClosing, ho_ConnectedRegions1;
		HObject  ho_PieceRegions, ho_RegionFillUp, ho_RegionTrans;
		HObject  ho_ConnectedTrans, ho_BadObjects1, ho_BadObjects2;
		HObject  ho_BadObjects3, ho_ObjectTran, ho_ObjectSelected;
		HObject  ho_RegionDifference, ho_Rectangle1, ho_RegionIntersection;
		HObject  ho_ImageReduced2, ho_ImagePart, ho_RegionOpening4;
		HObject  ho_RegionOpening5, ho_ConnectedRegions4, ho_SelectedRegions5;
		HObject  ho_ImageReduced1, ho_BadRegion, ho_BadRegions;

		// Local control variables
		HTuple  hv_WIDTH, hv_col2, hv_PieceNum, hv_MLPHandle;
		HTuple  hv_i, hv_row1, hv_FeatureVector, hv_FoundClassIDs;
		HTuple  hv_Confidence, hv_grayMeanValue, hv_GrayThreshold;
		HTuple  hv_Sgn, hv_Indices, hv_badNum, hv_Index, hv_Value;
		HTuple  hv_BabNum;

		//***********************************
		//**Step1:find column1 of bottom light Rectangel.
		//**Step2:Gen_Rectangle1 and ReduceImage.
		//**Step3:find 18 Pieces.
		//***********************************
		(*hv_IsBad) = 0;
		hv_WIDTH = 950;
		Threshold(ho_ImageEmphasize, &ho_Regions, 27, 255);
		OpeningRectangle1(ho_Regions, &ho_RegionOpening, 1, 130);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 5000, 99999);
		ShapeTrans(ho_SelectedRegions, &ho_RegionTrans1, "rectangle1");
		RegionFeatures(ho_SelectedRegions, "column1", &hv_col2);
		GenRectangle1(&ho_Rectangle, 0, hv_col2 - hv_WIDTH, 9999, hv_col2 + 30);

		ReduceDomain(ho_ImageEmphasize, ho_Rectangle, &ho_ImageReduced);
		GrayClosingRect(ho_ImageReduced, &ho_ImageClosing, 7, 7);

		Threshold(ho_ImageClosing, &ho_Regions1, 200, 255);
		FillUp(ho_Regions1, &ho_RegionFillUp1);
		OpeningCircle(ho_RegionFillUp1, &ho_RegionOpening1, 3.5);
		ClosingCircle(ho_RegionOpening1, &ho_RegionClosing, 5.5);

		Connection(ho_RegionClosing, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_PieceRegions, "height", "and", 300, 5000);

		CountObj(ho_PieceRegions, &hv_PieceNum);
		if (0 != (hv_PieceNum != 18))
		{
			return;

		}
		else
		{

			FillUp(ho_PieceRegions, &ho_RegionFillUp);
			ShapeTrans(ho_RegionFillUp, &ho_RegionTrans, "rectangle2");


			DispObj(ho_TileImage, hv_WindowHandle);
			SetDraw(hv_WindowHandle, "margin");
			DispObj(ho_RegionTrans, hv_WindowHandle);

			Connection(ho_RegionTrans, &ho_ConnectedTrans);
			GenEmptyObj(&ho_BadObjects1);
			GenEmptyObj(&ho_BadObjects2);
			GenEmptyObj(&ho_BadObjects3);

			ReadClassMlp(hv_Model, &hv_MLPHandle);

			{
				HTuple end_val46 = hv_PieceNum;
				HTuple step_val46 = 1;
				for (hv_i = 1; hv_i.Continue(end_val46, step_val46); hv_i += step_val46)
				{
					SelectObj(ho_ConnectedTrans, &ho_ObjectTran, hv_i);
					SelectObj(ho_RegionFillUp, &ho_ObjectSelected, hv_i);
					RegionFeatures(ho_ObjectSelected, "column2", &hv_col2);
					Difference(ho_ObjectTran, ho_ObjectSelected, &ho_RegionDifference);

					//*****MLP***
					RegionFeatures(ho_ObjectSelected, "row1", &hv_row1);
					GenRectangle1(&ho_Rectangle1, hv_row1, hv_col2 - 200, hv_row1 + 500, hv_col2);
					Intersection(ho_Rectangle1, ho_ObjectTran, &ho_RegionIntersection);
					ReduceDomain(ho_TileImage, ho_RegionIntersection, &ho_ImageReduced2);
					CropDomain(ho_ImageReduced2, &ho_ImagePart);

					CHH2::Gen_features_V1(ho_ImagePart, &hv_FeatureVector);
					ClassifyClassMlp(hv_MLPHandle, hv_FeatureVector, 1, &hv_FoundClassIDs, &hv_Confidence);
					if (0 != (hv_FoundClassIDs == 1))
					{
						ConcatObj(ho_BadObjects1, ho_RegionIntersection, &ho_BadObjects1);
					}


					//**************底部宽大的黑条************
					//opening_circle (RegionDifference, RegionOpening2, 8.5)
					//closing_circle (RegionOpening2, RegionClosing2, 5.5)
					//connection (RegionClosing2, ConnectedRegions2)
					//select_shape (ConnectedRegions2, BadRegions1, ['width','column2','area','column1'], 'and', [20,col2-50,1100,col2-200], [99999,col2+30,99999,col2-15])
					//concat_obj (BadObjects1, BadRegions1, BadObjects1)

					//**************底部细长的黑条************
					OpeningRectangle1(ho_RegionDifference, &ho_RegionOpening4, 1, 15);
					OpeningRectangle1(ho_RegionOpening4, &ho_RegionOpening5, 10, 1);

					Connection(ho_RegionOpening5, &ho_ConnectedRegions4);
					SelectShape(ho_ConnectedRegions4, &ho_SelectedRegions5, (HTuple("height").Append("column2")),
						"and", HTuple(80).TupleConcat(hv_col2 - 50), HTuple(99999).TupleConcat(hv_col2 + 30));

					GrayFeatures(ho_SelectedRegions5, ho_TileImage, "mean", &hv_grayMeanValue);

					hv_GrayThreshold = 30;
					TupleSgn(hv_grayMeanValue - hv_GrayThreshold, &hv_Sgn);
					TupleFind(hv_Sgn, -1, &hv_Indices);

					hv_badNum = hv_Indices.TupleLength();
					if (0 != (HTuple(hv_badNum != 0).TupleAnd(hv_Indices != -1)))
					{
						{
							HTuple end_val90 = hv_badNum - 1;
							HTuple step_val90 = 1;
							for (hv_Index = 0; hv_Index.Continue(end_val90, step_val90); hv_Index += step_val90)
							{
								SelectObj(ho_SelectedRegions5, &ho_ObjectSelected, HTuple(hv_Indices[hv_Index]) + 1);
								ConcatObj(ho_BadObjects2, ho_ObjectSelected, &ho_BadObjects2);
							}
						}
					}

					//**************黑色长条************
					ReduceDomain(ho_ImageClosing, ho_ObjectSelected, &ho_ImageReduced1);
					Threshold(ho_ImageReduced1, &ho_BadRegion, 0, 100);
					Connection(ho_BadRegion, &ho_BadRegion);
					RegionFeatures(ho_BadRegion, "outer_radius", &hv_Value);
					SelectShape(ho_BadRegion, &ho_BadRegions, ((HTuple("area").Append("outer_radius")).Append("orientation")),
						"and", (HTuple(500).Append(35)).TupleConcat(HTuple(20).TupleRad()), (HTuple(99999).Append(99999)).TupleConcat(HTuple(70).TupleRad()));
					ConcatObj(ho_BadObjects3, ho_BadRegions, &ho_BadObjects3);
				}
			}

			DispObj(ho_TileImage, hv_WindowHandle);
			SetColor(hv_WindowHandle, "red");
			SetDraw(hv_WindowHandle, "margin");
			SetLineWidth(hv_WindowHandle, 2);
			CountObj(ho_BadObjects1, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects1, &ho_BadObjects1, 8.5);
				DispObj(ho_BadObjects1, hv_WindowHandle);
			}

			CountObj(ho_BadObjects2, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects2, &ho_BadObjects2, 8.5);
				DispObj(ho_BadObjects2, hv_WindowHandle);
			}

			CountObj(ho_BadObjects3, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects3, &ho_BadObjects3, 8.5);
				DispObj(ho_BadObjects3, hv_WindowHandle);
			}

		}
		return;
	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "BottomAndTop_Camera4 Code Error.", "image", 400, 200, "red", "true");
		return;
	}
}

void CHH2::PengShang_Camera4(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_WindowHandle,
	HTuple *hv_IsBad)
{
	try{
		// Local iconic variables
		HObject  ho_ImageOpening, ho_ImageClosing, ho_Regions;
		HObject  ho_RegionOpening, ho_ConnectedRegions, ho_SelectedRegions;
		HObject  ho_RegionClosing, ho_RegionFillUp, ho_Rectangle;
		HObject  ho_ConnectedRegions1, ho_ROI, ho_Rois, ho_SelectedObject;
		HObject  ho_IntersectionRoi, ho_SelectedROI, ho_ImageReduced;
		HObject  ho_Region, ho_BadRegions1, ho_RegionUnion1, ho_RegionClosing1;
		HObject  ho_RegionFillUp1, ho_AllBadRegions, ho_BadRegion2;
		HObject  ho_ConnectedRegions3, ho_SelectedRegions1, ho_BadObjects;
		HObject  ho_ObjectSelected, ho_BadRegionTrans, ho_RegionDilation;

		// Local control variables
		HTuple  hv_WIDTH, hv_ImageWidth, hv_ImageHeight;
		HTuple  hv_Number, hv_Col2, hv_offset, hv_Index, hv_Ra;
		HTuple  hv_Rb, hv_Area, hv_GrayMean, hv_Num, hv_paichu;

		//****************************
		//**
		//****************************

		(*hv_IsBad) = 0;
		hv_WIDTH = 870;
		//SaveImagePath := 'f:/mlp/Auto_camera4/NewPiece/'

		GetImageSize(ho_ImageEmphasize, &hv_ImageWidth, &hv_ImageHeight);
		GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 7, 7);
		GrayClosingRect(ho_ImageOpening, &ho_ImageClosing, 5, 5);
		//median_image (ImageClosing1, ImageClosing, 'circle', 7, 'mirrored')

		Threshold(ho_ImageClosing, &ho_Regions, 20, 255);
		OpeningCircle(ho_Regions, &ho_RegionOpening, 15.5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("height").Append("column2")),
			"and", (HTuple(400).Append(0)), (HTuple(800).Append(1800)));

		CountObj(ho_SelectedRegions, &hv_Number);
		if (0 != (hv_Number == 18))
		{
			//************
			//closing_circle (SelectedRegions, RegionClosing, 10.5)
			//fill_up (RegionClosing, RegionFillUp)
			//region_features (RegionFillUp, 'column2', Col2s)
			//*     tuple_mean (Col2s, Col2)
			//Col1 := Col2-WIDTH
			//*     gen_rectangle1 (Rectangle, 0, Col1, 9999, Col2)
			//union1 (RegionFillUp, RegionUnion)
			//*     intersection (Rectangle, RegionUnion, RegionIntersection)
			//*     erosion_circle (RegionIntersection, RegionErosion, 6.5)

			//*     connection (RegionErosion, ConnectedRegions1)
			//shape_trans (ConnectedRegions1, RegionTrans2, 'convex')
			//erosion_circle (RegionTrans2, RegionTrans2, 5.5)
			//*     union1 (RegionTrans2, ROI)
			//****************

			//***
			ClosingCircle(ho_SelectedRegions, &ho_RegionClosing, 10.5);
			FillUp(ho_RegionClosing, &ho_RegionFillUp);
			SortRegion(ho_RegionFillUp, &ho_RegionFillUp, "upper_left", "true", "row");

			GenEmptyObj(&ho_Rois);
			hv_offset = 10;
			for (hv_Index = 1; hv_Index <= 18; hv_Index += 1)
			{
				SelectObj(ho_RegionFillUp, &ho_SelectedObject, hv_Index);
				RegionFeatures(ho_SelectedObject, "column2", &hv_Col2);
				GenRectangle1(&ho_Rectangle, 0, (hv_Col2 - hv_offset) - hv_WIDTH, hv_ImageHeight,
					hv_Col2 - hv_offset);
				Intersection(ho_Rectangle, ho_SelectedObject, &ho_IntersectionRoi);
				ErosionCircle(ho_IntersectionRoi, &ho_SelectedROI, 6.5);
				ConcatObj(ho_Rois, ho_SelectedROI, &ho_Rois);
			}

			Union1(ho_Rois, &ho_ROI);
			//***
			//reduce_domain (ImageOpening, RegionUnion2, ImageReduced)
			ReduceDomain(ho_ImageEmphasize, ho_ROI, &ho_ImageReduced);

			Threshold(ho_ImageReduced, &ho_Region, 0, 150);
			ClosingCircle(ho_Region, &ho_RegionClosing, 3.5);
			//opening_circle (RegionClosing, RegionClosing, 5.5)
			Connection(ho_RegionClosing, &ho_ConnectedRegions1);
			//select_shape (ConnectedRegions1, BadRegions1, ['area','convexity','column2'], 'and', [300,0.5,0], [99999,1.0,Col2-40])
			SelectShape(ho_ConnectedRegions1, &ho_BadRegions1, ((HTuple("area").Append("convexity")).Append("column1")),
				"and", ((HTuple(300).Append(0.4)).Append(0)), (HTuple(99999).Append(1.0)).TupleConcat(hv_Col2 - 50));
			Union1(ho_BadRegions1, &ho_RegionUnion1);
			ClosingCircle(ho_RegionUnion1, &ho_RegionClosing1, 5.5);
			FillUp(ho_RegionClosing1, &ho_RegionFillUp1);
			Connection(ho_RegionFillUp1, &ho_AllBadRegions);
			//*****
			Threshold(ho_ImageReduced, &ho_BadRegion2, 0, 10);
			Connection(ho_BadRegion2, &ho_ConnectedRegions3);
			SelectShape(ho_ConnectedRegions3, &ho_SelectedRegions1, "area", "and", 300, 99999);
			//******
			ConcatObj(ho_AllBadRegions, ho_SelectedRegions1, &ho_AllBadRegions);
			CountObj(ho_AllBadRegions, &hv_Number);
			GenEmptyObj(&ho_BadObjects);
			{
				HTuple end_val76 = hv_Number;
				HTuple step_val76 = 1;
				for (hv_Index = 1; hv_Index.Continue(end_val76, step_val76); hv_Index += step_val76)
				{

					SelectObj(ho_AllBadRegions, &ho_ObjectSelected, hv_Index);
					RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
					RegionFeatures(ho_ObjectSelected, "rb", &hv_Rb);
					RegionFeatures(ho_ObjectSelected, "area", &hv_Area);
					GrayFeatures(ho_ObjectSelected, ho_ImageOpening, "mean", &hv_GrayMean);
					if (0 != (HTuple((hv_Ra / hv_Rb)>8).TupleAnd(hv_GrayMean<90)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(hv_Area>3000).TupleAnd(hv_GrayMean<90)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(HTuple(hv_GrayMean<40).TupleAnd((hv_Ra / hv_Rb)>3)).TupleAnd(hv_Ra>30)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}
					else if (0 != (HTuple(hv_Area>1500).TupleAnd(hv_GrayMean<30)))
					{
						ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
					}

				}
			}
			CountObj(ho_BadObjects, &hv_Num);

			//***********************
			hv_paichu = HTuple();
			{
				HTuple end_val98 = hv_Num;
				HTuple step_val98 = 1;
				for (hv_Index = 1; hv_Index.Continue(end_val98, step_val98); hv_Index += step_val98)
				{
					SelectObj(ho_BadObjects, &ho_ObjectSelected, hv_Index);
					RegionFeatures(ho_ObjectSelected, "ra", &hv_Ra);
					if (0 != (hv_Ra<40))
					{
						hv_paichu = hv_paichu.TupleConcat(hv_Index);
					}
				}
			}
			RemoveObj(ho_BadObjects, &ho_BadObjects, hv_paichu);
			CountObj(ho_BadObjects, &hv_Num);
			//*************************

			if (0 != (hv_Num>0))
			{
				ShapeTrans(ho_BadObjects, &ho_BadRegionTrans, "convex");
				DilationCircle(ho_BadRegionTrans, &ho_RegionDilation, 8.5);
				if (HDevWindowStack::IsOpen())
					SetColor(hv_WindowHandle, "red");
				if (HDevWindowStack::IsOpen())
					DispObj(ho_RegionDilation, hv_WindowHandle);
				(*hv_IsBad) = 1;
			}
		}
		else
		{
			(*hv_IsBad) = 1;
			if (HDevWindowStack::IsOpen())
				DispText(hv_WindowHandle, "片数不等于18", "image", 300, 220, "red",
				HTuple(), HTuple());
		}

		return;
	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "PengShang_Camera4 Code Error.", "image", 400, 200, "red", "true");
		return;
	}

}

/****************Camera4****************************************************/

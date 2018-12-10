
#include "CHH.h"

void CHH::dev_open_window_fit_image (HObject ho_Image, HTuple hv_Row, HTuple hv_Column, HTuple hv_WidthLimit, HTuple hv_HeightLimit, 
									 const HTuple& FatherWindow,HTuple *hv_WindowHandle)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
	HTuple  hv_MaxHeight, hv_ResizeFactor, hv_ImageWidth, hv_ImageHeight;
	HTuple  hv_TempWidth, hv_TempHeight, hv_WindowWidth, hv_WindowHeight;

	//This procedure opens a new graphics window and adjusts the size
	//such that it fits into the limits specified by WidthLimit
	//and HeightLimit, but also maintains the correct image aspect ratio.
	//
	//If it is impossible to match the minimum and maximum extent requirements
	//at the same time (f.e. if the image is very long but narrow),
	//the maximum value gets a higher priority,
	//
	//Parse input tuple WidthLimit
	if (0 != (HTuple((hv_WidthLimit.TupleLength())==0).TupleOr(hv_WidthLimit<0)))
	{
		//hv_MinWidth = 500;
		//hv_MaxWidth = 800;
		hv_MinWidth = 500;
		hv_MaxWidth = 1100;
	}
	else if (0 != ((hv_WidthLimit.TupleLength())==1))
	{
		hv_MinWidth = 0;
		hv_MaxWidth = hv_WidthLimit;
	}
	else
	{
		hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
		hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
	}
	//Parse input tuple HeightLimit
	if (0 != (HTuple((hv_HeightLimit.TupleLength())==0).TupleOr(hv_HeightLimit<0)))
	{
		//hv_MinHeight = 400;
		//hv_MaxHeight = 600;
		hv_MinHeight = 400;
		hv_MaxHeight = 940;
	}
	else if (0 != ((hv_HeightLimit.TupleLength())==1))
	{
		hv_MinHeight = 0;
		hv_MaxHeight = hv_HeightLimit;
	}
	else
	{
		hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
		hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
	}
	//
	//Test, if window size has to be changed.
	hv_ResizeFactor = 1;
	GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
	//First, expand window to the minimum extents (if necessary).
	if (0 != (HTuple(hv_MinWidth>hv_ImageWidth).TupleOr(hv_MinHeight>hv_ImageHeight)))
	{
		hv_ResizeFactor = (((hv_MinWidth.TupleReal())/hv_ImageWidth).TupleConcat((hv_MinHeight.TupleReal())/hv_ImageHeight)).TupleMax();
	}
	hv_TempWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_TempHeight = hv_ImageHeight*hv_ResizeFactor;
	//Then, shrink window to maximum extents (if necessary).
	if (0 != (HTuple(hv_MaxWidth<hv_TempWidth).TupleOr(hv_MaxHeight<hv_TempHeight)))
	{
		hv_ResizeFactor = hv_ResizeFactor*((((hv_MaxWidth.TupleReal())/hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal())/hv_TempHeight)).TupleMin());
	}
	hv_WindowWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_WindowHeight = hv_ImageHeight*hv_ResizeFactor;
	//Resize window
	SetWindowAttr("background_color","black");
// 	int width = hv_WindowWidth[0].D();
// 	int height= hv_WindowHeight[0].D();
	OpenWindow(hv_Row,hv_Column,hv_WindowWidth,hv_WindowHeight,FatherWindow,"","",&(*hv_WindowHandle));
	HDevWindowStack::Push((*hv_WindowHandle));
	if (HDevWindowStack::IsOpen())
		SetPart(HDevWindowStack::GetActive(),0, 0, hv_ImageHeight-1, hv_ImageWidth-1);
	return;
}



void CHH::disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, 
				   HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
	HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
	HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
	HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
	HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_UseShadow;
	HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
	HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
	HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If Box[0] is set to 'true', the text is written within an orange box.
	//     If set to' false', no box is displayed.
	//     If set to a color string (e.g. 'white', '#FF00CC', etc.),
	//       the text is written in a box of that color.
	//     An optional second value for Box (Box[1]) controls if a shadow is displayed:
	//       'true' -> display a shadow in a default color
	//       'false' -> display no shadow (same as if no second value is given)
	//       otherwise -> use given string as color string for the shadow color
	//
	//Prepare window
	GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
	GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
	SetPart(hv_WindowHandle, 0, 0, hv_HeightWin-1, hv_WidthWin-1);
	//
	//default settings
	if (0 != (hv_Row==-1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column==-1))
	{
		hv_Column = 12;
	}
	if (0 != (hv_Color==HTuple()))
	{
		hv_Color = "";
	}
	//
	hv_String = ((""+hv_String)+"").TupleSplit("\n");
	//
	//Estimate extentions of text depending on font size.
	GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
	if (0 != (hv_CoordSystem==HTuple("window")))
	{
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
	}
	else
	{
		//Transform image to window coordinates
		hv_FactorRow = (1.*hv_HeightWin)/((hv_Row2Part-hv_Row1Part)+1);
		hv_FactorColumn = (1.*hv_WidthWin)/((hv_Column2Part-hv_Column1Part)+1);
		hv_R1 = ((hv_Row-hv_Row1Part)+0.5)*hv_FactorRow;
		hv_C1 = ((hv_Column-hv_Column1Part)+0.5)*hv_FactorColumn;
	}
	//
	//Display text box depending on text size
	hv_UseShadow = 1;
	hv_ShadowColor = "gray";
	if (0 != (HTuple(hv_Box[0])==HTuple("true")))
	{
		hv_Box[0] = "#fce9d4";
		hv_ShadowColor = "#f28d26";
	}
	if (0 != ((hv_Box.TupleLength())>1))
	{
		if (0 != (HTuple(hv_Box[1])==HTuple("true")))
		{
			//Use default ShadowColor set above
		}
		else if (0 != (HTuple(hv_Box[1])==HTuple("false")))
		{
			hv_UseShadow = 0;
		}
		else
		{
			hv_ShadowColor = ((const HTuple&)hv_Box)[1];
			//Valid color?
			try
			{
				SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
			}
			// catch (Exception) 
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				HDevExpDefaultException.ToHTuple(&hv_Exception);
				hv_Exception = "Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)";
				throw HalconCpp::HException(hv_Exception);
			}
		}
	}
	if (0 != (HTuple(hv_Box[0])!=HTuple("false")))
	{
		//Valid color?
		try
		{
			SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			hv_Exception = "Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)";
			throw HalconCpp::HException(hv_Exception);
		}
		//Calculate box extents
		hv_String = (" "+hv_String)+" ";
		hv_Width = HTuple();
		{
			HTuple end_val93 = (hv_String.TupleLength())-1;
			HTuple step_val93 = 1;
			for (hv_Index=0; hv_Index.Continue(end_val93, step_val93); hv_Index += step_val93)
			{
				GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent, 
					&hv_Descent, &hv_W, &hv_H);
				hv_Width = hv_Width.TupleConcat(hv_W);
			}
		}
		hv_FrameHeight = hv_MaxHeight*(hv_String.TupleLength());
		hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
		hv_R2 = hv_R1+hv_FrameHeight;
		hv_C2 = hv_C1+hv_FrameWidth;
		//Display rectangles
		GetDraw(hv_WindowHandle, &hv_DrawMode);
		SetDraw(hv_WindowHandle, "fill");
		//Set shadow color
		SetColor(hv_WindowHandle, hv_ShadowColor);
		if (0 != hv_UseShadow)
		{
			DispRectangle1(hv_WindowHandle, hv_R1+1, hv_C1+1, hv_R2+1, hv_C2+1);
		}
		//Set box color
		SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
		SetDraw(hv_WindowHandle, hv_DrawMode);
	}
	//Write text.
	{
		HTuple end_val115 = (hv_String.TupleLength())-1;
		HTuple step_val115 = 1;
		for (hv_Index=0; hv_Index.Continue(end_val115, step_val115); hv_Index += step_val115)
		{
			hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index%(hv_Color.TupleLength())];
			if (0 != (HTuple(hv_CurrentColor!=HTuple("")).TupleAnd(hv_CurrentColor!=HTuple("auto"))))
			{
				SetColor(hv_WindowHandle, hv_CurrentColor);
			}
			else
			{
				SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
			}
			hv_Row = hv_R1+(hv_MaxHeight*hv_Index);
			SetTposition(hv_WindowHandle, hv_Row, hv_C1);
			WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
		}
	}
	//Reset changed window settings
	SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
	SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
	return;
}

void CHH::gen_arrow_contour_xld (HObject *ho_Arrow, HTuple hv_Row1, HTuple hv_Column1, 
							HTuple hv_Row2, HTuple hv_Column2, HTuple hv_HeadLength, HTuple hv_HeadWidth)
{

	// Local iconic variables
	HObject  ho_TempArrow;

	// Local control variables
	HTuple  hv_Length, hv_ZeroLengthIndices, hv_DR;
	HTuple  hv_DC, hv_HalfHeadWidth, hv_RowP1, hv_ColP1, hv_RowP2;
	HTuple  hv_ColP2, hv_Index;

	//This procedure generates arrow shaped XLD contours,
	//pointing from (Row1, Column1) to (Row2, Column2).
	//If starting and end point are identical, a contour consisting
	//of a single point is returned.
	//
	//input parameteres:
	//Row1, Column1: Coordinates of the arrows' starting points
	//Row2, Column2: Coordinates of the arrows' end points
	//HeadLength, HeadWidth: Size of the arrow heads in pixels
	//
	//output parameter:
	//Arrow: The resulting XLD contour
	//
	//The input tuples Row1, Column1, Row2, and Column2 have to be of
	//the same length.
	//HeadLength and HeadWidth either have to be of the same length as
	//Row1, Column1, Row2, and Column2 or have to be a single element.
	//If one of the above restrictions is violated, an error will occur.
	//
	//
	//Init
	GenEmptyObj(&(*ho_Arrow));
	//
	//Calculate the arrow length
	DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Length);
	//
	//Mark arrows with identical start and end point
	//(set Length to -1 to avoid division-by-zero exception)
	hv_ZeroLengthIndices = hv_Length.TupleFind(0);
	if (0 != (hv_ZeroLengthIndices!=-1))
	{
		hv_Length[hv_ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	hv_DR = (1.0*(hv_Row2-hv_Row1))/hv_Length;
	hv_DC = (1.0*(hv_Column2-hv_Column1))/hv_Length;
	hv_HalfHeadWidth = hv_HeadWidth/2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))+(hv_HalfHeadWidth*hv_DC);
	hv_ColP1 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))-(hv_HalfHeadWidth*hv_DR);
	hv_RowP2 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))-(hv_HalfHeadWidth*hv_DC);
	hv_ColP2 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))+(hv_HalfHeadWidth*hv_DR);
	//
	//Finally create output XLD contour for each input point pair
	{
		HTuple end_val45 = (hv_Length.TupleLength())-1;
		HTuple step_val45 = 1;
		for (hv_Index=0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
		{
			if (0 != (HTuple(hv_Length[hv_Index])==-1))
			{
				//Create_ single points for arrows with identical start and end point
				GenContourPolygonXld(&ho_TempArrow, HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
			}
			else
			{
				//Create arrow contour
				GenContourPolygonXld(&ho_TempArrow, ((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])), 
					((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
			}
			ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
		}
	}
	return;
}

void CHH::CreateRectangleROI(HalconCpp::HImage& Image,HalconCpp::HObject* Rectangle,const HalconCpp::HTuple& WindowHandle)
{
	HalconCpp::HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	HalconCpp::SetColor(WindowHandle,"green");
	HalconCpp::SetDraw(WindowHandle,"margin");
	HalconCpp::DrawRectangle1(WindowHandle, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	HalconCpp::GenRectangle1(Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
	HalconCpp::DispObj(*Rectangle, WindowHandle);
}

void CHH::CreateCircleROI(HalconCpp::HImage& Image,HalconCpp::HObject* Circle,const HalconCpp::HTuple& WindowHandle)
{
	HalconCpp::HTuple hv_Row, hv_Column, hv_Radius;
	HalconCpp::SetColor(WindowHandle,"green");
	HalconCpp::SetDraw(WindowHandle,"margin");
	HalconCpp::DrawCircle(WindowHandle, &hv_Row, &hv_Column, &hv_Radius);
	HalconCpp::GenCircle(Circle, hv_Row, hv_Column,hv_Radius);
	//HalconCpp::DispObj(*Circle, WindowHandle);
}

//拼接图片
BOOL CHH::PingJie (HObject ho_Image, HObject *ho_TileImage, HTuple hv_MearsureColCenter, 
				   HTuple hv_EdgeOffset, HTuple hv_Offset, HTuple hv_HorProjectionGrayVal, HTuple *hv_DownRow)
{

	// Local iconic variables
	HObject  ho_Rectangle, ho_ImagePartdown, ho_ImagePartup;
	HObject  ho_Images;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_TmpCtrl_Row, hv_TmpCtrl_Phi;
	HTuple  hv_TmpCtrl_Len1, hv_TmpCtrl_Len2, hv_HorProjection;
	HTuple  hv_VertProjection, hv_Index, hv_CutRow;


  GetImageSize(ho_Image, &hv_Width, &hv_Height);

  hv_TmpCtrl_Row = 800;
 // hv_MearsureColCenter = 900;
  hv_TmpCtrl_Phi = HTuple(0).TupleRad();
  hv_TmpCtrl_Len1 = 500;
  hv_TmpCtrl_Len2 = 800;
  //******
  GenRectangle2(&ho_Rectangle, hv_TmpCtrl_Row, hv_MearsureColCenter, hv_TmpCtrl_Phi, 
      hv_TmpCtrl_Len1, hv_TmpCtrl_Len2);
  GrayProjections(ho_Rectangle, ho_Image, "simple", &hv_HorProjection, &hv_VertProjection);

  //***************
  //HorProjectionGrayVal 传入的参数
  try{
		  HTuple end_val13 = hv_HorProjection.TupleLength();
		  HTuple step_val13 = 1;
		  for (hv_Index=0; hv_Index.Continue(end_val13, step_val13); hv_Index += step_val13)
		  {
			  if (0 != (HTuple(hv_HorProjection[hv_Index])<hv_HorProjectionGrayVal))
			  {
				  hv_Index += hv_EdgeOffset;
				  if (0 != (HTuple(hv_HorProjection[hv_Index])<hv_HorProjectionGrayVal))
				  {
					  break;
				  }
			  }
		  }
	 
		  hv_CutRow = hv_Index;

		  if(hv_CutRow==hv_EdgeOffset)
			  hv_CutRow=0;

		  CropRectangle1(ho_Image, &ho_ImagePartdown, hv_CutRow, 0, hv_Height-1, hv_Width-1);
		  int row = hv_CutRow.I();
		  CropRectangle1(ho_Image, &ho_ImagePartup, 0, 0, (hv_CutRow-1)+hv_Offset, hv_Width-1);
		  GenEmptyObj(&ho_Images);
		  ConcatObj(ho_Images, ho_ImagePartdown, &ho_Images);
		  ConcatObj(ho_Images, ho_ImagePartup, &ho_Images);
		  (*hv_DownRow) = (hv_Height-hv_CutRow)-hv_Offset;
		  TileImagesOffset(ho_Images, &(*ho_TileImage), HTuple(0).TupleConcat((*hv_DownRow)), 
			  (HTuple(0).Append(0)), (HTuple(-1).Append(-1)), (HTuple(-1).Append(-1)), (HTuple(-1).Append(-1)), 
			  (HTuple(-1).Append(-1)), hv_Width, hv_Height);
	 }
	  catch(HException &except)	{
			/*CString tt(except.ErrorMessage().Text()) ;
			CString ttt("\r\n位于接接函数,建议修改拼接参数中的灰度值，建议值：50");
			CString error =tt+ttt;*/
			//AfxMessageBox(error);
			return FALSE;
	  }

  return TRUE;
}

//钩部特征
void CHH::GB_Feature (HObject ho_GrayImage, HObject *ho_GBRegionUnion, HObject *ho_ErrorGBTopRegions, 
    HTuple hv_GBTopCutColumn, HTuple *hv_ErrorGBRows, HTuple *hv_GB_Num, HTuple *hv_RowGBTopCenters, 
    HTuple *hv_ColumnGBTopCenters, HTuple *hv_RowGBBottomCenters, HTuple *hv_ColumnGBBottomCenters)
{

    // Local iconic variables
  HObject  ho_GBTopRectangleROI, ho_GBRectangleTopImageReduced;
  HObject  ho_ImageClosing2, ho_Region, ho_RegionOpening, ho_ConnectedRegions;
  HObject  ho_RegionTrans, ho_ErrorRectangle, ho_RectangleGB;
  HObject  ho_RectangleGBImageReduced, ho_ImageClosing1, ho_MeasureROI;
  HObject  ho_Rectangle;

  // Local control variables
  HTuple  hv_Area, hv_Extended, hv_i, hv_Height;
  HTuple  hv_ColumnGBTopMean, hv_Row11, hv_Column11, hv_Row2;
  HTuple  hv_Column2, hv_Sigma, hv_AmplitudeThreshold, hv_phi;
  HTuple  hv_length2, hv_Index2, hv_Measure_GBBottom, hv_Row_Measure_Bottom;
  HTuple  hv_Column_Measure_Bottom, hv_Amplitude_Measure_Bottom;
  HTuple  hv_Distance_Measure_Bottom, hv_Mean, hv_Indices;
  HTuple  hv_GreatMeanGrayThreshold, hv_Index1, hv_SelectNum;

  GenRectangle1(&ho_GBTopRectangleROI, 0, hv_GBTopCutColumn-50, 12287, hv_GBTopCutColumn+50);
  ReduceDomain(ho_GrayImage, ho_GBTopRectangleROI, &ho_GBRectangleTopImageReduced );

  //***************钩顶部threshold***************
  GrayClosingRect(ho_GBRectangleTopImageReduced, &ho_ImageClosing2, 100, 11);

  Threshold(ho_ImageClosing2, &ho_Region, 200, 255);
  OpeningRectangle1(ho_Region, &ho_RegionOpening, 1, 150);
  Connection(ho_RegionOpening, &ho_ConnectedRegions);
  ShapeTrans(ho_ConnectedRegions, &ho_RegionTrans, "rectangle1");
  AreaCenter(ho_RegionTrans, &hv_Area, &(*hv_RowGBTopCenters), hv_ColumnGBTopCenters);
  CountObj(ho_RegionTrans, &(*hv_GB_Num));


  //钩顶部少于18，寻找未出现钩顶的部分，程序返回
  (*hv_ErrorGBRows) = HTuple();
  if (0 != ((*hv_GB_Num)<18))
  {
    TupleInsert((*hv_RowGBTopCenters), 0, HTuple((*hv_RowGBTopCenters)[(*hv_GB_Num)-1])-12288, 
        &hv_Extended);
    TupleInsert(hv_Extended, hv_Extended.TupleLength(), HTuple((*hv_RowGBTopCenters)[0])+12288, 
        &hv_Extended);

    {
    HTuple end_val20 = (hv_Extended.TupleLength())-1;
    HTuple step_val20 = 1;
    for (hv_i=1; hv_i.Continue(end_val20, step_val20); hv_i += step_val20)
    {
      hv_Height = HTuple(hv_Extended[hv_i])-HTuple(hv_Extended[hv_i-1]);
      if (0 != (hv_Height>1000))
      {
        (*hv_ErrorGBRows) = (*hv_ErrorGBRows).TupleConcat(HTuple(hv_Extended[hv_i-1])+(hv_Height/2));
      }
    }
    }
  }
  if (0 != (((*hv_ErrorGBRows).TupleLength())>0))
  {
    TupleMean((*hv_ColumnGBTopCenters), &hv_ColumnGBTopMean);
    GenEmptyRegion(&(*ho_ErrorGBTopRegions));
    {
    HTuple end_val30 = ((*hv_ErrorGBRows).TupleLength())-1;
    HTuple step_val30 = 1;
    for (hv_i=0; hv_i.Continue(end_val30, step_val30); hv_i += step_val30)
    {
      GenRectangle2(&ho_ErrorRectangle, HTuple((*hv_ErrorGBRows)[hv_i]), hv_ColumnGBTopMean-150, 
          0, 250, 200);
      if (HDevWindowStack::IsOpen())
        SetDraw(HDevWindowStack::GetActive(),"margin");
      if (HDevWindowStack::IsOpen())
        SetLineWidth(HDevWindowStack::GetActive(),3);
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),"yellow");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_ErrorRectangle, HDevWindowStack::GetActive());
      ConcatObj((*ho_ErrorGBTopRegions), ho_ErrorRectangle, &(*ho_ErrorGBTopRegions)
          );
    }
    }
    return;
  }


  //形成钩部图像
  SmallestRectangle1(ho_RegionTrans, &hv_Row11, &hv_Column11, &hv_Row2, &hv_Column2);
  GenRectangle1(&ho_RectangleGB, hv_Row11-30, hv_Column11-360, hv_Row2+30, hv_Column2);

  Union1(ho_RectangleGB, &(*ho_GBRegionUnion));
  ReduceDomain(ho_GrayImage, (*ho_GBRegionUnion), &ho_RectangleGBImageReduced);


  //************************矩形测量,测量钩底中心列坐标************方法不准确
  GrayOpeningRect(ho_RectangleGBImageReduced, &ho_ImageClosing1, 11, 11);
  //*****此系数相应调整，以匹配***************
  hv_Sigma = 1.0;
  hv_AmplitudeThreshold = 20;
  hv_phi = HTuple(0).TupleRad();
  (*hv_RowGBBottomCenters) = HTuple();
  (*hv_ColumnGBBottomCenters) = HTuple();
  hv_length2 = 25;
  //***************************
  //gen_empty_region (GBBottomCircles)

  {
  HTuple end_val62 = (*hv_GB_Num);
  HTuple step_val62 = 1;
  for (hv_Index2=1; hv_Index2.Continue(end_val62, step_val62); hv_Index2 += step_val62)
  {
    //******RowGBTops[Index2-1], ColumnGBTops[Index2-1]-340*********
    //******顶点中心水平向左偏移位置340定为测量矩形坐标中心，宽度140，高度100****对应MeasureROI
    if (0 != (HTuple(HTuple((*hv_RowGBTopCenters)[hv_Index2-1])>0).TupleAnd(HTuple((*hv_RowGBTopCenters)[hv_Index2-1])<12288)))
    {
      GenRectangle2(&ho_MeasureROI, HTuple((*hv_RowGBTopCenters)[hv_Index2-1]), HTuple((*hv_ColumnGBTopCenters)[hv_Index2-1])-340, 
          hv_phi, 70, hv_length2);
      //dev_set_draw ('margin')
      //dev_display (MeasureROI)
      GenMeasureRectangle2(HTuple((*hv_RowGBTopCenters)[hv_Index2-1]), HTuple((*hv_ColumnGBTopCenters)[hv_Index2-1])-340, 
          hv_phi, 70, hv_length2, 2048, 12288, "nearest_neighbor", &hv_Measure_GBBottom);
      MeasurePos(ho_ImageClosing1, hv_Measure_GBBottom, hv_Sigma, hv_AmplitudeThreshold, 
          "negative", "first", &hv_Row_Measure_Bottom, &hv_Column_Measure_Bottom, 
          &hv_Amplitude_Measure_Bottom, &hv_Distance_Measure_Bottom);

      //********默认对比度找不出底边，阀值自减少5直至找出底边*******
      while (0 != ((hv_Row_Measure_Bottom.TupleLength())==0))
      {
        CloseMeasure(hv_Measure_GBBottom);
        hv_AmplitudeThreshold = hv_AmplitudeThreshold-5;
        GenMeasureRectangle2(HTuple((*hv_RowGBTopCenters)[hv_Index2-1]), HTuple((*hv_ColumnGBTopCenters)[hv_Index2-1])-340, 
            hv_phi, 70, hv_length2, 2048, 12288, "nearest_neighbor", &hv_Measure_GBBottom);
        MeasurePos(ho_ImageClosing1, hv_Measure_GBBottom, hv_Sigma, hv_AmplitudeThreshold, 
            "negative", "first", &hv_Row_Measure_Bottom, &hv_Column_Measure_Bottom, 
            &hv_Amplitude_Measure_Bottom, &hv_Distance_Measure_Bottom);
      }
      (*hv_RowGBBottomCenters) = (*hv_RowGBBottomCenters).TupleConcat(HTuple(hv_Row_Measure_Bottom[0]));
      (*hv_ColumnGBBottomCenters) = (*hv_ColumnGBBottomCenters).TupleConcat(HTuple(hv_Column_Measure_Bottom[0]));
      CloseMeasure(hv_Measure_GBBottom);
      hv_AmplitudeThreshold = 20;
    }
  }
  }

  TupleMean((*hv_ColumnGBBottomCenters), &hv_Mean);
  TupleSortIndex((*hv_ColumnGBBottomCenters), &hv_Indices);
  hv_AmplitudeThreshold = 20;
  hv_GreatMeanGrayThreshold = 18;
  for (hv_Index1=hv_Indices.TupleLength(); hv_Index1>=1; hv_Index1+=-1)
  {
    hv_SelectNum = ((const HTuple&)hv_Indices)[hv_Index1-1];
    if (0 != ((HTuple((*hv_ColumnGBBottomCenters)[hv_SelectNum])-hv_Mean)>hv_GreatMeanGrayThreshold))
    {
      hv_AmplitudeThreshold = hv_AmplitudeThreshold-5;
      GenRectangle2(&ho_Rectangle, HTuple((*hv_RowGBTopCenters)[hv_SelectNum]), HTuple((*hv_ColumnGBTopCenters)[hv_SelectNum])-340, 
          hv_phi, 70, hv_length2);
      GenMeasureRectangle2(HTuple((*hv_RowGBTopCenters)[HTuple(hv_Indices[hv_Index1-1])]), 
          HTuple((*hv_ColumnGBTopCenters)[hv_SelectNum])-340, hv_phi, 70, hv_length2, 
          2048, 12288, "nearest_neighbor", &hv_Measure_GBBottom);
      MeasurePos(ho_ImageClosing1, hv_Measure_GBBottom, hv_Sigma, hv_AmplitudeThreshold, 
          "negative", "first", &hv_Row_Measure_Bottom, &hv_Column_Measure_Bottom, 
          &hv_Amplitude_Measure_Bottom, &hv_Distance_Measure_Bottom);
      if (0 != ((hv_Column_Measure_Bottom.TupleLength())!=0))
      {
        while (0 != (HTuple(HTuple(hv_Column_Measure_Bottom[0])==HTuple((*hv_ColumnGBBottomCenters)[HTuple(hv_Indices[hv_Index1-1])])).TupleAnd(hv_AmplitudeThreshold!=0)))
        {
          hv_AmplitudeThreshold = hv_AmplitudeThreshold-5;
          GenRectangle2(&ho_Rectangle, HTuple((*hv_RowGBTopCenters)[hv_SelectNum]), 
              HTuple((*hv_ColumnGBTopCenters)[hv_SelectNum])-340, hv_phi, 70, hv_length2);
          GenMeasureRectangle2(HTuple((*hv_RowGBTopCenters)[hv_SelectNum]), HTuple((*hv_ColumnGBTopCenters)[hv_SelectNum])-340, 
              hv_phi, 70, hv_length2, 2048, 12288, "nearest_neighbor", &hv_Measure_GBBottom);
          MeasurePos(ho_ImageClosing1, hv_Measure_GBBottom, hv_Sigma, hv_AmplitudeThreshold, 
              "negative", "first", &hv_Row_Measure_Bottom, &hv_Column_Measure_Bottom, 
              &hv_Amplitude_Measure_Bottom, &hv_Distance_Measure_Bottom);
        }
        if (0 != (HTuple(hv_Column_Measure_Bottom[0])!=0))
        {
          TupleReplace((*hv_ColumnGBBottomCenters), hv_SelectNum, HTuple(hv_Column_Measure_Bottom[0]), 
              &(*hv_ColumnGBBottomCenters));
        }
      }
      CloseMeasure(hv_Measure_GBBottom);
    }
    else
    {
      break;
    }
    hv_AmplitudeThreshold = 20;
  }

  return;
}

// 片底部坐标和片区域
void CHH::PiecesAndBottomPoint (HObject ho_GrayImage, HObject *ho_SelectedRegionsPieces, 
							   HTuple hv_Bottom_Column, HTuple *hv_RowBottomsCenter, HTuple *hv_ColumnBottomsCenter)
{

	// Local iconic variables
	HObject  ho_ROI, ho_ImageReduced1, ho_ImageClosing;
	HObject  ho_Region, ho_RegionFillUp1, ho_RegionClosing, ho_emptyregion;
	HObject  ho_RegionExpand1, ho_RegionFillUp2, ho_RegionOpening2;
	HObject  ho_ConnectedRegions1, ho_RegionTranscircle, ho_ImageMeasureBottom;

	// Local control variables
	HTuple  hv_Value, hv_Height_inner_circle, hv_Area2;
	HTuple  hv_RowCenterPieces, hv_ColumnRowCenterPieces, hv_Num_Pieces;
	HTuple  hv_Threshold, hv_phi, hv_Index1, hv_length2, hv_MeasureHandle;
	HTuple  hv_RowEdge, hv_ColumnEdge, hv_Amplitude, hv_Distance;

	GenRectangle1(&ho_ROI, 0, hv_Bottom_Column, 12287, 2047);
	ReduceDomain(ho_GrayImage, ho_ROI, &ho_ImageReduced1);
	GrayClosingRect(ho_ImageReduced1, &ho_ImageClosing, 5, 5);
	Threshold(ho_ImageClosing, &ho_Region, 80, 255);
	//regiongrowing (ImageClosing, Region, 3, 3, 6, 100)
	FillUp(ho_Region, &ho_RegionFillUp1);
	ClosingCircle(ho_RegionFillUp1, &ho_RegionClosing, 15.5);
	GenEmptyRegion(&ho_emptyregion);
	ExpandGray(ho_RegionClosing, ho_ImageReduced1, ho_emptyregion, &ho_RegionExpand1, 
		6, "image", 10);
	FillUp(ho_RegionExpand1, &ho_RegionFillUp2);
	OpeningRectangle1(ho_RegionFillUp2, &ho_RegionOpening2, 156, 1);
	Connection(ho_RegionOpening2, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &(*ho_SelectedRegionsPieces), "width", "and", 
		500, 99999);
	RegionFeatures((*ho_SelectedRegionsPieces), "height", &hv_Value);

	ShapeTrans((*ho_SelectedRegionsPieces), &ho_RegionTranscircle, "inner_circle");
	RegionFeatures(ho_RegionTranscircle, "height", &hv_Height_inner_circle);
	AreaCenter(ho_RegionTranscircle, &hv_Area2, &hv_RowCenterPieces, &hv_ColumnRowCenterPieces);
	CountObj((*ho_SelectedRegionsPieces), &hv_Num_Pieces);
	hv_Threshold = 30;
	hv_phi = HTuple(-180).TupleRad();
	(*hv_RowBottomsCenter) = HTuple();
	(*hv_ColumnBottomsCenter) = HTuple();
	GrayClosingRect(ho_GrayImage, &ho_ImageMeasureBottom, 11, 11);
	{
		HTuple end_val24 = hv_Num_Pieces;
		HTuple step_val24 = 1;
		for (hv_Index1=1; hv_Index1.Continue(end_val24, step_val24); hv_Index1 += step_val24)
		{
			hv_length2 = HTuple(hv_Height_inner_circle[hv_Index1-1])/2;
			if (0 != (hv_length2>127))
			{
				hv_length2 = 127;
			}
			//gen_rectangle2 (MeasureBottomRect, RowCenterPieces[Index1-1], Bottom_Column+20, phi, 50, length2)
			GenMeasureRectangle2(HTuple(hv_RowCenterPieces[hv_Index1-1]), hv_Bottom_Column+20, 
				hv_phi, 50, hv_length2, 2048, 12288, "nearest_neighbor", &hv_MeasureHandle);
			MeasurePos(ho_ImageMeasureBottom, hv_MeasureHandle, 1, hv_Threshold, "negative", 
				"first", &hv_RowEdge, &hv_ColumnEdge, &hv_Amplitude, &hv_Distance);
			while (0 != ((hv_RowEdge.TupleLength())==0))
			{
				CloseMeasure(hv_MeasureHandle);
				hv_Threshold = hv_Threshold-5;
				GenMeasureRectangle2(HTuple(hv_RowCenterPieces[hv_Index1-1]), hv_Bottom_Column+50, 
					hv_phi, 50, hv_length2, 2048, 12288, "nearest_neighbor", &hv_MeasureHandle);
				MeasurePos(ho_ImageMeasureBottom, hv_MeasureHandle, 1, hv_Threshold, "negative", 
					"first", &hv_RowEdge, &hv_ColumnEdge, &hv_Amplitude, &hv_Distance);
			}
			(*hv_RowBottomsCenter) = (*hv_RowBottomsCenter).TupleConcat(hv_RowEdge);
			(*hv_ColumnBottomsCenter) = (*hv_ColumnBottomsCenter).TupleConcat(hv_ColumnEdge);
			hv_Threshold = 30;

		}
	}

	return;
}

//面阵圆中心
void CHH::AreaCenter (HObject ho_GrayImage, HObject ho_ROI, HObject *ho_ContCircle, 
    HTuple *hv_RowCenter, HTuple *hv_ColumnCenter, HTuple *hv_Radius)
{

  // Local iconic variables
  HObject  ho_ImageReduce, ho_Border, ho_UnionContours;
  HObject  ho_SelectedXLD, ho_CircleCenter;

  // Local control variables
  HTuple  hv_StartPhi, hv_EndPhi, hv_PointOrder;

  //*************
  //两白圈中黑环的半径和圆心坐标
  //*************


  ReduceDomain(ho_GrayImage, ho_ROI, &ho_ImageReduce);
  ThresholdSubPix(ho_ImageReduce, &ho_Border, 160);
  UnionAdjacentContoursXld(ho_Border, &ho_UnionContours, 10, 1, "attr_keep");
  SelectShapeXld(ho_UnionContours, &ho_SelectedXLD, "outer_radius", "and", 560, 1000);
  FitCircleContourXld(ho_SelectedXLD, "algebraic", -1, 0, 0, 3, 2, &(*hv_RowCenter), 
      &(*hv_ColumnCenter), &(*hv_Radius), &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
  GenCircleContourXld(&(*ho_ContCircle), (*hv_RowCenter), (*hv_ColumnCenter), (*hv_Radius), 
      0, 6.28318, "positive", 1);
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),"margin");
  GenCircle(&ho_CircleCenter, (*hv_RowCenter), (*hv_ColumnCenter), 6);
  return;
}
//检测顶面包络线
void CHH::DetectDiameter (HObject ho_Image, HObject *ho_InnerTorenceCircle, HObject *ho_OuterTorenceCircle, 
						 HObject *ho_Lines, HTuple hv_RowCenter, HTuple hv_ColumnCenter, HTuple hv_PixelLengthMM, 
						 HTuple hv_StandDiameterMM, HTuple hv_TorenceMM, HTuple hv_WindowHandle, HTuple *hv_Number, 
						 HTuple *hv_ShortNum, HTuple *hv_pcDistanceMaxs, HTuple *hv_ErrorNum,HTuple *hv_IsBadProduce)
{

	// Local iconic variables
	HObject  ho_InnerCircle, ho_OuterCircle, ho_RegionDifference;
	HObject  ho_ImageDoubleReduced, ho_Region, ho_RegionClosing;
	HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_ObjectSelected;
	HObject  ho_ROI_0;

	// Local control variables
	HTuple  hv_InnerTorenceRadius, hv_OuterTorenceRadius;
	HTuple  hv_InnerRadius, hv_OuterRadius, hv_AreaCenterRowsOut;
	HTuple  hv_AreaCenterColumsOut, hv_Index, hv_pcDistanceMin;
	HTuple  hv_pcDistanceMax, hv_Area, hv_RowCenter1, hv_ColumnCenter1;
	HTuple thresholdUp=230;


	(*hv_IsBadProduce) = 0;
	hv_InnerTorenceRadius = ((hv_StandDiameterMM-hv_TorenceMM)/hv_PixelLengthMM)/2;
	hv_OuterTorenceRadius = ((hv_StandDiameterMM+hv_TorenceMM)/hv_PixelLengthMM)/2;
	hv_InnerRadius = (((hv_StandDiameterMM-hv_TorenceMM)-1.5)/hv_PixelLengthMM)/2;
	hv_OuterRadius = (((hv_StandDiameterMM+hv_TorenceMM)+1.5)/hv_PixelLengthMM)/2;

	GenCircle(&ho_InnerCircle, hv_RowCenter, hv_ColumnCenter, hv_InnerRadius);
	GenCircle(&ho_OuterCircle, hv_RowCenter, hv_ColumnCenter, hv_OuterRadius);
	GenCircle(&(*ho_InnerTorenceCircle), hv_RowCenter, hv_ColumnCenter, hv_InnerTorenceRadius);
	GenCircle(&(*ho_OuterTorenceCircle), hv_RowCenter, hv_ColumnCenter, hv_OuterTorenceRadius);

	Difference(ho_OuterCircle, ho_InnerCircle, &ho_RegionDifference);
	Emphasize(ho_Image,&ho_Image,101,101,1);
	ReduceDomain(ho_Image, ho_RegionDifference, &ho_ImageDoubleReduced);
	Threshold(ho_ImageDoubleReduced, &ho_Region, 0, thresholdUp);
	ClosingCircle(ho_Region, &ho_RegionClosing, 4.5);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 5000, 20000);
	CountObj(ho_SelectedRegions, &(*hv_Number));


	//当环形图内选区不等于18时，错误，退出
	if (0 != ((*hv_Number)!=18))
	{
		(*hv_ShortNum) = 18-(*hv_Number);
		DispObj(ho_Image, hv_WindowHandle);
		disp_message(hv_WindowHandle, "超短钩部数："+(*hv_ShortNum), "image", 100, 670, 
			"black", "true");
		(*hv_IsBadProduce) = 1;
		//运行版执行以下函数，直接返回
		//return ()
	}


	//计算圆心到选区的最大距离
	(*hv_pcDistanceMaxs) = HTuple();
	hv_AreaCenterRowsOut = HTuple();
	hv_AreaCenterColumsOut = HTuple();
	GenEmptyRegion(&(*ho_Lines));
	{
		HTuple end_val37 = (*hv_Number);
		HTuple step_val37 = 1;
		for (hv_Index=1; hv_Index.Continue(end_val37, step_val37); hv_Index += step_val37)
		{
			SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index);
			DistancePr(ho_ObjectSelected, hv_RowCenter, hv_ColumnCenter, &hv_pcDistanceMin, 
				&hv_pcDistanceMax);
			AreaCenter(ho_ObjectSelected, &hv_Area, &hv_RowCenter1, &hv_ColumnCenter1);
			(*hv_pcDistanceMaxs) = (*hv_pcDistanceMaxs).TupleConcat(hv_pcDistanceMax);
			hv_AreaCenterRowsOut = hv_AreaCenterRowsOut.TupleConcat(hv_RowCenter1);
			hv_AreaCenterColumsOut = hv_AreaCenterColumsOut.TupleConcat(hv_ColumnCenter1);
				SetLineWidth(hv_WindowHandle,2);
			GenRegionLine(&ho_ROI_0, hv_RowCenter1, hv_ColumnCenter1, hv_RowCenter, hv_ColumnCenter);
			ConcatObj((*ho_Lines), ho_ROI_0, &(*ho_Lines));
		}
	}


	//显示线、圆心、公差线圈等
		SetLineWidth(hv_WindowHandle,1);
		SetColor(hv_WindowHandle,"red");
		DispObj((*ho_Lines), hv_WindowHandle);
		SetColor(hv_WindowHandle,"blue");
		DispObj((*ho_InnerTorenceCircle), hv_WindowHandle);
		SetColor(hv_WindowHandle,"red");
		DispObj((*ho_OuterTorenceCircle), hv_WindowHandle);
		DispCross(hv_WindowHandle, hv_RowCenter, hv_ColumnCenter, 45, 0);

	(*hv_ErrorNum)=0;
	//显示直径数据
	(*hv_pcDistanceMaxs) = (((*hv_pcDistanceMaxs))*hv_PixelLengthMM)*2;
	{
		HTuple end_val63 = (*hv_pcDistanceMaxs).TupleLength();
		HTuple step_val63 = 1;
		for (hv_Index=1; hv_Index.Continue(end_val63, step_val63); hv_Index += step_val63)
		{
			if (0 != (HTuple((*hv_pcDistanceMaxs)[hv_Index-1])<(hv_StandDiameterMM-hv_TorenceMM)))
			{
				disp_message(hv_WindowHandle, HTuple((*hv_pcDistanceMaxs)[hv_Index-1]), "image", 
					HTuple(hv_AreaCenterRowsOut[hv_Index-1]), HTuple(hv_AreaCenterColumsOut[hv_Index-1]), 
					"red", "true");
				(*hv_IsBadProduce) = 1;
				++(*hv_ErrorNum);
			}
			else if (0 != (HTuple((*hv_pcDistanceMaxs)[hv_Index-1])>(hv_StandDiameterMM+hv_TorenceMM)))
			{
				disp_message(hv_WindowHandle, HTuple((*hv_pcDistanceMaxs)[hv_Index-1]), "image", 
					HTuple(hv_AreaCenterRowsOut[hv_Index-1]), HTuple(hv_AreaCenterColumsOut[hv_Index-1]), 
					"blue", "true");
				(*hv_IsBadProduce) = 1;
				++(*hv_ErrorNum);
			}
			else
			{
				disp_message(hv_WindowHandle, HTuple((*hv_pcDistanceMaxs)[hv_Index-1]), "image", 
					HTuple(hv_AreaCenterRowsOut[hv_Index-1]), HTuple(hv_AreaCenterColumsOut[hv_Index-1]), 
					"black", "true");
			}
		}
	}
	//显示标准直径及公差文字
	disp_message(hv_WindowHandle, (("Φ"+hv_StandDiameterMM)+"±")+hv_TorenceMM, "image", 
		20, 670, "black", "true");

	if(*hv_IsBadProduce==0){
		disp_message(hv_WindowHandle, "包络线良好", "image", 
			60, 100, "green", "false");
	}
	return;
}

	//检测顶面气孔
void CHH::AreaHoles (HObject ho_GrayImage, HObject *ho_XYTransRegionError, HTuple hv_RowCenter, 
    HTuple hv_ColumnCenter, HTuple hv_OutRadius, HTuple hv_InnerRadius, HTuple hv_ErrorWidthMM, 
    HTuple hv_ErrorHeightMM, HTuple hv_WindowHandle, HTuple hv_PixelLength, HTuple dynDarkValue,HTuple *hv_Number, 
    HTuple *hv_WidthValue, HTuple *hv_HeightValue, HTuple *hv_AreaValue)
{

  // Local iconic variables
  HObject  ho_RegionOuterCircle, ho_RegionInnerCircle;
  HObject  ho_RegionOutRing, ho_ImageOutRing, ho_PolarTransImage;
  HObject  ho_ImageMean, ho_RegionDynThresh, ho_RegionOpening1;
  HObject  ho_RegionClosing, ho_emptyRegion, ho_RegionExpand;
  HObject  ho_ConnectedRegions, ho_SelectedRegionsError;

  // Local control variables
  HTuple  hv_Width, hv_Height, hv_WidthPolar, hv_HeightPolar;
  HTuple  hv_Index;

  //OutRadius := 619
  //InnerRadius := 389
 GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
  GenCircle(&ho_RegionOuterCircle, hv_RowCenter, hv_ColumnCenter, hv_OutRadius);
  GenCircle(&ho_RegionInnerCircle, hv_RowCenter, hv_ColumnCenter, hv_InnerRadius);
  //*******查找外圈减内圈区域*************
  Difference(ho_RegionOuterCircle, ho_RegionInnerCircle, &ho_RegionOutRing);
  ReduceDomain(ho_GrayImage, ho_RegionOutRing, &ho_ImageOutRing);

  GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
  GenCircle(&ho_RegionOuterCircle, hv_RowCenter, hv_ColumnCenter, hv_OutRadius);
  GenCircle(&ho_RegionInnerCircle, hv_RowCenter, hv_ColumnCenter, hv_InnerRadius);
  //*******查找外圈减内圈区域*************
  Difference(ho_RegionOuterCircle, ho_RegionInnerCircle, &ho_RegionOutRing);
  ReduceDomain(ho_GrayImage, ho_RegionOutRing, &ho_ImageOutRing);

  //******极坐标转换**************
  hv_WidthPolar = (2*3.1415926)*hv_OutRadius;
  hv_HeightPolar = hv_OutRadius-hv_InnerRadius;
  PolarTransImageExt(ho_ImageOutRing, &ho_PolarTransImage, hv_RowCenter, hv_ColumnCenter, 
      0, 6.28319, hv_OutRadius, hv_InnerRadius, hv_WidthPolar, hv_HeightPolar, "nearest_neighbor");
  //**************************`****

  //****************************
  MeanImage(ho_PolarTransImage, &ho_ImageMean, 4000, 1);
  DynThreshold(ho_PolarTransImage, ho_ImageMean, &ho_RegionDynThresh, dynDarkValue, "dark");
  OpeningCircle(ho_RegionDynThresh, &ho_RegionOpening1, 6.5);
  ClosingCircle(ho_RegionOpening1, &ho_RegionClosing, 3.5);
  GenEmptyRegion(&ho_emptyRegion);
  ExpandGray(ho_RegionClosing, ho_PolarTransImage, ho_emptyRegion, &ho_RegionExpand, 
      4, "image", 10);
  Connection(ho_RegionExpand, &ho_ConnectedRegions);
  try{
	  SelectShape(ho_ConnectedRegions, &ho_SelectedRegionsError, (HTuple("width").Append("height")), 
		  "and", (hv_ErrorHeightMM/hv_PixelLength).TupleConcat(hv_ErrorHeightMM/hv_PixelLength), 
		  (HTuple(4200).Append(250)));
	  CountObj(ho_SelectedRegionsError, &(*hv_Number));
  }
  catch(HException& e)
  {
	  e;
	  *hv_Number=0;
	   // disp_message(hv_WindowHandle, "无气孔", "image", 300, 100, "green", "false");
		return;
  }

  if (0 != ((*hv_Number)!=0))
  {
    RegionFeatures(ho_SelectedRegionsError, "width", &(*hv_WidthValue));
    RegionFeatures(ho_SelectedRegionsError, "height", &(*hv_HeightValue));
    RegionFeatures(ho_SelectedRegionsError, "area", &(*hv_AreaValue));

      ClearWindow(hv_WindowHandle);
      DispObj(ho_GrayImage, hv_WindowHandle);
    {
    HTuple end_val32 = (*hv_Number);
    HTuple step_val32 = 1;
    for (hv_Index=1; hv_Index.Continue(end_val32, step_val32); hv_Index += step_val32)
    {
      disp_message(hv_WindowHandle, (("气孔"+hv_Index)+":  宽：")+(HTuple((*hv_WidthValue)[hv_Index-1])*hv_PixelLength), 
          "image", (hv_Index-1)*300, 10, "red", "false");
      disp_message(hv_WindowHandle, (("气孔"+hv_Index)+":  高：")+(HTuple((*hv_HeightValue)[hv_Index-1])*hv_PixelLength), 
          "image", ((hv_Index-1)*300)+100, 10, "red", "false");
      disp_message(hv_WindowHandle, (("气孔"+hv_Index)+":面积：")+((HTuple((*hv_AreaValue)[hv_Index-1])*hv_PixelLength)*hv_PixelLength), 
          "image", ((hv_Index-1)*300)+200, 10, "red", "false");
    }
    }
    PolarTransRegionInv(ho_SelectedRegionsError, &(*ho_XYTransRegionError), hv_RowCenter, 
        hv_ColumnCenter, 0, 6.28319, hv_OutRadius, hv_InnerRadius, hv_WidthPolar, 
        hv_HeightPolar, hv_Width, hv_Height, "nearest_neighbor");
      SetDraw(hv_WindowHandle,"margin");
      SetLineWidth(hv_WindowHandle,3);
	  SetColored(hv_WindowHandle,12);
      DispObj((*ho_XYTransRegionError), hv_WindowHandle);
  }
  else
  {
      DispObj(ho_GrayImage, hv_WindowHandle);
   // disp_message(hv_WindowHandle, "无气孔", "image", 300, 100, "green", "false");
  }
  return;
}

//顶部外沿气孔
void CHH::AreaOutHole (HObject ho_Image, HObject *ho_BadTransRegions, HTuple hv_RowCenter, 
					  HTuple hv_ColumnCenter, HTuple hv_WindowHandle, HTuple *hv_GBIsOutHoles)
{

	// Local iconic variables
	HObject  ho_CircleOUt, ho_CircleInt, ho_RegionDoubleCicle;
	HObject  ho_ImageOutRing, ho_ImageEmphasize, ho_MearsureCircle;
	HObject  ho_StartCirlces, ho_CenterCircles, ho_EndCircles;
	HObject  ho_PolarTransImage, ho_PolarEmphasize, ho_Region;
	HObject  ho_ConnectedRegions, ho_SortedRegions, ho_ObjectFirst;
	HObject  ho_ObjectLast, ho_RegionUnionDisa, ho_SelectedWhiteRegions;
	HObject  ho_RegionDifference, ho_RegionUnion, ho_RegionClosing2;
	HObject  ho_RegionFillUp, ho_ConnectedRegions1, ho_SelectedRegions;
	HObject  ho_GoodRegions, ho_AllObjects, ho_Rectangle, ho_BadRegionDifference;
	HObject  ho_BadRegions;

	// Local control variables
	HTuple  hv_OutRadius, hv_InnerRadius, hv_WidthPolar;
	HTuple  hv_HeightPolar, hv_Width, hv_Height, hv_Mearsure_Radius;
	HTuple  hv_AngleStart, hv_AngleEnd, hv_Dgree_Center, hv_Dgree_Start;
	HTuple  hv_Dgree_End, hv_Number, hv_Sequence, hv_Area, hv_Row;
	HTuple  hv_Column, hv_MeanRow, hv_i;

	(*hv_GBIsOutHoles) = 0;

	hv_OutRadius = 800;
	hv_InnerRadius = 720;
	hv_WidthPolar = (2*(HTuple(180).TupleRad()))*hv_OutRadius;
	hv_HeightPolar = hv_OutRadius-hv_InnerRadius;

	GenCircle(&ho_CircleOUt, hv_RowCenter, hv_ColumnCenter, hv_OutRadius);
	GenCircle(&ho_CircleInt, hv_RowCenter, hv_ColumnCenter, hv_InnerRadius);
	Difference(ho_CircleOUt, ho_CircleInt, &ho_RegionDoubleCicle);

	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	ReduceDomain(ho_Image, ho_RegionDoubleCicle, &ho_ImageOutRing);
	Emphasize(ho_Image, &ho_ImageEmphasize, 301, 301, 1);
	hv_Mearsure_Radius = 1020;
	CHH::FirstPhi(ho_ImageEmphasize, &ho_MearsureCircle, &ho_StartCirlces, &ho_CenterCircles, 
		&ho_EndCircles, hv_RowCenter, hv_ColumnCenter, hv_Mearsure_Radius, &hv_AngleStart, 
		&hv_AngleEnd, &hv_Dgree_Center, &hv_Dgree_Start, &hv_Dgree_End);
	//展开环形图成矩形
		PolarTransImageExt(ho_ImageOutRing, &ho_PolarTransImage, hv_RowCenter, hv_ColumnCenter, 
		hv_AngleStart, hv_AngleEnd, hv_OutRadius, hv_InnerRadius, hv_WidthPolar, hv_HeightPolar, 
		"nearest_neighbor");

	Emphasize(ho_PolarTransImage, &ho_PolarEmphasize, 101, 101, 1);
	MeanImage(ho_PolarEmphasize, &ho_PolarEmphasize, 25, 25);
	LocalThreshold(ho_PolarEmphasize, &ho_Region, "adapted_std_deviation", "light", 
		"mask_size", 105);
	ClosingCircle(ho_Region, &ho_Region, 1.5);
	OpeningRectangle1(ho_Region, &ho_Region, 10,1);
	ClosingRectangle1(ho_Region, &ho_Region, 20, 1);
	Connection(ho_Region, &ho_ConnectedRegions);
	SortRegion(ho_ConnectedRegions, &ho_SortedRegions, "first_point", "true", "column");
	CountObj(ho_SortedRegions, &hv_Number);
	SelectObj(ho_SortedRegions, &ho_ObjectFirst, 1);
	SelectObj(ho_SortedRegions, &ho_ObjectLast, hv_Number);
	Union2(ho_ObjectFirst, ho_ObjectLast, &ho_RegionUnionDisa);
	SelectShape(ho_SortedRegions, &ho_SelectedWhiteRegions, "width", "and", 110, 99999);
	Difference(ho_SortedRegions, ho_RegionUnionDisa, &ho_RegionDifference);
	Difference(ho_RegionDifference, ho_SelectedWhiteRegions, &ho_RegionDifference);

	Union1(ho_RegionDifference, &ho_RegionUnion);

	ClosingCircle(ho_RegionUnion, &ho_RegionClosing2, 3.5);
	ClosingRectangle1(ho_RegionClosing2, &ho_RegionClosing2, 1, 15);
	FillUp(ho_RegionClosing2, &ho_RegionFillUp);
	Connection(ho_RegionFillUp, &ho_ConnectedRegions1);
	//选择好的blob
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions, ((HTuple("area").Append("ra")).Append("rb").Append("convexity")), 
		"and", ((HTuple(2800).Append(30)).Append(20).Append(0.75)), ((HTuple(9999).Append(60)).Append(50).Append(1.0)));
	SortRegion(ho_SelectedRegions, &ho_GoodRegions, "first_point", "true", "column");
	CountObj(ho_GoodRegions, &hv_Number);

	//如果小于18个blob,生成所有理想中的18个blob与GoodRegion对比
		if (0 != (hv_Number<18))
		{
			(*hv_GBIsOutHoles) = 1;
			TupleGenSequence(141, 5000, 279, &hv_Sequence);
			AreaCenter(ho_GoodRegions, &hv_Area, &hv_Row, &hv_Column);
			TupleMean(hv_Row, &hv_MeanRow);

			GenEmptyObj(&ho_AllObjects);
			for (hv_i=0; hv_i<=17; hv_i+=1)
			{
				GenRectangle2(&ho_Rectangle, hv_MeanRow, HTuple(hv_Sequence[hv_i]), 0, 30, 
					40);
				ConcatObj(ho_AllObjects, ho_Rectangle, &ho_AllObjects);
			}

			Difference(ho_AllObjects, ho_GoodRegions, &ho_BadRegionDifference);
			SelectShape(ho_BadRegionDifference, &ho_BadRegions, "area", "and", 3000, 99999);
			disp_message(hv_WindowHandle, "顶面气孔缺陷", "image", 20, 3000, "red", "true");
			PolarTransRegionInv(ho_BadRegions, &(*ho_BadTransRegions), hv_RowCenter, hv_ColumnCenter, 
				hv_AngleStart, hv_AngleEnd, hv_OutRadius, hv_InnerRadius, hv_WidthPolar, 
				hv_HeightPolar, hv_Width, hv_Height, "nearest_neighbor");

			SetDraw(hv_WindowHandle,"margin");
			SetColor(hv_WindowHandle,"red");
			SetLineWidth(hv_WindowHandle,2);
			DilationCircle((*ho_BadTransRegions), &(*ho_BadTransRegions), 45.5);
			DispObj((*ho_BadTransRegions), hv_WindowHandle);
		}
		return;
}



//槽边缘检测
void CHH::Gap_EdgesAndRegressLines (HObject ho_EmphaImage, HObject ho_ObjectSelected, 
    HObject *ho_SelectedEdges, HObject *ho_RegressContour0, HObject *ho_RegressContour1, 
    HObject *ho_GapWidthContours, HObject *ho_Arrow, HObject *ho_RegionError, HObject *ho_ErrorEdge, 
    HTuple *hv_Gap_LRCLengths, HTuple *hv_RowRegressCenter, HTuple *hv_ColRegressCenter)
{

  // Local iconic variables
  HObject  ho_RegionBorder, ho_RegionDilation, ho_ImageReduced1;
  HObject  ho_Edges, ho_UnionContours1, ho_ObjectSelected1;
  HObject  ho_ObjectSelected2, ho_GapWidthContour, ho_ObjectSelected3;
  HObject  ho_Arrow0, ho_Arrow1, ho_UpEdge, ho_Polygons_UpEdge;
  HObject  ho_Region_Polygon_UpEdges, ho_DownEdge, ho_Polygons_DownEdge;
  HObject  ho_Region_Polygon_DownEdges, ho_Polygons_RegressContour0;
  HObject  ho_Region_Polygon_RegressContour0, ho_Polygons_RegressContour1;
  HObject  ho_Region_Polygon_RegressContour1, ho_RegionUnionUp;
  HObject  ho_RegionUnionUpFill, ho_RegionError2, ho_RegionUnionDown;
  HObject  ho_RegionUnionDownFill, ho_RegionError3;

  // Local control variables
  HTuple  hv_Alpha, hv_Length, hv_Row11, hv_Column1;
  HTuple  hv_Row21, hv_Column2, hv_Diameter, hv_Number, hv_n;
  HTuple  hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd;
  HTuple  hv_Nr, hv_Nc, hv_Dist1, hv_DistanceMin, hv_DistanceMax;
  HTuple  hv_DistanceMin1, hv_DistanceMax1, hv_RowRegress0Points;
  HTuple  hv_ColRegress0Points, hv_RowRegress0, hv_ColRegress0;
  HTuple  hv_Area2, hv_RowRegressCenter0, hv_ColRegressCenter0;
  HTuple  hv_i, hv_RowRegressProj0, hv_ColRegressProj0, hv_Abs;
  HTuple  hv_Row1, hv_Col1, hv_Length1, hv_Phi1, hv_Row2;
  HTuple  hv_Col2, hv_Length2, hv_Phi2, hv_Row3, hv_Col3;
  HTuple  hv_Length3, hv_Phi3, hv_Phi, hv_Area3, hv_Column3;

  hv_Alpha = 0.2;
  Boundary(ho_ObjectSelected, &ho_RegionBorder, "inner_filled");
  DilationCircle(ho_RegionBorder, &ho_RegionDilation, 11.5);
  ReduceDomain(ho_EmphaImage, ho_RegionDilation, &ho_ImageReduced1);
  EdgesSubPix(ho_ImageReduced1, &ho_Edges, "lanser1_junctions", hv_Alpha, 20, 40);
  UnionAdjacentContoursXld(ho_Edges, &ho_UnionContours1, 50, 1, "attr_keep");
  SelectContoursXld(ho_UnionContours1, &(*ho_SelectedEdges), "contour_length", 500, 
      99999, -0.5, 0.5);
  LengthXld((*ho_SelectedEdges), &hv_Length);
  DiameterXld((*ho_SelectedEdges), &hv_Row11, &hv_Column1, &hv_Row21, &hv_Column2, 
      &hv_Diameter);
  CountObj((*ho_SelectedEdges), &hv_Number);

  GenEmptyObj(&(*ho_ErrorEdge));
  //最大宽度小于1000退出，显示错误ErrorEdge
  {
  HTuple end_val13 = hv_Number;
  HTuple step_val13 = 1;
  for (hv_n=1; hv_n.Continue(end_val13, step_val13); hv_n += step_val13)
  {
    SelectObj((*ho_SelectedEdges), &ho_ObjectSelected, hv_n);
    if (0 != (HTuple(hv_Diameter[hv_n-1])<1000))
    {
      ConcatObj((*ho_ErrorEdge), ho_ObjectSelected, &(*ho_ErrorEdge));
      return;
    }
  }
  }

  if (0 != (hv_Number==2))
  {
    SelectObj((*ho_SelectedEdges), &ho_ObjectSelected1, 1);
    SelectObj((*ho_SelectedEdges), &ho_ObjectSelected2, 2);
    FitLineContourXld((*ho_SelectedEdges), "regression", -1, 100, 5, 2, &hv_RowBegin, 
        &hv_ColBegin, &hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist1);
    GenContourPolygonXld(&(*ho_RegressContour0), HTuple(hv_RowBegin[0]).TupleConcat(HTuple(hv_RowEnd[0])), 
        HTuple(hv_ColBegin[0]).TupleConcat(HTuple(hv_ColEnd[0])));
    GenContourPolygonXld(&(*ho_RegressContour1), HTuple(hv_RowEnd[1]).TupleConcat(HTuple(hv_RowBegin[1])), 
        HTuple(hv_ColEnd[1]).TupleConcat(HTuple(hv_ColBegin[1])));
    //两回归线的距离
    DistanceLc((*ho_RegressContour0), HTuple(hv_RowEnd[1]), HTuple(hv_ColEnd[1]), 
        HTuple(hv_RowBegin[1]), HTuple(hv_ColBegin[1]), &hv_DistanceMin, &hv_DistanceMax);
    //回归线与分割线的距离
    DistanceLc(ho_ObjectSelected1, HTuple(hv_RowEnd[0]), HTuple(hv_ColEnd[0]), HTuple(hv_RowBegin[0]), 
        HTuple(hv_ColBegin[0]), &hv_DistanceMin1, &hv_DistanceMax1);

    hv_RowRegress0Points = HTuple();
    hv_ColRegress0Points = HTuple();
    GetContourXld((*ho_RegressContour0), &hv_RowRegress0, &hv_ColRegress0);
    if (0 != (HTuple(hv_ColRegress0[0])>HTuple(hv_ColRegress0[1])))
    {
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[1]));
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[0]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[1]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[0]));
    }
    else
    {
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[0]));
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[1]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[0]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[1]));
    }

    AreaCenterPointsXld((*ho_RegressContour0), &hv_Area2, &hv_RowRegressCenter0, 
        &hv_ColRegressCenter0);
    hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(hv_ColRegressCenter0);
    hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(hv_RowRegressCenter0);
    GenEmptyObj(&(*ho_GapWidthContours));
    {
    HTuple end_val51 = (hv_ColRegress0Points.TupleLength())-1;
    HTuple step_val51 = 1;
    for (hv_i=0; hv_i.Continue(end_val51, step_val51); hv_i += step_val51)
    {
      ProjectionPl(HTuple(hv_RowRegress0Points[hv_i]), HTuple(hv_ColRegress0Points[hv_i]), 
          HTuple(hv_RowBegin[1]), HTuple(hv_ColBegin[1]), HTuple(hv_RowEnd[1]), HTuple(hv_ColEnd[1]), 
          &hv_RowRegressProj0, &hv_ColRegressProj0);
      GenContourPolygonXld(&ho_GapWidthContour, HTuple(hv_RowRegress0Points[hv_i]).TupleConcat(hv_RowRegressProj0), 
          HTuple(hv_ColRegress0Points[hv_i]).TupleConcat(hv_ColRegressProj0));
      ConcatObj((*ho_GapWidthContours), ho_GapWidthContour, &(*ho_GapWidthContours)
          );
    }
    }
    LengthXld((*ho_GapWidthContours), &(*hv_Gap_LRCLengths));
    //左右端线距离偏离值大于一定值时，该边缘线平行度偏低，有异常
    TupleAbs(HTuple((*hv_Gap_LRCLengths)[1])-HTuple((*hv_Gap_LRCLengths)[0]), &hv_Abs);
    if (0 != (hv_Abs>20))
    {
      ConcatObj((*ho_ErrorEdge), (*ho_SelectedEdges), &(*ho_ErrorEdge));
    }
    //*************
    SelectObj((*ho_GapWidthContours), &ho_ObjectSelected3, 3);
    AreaCenterPointsXld(ho_ObjectSelected3, &hv_Area2, &(*hv_RowRegressCenter), &(*hv_ColRegressCenter));
    gen_arrow_contour_xld(&ho_Arrow0, hv_RowRegressProj0, hv_ColRegressProj0, hv_RowRegressCenter0, 
        hv_ColRegressCenter0, 30, 25);
    gen_arrow_contour_xld(&ho_Arrow1, hv_RowRegressCenter0, hv_ColRegressCenter0, 
        hv_RowRegressProj0, hv_ColRegressProj0, 30, 25);
    GenEmptyObj(&(*ho_Arrow));
    ConcatObj((*ho_Arrow), ho_Arrow0, &(*ho_Arrow));
    ConcatObj((*ho_Arrow), ho_Arrow1, &(*ho_Arrow));
  }
  else if (0 != (hv_Number==1))
  {
    SelectObj((*ho_SelectedEdges), &ho_ObjectSelected1, 1);
    FitLineContourXld((*ho_SelectedEdges), "regression", -1, 0, 5, 2, &hv_RowBegin, 
        &hv_ColBegin, &hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist1);
    if (0 != (hv_RowBegin<150))
    {
      TupleInsert(hv_RowBegin, 0, 0, &hv_RowBegin);
      TupleInsert(hv_RowEnd, 0, 0, &hv_RowEnd);
      TupleInsert(hv_ColBegin, 0, hv_ColBegin, &hv_ColBegin);
      TupleInsert(hv_ColEnd, 0, hv_ColEnd, &hv_ColEnd);

    }
    else
    {
      TupleInsert(hv_RowBegin, 1, 12287, &hv_RowBegin);
      TupleInsert(hv_RowEnd, 1, 12287, &hv_RowEnd);
      TupleInsert(hv_ColBegin, 1, hv_ColBegin, &hv_ColBegin);
      TupleInsert(hv_ColEnd, 1, hv_ColEnd, &hv_ColEnd);
    }
    GenContourPolygonXld(&(*ho_RegressContour0), HTuple(hv_RowBegin[0]).TupleConcat(HTuple(hv_RowEnd[0])), 
        HTuple(hv_ColBegin[0]).TupleConcat(HTuple(hv_ColEnd[0])));
    GenContourPolygonXld(&(*ho_RegressContour1), HTuple(hv_RowEnd[1]).TupleConcat(HTuple(hv_RowBegin[1])), 
        HTuple(hv_ColEnd[1]).TupleConcat(HTuple(hv_ColBegin[1])));
    if (0 != (HTuple(hv_RowBegin[1])<150))
    {
      ConcatObj((*ho_SelectedEdges), (*ho_RegressContour0), &(*ho_SelectedEdges));
    }
    else
    {
      ConcatObj((*ho_SelectedEdges), (*ho_RegressContour1), &(*ho_SelectedEdges));
    }
    SortContoursXld((*ho_SelectedEdges), &(*ho_SelectedEdges), "upper_left", "true", 
        "row");
    //两回归线的距离
    DistanceLc((*ho_RegressContour0), HTuple(hv_RowEnd[1]), HTuple(hv_ColEnd[1]), 
        HTuple(hv_RowBegin[1]), HTuple(hv_ColBegin[1]), &hv_DistanceMin, &hv_DistanceMax);
    //回归线与分割线的距离
    DistanceLc(ho_ObjectSelected1, HTuple(hv_RowEnd[0]), HTuple(hv_ColEnd[0]), HTuple(hv_RowBegin[0]), 
        HTuple(hv_ColBegin[0]), &hv_DistanceMin1, &hv_DistanceMax1);

    hv_RowRegress0Points = HTuple();
    hv_ColRegress0Points = HTuple();
    GetContourXld((*ho_RegressContour0), &hv_RowRegress0, &hv_ColRegress0);
    if (0 != (HTuple(hv_ColRegress0[0])>HTuple(hv_ColRegress0[1])))
    {
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[1]));
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[0]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[1]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[0]));
    }
    else
    {
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[0]));
      hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(HTuple(hv_ColRegress0[1]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[0]));
      hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(HTuple(hv_RowRegress0[1]));
    }

    AreaCenterPointsXld((*ho_RegressContour0), &hv_Area2, &hv_RowRegressCenter0, 
        &hv_ColRegressCenter0);
    hv_ColRegress0Points = hv_ColRegress0Points.TupleConcat(hv_ColRegressCenter0);
    hv_RowRegress0Points = hv_RowRegress0Points.TupleConcat(hv_RowRegressCenter0);
    GenEmptyObj(&(*ho_GapWidthContours));
    {
    HTuple end_val117 = (hv_ColRegress0Points.TupleLength())-1;
    HTuple step_val117 = 1;
    for (hv_i=0; hv_i.Continue(end_val117, step_val117); hv_i += step_val117)
    {
      ProjectionPl(HTuple(hv_RowRegress0Points[hv_i]), HTuple(hv_ColRegress0Points[hv_i]), 
          HTuple(hv_RowBegin[1]), HTuple(hv_ColBegin[1]), HTuple(hv_RowEnd[1]), HTuple(hv_ColEnd[1]), 
          &hv_RowRegressProj0, &hv_ColRegressProj0);
      GenContourPolygonXld(&ho_GapWidthContour, HTuple(hv_RowRegress0Points[hv_i]).TupleConcat(hv_RowRegressProj0), 
          HTuple(hv_ColRegress0Points[hv_i]).TupleConcat(hv_ColRegressProj0));
      ConcatObj((*ho_GapWidthContours), ho_GapWidthContour, &(*ho_GapWidthContours)
          );
    }
    }

    LengthXld((*ho_GapWidthContours), &(*hv_Gap_LRCLengths));
    SelectObj((*ho_GapWidthContours), &ho_ObjectSelected3, 3);
    AreaCenterPointsXld(ho_ObjectSelected3, &hv_Area2, &(*hv_RowRegressCenter), &(*hv_ColRegressCenter));
    gen_arrow_contour_xld(&ho_Arrow0, hv_RowRegressProj0, hv_ColRegressProj0, hv_RowRegressCenter0, 
        hv_ColRegressCenter0, 30, 25);
    gen_arrow_contour_xld(&ho_Arrow1, hv_RowRegressCenter0, hv_ColRegressCenter0, 
        hv_RowRegressProj0, hv_ColRegressProj0, 30, 25);
    GenEmptyObj(&(*ho_Arrow));
    ConcatObj((*ho_Arrow), ho_Arrow0, &(*ho_Arrow));
    ConcatObj((*ho_Arrow), ho_Arrow1, &(*ho_Arrow));
  }

  //********************************************
  //分割线与回归线生成单像素的Region(Contour转化成polygon再通过其point,转化为单像素的Region（lines）)
  //上分割线：Region_Polygon_UpEdges
  //下分割线：Region_Polygon_DownEdges
  //上回归线：Region_Polygon_RegressContour0
  //下回归线：Region_Polygon_RegressContour1

  SelectObj((*ho_SelectedEdges), &ho_UpEdge, 1);
  GenPolygonsXld(ho_UpEdge, &ho_Polygons_UpEdge, "ramer", 2);
  GetPolygonXld(ho_Polygons_UpEdge, &hv_Row1, &hv_Col1, &hv_Length1, &hv_Phi1);
  GenRegionPolygon(&ho_Region_Polygon_UpEdges, hv_Row1, hv_Col1);

  SelectObj((*ho_SelectedEdges), &ho_DownEdge, 2);
  GenPolygonsXld(ho_DownEdge, &ho_Polygons_DownEdge, "ramer", 2);
  GetPolygonXld(ho_Polygons_DownEdge, &hv_Row2, &hv_Col2, &hv_Length2, &hv_Phi2);
  GenRegionPolygon(&ho_Region_Polygon_DownEdges, hv_Row2, hv_Col2);

  GenPolygonsXld((*ho_RegressContour0), &ho_Polygons_RegressContour0, "ramer", 2);
  GetPolygonXld(ho_Polygons_RegressContour0, &hv_Row3, &hv_Col3, &hv_Length3, &hv_Phi3);
  GenRegionPolygon(&ho_Region_Polygon_RegressContour0, hv_Row3, hv_Col3);

  GenPolygonsXld((*ho_RegressContour1), &ho_Polygons_RegressContour1, "ramer", 2);
  GetPolygonXld(ho_Polygons_RegressContour1, &hv_Row3, &hv_Col3, &hv_Length, &hv_Phi);
  GenRegionPolygon(&ho_Region_Polygon_RegressContour1, hv_Row3, hv_Col3);


  //******************************************
  GenEmptyObj(&(*ho_RegionError));


  //******************************
 // 边缘线与其对应回归线转化成区域，合并，填充，做相应形态学操作,找到边缘线偏离其对应回归线较大区域
  Union2(ho_Region_Polygon_UpEdges, ho_Region_Polygon_RegressContour0, &ho_RegionUnionUp );
  FillUp(ho_RegionUnionUp, &ho_RegionUnionUpFill);
  OpeningRectangle1(ho_RegionUnionUpFill, &ho_RegionError2, 1, 20);
  AreaCenter(ho_RegionError2, &hv_Area2, &hv_Row2, &hv_Column2);
  if (0 != (hv_Area2!=0))
  {
    ConcatObj((*ho_RegionError), ho_RegionError2, &(*ho_RegionError));
  }

  Union2(ho_Region_Polygon_DownEdges, ho_Region_Polygon_RegressContour1, &ho_RegionUnionDown
      );
  FillUp(ho_RegionUnionDown, &ho_RegionUnionDownFill);
  OpeningRectangle1(ho_RegionUnionDownFill, &ho_RegionError3, 1, 20);
  AreaCenter(ho_RegionError3, &hv_Area3, &hv_Row3, &hv_Column3);
  if (0 != (hv_Area3!=0))
  {
    ConcatObj((*ho_RegionError), ho_RegionError3, &(*ho_RegionError));
  }

  return;
}

void CHH::GapFeature (HObject ho_EmphaImage, HObject *ho_RegionErrorOthers, HObject *ho_ErrorEdges, 
    HObject *ho_RegionErrors, HObject *ho_Edges, HObject *ho_Arrows, HObject *ho_RegressContours0, 
    HObject *ho_RegressContours1, HObject *ho_HorArrowlines, HObject *ho_GapToGapArrows, 
    HObject *ho_GapToGapContours, HTuple hv_side_column, HTuple hv_GapCutColumn, 
    HTuple hv_WindowHandle, HTuple hv_LengthPerColPiexl, HTuple hv_DgreePerPiexl, 
    HTuple *hv_RowRegressCenters, HTuple *hv_ColRegressCenters, HTuple *hv_GapCenterWidths, 
    HTuple *hv_GapCenterWidthsMM, HTuple *hv_GapsDegree)
{

  // Local iconic variables
  HObject  ho_ROI, ho_ImageReduced, ho_Region, ho_ConnectedRegions;
  HObject  ho_RegionFillUp, ho_RegionClosing, ho_SelectedRegionGaps;
  HObject  ho_ObjectSelected, ho_SelectedEdges, ho_RegressContour0;
  HObject  ho_RegressContour1, ho_GapWidthContours1, ho_Arrow;
  HObject  ho_RegionError, ho_ErrorEdge;

  // Local control variables
  HTuple  hv_Number, hv_SelectedRegionGapsHeight;
  HTuple  hv_SelectedRegionGapsWidth, hv_NormalGapHeight;
  HTuple  hv_NormalGapWidth, hv_n, hv_Area3, hv_Row, hv_Column;
  HTuple  hv_i, hv_LRCLength, hv_RowRegressCenter, hv_ColRegressCenter;
  HTuple  hv_Area2, hv_Row1, hv_Column2, hv_Number1;

  GenEmptyObj(&(*ho_Edges));
  GenEmptyObj(&(*ho_Arrows));
  GenEmptyObj(&(*ho_RegressContours0));
  GenEmptyObj(&(*ho_RegressContours1));
  GenEmptyObj(&(*ho_RegionErrors));
  GenEmptyObj(&(*ho_ErrorEdges));
  GenEmptyObj(&(*ho_RegionErrorOthers));
  (*hv_GapCenterWidths) = HTuple();
  (*hv_RowRegressCenters) = HTuple();
  (*hv_ColRegressCenters) = HTuple();


  GenRectangle1(&ho_ROI, 0, hv_side_column, 12287, hv_GapCutColumn);
  ReduceDomain(ho_EmphaImage, ho_ROI, &ho_ImageReduced);

  Threshold(ho_ImageReduced, &ho_Region, 0, 10);
  Connection(ho_Region, &ho_ConnectedRegions);
  FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
  OpeningCircle(ho_RegionFillUp, &ho_RegionClosing, 3.5);
  Connection(ho_RegionClosing, &ho_ConnectedRegions);
  SelectShape(ho_ConnectedRegions, &ho_SelectedRegionGaps, "width", "and", 500, 20000);
  SortRegion(ho_SelectedRegionGaps, &ho_SelectedRegionGaps, "first_point", "true", 
      "row");
  CountObj(ho_SelectedRegionGaps, &hv_Number);
  RegionFeatures(ho_SelectedRegionGaps, "height", &hv_SelectedRegionGapsHeight);
  RegionFeatures(ho_SelectedRegionGaps, "width", &hv_SelectedRegionGapsWidth);

  hv_NormalGapHeight = 100;
  hv_NormalGapWidth = 1190;
  //槽过宽，过短，退出
  {
  HTuple end_val29 = hv_Number;
  HTuple step_val29 = 1;
  for (hv_n=1; hv_n.Continue(end_val29, step_val29); hv_n += step_val29)
  {
    SelectObj(ho_SelectedRegionGaps, &ho_ObjectSelected, hv_n);
    if (0 != (HTuple(HTuple(hv_SelectedRegionGapsHeight[hv_n-1])>(hv_NormalGapHeight+100)).TupleOr(HTuple(hv_SelectedRegionGapsWidth[hv_n-1])<(hv_NormalGapWidth-30))))
    {
      ConcatObj((*ho_RegionErrorOthers), ho_ObjectSelected, &(*ho_RegionErrorOthers)
          );
      if (HDevWindowStack::IsOpen())
        ClearWindow(HDevWindowStack::GetActive());
      if (HDevWindowStack::IsOpen())
        DispObj(ho_EmphaImage, HDevWindowStack::GetActive());
      if (HDevWindowStack::IsOpen())
        DispObj(ho_ObjectSelected, HDevWindowStack::GetActive());
      AreaCenter(ho_ObjectSelected, &hv_Area3, &hv_Row, &hv_Column);
      disp_message(hv_WindowHandle, "片内异常", "image", hv_Row, hv_Column, "black", 
          "true");
      return;
    }
  }
  }
  //***********

  for (hv_i=1; hv_i<=19; hv_i+=1)
  {
    SelectObj(ho_SelectedRegionGaps, &ho_ObjectSelected, hv_i);
	CHH::Gap_EdgesAndRegressLines(ho_EmphaImage, ho_ObjectSelected, &ho_SelectedEdges, 
        &ho_RegressContour0, &ho_RegressContour1, &ho_GapWidthContours1, &ho_Arrow, 
        &ho_RegionError, &ho_ErrorEdge, &hv_LRCLength, &hv_RowRegressCenter, &hv_ColRegressCenter);
    //********边缘线过短，或者回归线两端的距离相减偏差过大
    CountObj(ho_ErrorEdge, &hv_Number);
    if (0 != (hv_Number>0))
    {
      if (HDevWindowStack::IsOpen())
        ClearWindow(HDevWindowStack::GetActive());
      if (HDevWindowStack::IsOpen())
        DispObj(ho_EmphaImage, HDevWindowStack::GetActive());
      if (HDevWindowStack::IsOpen())
        DispObj(ho_ErrorEdge, HDevWindowStack::GetActive());
      AreaCenter(ho_ObjectSelected, &hv_Area3, &hv_Row, &hv_Column);
      disp_message(hv_WindowHandle, "槽边缘异常", "image", hv_Row, hv_Column, "black", 
          "true");
      return;
    }
    ConcatObj((*ho_Edges), ho_SelectedEdges, &(*ho_Edges));
    ConcatObj((*ho_RegressContours0), ho_RegressContour0, &(*ho_RegressContours0)
        );
    ConcatObj((*ho_RegressContours1), ho_RegressContour1, &(*ho_RegressContours1)
        );
    ConcatObj((*ho_ErrorEdges), ho_ErrorEdge, &(*ho_ErrorEdges));
    AreaCenter(ho_RegionError, &hv_Area2, &hv_Row1, &hv_Column2);
    CountObj(ho_RegionError, &hv_Number1);
    if (0 != (hv_Number1!=0))
    {
      ConcatObj((*ho_RegionErrors), ho_RegionError, &(*ho_RegionErrors));
    }
    ConcatObj((*ho_Arrows), ho_Arrow, &(*ho_Arrows));
    (*hv_GapCenterWidths) = (*hv_GapCenterWidths).TupleConcat(HTuple(hv_LRCLength[2]));
    (*hv_RowRegressCenters) = (*hv_RowRegressCenters).TupleConcat(hv_RowRegressCenter);
    (*hv_ColRegressCenters) = (*hv_ColRegressCenters).TupleConcat(hv_ColRegressCenter);
  }

  return;
}
//检测片高
void CHH::PieceHeight (HObject ho_EmphaImage, HObject *ho_RegionLines, HObject *ho_BadMaxLines, 
					   HObject *ho_BadMinLines, HTuple hv_GBTopCutCol, HTuple hv_PieceBottomCutCol, 
					   HTuple hv_LengthPerRowPiexl, HTuple hv_StandHeight, HTuple hv_Torence, HTuple hv_GBPhiThreshold, 
					   HTuple hv_WindowHandle, HTuple *hv_PieceHeightIsError, HTuple *hv_GBIsError, 
					   HTuple *hv_ErrorMaxNum, HTuple *hv_ErrorMinNum, HTuple *hv_HeightMM, HTuple *hv_RowGBTopCenters, 
					   HTuple *hv_ColGBTopCenters, HTuple *hv_RowPieceBottomCenters, HTuple *hv_ColPieceBottomCenters)
{

	HObject  ho_GBTopRectangleROI, ho_GBRectangleTopImageReduced;
	HObject  ho_ImageClosing2, ho_Region, ho_RegionOpening, ho_ConnectedRegions;
	HObject  ho_RegionTrans1, ho_ImageClosing, ho_Rectangle;
	HObject  ho_ImageReduced, ho_SelectedRegions, ho_RegionTrans;
	HObject ho_RegionDifference,ho_RegionOpening1,ho_ConnectedBadRegions,ho_SelectedBadRegions,ho_RegionDilation,ho_BadGBPianLine;

	// Local control variables
	HTuple  hv_Area, hv_GB_Num, hv_Column, hv_PieceBottom_Num;
	HTuple  hv_Value, hv_Number, hv_Index, hv_SgnMax, hv_IndicesMax;
	HTuple  hv_SgnMin, hv_IndicesMin, hv_Phi, hv_Sgn, hv_Indices;

  disp_message(hv_WindowHandle, (("片高："+hv_StandHeight)+"±")+hv_Torence, "image", 
      20, 10, "black", "true");
  GenRectangle1(&ho_GBTopRectangleROI, 0, hv_GBTopCutCol-50, 12287, hv_GBTopCutCol+50);
  ReduceDomain(ho_EmphaImage, ho_GBTopRectangleROI, &ho_GBRectangleTopImageReduced
      );

  //***************钩顶部threshold***************
  GrayClosingRect(ho_GBRectangleTopImageReduced, &ho_ImageClosing2, 100, 11);

  Threshold(ho_ImageClosing2, &ho_Region, 200, 255);
  OpeningRectangle1(ho_Region, &ho_RegionOpening, 1, 180);
  Connection(ho_RegionOpening, &ho_ConnectedRegions);
  ShapeTrans(ho_ConnectedRegions, &ho_RegionTrans1, "rectangle1");
  AreaCenter(ho_RegionTrans1, &hv_Area, &(*hv_RowGBTopCenters), &(*hv_ColGBTopCenters));
  CountObj(ho_RegionTrans1, &hv_GB_Num);


  //钩顶部少于18，寻找未出现钩顶的部分，程序返回
  (*hv_PieceHeightIsError) = 0;
  *hv_GBIsError = 0;
  if (0 != (hv_GB_Num!=18))
  {
		*hv_GBIsError = 1;
		Difference(ho_Region, ho_RegionTrans1, &ho_RegionDifference);
		OpeningRectangle1(ho_RegionDifference, &ho_RegionOpening1, 2, 1);
		Connection(ho_RegionOpening1, &ho_ConnectedBadRegions);
		SelectShape(ho_ConnectedBadRegions, &ho_SelectedBadRegions, "height", "and", 180, 99999);
		DilationCircle(ho_SelectedBadRegions, &ho_RegionDilation, 100.5);
		SetLineWidth(hv_WindowHandle,2);
		SetDraw(hv_WindowHandle,"margin");
		SetColor(hv_WindowHandle,"red");
		DispObj(ho_RegionDilation,hv_WindowHandle);
    disp_message(hv_WindowHandle, "片顶不等于18,\r\n钩部异常", "image", 20, 1500, "red", "true");
    return;
  }

  //找片底部
  GrayClosingRect(ho_EmphaImage, &ho_ImageClosing, 11, 11);
  GenRectangle1(&ho_Rectangle, 0, hv_PieceBottomCutCol-30, 12287, hv_PieceBottomCutCol+400);
  ReduceDomain(ho_ImageClosing, ho_Rectangle, &ho_ImageReduced);
  Threshold(ho_ImageReduced, &ho_Region, 40, 255);
  Connection(ho_Region, &ho_ConnectedRegions);
  SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("height")), 
      "and", (HTuple(8000).Append(500)), (HTuple(999999).Append(650)));
  ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "rectangle1");
  AreaCenter(ho_RegionTrans, &hv_Area, &(*hv_RowPieceBottomCenters), &hv_Column);
  CountObj(ho_RegionTrans, &hv_PieceBottom_Num);
  if (0 != (hv_PieceBottom_Num!=18))
  {
    (*hv_PieceHeightIsError) = 1;
    disp_message(hv_WindowHandle, "片底不等于18", "image", 20, 1500, "red", "true");
    return;
  }

  RegionFeatures(ho_RegionTrans, "column1", &(*hv_ColPieceBottomCenters));
  GenRegionLine(&(*ho_RegionLines), (*hv_RowGBTopCenters), (*hv_ColGBTopCenters), 
      (*hv_RowPieceBottomCenters), (*hv_ColPieceBottomCenters));
  RegionFeatures((*ho_RegionLines), "width", &hv_Value);
  (*hv_HeightMM) = hv_Value*hv_LengthPerRowPiexl;

   DispObj((*ho_RegionLines),hv_WindowHandle);
  CountObj((*ho_RegionLines), &hv_Number);
  {
  HTuple end_val46 = hv_Number;
  HTuple step_val46 = 1;
  for (hv_Index=1; hv_Index.Continue(end_val46, step_val46); hv_Index += step_val46)
  {
    disp_message(hv_WindowHandle, "片高:"+HTuple((*hv_HeightMM)[hv_Index-1]), "image", 
        HTuple((*hv_RowPieceBottomCenters)[hv_Index-1])+50, HTuple((*hv_ColPieceBottomCenters)[hv_Index-1])-100, 
        "black", "true");
  }
  }

  TupleSgn((hv_StandHeight+hv_Torence)-(*hv_HeightMM), &hv_SgnMax);
  TupleFind(hv_SgnMax, -1, &hv_IndicesMax);
  if (0 != (hv_IndicesMax!=-1))
  {
    (*hv_ErrorMaxNum) = hv_IndicesMax.TupleLength();
    SelectObj((*ho_RegionLines), &(*ho_BadMaxLines), hv_IndicesMax+1);
    (*hv_PieceHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分片过长", "image", 20, 1500, "red", "true");
  }
  else
  {
    (*hv_ErrorMaxNum) = 0;
  }
  TupleSgn(((*hv_HeightMM)-hv_StandHeight)+hv_Torence, &hv_SgnMin);
  TupleFind(hv_SgnMin, -1, &hv_IndicesMin);
  if (0 != (hv_IndicesMin!=-1))
  {
    (*hv_ErrorMinNum) = hv_IndicesMin.TupleLength();
    SelectObj((*ho_RegionLines), &(*ho_BadMinLines), hv_IndicesMin+1);
    (*hv_PieceHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分片过短", "image", 120, 1500, "red", "true");
  }
  else
  {
    (*hv_ErrorMinNum) = 0;
  }

  RegionFeatures((*ho_RegionLines), "phi", &hv_Phi);
  hv_Phi = hv_Phi.TupleAbs();
  TupleSgn(hv_Phi-hv_GBPhiThreshold, &hv_Sgn);
  TupleFind(hv_Sgn, 1, &hv_Indices);
  if (0 != (hv_Indices!=-1))
  {
	  (*hv_GBIsError) = 1;
	  disp_message(hv_WindowHandle, "部分钩偏", "image", 920, 1500, "red", "true");
	  SelectObj(*ho_RegionLines, &ho_BadGBPianLine,hv_Indices);
	  SetLineWidth(hv_WindowHandle,2);
	  SetDraw(hv_WindowHandle,"margin");
	  SetColor(hv_WindowHandle,"red");
	  DispObj(ho_BadGBPianLine,hv_WindowHandle);
  }

  return;
}

void CHH::PieceHeight222 (HObject ho_EmphaImage, HObject *ho_RegionLines, HObject *ho_RegionGBHeightLines, 
    HObject *ho_BadMaxLines, HObject *ho_BadMinLines, HTuple hv_GBTopCutCol, HTuple hv_PieceBottomCutCol, 
    HTuple hv_LengthPerRowPiexl, HTuple hv_StandPieceHeight, HTuple hv_PieceHeightTorence, 
    HTuple hv_StandGBHeight, HTuple hv_GBHeightTorence, HTuple hv_GBPhiThreshold, 
    HTuple hv_WindowHandle, HTuple *hv_GBTopIsNo18Error, HTuple *hv_GBBottomIsNo18Error, 
    HTuple *hv_PieceBottomIsNo18Error, HTuple *hv_PieceHeightIsError, HTuple *hv_GBHeightIsError, 
    HTuple *hv_GBIsOffsetError, HTuple *hv_PieceHeights, HTuple *hv_GBHeights, HTuple *hv_Phi, 
    HTuple *hv_RowGBTopCenters, HTuple *hv_ColGBTopCenters, HTuple *hv_RowPieceBottomCenters, 
    HTuple *hv_ColPieceBottomCenters)
{

  // Local iconic variables
  HObject  ho_GBTopRectangleROI, ho_GBRectangleTopImageReduced;
  HObject  ho_ImageClosing2, ho_Region, ho_RegionOpening, ho_ConnectedRegions;
  HObject  ho_RegionTrans1, ho_RegionDifference, ho_RegionOpening1;
  HObject  ho_ConnectedBadRegions, ho_SelectedBadRegions, ho_RegionDilation;
  HObject  ho_RectangleGBBottom, ho_ImageGBBottom, ho_ImageOpenGBBottom;
  HObject  ho_RegionOpening2, ho_GBBottomRegions, ho_LocalGBBottomRegion;
  HObject  ho_LocalGBBottomRegion1, ho_RegionClosing1, ho_ConnectedRegions2;
  HObject  ho_BadlocaGBBottomRegions, ho_RegionDifference1;
  HObject  ho_RegionUnion, ho_RegionClosing2, ho_ConnectedRegions3;
  HObject  ho_LocalGBBottomRegion2, ho_RegionFillUp1, ho_ConnectedRegions1;
  HObject  ho_BadGBBottomRectangle, ho_ImageClosing, ho_Rectangle;
  HObject  ho_ImageReduced, ho_SelectedRegions, ho_SelectedGoodRegions;
  HObject  ho_RegionTrans, ho_RegionBadPieceBottom, ho_BadGBMaxLines;
  HObject  ho_BadGBMinLines, ho_BadGBPianLine;

  // Local control variables
  HTuple  hv_RowGBBottomCenters, hv_ColGBBottomCenters;
  HTuple  hv_Area, hv_GB_Num, hv_ColGBTopMean, hv_GBBottomCut;
  HTuple  hv_GBMiddleCut, hv_Number, hv_AreaGBBottom, hv_ColumnGBBottom;
  HTuple  hv_Number1, hv_RowGBBottomCenters1, hv_ColGBBottomCenters1;
  HTuple  hv_Number2, hv_RowGBBottomCenters2, hv_Sgn1, hv_Indices1;
  HTuple  hv_BadGBBottomCol, hv_Index, hv_Column, hv_PieceBottom_Num;
  HTuple  hv_Value, hv_SgnMax, hv_IndicesMax, hv_ErrorMaxNum;
  HTuple  hv_SgnMin, hv_IndicesMin, hv_ErrorMinNum, hv_NumberGBBottom;
  HTuple  hv_SgnGBMax, hv_IndicesGBMax, hv_SgnGBMin, hv_IndicesGBMin;
  HTuple  hv_Sgn, hv_Indices;

  //测量片长，钩长，钩顶部倾斜，钩部整体上下偏移

  (*hv_GBTopIsNo18Error) = 0;
  (*hv_GBBottomIsNo18Error) = 0;
  (*hv_PieceBottomIsNo18Error) = 0;

  (*hv_PieceHeightIsError) = 0;
  (*hv_GBHeightIsError) = 0;
  (*hv_GBIsOffsetError) = 0;

  (*hv_RowGBTopCenters) = HTuple();
  (*hv_ColGBTopCenters) = HTuple();
  hv_RowGBBottomCenters = HTuple();
  hv_ColGBBottomCenters = HTuple();
  (*hv_RowPieceBottomCenters) = HTuple();
  (*hv_ColPieceBottomCenters) = HTuple();
  (*hv_Phi) = HTuple();
  disp_message(hv_WindowHandle, (("片长："+hv_StandPieceHeight)+"±")+hv_PieceHeightTorence, 
      "image", 20, 10, "black", "true");
  disp_message(hv_WindowHandle, (("钩长："+hv_StandGBHeight)+"±")+hv_GBHeightTorence, 
      "image", 120, 10, "black", "true");


  //***************钩顶部threshold***************
  GenRectangle1(&ho_GBTopRectangleROI, 0, hv_GBTopCutCol-70, 12287, hv_GBTopCutCol+80);
  ReduceDomain(ho_EmphaImage, ho_GBTopRectangleROI, &ho_GBRectangleTopImageReduced
      );
  GrayClosingRect(ho_GBRectangleTopImageReduced, &ho_ImageClosing2, 100, 11);

  Threshold(ho_ImageClosing2, &ho_Region, 200, 255);
  OpeningRectangle1(ho_Region, &ho_RegionOpening, 1, 180);
  Connection(ho_RegionOpening, &ho_ConnectedRegions);
  ShapeTrans(ho_ConnectedRegions, &ho_RegionTrans1, "rectangle1");
  SelectShape(ho_RegionTrans1, &ho_RegionTrans1, "height", "and", 190, 99999);
  AreaCenter(ho_RegionTrans1, &hv_Area, &(*hv_RowGBTopCenters), &(*hv_ColGBTopCenters));
  CountObj(ho_RegionTrans1, &hv_GB_Num);


  //钩顶部少于18，寻找未出现钩顶的部分，程序返回
  if (0 != (hv_GB_Num!=18))
  {
    if (0 != (hv_GB_Num==0))
    {
		(*hv_GBTopIsNo18Error) = 1;
      return;
    }
    (*hv_GBTopIsNo18Error) = 1;
    //钩歪过度
    Difference(ho_Region, ho_RegionTrans1, &ho_RegionDifference);
    OpeningRectangle1(ho_RegionDifference, &ho_RegionOpening1, 2, 1);
    Connection(ho_RegionOpening1, &ho_ConnectedBadRegions);
    SelectShape(ho_ConnectedBadRegions, &ho_SelectedBadRegions, "height", "and", 180, 99999);
	DilationCircle(ho_SelectedBadRegions, &ho_RegionDilation, 100.5);
      SetLineWidth(hv_WindowHandle,2);
      SetColor(hv_WindowHandle,"red");
      SetDraw(hv_WindowHandle,"margin");
      DispObj(ho_RegionDilation, hv_WindowHandle);
    disp_message(hv_WindowHandle, "钩顶不等于18,钩部异常", "image", 20, 1500, "black", 
        "true");

    return;
  }

  //找钩底部
  TupleMean((*hv_ColGBTopCenters), &hv_ColGBTopMean);
  hv_GBBottomCut = hv_ColGBTopMean-450;
  hv_GBMiddleCut = hv_ColGBTopMean-90;
  GenRectangle1(&ho_RectangleGBBottom, 0, hv_GBBottomCut, 12288, hv_GBMiddleCut);
  ReduceDomain(ho_EmphaImage, ho_RectangleGBBottom, &ho_ImageGBBottom);
  GrayOpeningRect(ho_ImageGBBottom, &ho_ImageOpenGBBottom, 7, 7);


  LocalThreshold(ho_ImageOpenGBBottom, &ho_LocalGBBottomRegion, "adapted_std_deviation", 
      "dark", "mask_size", 443);
//######方法1
  OpeningRectangle1(ho_LocalGBBottomRegion, &ho_LocalGBBottomRegion1, 20, 1);
  ClosingRectangle1(ho_LocalGBBottomRegion1, &ho_RegionClosing1, 20, 1);
  OpeningCircle(ho_RegionClosing1, &ho_RegionClosing1, 3.5);
  Connection(ho_RegionClosing1, &ho_ConnectedRegions2);
  SelectShape(ho_ConnectedRegions2, &ho_BadlocaGBBottomRegions, "column1", "and", 
      hv_GBBottomCut-20, hv_GBBottomCut+40);
  Difference(ho_ConnectedRegions2, ho_BadlocaGBBottomRegions, &ho_RegionDifference1
      );
  Union1(ho_RegionDifference1, &ho_RegionUnion);
  ClosingRectangle1(ho_RegionUnion, &ho_RegionClosing2, 1, 50);
  OpeningRectangle1(ho_RegionClosing2, &ho_RegionOpening2, 30, 1);
  Connection(ho_RegionOpening2, &ho_ConnectedRegions3);
  SelectShape(ho_ConnectedRegions3, &ho_LocalGBBottomRegion1, ((HTuple("area").Append("column1")).Append("height")), 
      "and", (HTuple(10000).TupleConcat(hv_GBBottomCut+20)).TupleConcat(200), ((HTuple(99999).Append(2048)).Append(500)));
  ClosingCircle(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1, 25.5);
  FillUp(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1);
  OpeningRectangle1(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1, 1, 40);
  OpeningRectangle1(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1, 100, 1);
  Connection(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1);
  SelectShape(ho_LocalGBBottomRegion1, &ho_LocalGBBottomRegion1, "height", "and", 
      100, 99999);
  CountObj(ho_LocalGBBottomRegion1, &hv_Number1);
  SortRegion(ho_LocalGBBottomRegion1, &ho_GBBottomRegions, "first_point", "true", 
      "row");
  AreaCenter(ho_GBBottomRegions, &hv_AreaGBBottom, &hv_RowGBBottomCenters1, &hv_ColumnGBBottom);
  RegionFeatures(ho_GBBottomRegions, "column1", &hv_ColGBBottomCenters1);


//########方法2  测试钩长较准
  OpeningRectangle1(ho_LocalGBBottomRegion, &ho_LocalGBBottomRegion2, 20, 1);
  ClosingRectangle1(ho_LocalGBBottomRegion2, &ho_RegionClosing2, 1, 15);
  FillUp(ho_RegionClosing2, &ho_RegionFillUp1);
  Connection(ho_RegionFillUp1, &ho_ConnectedRegions1);

  SelectShape(ho_ConnectedRegions1, &ho_LocalGBBottomRegion2, ((HTuple("area").Append("column1")).Append("height")), 
      "and", (HTuple(10000).TupleConcat(hv_GBBottomCut+20)).TupleConcat(200), ((HTuple(99999).Append(2048)).Append(500)));
  ClosingCircle(ho_LocalGBBottomRegion2, &ho_LocalGBBottomRegion2, 25.5);
  FillUp(ho_LocalGBBottomRegion2, &ho_LocalGBBottomRegion2);
  CountObj(ho_LocalGBBottomRegion2, &hv_Number2);
  SortRegion(ho_LocalGBBottomRegion2, &ho_GBBottomRegions, "first_point", "true", 
      "row");
  AreaCenter(ho_GBBottomRegions, &hv_AreaGBBottom, &hv_RowGBBottomCenters, &hv_ColumnGBBottom);
  RegionFeatures(ho_GBBottomRegions, "column1", &hv_ColGBBottomCenters);

  if (0 != (HTuple(hv_Number1!=18).TupleAnd(hv_Number2!=18)))
  {
	  hv_Number = hv_Number1;
	  hv_ColGBBottomCenters = hv_ColGBBottomCenters1;
	  hv_RowGBBottomCenters = hv_RowGBBottomCenters1;
  }

  if (0 != (HTuple(hv_Number1!=18).TupleAnd(hv_Number2==18)))
  {
    hv_Number = hv_Number2;
  }

  if (0 != (HTuple(hv_Number1==18).TupleAnd(hv_Number2!=18)))
  {
    hv_Number = hv_Number1;
    hv_ColGBBottomCenters = hv_ColGBBottomCenters1;
    hv_RowGBBottomCenters = hv_RowGBBottomCenters1;
  }

  if (0 != (HTuple(hv_Number1==18).TupleAnd(hv_Number2==18)))
  {
    hv_Number = hv_Number2;
  }


  if (0 != (hv_Number!=18))
  {
    disp_message(hv_WindowHandle, "钩底不等于18,钩部异常", "image", 20, 1500, "black", 
        "true");
    (*hv_GBBottomIsNo18Error) = 1;
    if (0 != (hv_Number<18))
    {

		int num=hv_RowGBBottomCenters.TupleLength();
		if(num==0)
			return;

      TupleRemove(hv_RowGBBottomCenters, (hv_RowGBBottomCenters.TupleLength())-1, 
          &hv_RowGBBottomCenters1);
      TupleRemove(hv_RowGBBottomCenters, 0, &hv_RowGBBottomCenters2);
      TupleSgn((hv_RowGBBottomCenters2-hv_RowGBBottomCenters1)-1000, &hv_Sgn1);
      TupleFind(hv_Sgn1, 1, &hv_Indices1);
      hv_BadGBBottomCol = hv_ColGBBottomCenters.TupleMean();
      {
      HTuple end_val147 = (hv_Indices1.TupleLength())-1;
      HTuple step_val147 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val147, step_val147); hv_Index += step_val147)
      {
        GenRectangle2(&ho_BadGBBottomRectangle, HTuple(hv_RowGBBottomCenters[HTuple(hv_Indices1[hv_Index])])+700, 
            hv_BadGBBottomCol+200, 0, 250, 200);
    
          SetColor(hv_WindowHandle,"red");
          SetDraw(hv_WindowHandle,"margin");
          DispObj(ho_BadGBBottomRectangle, hv_WindowHandle);
      }
      }
      return;
    }

    if (0 != (hv_Number>18))
    {
        SetColor(hv_WindowHandle,"red");
        SetDraw(hv_WindowHandle,"margin");
        SelectShape(ho_GBBottomRegions, &ho_SelectedBadRegions, "width", "and", 0, 250);
        DispObj(ho_SelectedBadRegions,hv_WindowHandle);
      return;
    }
  }


  //找片底部
  GrayClosingRect(ho_EmphaImage, &ho_ImageClosing, 11, 11);
  GenRectangle1(&ho_Rectangle, 0, hv_PieceBottomCutCol-30, 12287, hv_PieceBottomCutCol+400);
  ReduceDomain(ho_ImageClosing, ho_Rectangle, &ho_ImageReduced);
  Threshold(ho_ImageReduced, &ho_Region, 40, 255);
  Connection(ho_Region, &ho_ConnectedRegions);
  SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 8000, 999999);
  SelectShape(ho_ConnectedRegions, &ho_SelectedGoodRegions, (HTuple("area").Append("height")), 
      "and", (HTuple(8000).Append(500)), (HTuple(999999).Append(650)));
  ShapeTrans(ho_SelectedGoodRegions, &ho_RegionTrans, "rectangle1");
  AreaCenter(ho_RegionTrans, &hv_Area, &(*hv_RowPieceBottomCenters), &hv_Column);
  CountObj(ho_RegionTrans, &hv_PieceBottom_Num);
  if (0 != (hv_PieceBottom_Num!=18))
  {
    (*hv_PieceBottomIsNo18Error) = 1;
    disp_message(hv_WindowHandle, "片底不等于18,部分片底异常", "image", 20, 1500, 
        "red", "true");
    Difference(ho_SelectedRegions, ho_SelectedGoodRegions, &ho_RegionBadPieceBottom
        );
  
      SetColor(hv_WindowHandle,"red");

      SetDraw(hv_WindowHandle,"margin");

      DispObj(ho_RegionBadPieceBottom, hv_WindowHandle);
    return;
  }

  RegionFeatures(ho_RegionTrans, "column1", &(*hv_ColPieceBottomCenters));
  GenRegionLine(&(*ho_RegionLines), (*hv_RowGBTopCenters), (*hv_ColGBTopCenters), 
      (*hv_RowPieceBottomCenters), (*hv_ColPieceBottomCenters));
  RegionFeatures((*ho_RegionLines), "width", &hv_Value);
  //片长
  (*hv_PieceHeights) = hv_Value*hv_LengthPerRowPiexl;

    SetColor(hv_WindowHandle,"green");
    DispObj((*ho_RegionLines), hv_WindowHandle);
  CountObj((*ho_RegionLines), &hv_Number);
  {
  HTuple end_val196 = hv_Number;
  HTuple step_val196 = 1;
  for (hv_Index=1; hv_Index.Continue(end_val196, step_val196); hv_Index += step_val196)
  {
    disp_message(hv_WindowHandle, "片高:"+HTuple((*hv_PieceHeights)[hv_Index-1]), 
        "image", HTuple((*hv_RowPieceBottomCenters)[hv_Index-1])+50, HTuple((*hv_ColPieceBottomCenters)[hv_Index-1])-100, 
        "black", "true");
  }
  }

  //检测片高的长度
  TupleSgn((hv_StandPieceHeight+hv_PieceHeightTorence)-(*hv_PieceHeights), &hv_SgnMax);
  TupleFind(hv_SgnMax, -1, &hv_IndicesMax);
  if (0 != (hv_IndicesMax!=-1))
  {
    hv_ErrorMaxNum = hv_IndicesMax.TupleLength();
    SelectObj((*ho_RegionLines), &(*ho_BadMaxLines), hv_IndicesMax+1);
    (*hv_PieceHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分片过长", "image", 20, 1500, "red", "true");
      SetColor(hv_WindowHandle,"red");
      DispObj((*ho_BadMaxLines), hv_WindowHandle);
  }
  else
  {
    hv_ErrorMaxNum = 0;
  }
  TupleSgn(((*hv_PieceHeights)-hv_StandPieceHeight)+hv_PieceHeightTorence, &hv_SgnMin);
  TupleFind(hv_SgnMin, -1, &hv_IndicesMin);
  if (0 != (hv_IndicesMin!=-1))
  {
    hv_ErrorMinNum = hv_IndicesMin.TupleLength();
    SelectObj((*ho_RegionLines), &(*ho_BadMinLines), hv_IndicesMin+1);
    (*hv_PieceHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分片过短", "image", 120, 1500, "red", "true");
      SetColor(hv_WindowHandle,"red");
      DispObj((*ho_BadMinLines), hv_WindowHandle);
  }
  else
  {
    hv_ErrorMinNum = 0;
  }

  //钩长
  (*hv_GBHeights) = (hv_ColGBBottomCenters-(*hv_ColPieceBottomCenters))*hv_LengthPerRowPiexl;
  GenRegionLine(&(*ho_RegionGBHeightLines), hv_RowGBBottomCenters, hv_ColGBBottomCenters, 
      (*hv_RowPieceBottomCenters), (*hv_ColPieceBottomCenters));

    SetColor(hv_WindowHandle,"green");
    DispObj((*ho_RegionGBHeightLines), hv_WindowHandle);
  CountObj((*ho_RegionGBHeightLines), &hv_NumberGBBottom);
  {
  HTuple end_val232 = hv_NumberGBBottom;
  HTuple step_val232 = 1;
  for (hv_Index=1; hv_Index.Continue(end_val232, step_val232); hv_Index += step_val232)
  {
    disp_message(hv_WindowHandle, "钩长:"+HTuple((*hv_GBHeights)[hv_Index-1]), "image", 
        HTuple(hv_RowGBBottomCenters[hv_Index-1])-50, HTuple(hv_ColGBBottomCenters[hv_Index-1]), 
        "black", "true");
  }
  }


  TupleSgn((hv_StandGBHeight+hv_GBHeightTorence)-(*hv_GBHeights), &hv_SgnGBMax);
  TupleFind(hv_SgnGBMax, -1, &hv_IndicesGBMax);
  if (0 != (hv_IndicesGBMax!=-1))
  {
    SelectObj((*ho_RegionGBHeightLines), &ho_BadGBMaxLines, hv_IndicesGBMax+1);
    (*hv_GBHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分钩过长", "image", 20, 1500, "red", "true");

      SetColor(hv_WindowHandle,"red");
      DispObj(ho_BadGBMaxLines, hv_WindowHandle);
  }
  TupleSgn(((*hv_GBHeights)-hv_StandGBHeight)+hv_GBHeightTorence, &hv_SgnGBMin);
  TupleFind(hv_SgnGBMin, -1, &hv_IndicesGBMin);
  if (0 != (hv_IndicesGBMin!=-1))
  {
    SelectObj((*ho_RegionGBHeightLines), &ho_BadGBMinLines, hv_IndicesGBMin+1);
    (*hv_GBHeightIsError) = 1;
    disp_message(hv_WindowHandle, "部分钩过短", "image", 120, 1500, "red", "true");

      SetColor(hv_WindowHandle,"red");
      DispObj(ho_BadGBMinLines, hv_WindowHandle);
  }

  //钩部整体偏移，钩顶与片底连线phi会增大，建议设置0.03
  RegionFeatures((*ho_RegionLines), "phi", &(*hv_Phi));
  (*hv_Phi) = (*hv_Phi).TupleAbs();
  TupleSgn((*hv_Phi)-hv_GBPhiThreshold, &hv_Sgn);
  TupleFind(hv_Sgn, 1, &hv_Indices);
  if (0 != (hv_Indices!=-1))
  {
    (*hv_GBIsOffsetError) = 1;
    disp_message(hv_WindowHandle, "部分钩整体偏移", "image", 920, 1500, "red", "true");
    SelectObj((*ho_RegionLines), &ho_BadGBPianLine, hv_Indices+1);

      SetColor(hv_WindowHandle,"red");
      DispObj(ho_BadGBPianLine, hv_WindowHandle);
  }

  return;
}




//检测面阵图中钩部的缺陷，需配合FirstPhi使用
void CHH::DM_Detect_GB (HObject ho_GrayImage, 
						HObject *ho_XYTransRegionError, 
						HTuple hv_RowCenter,  HTuple hv_ColumnCenter, HTuple hv_Mearsure_Radius, HTuple hv_RadiusStart, HTuple hv_RadiusEnd, HTuple Dist_Deviation,
    HTuple *hv_ErrorNum)
{

  // Local iconic variables
  HObject  ho_ImageEmphasize, ho_MearsureCircle;
  HObject  ho_StartCirlces, ho_CenterCircles, ho_EndCircles;
  HObject  ho_PolarTransImage, ho_Regions, ho_RegionClosing;
  HObject  ho_RegionFillUp, ho_ConnectedRegions, ho_SelectedRegions;
  HObject  ho_RegionConvex, ho_RegionTrans1, ho_RegionsError;

  // Local control variables
  HTuple  hv_Width, hv_Height, hv_AngleStart, hv_AngleEnd;
  HTuple  hv_Dgree_Center, hv_Dgree_Start, hv_Dgree_End, hv_WidthPolar;
  HTuple  hv_HeightPolar;

  Emphasize(ho_GrayImage, &ho_ImageEmphasize, 301, 301, 1);
  GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
  //Mearsure_Radius := 1030
  FirstPhi(ho_ImageEmphasize, &ho_MearsureCircle, &ho_StartCirlces, &ho_CenterCircles, 
      &ho_EndCircles, hv_RowCenter, hv_ColumnCenter, hv_Mearsure_Radius, &hv_AngleStart, 
      &hv_AngleEnd, &hv_Dgree_Center, &hv_Dgree_Start, &hv_Dgree_End);

  //RadiusStart := 1100
  //RadiusEnd := 920
  hv_WidthPolar = (2*(HTuple(180).TupleRad()))*hv_RadiusStart;
  hv_HeightPolar = hv_RadiusStart-hv_RadiusEnd;

  PolarTransImageExt(ho_ImageEmphasize, &ho_PolarTransImage, hv_RowCenter, hv_ColumnCenter, 
      hv_AngleStart, hv_AngleEnd, hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar, 
      "nearest_neighbor");

  //************极坐标图找到有粘料区域，并反极坐标转换**************
  Threshold(ho_PolarTransImage, &ho_Regions, 0, 134);
  ClosingCircle(ho_Regions, &ho_RegionClosing, 3.5);
  FillUp(ho_Regions, &ho_RegionFillUp);
  Connection(ho_RegionFillUp, &ho_ConnectedRegions);
  SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 1500, 99999);
  ShapeTrans(ho_SelectedRegions, &ho_RegionConvex, "convex");
  ShapeTrans(ho_RegionConvex, &ho_RegionTrans1, "inner_rectangle1");
  SelectShape(ho_RegionConvex, &ho_RegionsError, "dist_deviation", "and", Dist_Deviation, 100);
  PolarTransRegionInv(ho_RegionsError, &(*ho_XYTransRegionError), hv_RowCenter, hv_ColumnCenter, 
      hv_AngleStart, hv_AngleEnd, hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar, 
      hv_Width, hv_Height, "nearest_neighbor");
  //dev_set_line_width (3)
  //dev_display (XYTransRegionError)
  CountObj((*ho_XYTransRegionError), &(*hv_ErrorNum));
  return;

  //****************方法2:动态域值**************
  //mean_image (PolarTransImage, ImageMean, 3000, 1)
  //dyn_threshold (PolarTransImage, ImageMean, RegionDynThresh, 80, 'dark')
  //fill_up (RegionDynThresh, RegionFillUp1)
  //connection (RegionFillUp1, ConnectedRegions1)
  //shape_trans (ConnectedRegions1, RegionTrans, 'convex')
  //select_shape (RegionTrans, SelectedRegions1, 'area', 'and', 1500, 99999)
  //select_shape (SelectedRegions1, SelectedRegions2, 'compactness', 'and', 0, 1.26)
  //select_shape (SelectedRegions1, SelectedRegions, 'anisometry', 'and', 1.30, 2)
  //polar_trans_region_inv (SelectedRegions2, XYTransRegionError, RowCenter, ColumnCenter, AngleStart, AngleEnd, RadiusStart, RadiusEnd, WidthPolar, HeightPolar, Width, Height, 'nearest_neighbor')
  //polar_trans_region_inv (SelectedRegions, XYTransRegionError2, RowCenter, ColumnCenter, AngleStart, AngleEnd, RadiusStart, RadiusEnd, WidthPolar, HeightPolar, Width, Height, 'nearest_neighbor')
  //count_obj (XYTransRegionError, ErrorNum)
  //count_obj (XYTransRegionError2, ErrorNum2)
  //return ()
}

//面阵图片第1钩的解译，展开图需使用其相关参数
void CHH::FirstPhi (HObject ho_GrayImage, HObject *ho_MearsureCircle, HObject *ho_StartCircles, 
    HObject *ho_CenterCircles, HObject *ho_EndCircles, HTuple hv_ArcRowCenter_Measure, 
    HTuple hv_ArcColumnCenter_Measure, HTuple hv_ArcRadius_Measure, HTuple *hv_AngleStart, 
    HTuple *hv_AngleEnd, HTuple *hv_Dgree_Center, HTuple *hv_Dgree_Start, HTuple *hv_Dgree_End)
{

  // Local iconic variables
  HObject  ho_StartLine, ho_EndLine, ho_CenterLine;
  HObject  ho_ContourCenter, ho_ContourStart, ho_ContourEnd;

  // Local control variables
  HTuple  hv_Width, hv_Height, hv_Sigma, hv_AmplitudeThreshold;
  HTuple  hv_RoiWidthLen2, hv_ArcAngleStart_Measure, hv_ArcAngleExtent_Measure;
  HTuple  hv_FunctionSize, hv_MsrHandle_Measure, hv_FuzzyThreshold;
  HTuple  hv_Row1_Measure, hv_Column1_Measure, hv_Amplitude1_Measure;
  HTuple  hv_Row2_Measure, hv_Column2_Measure, hv_Amplitude2_Measure;
  HTuple  hv_TmpCtrl_RowCenter, hv_TmpCtrl_ColumnCenter, hv_Score_Measure;
  HTuple  hv_Width_Measure, hv_Distance_Measure, hv_Num1;
  HTuple  hv_Num2, hv_Num3, hv_AngleRad, hv_Index1, hv_Angle;
  HTuple  hv_FirstCenterphi, hv_StartDgress;


  GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
  hv_Sigma = 1;
  hv_AmplitudeThreshold = 30;
  hv_RoiWidthLen2 = 10;
  hv_ArcAngleStart_Measure = HTuple(0).TupleRad();
  hv_ArcAngleExtent_Measure = HTuple(360).TupleRad();
  CreateFunct1dPairs((((HTuple(90).Append(100)).Append(160)).Append(170)), (((HTuple(0).Append(1)).Append(1)).Append(0)), 
      &hv_FunctionSize);
  GenCircle(&(*ho_MearsureCircle), hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, 
      hv_ArcRadius_Measure);
  GenMeasureArc(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, hv_ArcRadius_Measure, 
      hv_ArcAngleStart_Measure, hv_ArcAngleExtent_Measure, HTuple(1).TupleMax2(hv_RoiWidthLen2.TupleMin2(hv_ArcRadius_Measure)), 
      hv_Width, hv_Height, "nearest_neighbor", &hv_MsrHandle_Measure);
  hv_FuzzyThreshold = 0.5;
  CreateFunct1dPairs((((HTuple(90).Append(100)).Append(160)).Append(170)), (((HTuple(0).Append(1)).Append(1)).Append(0)), 
      &hv_FunctionSize);
  SetFuzzyMeasure(hv_MsrHandle_Measure, "size", hv_FunctionSize);
  FuzzyMeasurePairs(ho_GrayImage, hv_MsrHandle_Measure, hv_Sigma, hv_AmplitudeThreshold, 
      0.5, "negative", &hv_Row1_Measure, &hv_Column1_Measure, &hv_Amplitude1_Measure, 
      &hv_Row2_Measure, &hv_Column2_Measure, &hv_Amplitude2_Measure, &hv_TmpCtrl_RowCenter, 
      &hv_TmpCtrl_ColumnCenter, &hv_Score_Measure, &hv_Width_Measure, &hv_Distance_Measure);
  CloseMeasure(hv_MsrHandle_Measure);

//   hv_Num1 = hv_Row1_Measure.TupleLength();
//   CHH_CreatCircleAndLine(&(*ho_StartCircles), &ho_StartLine, hv_Num1, hv_Row1_Measure, 
//       hv_Column1_Measure, 6);
//   hv_Num2 = hv_Row2_Measure.TupleLength();
//   CHH_CreatCircleAndLine(&(*ho_EndCircles), &ho_EndLine, hv_Num1, hv_Row2_Measure, 
//       hv_Column2_Measure, 6);
//   hv_Num3 = hv_TmpCtrl_RowCenter.TupleLength();
//   CHH_CreatCircleAndLine(&(*ho_CenterCircles), &ho_CenterLine, hv_Num3, hv_TmpCtrl_RowCenter, 
//       hv_TmpCtrl_ColumnCenter, 6);

  if (0 != ((hv_TmpCtrl_RowCenter.TupleLength())!=0))
  {
    //***计算角度*********
    hv_AngleRad = HTuple();
    {
    HTuple end_val26 = (hv_TmpCtrl_RowCenter.TupleLength())-1;
    HTuple step_val26 = 1;
    for (hv_Index1=1; hv_Index1.Continue(end_val26, step_val26); hv_Index1 += step_val26)
    {
      AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_TmpCtrl_RowCenter[hv_Index1-1]), 
          HTuple(hv_TmpCtrl_ColumnCenter[hv_Index1-1]), hv_ArcRowCenter_Measure, 
          hv_ArcColumnCenter_Measure, HTuple(hv_TmpCtrl_RowCenter[hv_Index1]), HTuple(hv_TmpCtrl_ColumnCenter[hv_Index1]), 
          &hv_Angle);
      GenContourPolygonXld(&ho_ContourCenter, hv_ArcRowCenter_Measure.TupleConcat(HTuple(hv_TmpCtrl_RowCenter[hv_Index1-1])), 
          hv_ArcColumnCenter_Measure.TupleConcat(HTuple(hv_TmpCtrl_ColumnCenter[hv_Index1-1])));
      hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    }
    }
    AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_TmpCtrl_RowCenter[(hv_TmpCtrl_RowCenter.TupleLength())-1]), 
        HTuple(hv_TmpCtrl_ColumnCenter[(hv_TmpCtrl_RowCenter.TupleLength())-1]), 
        hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_TmpCtrl_RowCenter[0]), 
        HTuple(hv_TmpCtrl_ColumnCenter[0]), &hv_Angle);
    hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    (*hv_Dgree_Center) = hv_AngleRad.TupleDeg();
    //************极坐标转换参数**************
    AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, hv_ArcRowCenter_Measure, 
        hv_ArcColumnCenter_Measure+1000, hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, 
        HTuple(hv_TmpCtrl_RowCenter[0]), HTuple(hv_TmpCtrl_ColumnCenter[0]), &hv_FirstCenterphi);
    hv_StartDgress = (hv_FirstCenterphi.TupleDeg())-10;
    (*hv_AngleStart) = hv_StartDgress.TupleRad();
    (*hv_AngleEnd) = 6.28319+(hv_StartDgress.TupleRad());
  }

  if (0 != ((hv_Row1_Measure.TupleLength())!=0))
  {
    //***计算角度*********
    hv_AngleRad = HTuple();
    {
    HTuple end_val44 = (hv_Row1_Measure.TupleLength())-1;
    HTuple step_val44 = 1;
    for (hv_Index1=1; hv_Index1.Continue(end_val44, step_val44); hv_Index1 += step_val44)
    {
      AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_Row1_Measure[hv_Index1-1]), 
          HTuple(hv_Column1_Measure[hv_Index1-1]), hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, 
          HTuple(hv_Row1_Measure[hv_Index1]), HTuple(hv_Column1_Measure[hv_Index1]), 
          &hv_Angle);
      GenContourPolygonXld(&ho_ContourStart, hv_ArcRowCenter_Measure.TupleConcat(HTuple(hv_Row1_Measure[hv_Index1-1])), 
          hv_ArcColumnCenter_Measure.TupleConcat(HTuple(hv_Column1_Measure[hv_Index1-1])));
      hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    }
    }
    AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_Row1_Measure[(hv_Row1_Measure.TupleLength())-1]), 
        HTuple(hv_Column1_Measure[(hv_Column1_Measure.TupleLength())-1]), hv_ArcRowCenter_Measure, 
        hv_ArcColumnCenter_Measure, HTuple(hv_Row1_Measure[0]), HTuple(hv_Column1_Measure[0]), 
        &hv_Angle);
    hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    (*hv_Dgree_Start) = hv_AngleRad.TupleDeg();
  }

  if (0 != ((hv_Row2_Measure.TupleLength())!=0))
  {
    //***计算角度*********
    hv_AngleRad = HTuple();
    {
    HTuple end_val57 = (hv_Row1_Measure.TupleLength())-1;
    HTuple step_val57 = 1;
    for (hv_Index1=1; hv_Index1.Continue(end_val57, step_val57); hv_Index1 += step_val57)
    {
      AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_Row2_Measure[hv_Index1-1]), 
          HTuple(hv_Column2_Measure[hv_Index1-1]), hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, 
          HTuple(hv_Row2_Measure[hv_Index1]), HTuple(hv_Column2_Measure[hv_Index1]), 
          &hv_Angle);
      GenContourPolygonXld(&ho_ContourEnd, hv_ArcRowCenter_Measure.TupleConcat(HTuple(hv_Row2_Measure[hv_Index1-1])), 
          hv_ArcColumnCenter_Measure.TupleConcat(HTuple(hv_Column2_Measure[hv_Index1-1])));
      hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    }
    }
    AngleLl(hv_ArcRowCenter_Measure, hv_ArcColumnCenter_Measure, HTuple(hv_Row2_Measure[(hv_Row1_Measure.TupleLength())-1]), 
        HTuple(hv_Column2_Measure[(hv_Column1_Measure.TupleLength())-1]), hv_ArcRowCenter_Measure, 
        hv_ArcColumnCenter_Measure, HTuple(hv_Row2_Measure[0]), HTuple(hv_Column2_Measure[0]), 
        &hv_Angle);
    hv_AngleRad = hv_AngleRad.TupleConcat(hv_Angle);
    (*hv_Dgree_End) = hv_AngleRad.TupleDeg();
  }

  return;
}

//槽宽和槽区域不良区域/槽分度
void CHH::NewGapWidthAndBadGap (HObject ho_EmphaImage, HObject *ho_GapUnFilterRegions, 
								HObject *ho_GapFilterRegions, HObject *ho_GapBadRegions, HObject *ho_MaxRegions, 
								HObject *ho_MixRegions, HObject *ho_Arrows, HTuple hv_GapBottomCutColumn, HTuple hv_GapTopCutColumn, 
								HTuple hv_threshold_min, HTuple hv_threshold_max, HTuple hv_GapBadHeight, HTuple hv_GapBadWidth, 
								HTuple hv_LengthPerColPiexl, HTuple hv_LengthPerRowPiexl, HTuple hv_StandGapWidth, 
								HTuple hv_GapWidthTorence, HTuple hv_StandGapAngle, HTuple hv_GapAngleTorence, 
								HTuple hv_WindowHandle, HTuple *hv_GapRegionNoEqual19, HTuple *hv_GapIsConvex_Que, 
								HTuple *hv_GapWidthIsError, HTuple *hv_GapAngleIsError, HTuple *hv_GapLenths, 
								HTuple *hv_GapAngles, HTuple *hv_RowGapCenters, HTuple *hv_ColGapCenters)
{

	// Local iconic variables
	HObject  ho_ROI, ho_ImageReduced, ho_ConnectedRegions;
	HObject  ho_ObjectSelected, ho_RegionClosing2, ho_RegionFillUp2;
	HObject  ho_RegionClosing3, ho_RegionDifference, ho_ConnectedRegions5;
	HObject  ho_ConvexRegion, ho_RegionUnion1, ho_RegionOpening3;
	HObject  ho_RegionDifference1, ho_ConnectedRegions8, ho_QueRegions;
	HObject  ho_EmptyRegion, ho_ArrowUp, ho_ArrowDown, ho_RegionUnion;
	HObject  ho_MaxRegion, ho_MixRegion;

	// Local control variables
	HTuple  hv_Number, hv_BadHeight, hv_BadWidth;
	HTuple  hv_i, hv_num, hv_GapRowCenterArea, hv_GapRowCenter;
	HTuple  hv_GapColumnCenter, hv_Thickness, hv_Histogramm;
	HTuple  hv_GapLength, hv_Length, hv_MaxSgn, hv_MinSgn, hv_MaxIndices;
	HTuple  hv_MixIndices, hv_k, hv_Area, hv_Rows0, hv_Rows18;
	HTuple  hv_Rows1, hv_Rows2;

	disp_message(hv_WindowHandle, (("槽宽度:"+hv_StandGapWidth)+"±")+hv_GapWidthTorence, 
		"image", 120, 10, "blue", "true");
	disp_message(hv_WindowHandle, (("槽分度:"+hv_StandGapAngle)+"±")+hv_GapAngleTorence, 
		"image", 220, 10, "blue", "true");

	(*hv_GapRegionNoEqual19) = 0;
	(*hv_GapIsConvex_Que) = 0;
	(*hv_GapWidthIsError) = 0;
	(*hv_GapAngleIsError) = 0;

	GenRectangle1(&ho_ROI, 0, hv_GapBottomCutColumn, 12288, hv_GapTopCutColumn);
	ReduceDomain(ho_EmphaImage, ho_ROI, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_ROI, hv_threshold_min, hv_threshold_max);
	Connection(ho_ROI, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &(*ho_GapUnFilterRegions), "width", "and", (hv_GapTopCutColumn-hv_GapBottomCutColumn)-200, 
		99999);
	CountObj((*ho_GapUnFilterRegions), &hv_Number);


		SetDraw(hv_WindowHandle,"margin");
		SetLineWidth(hv_WindowHandle,2);
		SetColor(hv_WindowHandle,"red");

	if (0 != (hv_Number!=19))
	{
		disp_message(hv_WindowHandle, "槽部选区不等于19，错误", "image", 220, 1500, "black", 
			"true");
			DispObj((*ho_GapUnFilterRegions), hv_WindowHandle);
		(*hv_GapRegionNoEqual19) = 1;
		return;
	}

	if (0 != (hv_Number==19))
	{

		(*hv_GapLenths) = HTuple();
		GenEmptyObj(&(*ho_GapBadRegions));
		GenEmptyObj(&(*ho_GapFilterRegions));
		GenEmptyObj(&(*ho_MaxRegions));
		GenEmptyObj(&(*ho_MixRegions));
		GenEmptyObj(&(*ho_Arrows));
		hv_BadHeight = hv_GapBadHeight/hv_LengthPerColPiexl;
		hv_BadWidth = hv_GapBadWidth/hv_LengthPerRowPiexl;

	
		{
			HTuple end_val34 = hv_Number;
			HTuple step_val34 = 1;
			for (hv_i=1; hv_i.Continue(end_val34, step_val34); hv_i += step_val34)
			{
				//i := 4
				SelectObj((*ho_GapUnFilterRegions), &ho_ObjectSelected, hv_i);

				ClosingCircle(ho_ObjectSelected, &ho_RegionClosing2, 3.5);
				FillUp(ho_RegionClosing2, &ho_RegionFillUp2);

				ClosingRectangle1(ho_RegionFillUp2, &ho_RegionClosing3, 100, 1);
				Difference(ho_RegionClosing3, ho_RegionFillUp2, &ho_RegionDifference);
				Connection(ho_RegionDifference, &ho_ConnectedRegions5);
				SelectShape(ho_ConnectedRegions5, &ho_ConvexRegion, (HTuple("height").Append("width")), 
					"and", hv_BadHeight.TupleConcat(hv_BadWidth), (HTuple(99999).Append(99999)));
				CountObj(ho_ConvexRegion, &hv_num);

				if (0 != (hv_num!=0))
				{
					(*hv_GapIsConvex_Que) = 1;
					disp_message(hv_WindowHandle, "部分槽边缘有粘料", "image", 620, 1500, "red", 
						"true");
				}

				Union2(ho_RegionFillUp2, ho_ConvexRegion, &ho_RegionUnion1);
				OpeningRectangle1(ho_RegionUnion1, &ho_RegionOpening3, 100, 1);
				Difference(ho_RegionUnion1, ho_RegionOpening3, &ho_RegionDifference1);
				Connection(ho_RegionDifference1, &ho_ConnectedRegions8);
				OpeningRectangle1(ho_ConnectedRegions8, &ho_RegionDifference1, 1, 10);
				Connection(ho_RegionDifference1, &ho_RegionDifference1);
 				SelectShape(ho_RegionDifference1, &ho_QueRegions, (HTuple("height").Append("width")),
					"and", hv_BadHeight.TupleConcat(hv_BadWidth), (HTuple(99999).Append(99999)));
			
				SelectShape(ho_QueRegions, &ho_QueRegions, "column2", "and", 0, hv_GapTopCutColumn-10);
				CountObj(ho_QueRegions, &hv_num);

				if (0 != (hv_num!=0))
				{
					(*hv_GapIsConvex_Que) = 1;
					disp_message(hv_WindowHandle, "部分片边缘有缺料", "image", 720, 1500, "red", 
						"true");
				}

				//滤波后的槽区域
					GenEmptyRegion(&ho_EmptyRegion);
				//expand_gray (RegionOpening3, ImageReduced, EmptyRegion, RegionOpening3, 5, 'image', 10)
				AreaCenter(ho_RegionOpening3, &hv_GapRowCenterArea, &hv_GapRowCenter, &hv_GapColumnCenter);

				GetRegionThickness(ho_RegionOpening3, &hv_Thickness, &hv_Histogramm);
				hv_GapLength = hv_Thickness.TupleMean();

				ConcatObj((*ho_GapBadRegions), ho_ConvexRegion, &(*ho_GapBadRegions));
				ConcatObj((*ho_GapBadRegions), ho_QueRegions, &(*ho_GapBadRegions));
				hv_Length = hv_GapLength*hv_LengthPerColPiexl;
				gen_arrow_contour_xld(&ho_ArrowUp, hv_GapRowCenter, hv_GapColumnCenter, hv_GapRowCenter-(hv_GapLength/2), 
					hv_GapColumnCenter, 20, 10);
				gen_arrow_contour_xld(&ho_ArrowDown, hv_GapRowCenter, hv_GapColumnCenter, hv_GapRowCenter+(hv_GapLength/2), 
					hv_GapColumnCenter, 20, 10);

				disp_message(hv_WindowHandle, "槽宽度："+hv_Length, "image", hv_GapRowCenter, hv_GapColumnCenter, 
					"black", "true"); 
				ConcatObj((*ho_GapFilterRegions), ho_RegionOpening3, &(*ho_GapFilterRegions)
					);
				ConcatObj((*ho_Arrows), ho_ArrowUp, &(*ho_Arrows));
				ConcatObj((*ho_Arrows), ho_ArrowDown, &(*ho_Arrows));
				(*hv_GapLenths) = (*hv_GapLenths).TupleConcat(hv_Length);
			}
		}

		Union1((*ho_GapFilterRegions), &ho_RegionUnion);
		Connection(ho_RegionUnion, &(*ho_GapFilterRegions));
		Union1((*ho_GapBadRegions), &ho_RegionUnion);
		Connection(ho_RegionUnion, &(*ho_GapBadRegions));
		(*hv_GapLenths)[0] = HTuple((*hv_GapLenths)[0])+HTuple((*hv_GapLenths)[((*hv_GapLenths).TupleLength())-1])-0.037;
		TupleRemove((*hv_GapLenths), ((*hv_GapLenths).TupleLength())-1, &(*hv_GapLenths));
		TupleSgn((hv_StandGapWidth+hv_GapWidthTorence)-(*hv_GapLenths), &hv_MaxSgn);
		TupleSgn((hv_StandGapWidth-hv_GapWidthTorence)-(*hv_GapLenths), &hv_MinSgn);
		TupleFind(hv_MaxSgn, -1, &hv_MaxIndices);
		TupleFind(hv_MinSgn, 1, &hv_MixIndices);

		if (0 != (hv_MaxIndices!=-1))
		{
			{
				HTuple end_val102 = (hv_MaxIndices.TupleLength())-1;
				HTuple step_val102 = 1;
				for (hv_k=0; hv_k.Continue(end_val102, step_val102); hv_k += step_val102)
				{
					SelectObj((*ho_GapFilterRegions), &ho_MaxRegion, HTuple(hv_MaxIndices[hv_k])+1);
					ConcatObj((*ho_MaxRegions), ho_MaxRegion, &(*ho_MaxRegions));
				}
			}
			Union1((*ho_MaxRegions), &ho_RegionUnion);
			Connection(ho_RegionUnion, &(*ho_MaxRegions));
			(*hv_GapWidthIsError) = 1;
			disp_message(hv_WindowHandle, "部分槽宽度大于设定值", "image", 220, 1500, "red", 
				"true");

		}
		if (0 != (hv_MixIndices!=-1))
		{
			{
				HTuple end_val114 = (hv_MixIndices.TupleLength())-1;
				HTuple step_val114 = 1;
				for (hv_k=0; hv_k.Continue(end_val114, step_val114); hv_k += step_val114)
				{
					SelectObj((*ho_GapFilterRegions), &ho_MixRegion, HTuple(hv_MixIndices[hv_k])+1);
					ConcatObj((*ho_MixRegions), ho_MixRegion, &(*ho_MixRegions));
				}
			}
			Union1((*ho_MixRegions), &ho_RegionUnion);
			Connection(ho_RegionUnion, &(*ho_MixRegions));
			(*hv_GapWidthIsError) = 1;
			disp_message(hv_WindowHandle, "部分槽宽度小于设定值", "image", 320, 1500, "red", 
				"true");
		}
	}

	AreaCenter((*ho_GapFilterRegions), &hv_Area, &(*hv_RowGapCenters), &(*hv_ColGapCenters));
	hv_Rows0 = HTuple((*hv_RowGapCenters)[0])-(12288-HTuple((*hv_RowGapCenters)[((*hv_RowGapCenters).TupleLength())-1]));
	hv_Rows18 = HTuple((*hv_RowGapCenters)[((*hv_RowGapCenters).TupleLength())-1])+HTuple((*hv_RowGapCenters)[0]);
	TupleReplace((*hv_RowGapCenters), 0, hv_Rows0, &(*hv_RowGapCenters));
	TupleReplace((*hv_RowGapCenters), ((*hv_RowGapCenters).TupleLength())-1, hv_Rows18, 
		&(*hv_RowGapCenters));
	TupleRemove((*hv_RowGapCenters), ((*hv_RowGapCenters).TupleLength())-1, &hv_Rows1);
	TupleRemove((*hv_RowGapCenters), 0, &hv_Rows2);

	(*hv_GapAngles) = (hv_Rows2-hv_Rows1)*(360.0/12288);
	{
		HTuple end_val135 = ((*hv_RowGapCenters).TupleLength())-2;
		HTuple step_val135 = 1;
		for (hv_i=1; hv_i.Continue(end_val135, step_val135); hv_i += step_val135)
		{
			disp_message(hv_WindowHandle, "槽分度:"+HTuple((*hv_GapAngles)[hv_i]), "image", 
				HTuple((*hv_RowGapCenters)[hv_i]), HTuple((*hv_ColGapCenters)[hv_i])-800, 
				"black", "true");
		}
	}

	TupleSgn((hv_StandGapAngle+hv_GapAngleTorence)-(*hv_GapAngles), &hv_MaxSgn);
	TupleSgn((hv_StandGapWidth-hv_GapAngleTorence)-(*hv_GapAngles), &hv_MinSgn);
	TupleFind(hv_MaxSgn, -1, &hv_MaxIndices);
	TupleFind(hv_MinSgn, 1, &hv_MixIndices);

	if (0 != (hv_MaxIndices!=-1))
	{
		(*hv_GapAngleIsError) = 1;
		disp_message(hv_WindowHandle, "部分槽分度大于设定值", "image", 420, 1500, "red", 
			"true");
	}
	if (0 != (hv_MixIndices!=-1))
	{
		(*hv_GapAngleIsError) = 1;
		disp_message(hv_WindowHandle, "部分槽分度小于设定值", "image", 520, 1500, "red", 
			"true");
	}

	return;
}

void CHH::NewGapWidthAndBadGap222 (HObject ho_EmphaImage, HObject *ho_GapUnFilterRegions, 
							   HObject *ho_GapFilterRegions, HTuple hv_GapBottomCutColumn, HTuple hv_GapTopCutColumn, 
							   HTuple hv_threshold_min, HTuple hv_threshold_max, HTuple hv_GapBadHeight, HTuple hv_GapBadWidth, 
							   HTuple hv_LengthPerColPiexl, HTuple hv_LengthPerRowPiexl, HTuple hv_StandGapWidth, 
							   HTuple hv_GapWidthTorence, HTuple hv_StandGapAngle, HTuple hv_GapAngleTorence, 
							   HTuple hv_WindowHandle, HTuple *hv_GapRegionNoEqual19, HTuple *hv_GapIsConvex_Que, 
							   HTuple *hv_GapWidthIsError, HTuple *hv_GapAngleIsError, HTuple *hv_GapLenths, 
							   HTuple *hv_GapAngles, HTuple *hv_RowGapCenters, HTuple *hv_ColGapCenters)
{

	// Local iconic variables
	HObject  ho_ROI, ho_ImageReduced, ho_ConnectedRegions;
	HObject  ho_RegionDifference2, ho_BadGapRegions, ho_GapBadRegions;
	HObject  ho_MaxRegions, ho_MixRegions, ho_Arrows, ho_ObjectSelected;
	HObject  ho_RegionClosing2, ho_RegionFillUp2, ho_RegionClosing3;
	HObject  ho_RegionDifference, ho_ConnectedRegions5, ho_ConvexRegion;
	HObject  ho_RegionUnion1, ho_RegionOpening3, ho_QueRegions;
	HObject  ho_RegionTransBig, ho_RegionDilation, ho_RegionBig;
	HObject  ho_RegionClosing5, ho_OpeningRegion6, ho_ConnectedRegions7;
	HObject  ho_ClosingRegions7, ho_RegionWhiteUnion2, ho_ConnectedBadRegions;
	HObject  ho_EmptyRegion, ho_ArrowUp, ho_ArrowDown, ho_RegionUnion;
	HObject  ho_MaxRegion, ho_MixRegion,ho_ImageMeanReduced;

	// Local control variables
	HTuple  hv_Number, hv_BadHeight, hv_BadWidth;
	HTuple  hv_i, hv_num, hv_GapRowCenterArea, hv_GapRowCenter;
	HTuple  hv_GapColumnCenter, hv_Thickness, hv_Histogramm;
	HTuple  hv_GapLength, hv_Length, hv_MaxSgn, hv_MinSgn, hv_MaxIndices;
	HTuple  hv_MixIndices, hv_k, hv_Area, hv_Rows0, hv_Rows18;
	HTuple  hv_Rows1, hv_Rows2;

	disp_message(hv_WindowHandle, (("槽宽度:"+hv_StandGapWidth)+"±")+hv_GapWidthTorence, 
		"image", 120, 10, "blue", "true");
	//disp_message(hv_WindowHandle, (("槽分度:"+hv_StandGapAngle)+"±")+hv_GapAngleTorence, 
	//	"image", 220, 10, "blue", "true");

	(*hv_GapRegionNoEqual19) = 0;
	(*hv_GapIsConvex_Que) = 0;
	(*hv_GapWidthIsError) = 0;
	(*hv_GapAngleIsError) = 0;

	GenRectangle1(&ho_ROI, 0, hv_GapBottomCutColumn, 12288, hv_GapTopCutColumn);
	ReduceDomain(ho_EmphaImage, ho_ROI, &ho_ImageReduced);
	MeanImage(ho_ImageReduced,&ho_ImageMeanReduced,5,5);
	Threshold(ho_ImageMeanReduced, &ho_ROI, hv_threshold_min, hv_threshold_max);
	Connection(ho_ROI, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &(*ho_GapUnFilterRegions), "width", "and", (hv_GapTopCutColumn-hv_GapBottomCutColumn)-200, 
		99999);
	CountObj((*ho_GapUnFilterRegions), &hv_Number);

	
		SetDraw(hv_WindowHandle,"margin");
	
		SetLineWidth(hv_WindowHandle,2);
	
		SetColor(hv_WindowHandle,"red");



	if (0 != (hv_Number!=19))
	{
		if (0 != (hv_Number==0))
		{
			return;
		}
		disp_message(hv_WindowHandle, "槽部选区不等于19，槽内有连铜，错误", "image", 
			220, 1500, "black", "true");
		Difference(ho_ConnectedRegions, (*ho_GapUnFilterRegions), &ho_RegionDifference2
			);
		SelectShape(ho_RegionDifference2, &ho_BadGapRegions, "width", "and", 300, 99999);
		
			DispObj(ho_BadGapRegions, hv_WindowHandle);
		(*hv_GapRegionNoEqual19) = 1;
		return;
	}

	if (0 != (hv_Number==19))
	{

		(*hv_GapLenths) = HTuple();
		GenEmptyObj(&ho_GapBadRegions);
		GenEmptyObj(&(*ho_GapFilterRegions));
		GenEmptyObj(&ho_MaxRegions);
		GenEmptyObj(&ho_MixRegions);
		GenEmptyObj(&ho_Arrows);
		hv_BadHeight = hv_GapBadHeight/hv_LengthPerColPiexl;
		hv_BadWidth = hv_GapBadWidth/hv_LengthPerRowPiexl;


		{
			HTuple end_val45 = hv_Number;
			HTuple step_val45 = 1;
			for (hv_i=1; hv_i.Continue(end_val45, step_val45); hv_i += step_val45)
			{

				SelectObj((*ho_GapUnFilterRegions), &ho_ObjectSelected, hv_i);

				ClosingCircle(ho_ObjectSelected, &ho_RegionClosing2, 3.5);
				FillUp(ho_RegionClosing2, &ho_RegionFillUp2);

				OpeningRectangle1(ho_RegionFillUp2, &ho_RegionOpening3, 100, 1);

					Difference(ho_RegionFillUp2,ho_RegionOpening3, &ho_RegionDifference);
					Connection(ho_RegionDifference, &ho_ConnectedRegions5);
					OpeningRectangle1(ho_ConnectedRegions5, &ho_RegionDifference, 1, 10);
					Connection(ho_RegionDifference, &ho_RegionDifference);
			
					SelectShape(ho_ConnectedRegions5, &ho_ConvexRegion, (HTuple("height").Append("width").Append("area")), 
						"and", (hv_BadHeight+0.1).TupleConcat(hv_BadWidth+0.1).Append(1000), (HTuple(99999).Append(99999).Append(99999)));
					CountObj(ho_ConvexRegion, &hv_num);

				if (0 != (hv_num!=0))
				{
					(*hv_GapIsConvex_Que) = 1;
					DilationCircle(ho_ConvexRegion, &ho_ConvexRegion, 18.5);
					Union1(ho_ConvexRegion, &ho_ConvexRegion);
					disp_message(hv_WindowHandle, "部分片边缘有缺料", "image", 620, 1500, "red", 
						"true");
				}

		
				//水平膨胀槽区域        
				ShapeTrans(ho_RegionOpening3, &ho_RegionTransBig, "convex");
				DilationRectangle1(ho_RegionTransBig, &ho_RegionDilation, 180, 0.5);
				ReduceDomain(ho_EmphaImage, ho_RegionDilation, &ho_ImageReduced);
				Threshold(ho_ImageReduced, &ho_RegionBig, 20, 255);
				ClosingCircle(ho_RegionBig, &ho_RegionClosing5, 15.5);
				OpeningCircle(ho_RegionClosing5, &ho_OpeningRegion6, 5.5);
				Connection(ho_OpeningRegion6, &ho_ConnectedRegions7);
				ClosingCircle(ho_ConnectedRegions7, &ho_ClosingRegions7, 15.5);
				Union1(ho_ClosingRegions7, &ho_RegionWhiteUnion2);
				Connection(ho_RegionWhiteUnion2, &ho_ConnectedBadRegions);
				SelectShape(ho_ConnectedBadRegions, &ho_QueRegions, (HTuple("height").Append("width")), 
					"and", hv_BadHeight.TupleConcat(hv_BadWidth), (HTuple(9999999).Append(9999999)));
				DilationCircle(ho_QueRegions, &ho_QueRegions, 20);
				CountObj(ho_QueRegions, &hv_num);

				if (0 != (hv_num!=0))
				{
					(*hv_GapIsConvex_Que) = 1;
					DilationCircle(ho_QueRegions, &ho_QueRegions, 18.5);
					Union1(ho_QueRegions, &ho_QueRegions);
					disp_message(hv_WindowHandle, "部分槽内有粘料", "image", 720, 1500, "red", 
						"true");
				}

				//滤波后的槽区域
				GenEmptyRegion(&ho_EmptyRegion);
				//expand_gray (RegionOpening3, ImageReduced, EmptyRegion, RegionOpening3, 5, 'image', 10)
				AreaCenter(ho_RegionOpening3, &hv_GapRowCenterArea, &hv_GapRowCenter, &hv_GapColumnCenter);

				GetRegionThickness(ho_RegionOpening3, &hv_Thickness, &hv_Histogramm);
				hv_GapLength = hv_Thickness.TupleMean();

				ConcatObj(ho_GapBadRegions, ho_ConvexRegion, &ho_GapBadRegions);
				ConcatObj(ho_GapBadRegions, ho_QueRegions, &ho_GapBadRegions);
				hv_Length = hv_GapLength*hv_LengthPerColPiexl;
				gen_arrow_contour_xld(&ho_ArrowUp, hv_GapRowCenter, hv_GapColumnCenter, hv_GapRowCenter-(hv_GapLength/2), 
					hv_GapColumnCenter, 20, 10);
				gen_arrow_contour_xld(&ho_ArrowDown, hv_GapRowCenter, hv_GapColumnCenter, hv_GapRowCenter+(hv_GapLength/2), 
					hv_GapColumnCenter, 20, 10);

				disp_message(hv_WindowHandle, hv_Length, "image", hv_GapRowCenter, hv_GapColumnCenter, 
					"black", "true");
				ConcatObj((*ho_GapFilterRegions), ho_RegionOpening3, &(*ho_GapFilterRegions)
					);
				ConcatObj(ho_Arrows, ho_ArrowUp, &ho_Arrows);
				ConcatObj(ho_Arrows, ho_ArrowDown, &ho_Arrows);
				(*hv_GapLenths) = (*hv_GapLenths).TupleConcat(hv_Length);

			}
		}

		Union1((*ho_GapFilterRegions), &ho_RegionUnion);
		Connection(ho_RegionUnion, &(*ho_GapFilterRegions));
		Union1(ho_GapBadRegions, &ho_RegionUnion);
		Connection(ho_RegionUnion, &ho_GapBadRegions);
		(*hv_GapLenths)[0] = HTuple((*hv_GapLenths)[0])+HTuple((*hv_GapLenths)[((*hv_GapLenths).TupleLength())-1]);
		TupleRemove((*hv_GapLenths), ((*hv_GapLenths).TupleLength())-1, &(*hv_GapLenths));
		TupleSgn((hv_StandGapWidth+hv_GapWidthTorence)-(*hv_GapLenths), &hv_MaxSgn);
		TupleSgn((hv_StandGapWidth-hv_GapWidthTorence)-(*hv_GapLenths), &hv_MinSgn);
		TupleFind(hv_MaxSgn, -1, &hv_MaxIndices);
		TupleFind(hv_MinSgn, 1, &hv_MixIndices);


		if (0 != (hv_MaxIndices!=-1))
		{
			{
				HTuple end_val127 = (hv_MaxIndices.TupleLength())-1;
				HTuple step_val127 = 1;
				for (hv_k=0; hv_k.Continue(end_val127, step_val127); hv_k += step_val127)
				{
					SelectObj((*ho_GapFilterRegions), &ho_MaxRegion, HTuple(hv_MaxIndices[hv_k])+1);
					ConcatObj(ho_MaxRegions, ho_MaxRegion, &ho_MaxRegions);
				}
			}
			Union1(ho_MaxRegions, &ho_RegionUnion);
			Connection(ho_RegionUnion, &ho_MaxRegions);
			DispObj(ho_MaxRegions,hv_WindowHandle);
			(*hv_GapWidthIsError) = 1;
			disp_message(hv_WindowHandle, "部分槽宽度大于设定值", "image", 220, 1500, "red", 
				"true");

		}
		if (0 != (hv_MixIndices!=-1))
		{
			{
				HTuple end_val138 = (hv_MixIndices.TupleLength())-1;
				HTuple step_val138 = 1;
				for (hv_k=0; hv_k.Continue(end_val138, step_val138); hv_k += step_val138)
				{
					SelectObj((*ho_GapFilterRegions), &ho_MixRegion, HTuple(hv_MixIndices[hv_k])+1);
					ConcatObj(ho_MixRegions, ho_MixRegion, &ho_MixRegions);
				}
			}
			Union1(ho_MixRegions, &ho_RegionUnion);
			Connection(ho_RegionUnion, &ho_MixRegions);
			DispObj(ho_MixRegions,hv_WindowHandle);
			(*hv_GapWidthIsError) = 1;
			disp_message(hv_WindowHandle, "部分槽宽度小于设定值", "image", 320, 1500, "red", 
				"true");
		}
	}

	AreaCenter((*ho_GapFilterRegions), &hv_Area, &(*hv_RowGapCenters), &(*hv_ColGapCenters));
	hv_Rows0 = HTuple((*hv_RowGapCenters)[0])-(12288-HTuple((*hv_RowGapCenters)[((*hv_RowGapCenters).TupleLength())-1]));
	hv_Rows18 = HTuple((*hv_RowGapCenters)[((*hv_RowGapCenters).TupleLength())-1])+HTuple((*hv_RowGapCenters)[0]);
	TupleReplace((*hv_RowGapCenters), 0, hv_Rows0, &(*hv_RowGapCenters));
	TupleReplace((*hv_RowGapCenters), ((*hv_RowGapCenters).TupleLength())-1, hv_Rows18, 
		&(*hv_RowGapCenters));
	TupleRemove((*hv_RowGapCenters), ((*hv_RowGapCenters).TupleLength())-1, &hv_Rows1);
	TupleRemove((*hv_RowGapCenters), 0, &hv_Rows2);

	(*hv_GapAngles) = (hv_Rows2-hv_Rows1)*(360.0/12288);
	{
		HTuple end_val158 = ((*hv_RowGapCenters).TupleLength())-2;
		HTuple step_val158 = 1;
		for (hv_i=1; hv_i.Continue(end_val158, step_val158); hv_i += step_val158)
		{
			disp_message(hv_WindowHandle, "槽分度:"+HTuple((*hv_GapAngles)[hv_i]), "image", 
				HTuple((*hv_RowGapCenters)[hv_i])-100, HTuple((*hv_ColGapCenters)[hv_i])-900, 
				"black", "true");
		}
	}

	TupleSgn((hv_StandGapAngle+hv_GapAngleTorence)-(*hv_GapAngles), &hv_MaxSgn);
	TupleSgn((hv_StandGapWidth-hv_GapAngleTorence)-(*hv_GapAngles), &hv_MinSgn);
	TupleFind(hv_MaxSgn, -1, &hv_MaxIndices);
	TupleFind(hv_MinSgn, 1, &hv_MixIndices);


	if (0 != (hv_MaxIndices!=-1))
	{
		(*hv_GapAngleIsError) = 1;
		disp_message(hv_WindowHandle, "部分槽分度大于设定值", "image", 420, 1500, "red", 
			"true");
	}
	if (0 != (hv_MixIndices!=-1))
	{
		(*hv_GapAngleIsError) = 1;
		disp_message(hv_WindowHandle, "部分槽分度小于设定值", "image", 520, 1500, "red", 
			"true");
	}
	if (0 != ((*hv_GapIsConvex_Que)==1))
	{
		
			DispObj(ho_GapBadRegions, hv_WindowHandle);
	}

	return;
}



//面阵，标定圆心和像素大小
void CHH::Area_CalibraCenter_mm (HObject ho_CalibraImage, HObject *ho_ContCircle, 
								HObject *ho_CrossCenter, HTuple hv_WindowHandle, HTuple hv_StandDiameter, HTuple *hv_RowCenter, 
								HTuple *hv_ColumnCenter, HTuple *hv_LenPixelToMM)
{

	// Local iconic variables
	HObject  ho_ImageEmphasize, ho_Circle, ho_ImageReduced;
	HObject  ho_Border, ho_SortedContours, ho_ObjectSelected;

	// Local control variables
	HTuple  hv_Row1, hv_Column1, hv_Radius1, hv_Radius;
	HTuple  hv_StartPhi, hv_EndPhi, hv_PointOrder;

	Emphasize(ho_CalibraImage, &ho_ImageEmphasize, 101, 101, 1);
		SetColor(hv_WindowHandle,"red");
		SetLineWidth(hv_WindowHandle,1);
	disp_message(hv_WindowHandle, "画圆，以使红线圆包围住被测物件", "image", 50, 50, 
		"black", "true");

	DrawCircle(hv_WindowHandle, &hv_Row1, &hv_Column1, &hv_Radius1);
	GenCircle(&ho_Circle, hv_Row1, hv_Column1, hv_Radius1);
	ReduceDomain(ho_ImageEmphasize, ho_Circle, &ho_ImageReduced);
	ThresholdSubPix(ho_ImageReduced, &ho_Border, 200);
	SortContoursXld(ho_Border, &ho_SortedContours, "upper_left", "true", "row");
	SelectObj(ho_Border, &ho_ObjectSelected, 1);
	FitCircleContourXld(ho_ObjectSelected, "algebraic", -1, 0, 0, 3, 2, &(*hv_RowCenter), 
		&(*hv_ColumnCenter), &hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	GenCircleContourXld(&(*ho_ContCircle), (*hv_RowCenter), (*hv_ColumnCenter), hv_Radius, 
		0, 6.28318, "positive", 1);
	GenCrossContourXld(&(*ho_CrossCenter), (*hv_RowCenter), (*hv_ColumnCenter), 15, 
		0.785398);

	(*hv_LenPixelToMM) = hv_StandDiameter/(hv_Radius*2);
		SetLineWidth(hv_WindowHandle,3);
		ClearWindow(hv_WindowHandle);
		DispObj(ho_CalibraImage, hv_WindowHandle);
		SetColor(hv_WindowHandle,"green");
		DispObj((*ho_ContCircle), hv_WindowHandle);
		SetColor(hv_WindowHandle,"red");
		DispObj((*ho_CrossCenter),hv_WindowHandle);
		SetLineWidth(hv_WindowHandle,1);

		disp_message(hv_WindowHandle, (((((((((((("Stand Diameter = "+hv_StandDiameter)+" mm")+"\r\n")+"1 Pixel = ")+
			(*hv_LenPixelToMM))+" mm")+"\r\n")+"CircleCenterRow = ")+
			(*hv_RowCenter))+"\r\n")+"CircleCenterCol = ")+
			(*hv_ColumnCenter))+"\r\n", 
			"image", 50, 50, "black", "true");
	return;
}
//自动搜寻电机中心坐标
BOOL CHH::ProduceCenter(HObject& Image,	HObject* ContCircle,	HTuple* RowCenter,	HTuple* ColumnCenter)
{
	HObject Border,SelectedXLD,image,ObjectSelected;
	HTuple Radius,	StartPhi,	EndPhi,	PointOrder,Num;
	try{
	Emphasize(Image, &image, 201, 201, 1);
	ThresholdSubPix(image, &Border, 200);

	SelectShapeXld(Border, &SelectedXLD, (HTuple("circularity").Append("outer_radius").Append("struct_factor")), "and", (HTuple(0.8).Append(500).Append(0.0)), (HTuple(1).Append(600).Append(0.05)));
	SelectObj(SelectedXLD, &ObjectSelected, 1);
	FitCircleContourXld(ObjectSelected, "algebraic", -1, 0, 0, 3, 2, RowCenter, ColumnCenter,&Radius, &StartPhi, &EndPhi, &PointOrder);
	GenCircleContourXld(ContCircle,*RowCenter, *ColumnCenter, Radius, 0, 6.28318, "positive", 1);
	CountObj(*ContCircle,&Num);
	if(Num!=1){
		return FALSE;
	}
	return TRUE;

	}catch(HException& e){
		e;
		return FALSE;
	}

}

void CHH::DeteterDiameter2 (HObject ho_Image, HObject *ho_TransRegions, HObject *ho_TransPoints, 
						   HObject *ho_BadLines, HObject *ho_GoodLines, HObject *ho_RegionCircle, HTuple hv_PixelLengthMM, 
						   HTuple hv_OffsetMM, HTuple hv_StandDiameter, HTuple hv_Torence, HTuple hv_WindowsHandle, 
						   HTuple *hv_IsHavePIcError, HTuple *hv_GBIsTooShortError, HTuple *hv_GBRowCeters, 
						   HTuple *hv_GBColumnCenters, HTuple *hv_DiameterLens)
{

	// Local iconic variables
	HObject  ho_Regions, ho_RegionFillUp, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionErosion, ho_ImageReduced;
	HObject  ho_Region, ho_RegionFillUp1, ho_RegionOutCicle;
	HObject  ho_RegionBig, ho_RegionSmall, ho_ImageCircle, ho_Regions1;
	HObject  ho_ConnectedRegions1, ho_SelectedRegions1, ho_MearsureCircle;
	HObject  ho_StartCirlces, ho_CenterCircles, ho_EndCircles;
	HObject  ho_PolarTransImage, ho_SelectedRegionsZhang, ho_SortedRegions;
	HObject  ho_RegionTrans, ho_RegionPoint, ho_RegionPoints;
	HObject  ho_SortedRegions1, ho_RegionDilation1, ho_RegionDilation;
	HObject  ho_RegionLine;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_Area, hv_Row;
	HTuple  hv_Column, hv_BigDiameter, hv_SmallDiameter, hv_RowCenter;
	HTuple  hv_ColumnCenter, hv_Mearsure_Radius, hv_AngleStart;
	HTuple  hv_AngleEnd, hv_Dgree_Center, hv_Dgree_Start, hv_Dgree_End;
	HTuple  hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar;
	HTuple  hv_Area1, hv_PointColumnCenters, hv_PointRowCenters;
	HTuple  hv_Area2, hv_Diameters, hv_i, hv_Distance;


	(*hv_IsHavePIcError) = 0;
	(*hv_GBIsTooShortError) = 0;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	Threshold(ho_Image, &ho_Regions, 183, 254);
	FillUp(ho_Regions, &ho_RegionFillUp);
	Connection(ho_RegionFillUp, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 2000, 99999);
	ErosionCircle(ho_SelectedRegions, &ho_RegionErosion, 25.5);

	ReduceDomain(ho_Image, ho_RegionErosion, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region, 0, 180);
	FillUp(ho_Region, &ho_RegionFillUp1);
	AreaCenter(ho_RegionFillUp1, &hv_Area, &hv_Row, &hv_Column);


	if (0 != (hv_Area<500000))
	{
		disp_message(hv_WindowsHandle, "无检测零件", "image", 80, 3000, "red", "true");
		(*hv_IsHavePIcError) = 1;
		return;
	}

	disp_message(hv_WindowsHandle, (("Φ"+hv_StandDiameter)+"±")+hv_Torence, "image", 
		20, 670, "black", "true");

	ShapeTrans(ho_Region, &ho_RegionOutCicle, "outer_circle");
	DilationCircle(ho_RegionOutCicle, &ho_RegionBig, 40.5);
	ErosionCircle(ho_RegionOutCicle, &ho_RegionSmall, 80.5);
	Difference(ho_RegionBig, ho_RegionSmall, &(*ho_RegionCircle));

	RegionFeatures(ho_RegionBig, "max_diameter", &hv_BigDiameter);
	RegionFeatures(ho_RegionSmall, "max_diameter", &hv_SmallDiameter);
	AreaCenter((*ho_RegionCircle), &hv_Area, &hv_RowCenter, &hv_ColumnCenter);

	ReduceDomain(ho_Image, (*ho_RegionCircle), &ho_ImageCircle);

	Threshold(ho_ImageCircle, &ho_Regions1, 0, 184);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, "area", "and", 500, 99999);

	hv_Mearsure_Radius = hv_SmallDiameter/2;
	CHH::FirstPhi(ho_Image, &ho_MearsureCircle, &ho_StartCirlces, &ho_CenterCircles, 
		&ho_EndCircles, hv_RowCenter, hv_ColumnCenter, hv_Mearsure_Radius, &hv_AngleStart, 
		&hv_AngleEnd, &hv_Dgree_Center, &hv_Dgree_Start, &hv_Dgree_End);

	hv_RadiusStart = hv_BigDiameter/2;
	hv_RadiusEnd = hv_SmallDiameter/2;
	hv_WidthPolar = (2*(HTuple(180).TupleRad()))*hv_RadiusStart;
	hv_HeightPolar = hv_RadiusStart-hv_RadiusEnd;

	PolarTransImageExt(ho_ImageCircle, &ho_PolarTransImage, hv_RowCenter, hv_ColumnCenter, 
		hv_AngleStart, hv_AngleEnd, hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar, 
		"nearest_neighbor");
	Threshold(ho_PolarTransImage, &ho_Regions1, 0, 184);
	Connection(ho_Regions1, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegionsZhang, (HTuple("area").Append("row1")), 
		"and", (HTuple(1000).Append(20)), (HTuple(9999999).Append(999999)));
	SortRegion(ho_SelectedRegionsZhang, &ho_SortedRegions, "first_point", "true", "column");
	ShapeTrans(ho_SortedRegions, &ho_RegionTrans, "rectangle1");
	AreaCenter(ho_RegionTrans, &hv_Area1, &hv_Row, &hv_PointColumnCenters);
	RegionFeatures(ho_SortedRegions, "row1", &hv_PointRowCenters);
	GenRegionPoints(&ho_RegionPoint, hv_PointRowCenters, hv_PointColumnCenters);
	Connection(ho_RegionPoint, &ho_RegionPoints);
	SortRegion(ho_RegionPoints, &ho_SortedRegions1, "first_point", "true", "column");
	DilationCircle(ho_SortedRegions1, &ho_RegionDilation1, 15.5);

	PolarTransRegionInv(ho_SortedRegions, &(*ho_TransRegions), hv_RowCenter, hv_ColumnCenter, 
		hv_AngleStart, hv_AngleEnd, hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar, 
		hv_Width, hv_Height, "nearest_neighbor");
	PolarTransRegionInv(ho_RegionDilation1, &(*ho_TransPoints), hv_RowCenter, hv_ColumnCenter, 
		hv_AngleStart, hv_AngleEnd, hv_RadiusStart, hv_RadiusEnd, hv_WidthPolar, hv_HeightPolar, 
		hv_Width, hv_Height, "nearest_neighbor");
	AreaCenter((*ho_TransPoints), &hv_Area2, &(*hv_GBRowCeters), &(*hv_GBColumnCenters));

	DilationCircle((*ho_TransPoints), &ho_RegionDilation, 15.5);

	hv_Diameters = HTuple();
	if (0 != (((*hv_GBRowCeters).TupleLength())!=18))
	{
		(*hv_GBIsTooShortError) = 1;
		disp_message(hv_WindowsHandle, "钩部不足18", "image", 80, 3000, "red", "true");
		return;
	}
	GenEmptyRegion(&(*ho_BadLines));
	GenEmptyRegion(&(*ho_GoodLines));
	for (hv_i=0; hv_i<=8; hv_i+=1)
	{
		DistancePp(HTuple((*hv_GBRowCeters)[hv_i]), HTuple((*hv_GBColumnCenters)[hv_i]), 
			HTuple((*hv_GBRowCeters)[hv_i+9]), HTuple((*hv_GBColumnCenters)[hv_i+9]), 
			&hv_Distance);
		GenRegionLine(&ho_RegionLine, HTuple((*hv_GBRowCeters)[hv_i]), HTuple((*hv_GBColumnCenters)[hv_i]), 
			HTuple((*hv_GBRowCeters)[hv_i+9]), HTuple((*hv_GBColumnCenters)[hv_i+9]));
		hv_Distance = (hv_Distance*hv_PixelLengthMM)+hv_OffsetMM;
		(*hv_DiameterLens).Clear();
		(*hv_DiameterLens).Append(hv_Distance);
		(*hv_DiameterLens).Append(hv_Distance);

		if (0 != ((*hv_DiameterLens)>(hv_StandDiameter+hv_Torence)))
		{
			ConcatObj((*ho_BadLines), ho_RegionLine, &(*ho_BadLines));
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i]), 
				HTuple((*hv_GBColumnCenters)[hv_i]), "red", "true");
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i+9]), 
				HTuple((*hv_GBColumnCenters)[hv_i+9]), "red", "true");
			disp_message(hv_WindowsHandle, "包络线偏小", "image", 20, 3000, "red", "false");
			*hv_GBIsTooShortError=1;
		}
		else if (0 != ((*hv_DiameterLens)<(hv_StandDiameter-hv_Torence)))
		{
			ConcatObj((*ho_BadLines), ho_RegionLine, &(*ho_BadLines));
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i]), 
				HTuple((*hv_GBColumnCenters)[hv_i]), "red", "true");
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i+9]), 
				HTuple((*hv_GBColumnCenters)[hv_i+9]), "red", "true");
			*hv_GBIsTooShortError=1;
			disp_message(hv_WindowsHandle, "包络线偏小", "image", 120, 3000, "red", "false");
		}
		else
		{
			ConcatObj((*ho_GoodLines), ho_RegionLine, &(*ho_GoodLines));
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i]), 
				HTuple((*hv_GBColumnCenters)[hv_i]), "black", "true");
			disp_message(hv_WindowsHandle, hv_Distance, "image", HTuple((*hv_GBRowCeters)[hv_i+9]), 
				HTuple((*hv_GBColumnCenters)[hv_i+9]), "black", "true");
		}
	}

		SetLineWidth(hv_WindowsHandle,2);
		SetColor(hv_WindowsHandle,"red");
		DispObj((*ho_BadLines), hv_WindowsHandle);
		SetColor(hv_WindowsHandle,"green");
		DispObj((*ho_GoodLines), hv_WindowsHandle);

	return;
}

//找到产品中心
void CHH::AutoCirclePoint (HObject ho_Image, HObject *ho_ContCircle, HTuple hv_DeadRowCenter, 
						  HTuple hv_DeadColumnCenter, HTuple hv_WindowHandle, HTuple *hv_DMIsNoCenterPoint, 
						  HTuple *hv_RowCenter, HTuple *hv_ColumnCenter)
{

	// Local iconic variables
	HObject  ho_ImageEmphasize, ho_CircleBig, ho_CircleSmall;
	HObject  ho_RegionCircle, ho_ImageReduce1, ho_Border, ho_UnionContours1;
	HObject  ho_SelectedXLD, ho_ObjectSelected;

	// Local control variables
	HTuple  hv_Num, hv_Radius, hv_StartPhi, hv_EndPhi;
	HTuple  hv_PointOrder;

	Emphasize(ho_Image, &ho_ImageEmphasize, 201, 201, 1);
	GenCircle(&ho_CircleBig, hv_DeadRowCenter, hv_DeadColumnCenter, 600);
	GenCircle(&ho_CircleSmall, hv_DeadRowCenter, hv_DeadColumnCenter, 490);
	Difference(ho_CircleBig, ho_CircleSmall, &ho_RegionCircle);
	ReduceDomain(ho_ImageEmphasize, ho_RegionCircle, &ho_ImageReduce1);

	ThresholdSubPix(ho_ImageReduce1, &ho_Border, 230);

	UnionAdjacentContoursXld(ho_Border, &ho_UnionContours1, 50, 1, "attr_keep");
	SelectShapeXld(ho_UnionContours1, &ho_SelectedXLD, ((HTuple("circularity").Append("outer_radius")).Append("struct_factor")), 
		"and", ((HTuple(0.8).Append(500)).Append(0)), ((HTuple(1).Append(600)).Append(0.05)));
	CountObj(ho_SelectedXLD, &hv_Num);
	(*hv_DMIsNoCenterPoint) = 0;
	if (0 != (hv_Num==0))
	{
		disp_message(hv_WindowHandle, "没找到中心点", "image", 50, 2800, "red", "true");
		(*hv_DMIsNoCenterPoint) = 1;
		return;
	}
	SelectObj(ho_SelectedXLD, &ho_ObjectSelected, 1);
	FitCircleContourXld(ho_ObjectSelected, "algebraic", -1, 0, 0, 3, 2, &(*hv_RowCenter), 
		&(*hv_ColumnCenter), &hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	GenCircleContourXld(&(*ho_ContCircle), (*hv_RowCenter), (*hv_ColumnCenter), hv_Radius, 
		0, 6.28318, "positive", 1);

		SetLineWidth(hv_WindowHandle,2);
		DispObj((*ho_ContCircle), hv_WindowHandle);
	return;
}
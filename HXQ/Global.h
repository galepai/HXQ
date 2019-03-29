
/**
*	功能：全局变量
*	作者：chenhui
*/


#ifndef GLOBAL_H
#define GLOBAL_H



#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include <QString>

struct DetectParam
{
	float diameterUp;
	float diameterDown;
	float slotWidthUP;
	float slotWidthDown;
	float gouWidthUP;
	float gouWidthDown;
	float maociWidth;
	float maociHeight;
	float scale_1;
	float scale_2;
	float scale_3;
	float scale_4;
	float scale_5;
	float scale_6;
	float scale_7;
	float scale_8;

};

struct SaveParam
{
	int SaveTopBadIndex;
	int SaveSideBadIndex;

	int IsSaveTopBad;
	int IsSaveSideBad;
	int IsSaveTopAll;
	int IsSaveSideAll;

	int SaveTopBadNum;
	int SaveSideBadNum;
	int SaveTopAllNum;
	int SaveSideAllNum;

	QString SaveTopBadPath;
	QString SaveSideBadPath;
	QString SaveTopAllPath;
	QString SaveSideAllPath;

	QString SaveImageFormat;

	int IsSaveLog;
};

extern QMutex g_mutex_Camera;
extern QMutex g_mutex_Result;
extern QMutex g_mutex_SaveImage;
extern QWaitCondition g_condition_Camera;
extern bool g_UpWaveEnable;
extern std::queue<bool> g_Result_Queue;
extern DetectParam g_DetectParam;
extern SaveParam g_SaveParam;

#endif // GLOBAL_H
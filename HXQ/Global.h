
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
};

extern QMutex g_mutex_Camera;
extern QMutex g_mutex_Result;
extern QMutex g_mutex_SaveImage;
extern QWaitCondition g_condition_Camera;
extern bool g_UpWaveEnable;
extern std::queue<bool> g_Result_Queue;
//extern int g_SaveTopBadIndex,g_SaveSideBadIndex;
extern int g_IsSaveTopBad, g_IsSaveSideBad, g_IsSaveTopAll, g_IsSaveSideAll;
extern QString  SaveTopBadPath,g_SaveSideBadPath ,g_SaveTopAllPathg_SaveSideAllPath;
extern DetectParam g_DetectParam;
extern SaveParam g_SaveParam;

#endif // GLOBAL_H
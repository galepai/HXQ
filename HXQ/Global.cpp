
#include "Global.h"


QMutex g_mutex_Camera;
QMutex g_mutex_Result;
QMutex g_mutex_SaveImage;
QWaitCondition g_condition_Camera;
bool g_UpWaveEnable = true;
std::queue<bool> g_Result_Queue;
//int g_SaveTopBadIndex, g_SaveSideBadIndex;
int g_IsSaveTopBad, g_IsSaveSideBad, g_IsSaveTopAll, g_IsSaveSideAll;
DetectParam g_DetectParam = { 0 };
SaveParam g_SaveParam = { 0 };
QString  SaveTopBadPath, g_SaveSideBadPath, g_SaveTopAllPathg_SaveSideAllPath;




#include "Global.h"


QMutex mutex_Camera;
QMutex mutex_Result;
QWaitCondition condition_Camera;
bool g_UpWaveEnable = true;
std::queue<bool> g_Result_Queue;
int g_SaveTopBadIndex, g_SaveSideBadIndex;
DetectParam g_DetectParam = { 0 };



#include "Global.h"


QMutex g_mutex_Camera;
QMutex g_mutex_Result;
QMutex g_mutex_SaveImage;
QWaitCondition g_condition_Camera;
bool g_UpWaveEnable = true;
std::queue<bool> g_Result_Queue;
DetectParam g_DetectParam = { 0 };
SaveParam g_SaveParam = { 0 };



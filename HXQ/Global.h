
/**
*	功能：全局变量
*	作者：chenhui
*/


#ifndef GLOBAL_H
#define GLOBAL_H



#include <QMutex>
#include <QWaitCondition>
#include <queue>

extern QMutex mutex_Camera;
extern QMutex mutex_Result;
extern QWaitCondition condition_Camera;
extern bool g_UpWaveEnable;
extern std::queue<bool> g_Result_Queue;
extern int g_SaveTopBadIndex,g_SaveSideBadIndex;

#endif // GLOBAL_H
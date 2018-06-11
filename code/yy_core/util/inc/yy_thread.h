/************************************************************************/
/*
@author:  junliang
@brief:   thread
sample:
MonitorTask monitor_task;
Thread thread(MakeDelegate(&monitor_task, &MonitorTask::run));
thread.open(1);
...
thread.close();
@time:    20130510
*/
/************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include "reflection/yy_type.h"
#include "core/inc/FastDelegate.h"

#ifdef WIN32
#    include <Windows.h>
#endif

NS_YY_BEGIN


typedef fastdelegate::FastDelegate0<void> ThreadFunCallback;
 

#define MAX_THRED_NUM        10

class Thread
{
public:
    Thread();
    Thread(ThreadFunCallback cb);
    ~Thread();

public:
    void Bind(ThreadFunCallback cb);
    void Bind();
    void Open(int num);
    void Close();

public:
    bool Stop();
    void Run();

private:

#ifdef WIN32
    HANDLE m_handle[MAX_THRED_NUM];        //线程ID
#endif

    int m_num;                            //线程数目
    bool m_stop;                        //是否停止
    ThreadFunCallback m_delegate;        //函数指针
};


NS_YY_END

#endif
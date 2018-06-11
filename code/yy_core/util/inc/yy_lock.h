/************************************************************************/
/*
@author:  junliang
@brief:   lock

线程锁
win32: 临界区, 互斥体, 读写锁
posix: 互斥体。pthread_mutex_t

进程锁
win32: 互斥体
posix: 互斥体
IPC(命名管道，共享内存)会用到进程锁

sample: 

CLock lock;
void func()
{
    AutoLock al(&lock)
    ...
}


@time:    20130510
*/
/************************************************************************/
#ifndef _YY_LOCK_H_
#define _YY_LOCK_H_

#include "reflection/yy_type.h"
#include "core/inc/yy_exception.h"
#include <Windows.h>

NS_YY_BEGIN


class ILock
{
public:
    virtual void lock()=0;
    virtual void unlock()=0;
};

class AutoLock
{
public:
    AutoLock(ILock* pLock)
    {
        throw_assert(NULL != pLock, "param is NULL.");
        m_lock=pLock;
        m_lock->lock();
    }

    ~AutoLock()
    {
        m_lock->unlock();
    }
private:
    ILock* m_lock;
};


class CLock : public ILock
{
public:
    CLock(){InitializeCriticalSection(&cs_handle_);}
    ~CLock(){DeleteCriticalSection(&cs_handle_);}
    void lock(){EnterCriticalSection(&cs_handle_);}
    void unlock(){LeaveCriticalSection(&cs_handle_);}
private:
    CRITICAL_SECTION cs_handle_;
};

NS_YY_END
#endif
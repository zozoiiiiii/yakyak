/************************************************************************/
/*
@author:  junliang
@brief:   lock

thread lock:
win32: CriticalSection, mutex
posix: pthread_mutex_t


process lock:
win32: mutex
posix: mutex

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
#ifdef _WIN32
#include <Windows.h>
#elif __ANDROID__
#elif __APPLE__
#endif

NS_YY_BEGIN


class ILock
{
public:
    virtual void Lock()=0;
    virtual void Unlock()=0;
	virtual void Wait() = 0;
};

class AutoLock
{
public:
    AutoLock(ILock* pLock)
    {
        throw_assert(NULL != pLock, "param is NULL.");
        m_lock=pLock;
        m_lock->Lock();
    }

    ~AutoLock()
    {
        m_lock->Unlock();
    }
private:
    ILock* m_lock;
};

#ifdef _WIN32
class CLock : public ILock
{
public:
	CLock() { InitializeCriticalSection(&cs_handle_); }
	~CLock() { DeleteCriticalSection(&cs_handle_); }
	void Lock() { EnterCriticalSection(&cs_handle_); }
	void Unlock() { LeaveCriticalSection(&cs_handle_); }
private:
	CRITICAL_SECTION cs_handle_;
};
#elif __ANDROID__
class CLock : public ILock
{
public:
	CLock()
	{
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_cond, NULL);
	}

	~CLock()
	{
		pthread_cond_destroy(&m_cond);
		pthread_mutex_destroy(&m_mutex);
	}

	void Lock() { pthread_mutex_lock(&m_mutex); }
	void Unlock() {pthread_mutex_unlock(&m_mutex);}
	void Wait()
	{
		pthread_cond_wait(&m_cond, &m_mutex);
	}

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};
#elif __APPLE__
#endif

NS_YY_END
#endif
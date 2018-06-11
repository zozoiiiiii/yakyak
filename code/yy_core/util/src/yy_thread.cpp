#include "util/inc/yy_thread.h"
#include "core/inc/yy_exception.h"
#include "util/inc/yy_log.h"
#include <process.h>


NS_YY_BEGIN


unsigned int __stdcall ThreadFunc(void* param)
{
    Thread* t=(Thread*)param;
    while(!t->Stop())
        t->Run();

    return 0;
}

Thread::Thread():m_stop(false), m_num(0), m_delegate(NULL)
{}

Thread::Thread(ThreadFunCallback func):m_stop(false), m_num(0), m_delegate(func)
{}

Thread::~Thread()
{
    Close();
}

void Thread::Bind(ThreadFunCallback func)
{
    m_delegate=func;
}

void Thread::Open(int num)
{
    throw_assert(m_delegate, "thread func is NULL.");

    throw_assert(num < MAX_THRED_NUM, "max thred num:"<<MAX_THRED_NUM<<"thread num:" << num);

    m_num=num;
    for(int i=0; i<m_num; i++)
    {
#ifdef WIN32
        unsigned int threadID;
        m_handle[i]=(HANDLE)_beginthreadex(NULL,                //安全属性
                                                    0,                //栈空间
                                                    ThreadFunc,        //线程入口函数
                                                    this,            //传递的参数
                                                    0,                //标志位
                                                    &threadID);        //线程ID

        throw_assert(0!=m_handle[i], "create thread failed.");

#elif YY_OS_LINUX
        pthread_t thread_id;
        pthread_create( &thread_id, NULL , ThreadFunc , this );
#endif
    }
}

void Thread::Close()
{
    if(0 == m_num)
        return;

    m_stop=true;

#ifdef WIN32
    //等待所有线程退出
    WaitForMultipleObjects((DWORD)m_num, m_handle, TRUE, INFINITE);

    //回收资源
    while(m_num--)
        CloseHandle(m_handle[m_num]);

#elif YY_OS_LINUX
    pthread_exit(NULL);
#endif
}


bool Thread::Stop()
{
    return m_stop;
}

void Thread::Run()
{
    m_delegate();
}

NS_YY_END

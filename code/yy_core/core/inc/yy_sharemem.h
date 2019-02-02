/************************************************************************/
/*
@author:  junliang
@brief:   命名共享内存，跨进程
sample:
ShareMem<ShareMemoryTest> g_smdt("test");
g_smdt->index = num;
g_smdt->msg = strMsg;

@time:    20130510
*/
/************************************************************************/
#ifndef _YY_SHAREMEMORY_H_
#define _YY_SHAREMEMORY_H_


#include <Windows.h>
#include "core/inc/yy_exception.h"

NS_YY_BEGIN

template<class T>
class ShareMem
{
public:
    ShareMem(const char* name);
    ShareMem();
    ~ShareMem();

    // 打开共享内存，如果不存在，则创建
    void open(const char* name);
    void close();

    //注意锁
    T *operator->();
    T* getData();

    void clear();

private:
    sint64 m_size;                  //共享内存的最大值, int只能表示兆，太小。
    T* m_pData;                     //共享内存中存放的数据
    bool m_created;                 //共享内存是否已经被打开
    HANDLE m_handle;                //共享内存文件的句柄
};

template<class T>
ShareMem<T>::ShareMem() : m_size(0), m_created(false), m_pData(NULL), m_handle(INVALID_HANDLE_VALUE)
{}


template<class T>
ShareMem<T>::ShareMem(const char* name) : m_pData(NULL)
{
    open(name);
}

template<class T>
ShareMem<T>::~ShareMem()
{
    close();
}

template<class T>
void ShareMem<T>::close()
{
    clear();
    if (m_pData)
    {
        //把映射对象从当前应用程序的地址空间unmap
        UnmapViewOfFile(m_pData);
        m_pData = NULL;
    }

    if (INVALID_HANDLE_VALUE == m_handle)
    {
        CloseHandle(m_handle);
    }
}

template<class T>
T *ShareMem<T>::operator->()
{
    return m_pData;
}

template<class T>
T* ShareMem<T>::getData()
{
    return m_pData;
}

template<class T>
void ShareMem<T>::clear()
{
    memset(m_pData, 0, m_size); 
}

template<class T>
void ShareMem<T>::open(const char* name)
{
    m_size = sizeof(T);

    //创建文件映射内核对象
    m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE,
        NULL,                            // 默认的安全属性
        PAGE_READWRITE,                    // 可读写文件的数据
        0,                                // size: high 32-bits；对于4GB以下的文件，该参数总是0
        sizeof(T),    // size: low 32-bits；映射文件的字节数
        name);                            // 命名该映射对象，用于和其他进程共享该对象

    if(m_handle == NULL)
    {
        int err = GetLastError();
        throw_assert(false, "CreateFileMappingA error no:" << err);
    }

    //判断是否第一个访问共享内存的程序
    m_created = (GetLastError() == ERROR_ALREADY_EXISTS); 

    //// 设置共享内存的访问权限
    //SetNamedSecurityInfo(name,
    //    SE_KERNEL_OBJECT,
    //    DACL_SECURITY_INFORMATION,    //// change only the object's DACL
    //    0,    // do not change owner or group
    //    0,    // do not change owner or group
    //    (PACL)NULL,    //no   DACL   specified   -   allows   everyone   access 
    //    NULL);    // do not change SACL系统访问控制列表


    //将一个文件映射对象映射到当前应用程序的地址空间
    m_pData = (T*)MapViewOfFile(m_handle,
        FILE_MAP_READ | FILE_MAP_WRITE,        //可以读写文件
        0,    // high offset:  map from
        0,    // low offset:   beginning
        0);     // default: map entire file

    if(m_pData == NULL)
    {
        CloseHandle(m_handle);
        int err = GetLastError();
        throw_assert(false, "MapViewOfFile error:"<< err);
    }

    //如果是第一个创建共享内存的程序，则初始化
    if (!m_created) 
    {
        clear();
    }
}


NS_YY_END
#endif
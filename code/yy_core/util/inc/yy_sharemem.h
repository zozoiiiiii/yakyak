/************************************************************************/
/*
@author:  junliang
@brief:   ���������ڴ棬�����
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

    // �򿪹����ڴ棬��������ڣ��򴴽�
    void open(const char* name);
    void close();

    //ע����
    T *operator->();
    T* getData();

    void clear();

private:
    sint64 m_size;                  //�����ڴ�����ֵ, intֻ�ܱ�ʾ�ף�̫С��
    T* m_pData;                     //�����ڴ��д�ŵ�����
    bool m_created;                 //�����ڴ��Ƿ��Ѿ�����
    HANDLE m_handle;                //�����ڴ��ļ��ľ��
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
        //��ӳ�����ӵ�ǰӦ�ó���ĵ�ַ�ռ�unmap
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

    //�����ļ�ӳ���ں˶���
    m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE,
        NULL,                            // Ĭ�ϵİ�ȫ����
        PAGE_READWRITE,                    // �ɶ�д�ļ�������
        0,                                // size: high 32-bits������4GB���µ��ļ����ò�������0
        sizeof(T),    // size: low 32-bits��ӳ���ļ����ֽ���
        name);                            // ������ӳ��������ں��������̹���ö���

    if(m_handle == NULL)
    {
        int err = GetLastError();
        throw_assert(false, "CreateFileMappingA error no:" << err);
    }

    //�ж��Ƿ��һ�����ʹ����ڴ�ĳ���
    m_created = (GetLastError() == ERROR_ALREADY_EXISTS); 

    //// ���ù����ڴ�ķ���Ȩ��
    //SetNamedSecurityInfo(name,
    //    SE_KERNEL_OBJECT,
    //    DACL_SECURITY_INFORMATION,    //// change only the object's DACL
    //    0,    // do not change owner or group
    //    0,    // do not change owner or group
    //    (PACL)NULL,    //no   DACL   specified   -   allows   everyone   access 
    //    NULL);    // do not change SACLϵͳ���ʿ����б�


    //��һ���ļ�ӳ�����ӳ�䵽��ǰӦ�ó���ĵ�ַ�ռ�
    m_pData = (T*)MapViewOfFile(m_handle,
        FILE_MAP_READ | FILE_MAP_WRITE,        //���Զ�д�ļ�
        0,    // high offset:  map from
        0,    // low offset:   beginning
        0);     // default: map entire file

    if(m_pData == NULL)
    {
        CloseHandle(m_handle);
        int err = GetLastError();
        throw_assert(false, "MapViewOfFile error:"<< err);
    }

    //����ǵ�һ�����������ڴ�ĳ������ʼ��
    if (!m_created) 
    {
        clear();
    }
}


NS_YY_END
#endif
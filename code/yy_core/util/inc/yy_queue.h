/************************************************************************/
/*
@auhtor:  junliang
@brief:   ��Ϣ����
@time:    20130510
*/
/************************************************************************/
#ifndef _YY_QUEUE_H_
#define _YY_QUEUE_H_

#include "core/inc/yy_exception.h"
#include <string.h>
#include <stdlib.h>
NS_YY_BEGIN

template<class T, size_t SIZE>
class MsgQueue
{
public:
    MsgQueue();
    ~MsgQueue();

    // ����Ϣ����β׷����Ϣ
    void PushBack(const T* data);

    // ��Ϣ�����Ƿ�Ϊ��
    bool IsEmpty();

    // ����Ϣ����ͷȡ��Ϣ
    void Pop(T* data);
private:
    int m_curNum;                            // ��ǰ����
    T m_queue[SIZE];                        // ���Ԫ�صĶ���ָ��
};


template<class T, size_t SIZE>
MsgQueue<T,SIZE>::MsgQueue()
{
    m_curNum = 0;
    memset(m_queue, 0, sizeof(T)*SIZE);
}

template<class T, size_t SIZE>
MsgQueue<T,SIZE>::~MsgQueue()
{}

template<class T, size_t SIZE>
void MsgQueue<T,SIZE>::PushBack(const T* data)
{
    memcpy(&m_queue[m_curNum], data, sizeof(T));
    m_curNum++;
}

template<class T, size_t SIZE>
bool MsgQueue<T,SIZE>::IsEmpty()
{
    return m_curNum<=0 ? true:false;
}

template<class T, size_t SIZE>
void MsgQueue<T,SIZE>::Pop(T* data)
{
    memcpy(data, &m_queue[0], sizeof(T));

    //���ֽ��Կ���
    memmove(&m_queue[0], &m_queue[1], sizeof(T)*(m_max_size-1));
    m_curNum--;
    
}

NS_YY_END
#endif
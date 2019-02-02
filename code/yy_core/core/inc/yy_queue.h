/************************************************************************/
/*
@auhtor:  junliang
@brief:   消息队列
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

    // 在消息队列尾追加消息
    void PushBack(const T* data);

    // 消息队列是否为空
    bool IsEmpty();

    // 从消息队列头取消息
    void Pop(T* data);
private:
    int m_curNum;                            // 当前数量
    T m_queue[SIZE];                        // 存放元素的队列指针
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

    //按字节自拷贝
    memmove(&m_queue[0], &m_queue[1], sizeof(T)*(m_max_size-1));
    m_curNum--;
    
}

NS_YY_END
#endif
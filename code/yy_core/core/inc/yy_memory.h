/************************************************************************/
/*
@auhtor:  junliang
@brief:   memory allocate, 

没有实现内存池, 是因为内存碎片没有想象的那么严重, 而且防止内存泄露是一个程序员基本能力.
这里把所有内存开辟和回收在此统一处理，是为了方便追踪和优化。

使用: 

    c方式：
    char* buf=MPAlloc(10);
    MPFree(buf)

    c++方式, 采用smallObject的方式:
    class Test:public NewAlloc
    {
    public:
        void func()
        {
            char* buf=new buf[10];
            delete buf;
        }
    }


    //会自动释放的一块内存类
    AutoMem<char, 512> strBuf(len);

优化: tcmalloc

@time:    20130510
*/
/************************************************************************/
#pragma once

#include <stdlib.h>
#include "reflection/yy_type.h"

//封装malloc, free api. 如果以后要优化内存管理，可以修改该实现
#define MPALLOC(size)            MemPool::Instance()->Alloc(size);
#define MPFREE(p)                if(p){MemPool::Instance()->Free(p); p=NULL;}

NS_YY_BEGIN
//c语言方式开辟内存，不会调用构造函数和析构函数
class MemPool
{
public:
    static MemPool* Instance();
    void* Alloc(size_t size);
    void Free(void* p);
};

//c++方式开辟内存, 会调用构造函数和析构函数，new/delete的封装,通过继承方式来使用。
class NewAlloc
{
public:
    void* operator new(size_t size);
    void operator delete(void* p);

    //new[]操作符中的个数参数是数组的大小加上额外的存储对象数目的一些字节。尽量避免分配对象数组
    void* operator new[](size_t size);
    void operator delete[](void* p);
};


template<typename type, size_t size>
class AutoMem : public NewAlloc
{
public:
    AutoMem(size_t len){
		memset(m_stack, 0, size);
		if (len >= size)
		{
			m_pMem = new type[len+1];
			memset(m_pMem, 0, len+1);
		}
        else
            m_pMem = m_stack;

        m_len = (int)len;
    }

    ~AutoMem(){
        if (m_pMem != m_stack)
            delete[] m_pMem;
    }

    type* GetBuf(){
        return m_pMem;
    }

    int GetLen(){
        return m_len;
    }
private:
    //私有化，防止默认转换
    AutoMem(){}
    AutoMem(const AutoMem&){}
    AutoMem& operator=(const AutoMem&){}

private:
    type m_stack[size];
    type* m_pMem;
    int m_len;
};

NS_YY_END
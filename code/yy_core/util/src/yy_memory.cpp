#include "util/inc/yy_memory.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN
MemPool* MemPool::Instance()
{
    static MemPool s_memory_pool;
    return &s_memory_pool;
}

void* MemPool::Alloc(size_t size)
{
    void* p = malloc(size);
    throw_assert(NULL != p, "malloc error.");

    return p;
}

void MemPool::Free(void* p)
{
    if(NULL == p)
        return;

    free(p);
}

void* NewAlloc::operator new(size_t size)
{
    return MPALLOC(size);
}

void NewAlloc::operator delete(void* p)
{
    MPFREE(p);
}
void* NewAlloc::operator new[](size_t size)
{
    throw_assert(false, "not support new[]");
}

void NewAlloc::operator delete[](void* p)
{
    throw_assert(false, "not support delete[]");
}

NS_YY_END
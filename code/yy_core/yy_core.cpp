#include "yy_core.h"
#include "core/src/object_mgr.h"
#include "core/src/reflection_mgr.h"
#include "core/inc/yy_string.h"

YY::IObjectMgr* g_pEntMgr=NULL;


YY::IObjectMgr* CreateEntMgr()
{
    return new YY::ObjectMgr;
}

YY::IObjectMgr* GetGlobalEntMgr()
{
    throw_assert(NULL!=g_pEntMgr, "null check.");
    return g_pEntMgr;
}


void SetGlobalEntMgr(YY::IObjectMgr* pEntMgr)
{
    g_pEntMgr = pEntMgr;
}

YY::IReflectionMgr* GetGlobalReflectionMgr()
{
    throw_assert(NULL!=g_pEntMgr, "null check.");
    return GetGlobalEntMgr()->GetReflectionMgr();
}
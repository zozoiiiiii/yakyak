#include "yy_core.h"
#include "core/src/entitymgr.h"
#include "core/src/reflection_mgr.h"
#include "util/inc/yy_string.h"

YY::IEntityMgr* g_pEntMgr=NULL;


YY::IEntityMgr* CreateEntMgr()
{
    return new YY::EntityMgr;
}

YY::IEntityMgr* GetGlobalEntMgr()
{
    throw_assert(NULL!=g_pEntMgr, "null check.");
    return g_pEntMgr;
}


void SetGlobalEntMgr(YY::IEntityMgr* pEntMgr)
{
    g_pEntMgr = pEntMgr;
}

YY::IReflectionMgr* GetGlobalReflectionMgr()
{
    throw_assert(NULL!=g_pEntMgr, "null check.");
    return GetGlobalEntMgr()->GetReflectionMgr();
}
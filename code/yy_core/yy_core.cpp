#include "yy_core.h"
#include "core/src/object_mgr.h"
#include "core/src/reflection_mgr.h"
#include "core/inc/yy_string.h"

YY::IObjectMgr* g_pEntMgr=NULL;


YY::IObjectMgr* CreateEntMgr()
{
    return new YY::ObjectMgr;
}

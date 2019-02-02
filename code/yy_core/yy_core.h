/************************************************************************/
/*
@author:  junliang
@brief:   support api which cross platform
@time:    20130401
*/
/************************************************************************/
#pragma once
#include "core/inc/yy_exception.h"
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/FastDelegate.h"
#include "core/inc/FastDelegateBind.h"
#include "core/inc/yy_ObjectMgr.h"
#include "reflection/yy_reflection.h"
#include "core/inc/yy_memory.h"
#include "core/inc/yy_string.h"
#include "core/inc/yy_file.h"
#include "core/inc/yy_time.h"
#include "core/inc/yy_entity.h"


//#include "util/inc/yy_console.h"
//#include "util/inc/yy_dump.h"
//#include "util/inc/yy_encrypt.h"
//#include "util/inc/yy_ini.h"
//#include "util/inc/yy_lock.h"
//#include "util/inc/yy_log.h"
//#include "util/inc/yy_process.h"
//#include "util/inc/yy_queue.h"
//#include "util/inc/yy_reg.h"
//#include "util/inc/yy_sharemem.h"
//#include "util/inc/yy_thread.h"

//#include "mmq/inc/yy_msg.h"
//#include "mmq/inc/yy_mmq.h"

#include "math/inc/yy_math_func.h"
#include "math/inc/yy_aabb.h"

YY::IObjectMgr* CreateEntMgr();
YY::IObjectMgr* GetGlobalEntMgr();
void SetGlobalEntMgr(YY::IObjectMgr* pEntMgr);
YY::IReflectionMgr* GetGlobalReflectionMgr();


#define YY_REG(cls) \
    extern YY::IReflectionMgr* GetGlobalReflectionMgr(); \
    GetGlobalReflectionMgr()->RegCreator(cls::MetaClassInstance());


#define LOG_ERR(Msg)          GetGlobalEntMgr()->GetLog()->LogData((int)Log::LL_ERROR, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_WARN(Msg)         GetGlobalEntMgr()->GetLog()->LogData((int)Log::LL_WARN, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_TRACE(Msg)        GetGlobalEntMgr()->GetLog()->LogData((int)Log::LL_TRACE, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_DEBUG(Msg)        GetGlobalEntMgr()->GetLog()->LogData((int)Log::LL_DEBUG, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
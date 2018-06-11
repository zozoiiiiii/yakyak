/************************************************************************/
/*
@author:  junliang
@brief:   support api which cross platform


reflection <  core <  math,mmq,util

@time:    20130401
*/
/************************************************************************/
#pragma once
#include "core/inc/yy_exception.h"
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/FastDelegate.h"
#include "core/inc/FastDelegateBind.h"
#include "core/inc/yy_entitymgr.h"
#include "reflection/yy_reflection.h"


//#include "util/inc/yy_console.h"
//#include "util/inc/yy_dump.h"
//#include "util/inc/yy_encrypt.h"
//#include "util/inc/yy_file.h"
//#include "util/inc/yy_ini.h"
//#include "util/inc/yy_lock.h"
#include "util/inc/yy_log.h"
#include "util/inc/yy_memory.h"
//#include "util/inc/yy_process.h"
//#include "util/inc/yy_queue.h"
//#include "util/inc/yy_reg.h"
//#include "util/inc/yy_sharemem.h"
#include "util/inc/yy_string.h"
//#include "util/inc/yy_thread.h"
#include "util/inc/yy_time.h"

//#include "mmq/inc/yy_msg.h"
//#include "mmq/inc/yy_mmq.h"

#include "math/inc/yy_math_func.h"
#include "math/inc/yy_aabb.h"

YY::IEntityMgr* CreateEntMgr();
YY::IEntityMgr* GetGlobalEntMgr();
void SetGlobalEntMgr(YY::IEntityMgr* pEntMgr);
YY::IReflectionMgr* GetGlobalReflectionMgr();


#define YY_REG(cls) \
    extern YY::IReflectionMgr* GetGlobalReflectionMgr(); \
    GetGlobalReflectionMgr()->RegCreator(cls::MetaClassInstance());

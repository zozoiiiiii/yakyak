/************************************************************************/
/* 
@author:    junliang
@brief:     plugin manager
@time:      5/3/2016
*/
/************************************************************************/
#pragma once
#include <map>
#include <string>
#include "yy_core.h"

class DllMgr
{
public:
    DllMgr();
    ~DllMgr();
    void Load(YY::IObjectMgr* pEntMgr, const char* file);

private:
    void Insert(YY::IObjectMgr* pEntMgr, const char* name, const char* path);

};
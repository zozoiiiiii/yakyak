/************************************************************************/
/*
@auhtor:  junliang
@brief:   windows-minidump
            linux-coredump(通过命令ulimit开启即可)

使用方法:
            在main中调用Dump::Instance();

@time:    20150922
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include <shlwapi.h>
#include <string>
#include <stdlib.h>
#include "../inc/yy_string.h"


#pragma comment(lib, "dbghelp.lib")
#include <Windows.h>
#include <imagehlp.h>
#include <assert.h>

NS_YY_BEGIN


class Dump
{
public:
    static Dump* Instance();
    ~Dump();

private:
    Dump();
    Dump(const Dump& rhs){}

    // 创建dump
    static    void    Create( EXCEPTION_POINTERS* pep );

    static    LONG __stdcall snUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

    // 创建dump的回调
    static    BOOL CALLBACK CB_Rountine( 
        PVOID pParam,
        const PMINIDUMP_CALLBACK_INPUT pInput, 
        PMINIDUMP_CALLBACK_OUTPUT       pOutput );

private:
    LPTOP_LEVEL_EXCEPTION_FILTER    m_previousFilter;
};

NS_YY_END
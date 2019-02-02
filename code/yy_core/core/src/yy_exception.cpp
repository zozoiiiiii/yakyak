#include "core/inc/yy_exception.h"

#ifdef _WIN32

#pragma comment(lib, "dbghelp.lib") 

#include <assert.h>
#include <windows.h>
#include <DbgHelp.h>
#include <malloc.h>
#include <iostream>

#define MAXSTACKDEEPTH 4
std::string StackInfo()
{
    unsigned int   i;
    void         * stack[MAXSTACKDEEPTH];
    unsigned short frames;
    SYMBOL_INFO  * symbol;

    HANDLE         process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    //参数2限制：xp, server 2003要求最大不能超过63
    frames = CaptureStackBackTrace(1, MAXSTACKDEEPTH, stack, NULL);

    //SYMBOL_INFO最后一项CHAR        Name[1];，所以长度要额外多，来存放name
    char buffer[sizeof(SYMBOL_INFO)+MAX_SYM_NAME * sizeof(TCHAR)];
    symbol = (SYMBOL_INFO *)buffer;
    symbol->MaxNameLen = MAX_SYM_NAME;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    std::string strRet;
    char buf[200] = { 0 };

    // 打印3层目录足够
    if (frames >= 3)
    {
        frames = 3;
    }

    for (i = 0; i < frames; i++)
    {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        strRet += "(";
        strRet += symbol->Name;

        sprintf_s(buf, sizeof(buf), ":0x%08x)->", symbol->Address);
        strRet += buf;
    }

    return strRet;
}

#elif __ANDROID__

#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <string>


    struct android_backtrace_state
{
    void **current;
    void **end;
};

_Unwind_Reason_Code android_unwind_callback(struct _Unwind_Context* context,
    void* arg)
{
    android_backtrace_state* state = (android_backtrace_state *)arg;
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc)
    {
        if (state->current == state->end)
        {
            return _URC_END_OF_STACK;
        }
        else
        {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}


std::string StackInfo()
{
    const int max = 100;
    void* buffer[max];

    android_backtrace_state state;
    state.current = buffer;
    state.end = buffer + max;

    _Unwind_Backtrace(android_unwind_callback, &state);

    int count = (int)(state.current - buffer);


    std::string strRet;
    char buf[200] = { 0 };
    for (int idx = 0; idx < count; idx++)
    {
        const void* addr = buffer[idx];
        const char* symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname)
        {
            symbol = info.dli_sname;
        }
        int status = 0;
        char *demangled = __cxxabiv1::__cxa_demangle(symbol, 0, 0, &status);


        sprintf(buf, "%03d: 0x%p %s",
            idx,
            addr,
            (NULL != demangled && 0 == status) ?
            demangled : symbol);
        strRet += buf;


        if (NULL != demangled)
            free(demangled);
    }

    return strRet;
}
#elif __APPLE__
#endif


NS_YY_BEGIN
AssertFailureException::AssertFailureException(const std::string& expression, const std::string& file, int line, const std::string& msg)
{
    assert(false);
    std::ostringstream outputStream;
    if (!msg.empty())
    {
        outputStream << msg << ": ";
    }

    outputStream << "Assertion '" << expression << "'";
    outputStream << " failed in file '" << file << "' line " << line << StackInfo();
    m_what = outputStream.str();
}

NS_YY_END
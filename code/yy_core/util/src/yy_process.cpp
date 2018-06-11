#include "util/inc/yy_process.h"
#include "util/inc/yy_file.h"
#include "core/inc/yy_exception.h"
#include "util/inc/yy_string.h"
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <Windows.h>

#define MAXEXELEN 512

NS_YY_BEGIN

int SplitCmdLine(const char* cmd, char* buf, int size, const char* args[], int arg_max)
{
    throw_assert(NULL != cmd && NULL != buf && size > 0, "parameter is invalid.");

    int argc = 0;

    memcpy(buf, cmd, size);

    char* s = buf;

    if (*s == '\"')
    {
        char* beg = ++s;

        s = strchr(beg, '\"');

        if (NULL == s)
        {
            return 0;
        }

        *s++ = 0;

        if (argc < arg_max)
        {
            args[argc++] = beg;
        }
    }
    else
    {
        for (; *s; ++s)
        {
            if ((*s == ' ') || (*s == '    '))
            {
                *s++ = 0;
                break;
            }
        }

        if (argc < arg_max)
        {
            args[argc++] = buf;
        }
    }

    char* arg = NULL;

    for (; *s; ++s)
    {
        if ((*s == ' ') || (*s == '    '))
        {
            *s = 0;

            if (arg != NULL)
            {
                if (argc < arg_max)
                {
                    args[argc++] = arg;
                }

                arg = NULL;
            }
        }
        else
        {
            if (NULL == arg)
            {
                arg = s;
            }
        }
    }

    if (arg != NULL)
    {
        if (argc < arg_max)
        {
            args[argc++] = arg;
        }
    }

    return argc;
}

int StartProcess(const char* _cmdline, bool _hide)
{
    //if(!IsFileExist(pPath))
    //    THROWEXCEPT<<"%s is not exit.", pPath);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (_hide)
    {
        GetStartupInfoA(&si);
        si.wShowWindow = SW_HIDE;
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    }

    ZeroMemory(&pi, sizeof(pi));

    char buf[MAXEXELEN] = { 0 };
    char* pNewCmdLine = buf;
    if (NULL == _cmdline)
    {
        pNewCmdLine = NULL;
    }
    else
    {
        strcpy_s(buf, sizeof(buf), _cmdline);
    }

    BOOL ret = CreateProcessA(NULL,    // No module name (use command line)
        pNewCmdLine,                    // Command line,可读可写
        NULL,                    // Process handle not inheritable
        NULL,                    // Thread handle not inheritable
        FALSE,                    // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,        // 创建子进程基于新窗口
        NULL,                    // Use parent's environment block
        NULL,            // 指定子进程的工作目录
        &si,
        &pi);                    // Pointer to PROCESS_INFORMATION structure

    throw_assert(ret, "failed to create process:" << pNewCmdLine);

    //关闭子进程及其主线程句柄，减少引用计数，这样子进程的主线程退出时，引用计数为0，系统可以释放主线程对象
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return pi.dwProcessId;
}

void StopProcess(int process_id)
{
    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (process_handle == NULL)
    {
        //可能此进程已经不存在，所以此处直接返回
        return;
    }

    //异步的，所以需要waitforsingleobject
    int ret = TerminateProcess(process_handle, 0);
    throw_assert (ret != 0, "failed to terminate process:" << process_id << " errno:"<< GetLastError());

    //等待进程真的退出
    DWORD dw = WaitForSingleObject(process_handle, 10000);
    switch (dw)
    {
    case WAIT_OBJECT_0:
        //ok
        break;
    case WAIT_TIMEOUT:
        throw_assert(false, "StopProcess timeout");
        break;
    case WAIT_FAILED:
        throw_assert(false, "StopProcess invalid handle");
        break;
    }

    CloseHandle(process_handle);
}

int FindProcess(const char* pPath)
{
    throw_assert (IsFileExist(pPath), pPath<<" is not exit.");

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    throw_assert(hProcessSnap != INVALID_HANDLE_VALUE,  "CreateToolhelp32Snapshot (of processes)");

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    BOOL ret = Process32First(hProcessSnap, &pe32);
    throw_assert(ret, "Process32First error"); // Show cause of failure

    do
    {

        HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
        MODULEENTRY32 me32;

        // Take a snapshot of all modules in the specified process.
        hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
        if (hModuleSnap == INVALID_HANDLE_VALUE)
        {
            continue;
            //throwexcept( "CreateToolhelp32Snapshot (of modules)" );
        }

        // Set the size of the structure before using it.
        me32.dwSize = sizeof(MODULEENTRY32);

        // Retrieve information about the first module,
        // and exit if unsuccessful
        BOOL ret = Module32First(hModuleSnap, &me32);
        throw_assert(ret, "Module32First");

        // Now walk the module list of the process,
        // and display information about each module
        do
        {

            bool ret = false;
#ifdef UNICODE
            std::wstring wstr1 = me32.szExePath;
            std::wstring wstr2 = AnsiToWStr(pPath);
            if (wstr1 == wstr2)
                ret = true;
#else
            if (strcmp(me32.szExePath, pPath) == 0)
                ret = true;
#endif  // !UNICODE

            if (ret)
            {
                return pe32.th32ProcessID;
            }

        } while (Module32Next(hModuleSnap, &me32));

        CloseHandle(hModuleSnap);


    } while (Process32Next(hProcessSnap, &pe32));

    return 0;
}

bool ProcessExist(int process_id)
{
    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (process_handle == NULL)
    {
        return false;
    }

    bool result = false;
    DWORD result_code = WaitForSingleObject(process_handle, 0);
    do 
    {
        if(WAIT_TIMEOUT == result_code)
        {
            result = true;
            break;
        }

        if(WAIT_OBJECT_0 == result_code)
        {
            result = false;
            break;
        }

        throw_assert(false, "should not here.");
    } while (false);

    CloseHandle(process_handle);
    return result;
}
int GetCurProcessID()
{
    return GetCurrentProcessId();
}
NS_YY_END

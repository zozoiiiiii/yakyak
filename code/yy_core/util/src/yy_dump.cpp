#include "util/inc/yy_dump.h"
#include "util/inc/yy_time.h"
#include "util/inc/yy_file.h"
NS_YY_BEGIN

bool IsDataSectionNeeded( const char* pModuleName ) 
{
    if( NULL == pModuleName ) {
        return false;
    }

    char szFileName[MAX_PATH] = {0};

    _splitpath_s(pModuleName, NULL, 0, NULL, 0, szFileName, ARRAYSIZE(szFileName), NULL, 0);

    return ( 0 == strcmp( szFileName, "ntdll")  ? true : false );
}

Dump* Dump::Instance(void)
{
    static    Dump    s_dump;
    return &s_dump;
}

Dump::~Dump(void)
{
    if ( NULL != m_previousFilter )
    {
        SetUnhandledExceptionFilter( m_previousFilter );
        m_previousFilter = NULL;
    }
}

LONG __stdcall Dump::snUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
    Dump::Create( pExceptionInfo );

    exit( pExceptionInfo->ExceptionRecord->ExceptionCode  ); 

    return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CALLBACK Dump::CB_Rountine(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT   pInput,   PMINIDUMP_CALLBACK_OUTPUT pOutput ) 
{
    if( 0 == pInput|| 0 == pOutput) return FALSE; 
    switch( pInput->CallbackType ) 
    {
    case ModuleCallback: 

        //如果要包含堆内容
        if( pOutput->ModuleWriteFlags & ModuleWriteDataSeg )
        {
            const wchar_t* val= pInput->Module.FullPath;
            std::string tmp = YY::WStrToAnsi(val);
            if( !IsDataSectionNeeded( tmp.c_str() ) ) 
            {
                pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 
            }
        }
        // fall through
    case IncludeModuleCallback: 
    case IncludeThreadCallback: 
    case ThreadCallback: 
    case ThreadExCallback: 
        return TRUE; 
    default:;
    }
    return FALSE; 
}

void Dump::Create( EXCEPTION_POINTERS* pep ) 
{
    std::string strExePath = YY::ExePath();
    std::string strTimeStamp = YY::SStrTimeStamp();
    std::string strdumpPath = strExePath + "\\yy_dump" + strTimeStamp + ".dmp";
    HANDLE hFile = CreateFileA(strdumpPath.c_str(),
        GENERIC_READ | GENERIC_WRITE, 
        0, NULL, CREATE_ALWAYS,  MiniDumpNormal, NULL );

    if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei; 
        mdei.ThreadId           = GetCurrentThreadId(); 
        mdei.ExceptionPointers  = pep; 
        mdei.ClientPointers     = FALSE; 
        MINIDUMP_CALLBACK_INFORMATION mci; 
        mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)(Dump::CB_Rountine);
        mci.CallbackParam       = 0;

        //保存所有内存内容
        //MINIDUMP_TYPE mdt = MiniDumpNormal;
        MINIDUMP_TYPE mdt = MiniDumpWithFullMemory;

        BOOL bOK = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
            hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci );

        CloseHandle( hFile );
    }
}

Dump::Dump(void)
{
    m_previousFilter = ::SetUnhandledExceptionFilter( Dump::snUnhandledExceptionFilter );
}


NS_YY_END
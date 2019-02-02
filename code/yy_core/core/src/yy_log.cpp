#include "core/inc/yy_log.h"
#include "core/inc/yy_time.h"
#include "core/inc/yy_string.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
//#include <windows.h>


NS_YY_BEGIN


Log::Log(int level):m_log_level(level)
{
    m_log_file = fopen("trace.txt", "w");
    throw_assert(NULL != m_log_file, "faied to create log file." );
}

Log::~Log()
{
    if(m_log_file)
        fclose(m_log_file);
}

void Log::SetLevel(int level)
{
    m_log_level = level;
}

void Log::LogData(int level, const std::string& file, int line, const std::string& buf)
{
    static std::string s_log_level[LL_MAX]=
    {
        "error",
        "warning",
        "trace",
        "debug"
    };

	std::ostringstream outputStream;
	outputStream << file << ":" << line << ", " << buf << "\n";

    std::string strLogLine = "[" + s_log_level[level] + "]";
        strLogLine += "[" + StrTimeStamp() + "]" + outputStream.str();

    LogtoConsole(strLogLine.c_str());
    LogtoFile(strLogLine.c_str());
}

void Log::LogtoConsole(const char* log)
{
    //printf is faster than cout
    //std::cout <<log<< std::endl;
    printf("%s", log);
}

void Log::LogtoFile(const char* log)
{

    size_t count=strlen(log);

    size_t write_count=fwrite(log, sizeof(char), count, m_log_file);
    throw_assert(write_count == count, "log to file error.");

    // if do not flush to memory immediately, data will lost when app crashed.
    fflush(m_log_file);
}

NS_YY_END

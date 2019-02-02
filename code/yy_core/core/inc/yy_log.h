/************************************************************************/
/*
@author:  junliang
@brief:   参照glog的简单日志库，c++方式
打开文件使用fopen而不是fopen_s, 为了程序运行时也可以打开日志文件。
fopen_s是not sharable. windows提供的sharable,但是没有fopen使用简单：
FPlogHigh = _fsopen("filename.txt", "a+",_SH_DENYWR);

在一个exe, 多个dll的工程中，会实例化多次。但是没关系啊，一个工程对应一个文件。
如果文件名是同一个，那在多个工程中fopen a+的方式也是可以的。所以Log不需要exe和dll之间单例。

@sample:
YY_LOG_TRACE("hello"<<"world");
@time:    20130510
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "core/inc/yy_exception.h"
#include "core/inc/yy_string.h"
#include "core/inc/yy_file.h"
#include <stdio.h>

NS_YY_BEGIN


class Log
{
public:
    enum LogLevel{LL_ERROR=0, LL_WARN, LL_TRACE, LL_DEBUG, LL_MAX};

    /// Helper class for formatting assertion message
    class StreamFormatter
    {
    public:
        operator std::string() const{
            return stream.str();
        }

        template<typename T>
        StreamFormatter& operator << (const T& value){
            stream << value;
            return *this;
        }

    private:
        std::ostringstream stream;
    };
public:
    Log(int level=LL_TRACE);
    ~Log();
    static Log* Instance()
    {
        static Log s_log;
        return &s_log;
    }

    void SetLevel(int level=LL_TRACE);
    void LogData(int level, const std::string& file, int line, const std::string& buf);
private:
    void LogtoConsole(const char* log);
    void LogtoFile(const char* log);
private:
    int m_log_level;
    FILE* m_log_file;
};

NS_YY_END

//interface
//#define YY_LOG_ERR(Msg)          YY::Log::Instance()->LogData((int)YY::Log::LL_ERROR, YY::Log::StreamFormatter()<<Msg)
//#define YY_LOG_TRACE(Msg)        YY::Log::Instance()->LogData((int)YY::Log::LL_TRACE, YY::Log::StreamFormatter()<<Msg)
//#define YY_LOG_DEBUG(Msg)        YY::Log::Instance()->LogData((int)YY::Log::LL_DEBUG, YY::Log::StreamFormatter()<<Msg)
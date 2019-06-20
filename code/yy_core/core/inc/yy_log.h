/************************************************************************/
/*
@author:  junliang
@brief:   log system
@time:    2018/10/30
*/
/************************************************************************/
#pragma once
#include <stdio.h>
#include <string>
#include <sstream>
#include "reflection/yy_type.h"

NS_YY_BEGIN

class Log
{
public:
	enum LogLevel { LL_ERROR = 0, LL_WARN, LL_TRACE, LL_DEBUG, LL_MAX };

	/// Helper class for formatting assertion message
	class StreamFormatter
	{
	public:
		operator std::string() const {
			return stream.str();
		}

		template<typename T>
		StreamFormatter& operator << (const T& value) {
			stream << value;
			return *this;
		}

	private:
		std::ostringstream stream;
	};
public:
	Log(int level = LL_TRACE, const std::string& path = "");
	~Log();

	void SetLogPath(const std::string& path);
	void SetLevel(int level = LL_TRACE);
	void SetAppend();
	void LogData(int level, const std::string& file, int line, const std::string& buf);
private:
	void LogtoConsole(const char* log);
	void LogtoFile(const char* log);
private:
	int m_log_level;
	FILE* m_log_file;
	std::string m_file;
	bool m_bAppend;	// append to old log file.
};

NS_YY_END
#include "../inc/yy_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <assert.h>
#include <string.h>

NS_YY_BEGIN

Log::Log(int level, const std::string& path) :m_log_level(level), m_bAppend(false), m_log_file(nullptr)
{
	m_file = path + "trace.txt";
}

Log::~Log()
{
	if (m_log_file)
		fclose(m_log_file);
}

void Log::SetAppend()
{
	m_bAppend = true;

	if (m_log_file)
		fclose(m_log_file);

	m_log_file = fopen(m_file.c_str(), "a+");
	assert(NULL != m_log_file);
}


void Log::SetLogPath(const std::string& path)
{
	m_file = path;
}

void Log::SetLevel(int level)
{
	m_log_level = level;
}

void Log::LogData(int level, const std::string& file, int line, const std::string& buf)
{
	static std::string s_log_level[LL_MAX] =
	{
		"error",
		"warning",
		"trace",
		"debug"
	};


	std::ostringstream outputStream;
	outputStream << file << ":" << line << ", " << buf << "\n";

	std::string strLogLine = "[" + s_log_level[level] + "]" + outputStream.str();

	LogtoConsole(strLogLine.c_str());
	LogtoFile(strLogLine.c_str());
}

void Log::LogtoConsole(const char* log)
{
	//printf is faster than cout
	//std::cout <<log<< std::endl;
	printf("%s", log);
	//qDebug("%s", log);
}

void Log::LogtoFile(const char* log)
{
	if (nullptr == m_log_file)
		m_log_file = fopen(m_file.c_str(), "w+");

	size_t count = strlen(log);

	size_t write_count = fwrite(log, sizeof(char), count, m_log_file);

	// if do not flush to memory immediately, data will lost when app crashed.
	fflush(m_log_file);
}

NS_YY_END
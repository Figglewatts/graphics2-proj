#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include <fstream>
#include <sstream>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(L_, MESSAGE)												\
	do 																	\
	{																	\
		Logger *log = Logger::Instance();								\
		std::ostringstream __logstream__;								\
		__logstream__ << MESSAGE;										\
		log->Log(__func__, __FILENAME__, __LINE__, L_, __logstream__.str());\
	} while (0)

enum class LogLevel : unsigned
{
	INFO = 0,
	WARN = 1,
	ERR = 2
};

std::string get_log_level(LogLevel level);

// yeah, it's a singleton
class Logger
{
private:
	std::ofstream _logfile;
	static Logger *_loggerInstance;
	static const std::string LOGFILE_NAME;

	Logger(std::string logfileName);
public:
	~Logger();

	void Log(char const* function, char const* file,
		unsigned line, LogLevel level, std::string logmsg);

	static Logger *Instance();

};

#endif // LOGGER_H
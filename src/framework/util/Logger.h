#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include <fstream>
#include <sstream>

#define LOG(L_, MESSAGE)												\
	do 																	\
	{																	\
		Logger *log = Logger::Instance();								\
		std::ostringstream __logstream__;								\
		__logstream__ << std::string(MESSAGE);							\
		log->Log(__func__, __FILE__, __LINE__, L_, __logstream__.str());\
	} while (0)

enum class LogLevel : unsigned int
{
	INFO,
	WARN, 
	ERR,
	FATAL
};

static std::string log_level_strings[] = {
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
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

	void Log(const std::string& function, const std::string& file,
		unsigned line, LogLevel level, const std::string& logmsg);

	static Logger *Instance();

};

#endif // LOGGER_H
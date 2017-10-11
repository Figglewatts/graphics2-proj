#include "Logger.h"
#include "date.h"
#include <chrono>
#include <iostream>

Logger *Logger::_loggerInstance = nullptr;
const std::string Logger::LOGFILE_NAME = "log.txt";

std::string get_log_level(LogLevel level)
{
	return (std::string[]) {
		"INFO",
		"WARN",
		"ERR"
	}[static_cast<unsigned>(level)];
}

Logger *Logger::Instance()
{
	if (!_loggerInstance)
	{
		_loggerInstance = new Logger(LOGFILE_NAME);
	}
	return _loggerInstance;
}

Logger::Logger(std::string logfileName)
{
	_logfile = std::ofstream(logfileName);
}

Logger::~Logger()
{
	_logfile.close();
	delete _loggerInstance;
}

void Logger::Log(char const* function, char const* file,
	unsigned line, LogLevel level, std::string logmsg)
{
	_logfile << date::format("%F %T", std::chrono::system_clock::now()) << " "
		<< "<" << file << ":"
		<< function << "()#" << line << "> "
		<< get_log_level(level) << " | "
		<< logmsg
		<< std::endl;
	_logfile.flush();
}
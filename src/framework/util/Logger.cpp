#include "Logger.h"
#include "date.h"
#include <chrono>
#include <iostream>
#include <cstring>

Logger *Logger::_loggerInstance = nullptr;
const std::string Logger::LOGFILE_NAME = "app.log";

std::string get_log_level(LogLevel level)
{
	return log_level_strings[static_cast<unsigned>(level)];
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

void Logger::Log(const std::string& function, const std::string& file,
	unsigned line, LogLevel level, const std::string& logmsg)
{
#ifdef _WIN32
	std::string filename = (std::strrchr(&file[0], '\\') ? std::strrchr(&file[0], '\\') + 1 : file);
#else
	std::string filename = (std::strrchr(&file[0], '/') ? std::strrchr(&file[0], '/') + 1 : file);
#endif
	_logfile << date::format("%F %T", std::chrono::system_clock::now()) << " "
		<< "<" << filename << ":"
		<< function << "()#" << line << "> "
		<< get_log_level(level) << " | "
		<< logmsg
		<< std::endl;
	_logfile.flush();
}
#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include <fstream>
#include <sstream>
#include <memory> // std::unique_ptr
#include <iostream>
#include <string>
#include <cstdio>

#define LOG(L_, ...)												\
	do 																	\
	{																	\
		Logger *log = Logger::Instance();								\
		log->Log(__func__, __FILE__, __LINE__, L_, string_format(__VA_ARGS__));\
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

// from https://stackoverflow.com/a/26221725
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

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
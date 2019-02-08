#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#define MAX_LOG_SIZE 2000

typedef enum logLevel
{
	ERROR = 0,
	INFO = 1,
	DEBUG = 2,
	LOG_LEVEL_BUTT
}LogLevel;

typedef enum error
{
	SUCCESS = 0,
	ERROR_INVALID_LOG_LEVEL,
	ERROR_FILE_OPEN_FAILED
}Error;


class Logger
{
public:
	Logger();
	~Logger();
	static Logger *get();
	static void release();
	int setLogLevel(LogLevel level);
	int setLogStream(const char *path, bool truncate);
	bool isLoggingEnabled(LogLevel level);
	int logMessage(LogLevel level, const char *file, const char *function, 
					const int line, const char* format, ...);
private:	
	FILE *stream;
	LogLevel logLevel;

	
     // Singleton global logger that will be returned by Logger::get.
    static Logger* logger;
};

#define LOG(level, format, ...) do { \
    Logger *_logger = Logger::get(); \
    if (_logger->isLoggingEnabled(level)) { \
        _logger->logMessage(level, __FILE__, __FUNCTION__, __LINE__,\
                           format "\n", ##__VA_ARGS__); \
    } \
} while (0)

#endif /*LOGGER_H*/
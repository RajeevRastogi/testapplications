#include "Logger.h"
#include <sys/types.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <windows.h>

/*
 * 1. An utility logger to log all application events.
 * 2. Log will be pritned to configured storage (by default console).
 * 3. Log format is as below:
 *  	// DD:MM:YYYY, HH:MM:SS:MSMS : FILENAME:LINENO IN FUNCNAME LOGLEVEL[PID:THREAD]: Actual message
 * 4. Max size of application event log is 2000 bytes (excluding header details).
 */
Logger* Logger::logger = NULL;
HANDLE gMutex = CreateMutex(
    NULL,
    FALSE,
    NULL);

/**
 * Readable names for each #LogLevel value.
 * Should be in sync with the LogLevel enum.
 */
static const char* logLevelNames[] = {"ERROR", "INFO","DEBUG"};

Logger::Logger()
		:stream(stderr)
		,logLevel(INFO)
{
}

Logger::~Logger()
{
    WaitForSingleObject(gMutex, INFINITE);
    if (stream != stderr)
        fclose(stream);

	ReleaseMutex(gMutex);
}

Logger* Logger::get()
{	
	if (logger == NULL)
	{		
		// Check if concurrently someone has created logger after first check.
		WaitForSingleObject(gMutex, INFINITE);

		//see if still logger is not created
		if (logger == NULL)
		{
			logger = new Logger();
		}

		ReleaseMutex(gMutex);
	}

	return logger;
}

void Logger::release()
{
	// Destroy the singleton object.
	// NOTE: we can also keep a reference count to see if someone is still using
	// this object but for simplicity and performance this has been omitted.
	// Application needs to take care of this i.e. dont call this function unless
	// all task done.
	WaitForSingleObject(gMutex, INFINITE);
	delete logger;
	logger = NULL;
	ReleaseMutex(gMutex);
}

// set the configured log level.
int Logger::setLogLevel(LogLevel level)
{
	WaitForSingleObject(gMutex, INFINITE);
	if (level >= LOG_LEVEL_BUTT)
	{
		ReleaseMutex(gMutex);
		return ERROR_INVALID_LOG_LEVEL;
	}

	logLevel = level;	
	ReleaseMutex(gMutex);
	return SUCCESS;
}

// set the configured log destination.
int Logger::setLogStream(const char *path, bool truncate)
{	
	FILE *fd;
	errno_t err = fopen_s(&fd, path, truncate ? "w" : "a");
    if (SUCCESS != err) {
		ReleaseMutex(gMutex);
		return ERROR_FILE_OPEN_FAILED;
    }

	WaitForSingleObject(gMutex, INFINITE);
	if (stream != stderr)
	{
		fclose(stream);		
	}

	stream = fd;
	ReleaseMutex(gMutex);
	return SUCCESS;
}

// check if logging is enabled.
bool Logger::isLoggingEnabled(LogLevel level)
{
	WaitForSingleObject(gMutex, INFINITE);
	bool enabled = (logLevel <= level);
	ReleaseMutex(gMutex);
	return enabled;
}

// maain function to form the log message in required format and flush.
int Logger::logMessage(LogLevel level, const char *file, 
						const char *function, const int line, 
						const char* format, ...)
{
    va_list ap;
    int needed;
	string message;
	SYSTEMTIME st;
    char *buffer = new char[MAX_LOG_SIZE];

    // Compute the header of the log message except for the initial timestamp
    // (i.e. filename, line number, function name, current process and thread id).	
    _snprintf_s(buffer, MAX_LOG_SIZE, MAX_LOG_SIZE, "%s:%d in %s %s[%d:%d]: ",
            file, line, function, 
            logLevelNames[level], GetCurrentProcessId(), GetCurrentThreadId());
	message += buffer;

	// compute the actual given log message.
    va_start(ap, format);
    needed = _vsnprintf_s(buffer, MAX_LOG_SIZE, _TRUNCATE, format, ap);	
    va_end(ap);    
	message += buffer;

    if (needed == -1 || needed > MAX_LOG_SIZE) {
        // whole log message probably could not fit in our fixed size buffer
		// truncate it.	
        _snprintf_s(buffer, MAX_LOG_SIZE, MAX_LOG_SIZE, "... (possibly few chars truncated)\n");
		message += buffer;
    }

	// Append current timestamp
	GetLocalTime(&st);

	// Overall log Format is:
	// DD:MM:YYYY, HH:MM:SS:MSMS : FILENAME:LINENO IN FUNCNAME LOGLEVEL[PID:THREAD]: Actual message	
	WaitForSingleObject(gMutex, INFINITE);
	fprintf(stream, "%d:%d:%d, %d:%d:%d:%d : %s", st.wDay,st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, message.c_str());
    fflush(stream);
	ReleaseMutex(gMutex);
	return SUCCESS;
}
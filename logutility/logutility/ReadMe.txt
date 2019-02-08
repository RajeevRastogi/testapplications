========================================================================
    Logger : logutility Project Overview
========================================================================
Problem Statement:
Implement an utility class to log application events to a file or any other storage with a configurable 
level of logging - DEBUG, INFO, ERROR. The function header: void log(level, char* fmt, ...). 
CValler example: log(DEBUG, “Server initialized successfully”);

Solution Implementation
 *
 * 1. An utility logger to log all application events.
 * 2. Log will be pritned to configured storage (by default console).
 * 3. Log format is as below:
 *  	DD:MM:YYYY, HH:MM:SS:MSMS : FILENAME:LINENO IN FUNCNAME LOGLEVEL[PID:THREAD]: Actual message
 * 4. Max size of application event log is 2000 bytes (excluding header details).
 *
 Files Meaning: 
 main.c: Test application to invoke different kind of logging test.
 test.* Also test file to test logger with various parameters.
 logger.*: Actual functionality implementation.
 ////////////////////////////////////////////////////////////////////////////

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file log.h
 *
 *  @brief This file contains function for GMSEC API C logging framework
 */

#ifndef GMSEC_API_C_LOG_H
#define GMSEC_API_C_LOG_H


#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>  /* For LogEntry and LogLevel */


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @fn void logSetReportingLevel(GMSEC_LogLevel level)
	 *
	 * @brief This function sets the maximum logging level
	 *
	 * @param[in] level - logging level
	 *
	 * @sa logGetReportingLevel()
	 */
	GMSEC_API void logSetReportingLevel(GMSEC_LogLevel level);


	/**
	 * @fn GMSEC_LogLevel logGetReportingLevel()
	 *
	 * @brief This function returns the maximum logging level set by the library or
	 * the maximum logging level set by the user
	 *
	 * @return the current logging level
	 *
	 * @sa logSetReportingLevel()
	 */
	GMSEC_API GMSEC_LogLevel logGetReportingLevel();


	/**
	 * @fn GMSEC_BOOL logIsLoggingEnabled(GMSEC_LogLevel level)
	 *
	 * @brief This function returns whether logging is enable for the specified level
	 *
	 * @param[in] level - logging level
	 *
	 * @sa logSetReportingLevel()
	 * @sa logGetReportingLevel()
	 */
	GMSEC_API GMSEC_BOOL logIsLoggingEnabled(GMSEC_LogLevel level);


	/**
	 * @fn void logRegisterHandler(GMSEC_LogHandler* handler)
	 *
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 *
	 * @param[in] handler - the logger handler
	 */
	GMSEC_API void logRegisterHandler(GMSEC_LogHandler* handler);


	/**
	 * @fn void logRegisterHandlerByLevel(GMSEC_LogLevel level, GMSEC_LogHandler* handler)
	 *
	 * @brief This function registers a LogHandler that will be called for a specific logging level.
	 *
	 * @param[in] level   - the logging level
	 * @param[in] handler - the logger handler
	 */
	GMSEC_API void logRegisterHandlerByLevel(GMSEC_LogLevel level, GMSEC_LogHandler* handler);


	/**
	 * @fn const char* logLevelToString(GMSEC_LogLevel level)
	 *
	 * @brief This function converts the log level number to a string value
	 *
	 * @param[in] level - the logging level
	 */
	GMSEC_API const char* logLevelToString(GMSEC_LogLevel level);


	/**
	 * @fn GMSEC_LogLevel logLevelFromString(const char* level)
	 *
	 * @brief This function converts the string value to the log level number
	 *
	 * @param[in] level - the logging level as a string
	 */
	GMSEC_API GMSEC_LogLevel logLevelFromString(const char* level);


	GMSEC_API void logPrint(const char* file, int line, GMSEC_LogLevel level, const char* fmt, ...);


#ifdef LOG_LEVEL_DEFINE
#error "Attempting to redefine the log levels"
#endif
#define LOG_LEVEL_DEFINE

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL logDEBUG
#endif

#ifndef GMSEC_VC6
	/* variadic macros are not supported by VC6 :( */

#define LOG(level,...)			                           \
	if ((level <= logNONE) && (level >= logNLEVEL)) ;      \
	else if (level > LOG_MAX_LEVEL) ;			           \
	else if (!logIsLoggingEnabled(level)) ;	               \
	else logPrint(__FILE__, __LINE__, level, __VA_ARGS__)


#define GMSEC_ERROR(...)      LOG(logERROR,   __VA_ARGS__)
#define GMSEC_SECURE(...)     LOG(logSECURE,  __VA_ARGS__)
#define GMSEC_WARNING(...)    LOG(logWARNING, __VA_ARGS__)
#define GMSEC_INFO(...)       LOG(logINFO,    __VA_ARGS__)
#define GMSEC_VERBOSE(...)    LOG(logVERBOSE, __VA_ARGS__)
#define GMSEC_DEBUG(...)      LOG(logDEBUG,   __VA_ARGS__)

#endif /* GMSEC_VC6 */


#ifdef __cplusplus
}	// extern "C"
#endif

#endif

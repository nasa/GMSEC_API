
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file log_c.h
 *
 *  @brief This file contains function for GMSEC API C logging framework
 *
**/

#ifndef gmsec_util_log_c_h
#define gmsec_util_log_c_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>


#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn void gmsec_SetReportingLevel(enum LogLevel level)
	 *
	 * @brief This function sets the maximum logging level
	 *
	 * @param[in] level - logging level
	 *
	 * @sa gmsec_GetReportingLevel()
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_SetReportingLevel(enum LogLevel level);

	/** @fn enum LogLevel gmsec_GetReportingLevel()
	 *
	 * @brief This function returns the maximum logging level set by the library or
	 * the maximum logging level set by the user
	 *
	 * @param[in] level - logging level
	 *
	 * @sa gmsec_GetReportingLevel()
	 */
	GMSEC_DEPRECATED GMSEC_API enum LogLevel gmsec_GetReportingLevel();

	/** @fn GMSEC_BOOL gmsec_IsLoggingEnabled(enum LogLevel level)
	 *
	 * @brief This function returns logging status for a level
	 *
	 * @param[in] level - logging level
	 *
	 * @sa gmsec_SetReportingLevel() @n
	 *     gmsec_GetReportingLevel()
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_BOOL gmsec_IsLoggingEnabled(enum LogLevel level);

	/** @fn void gmsec_RegisterLogHandler(GMSEC_LOGGER_HANDLER *handler)
	 *
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 *
	 * @param[in] handler - the logger handler
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_RegisterLogHandler(GMSEC_LOGGER_HANDLER *handler);

	/** @fn void gmsec_RegisterLogHandlerByLevel(enum LogLevel level, GMSEC_LOGGER_HANDLER *handler)
	 *
	 * @brief This function registers a LogHandler that will be called for a specific logging level.
	 *
	 * @param[in] level - the logging level
	 * @param[in] handler - the logger handler
	 */
	GMSEC_DEPRECATED GMSEC_API void gmsec_RegisterLogHandlerByLevel(enum LogLevel level, GMSEC_LOGGER_HANDLER *handler);

	/** @fn const char* gmsec_LogLevelToString(enum LogLevel level)
	 *
	 * @brief This function converts the log level number to a string value
	 *
	 * @param[in] level - the logging level
	 */
	GMSEC_DEPRECATED GMSEC_API const char* gmsec_LogLevelToString(enum LogLevel level);

	/** @fn enum LogLevel gmsec_LogLevelFromString(const char *level)
	 *
	 * @brief This function converts the string value to the log level number
	 *
	 * @param[in] level - the logging level
	 */
	GMSEC_DEPRECATED GMSEC_API enum LogLevel gmsec_LogLevelFromString(const char *level);

	GMSEC_DEPRECATED GMSEC_API void gmsec_CAPILog(enum LogLevel level, char *fmt, ...);

#ifdef LOG_LEVEL_DEFINE
#error "Attempting to redefine the log levels"
#endif
#define LOG_LEVEL_DEFINE

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL logDEBUG
#endif

#ifndef GMSEC_VC6
	/* variadic macros are not supported by VC6 :( */

#define LOG(level,...)			  \
	if ((level <= logNONE) && (level >= logNLEVEL)) ; \
	else if (level > LOG_MAX_LEVEL) ;			  \
	else if (!gmsec_IsLoggingEnabled(level)) ;	  \
	else gmsec_CAPILog(level, __VA_ARGS__)


#define LOG_ERROR(...)		LOG(logERROR, __VA_ARGS__)
#define LOG_WARNING(...)	LOG(logWARNING, __VA_ARGS__)
#define LOG_INFO(...)		LOG(logINFO, __VA_ARGS__)
#define LOG_VERBOSE(...)	LOG(logVERBOSE, __VA_ARGS__)
#define LOG_DEBUG(...)		LOG(logDEBUG, __VA_ARGS__)

#endif /* GMSEC_VC6 */


#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_util_log_c_h */

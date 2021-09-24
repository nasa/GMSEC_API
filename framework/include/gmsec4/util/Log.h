/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Log.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API_UTIL_LOG_H
#define GMSEC_API_UTIL_LOG_H

#include <gmsec4_defs.h>           // For LogLevel

#include <gmsec4/util/LoggerStream.h>
#include <gmsec4/util/LogHandler.h>
#include <gmsec4/util/LogStream.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace util
{


/**
 * @class Log
 *
 * @brief This class defines the logging interface.
 *
 */
class GMSEC_API Log
{
public:
	/**
	 * @fn static void setReportingLevel(LogLevel level)
	 *
	 * @brief This function sets the maximum logging level
	 *
	 * @param level - LogLevel with possible values logNONE, logERROR, 
	 *                logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *                logDEBUG
	 */
	static void setReportingLevel(LogLevel level);


	/**
	 * @fn static LogLevel getReportingLevel()
	 *
	 * @brief This function returns the maximum logging level set by the library or
	 * the maximum logging level set by the user
	 *
	 * @return LogLevel with possible values logNONE, logERROR,
	 *         logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG
	 */
	static LogLevel getReportingLevel();


	/**
	 * @fn static void registerHandler(LogHandler *handler)
	 *
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 *
	 * @param handler - Pointer to an instance of a class whose base 
	 *                  class is LogHandler.
	 */
	static void registerHandler(LogHandler* handler);


	/**
	 * @fn static void registerHandler(LogLevel level, LogHandler* handler)
	 *
	 * @brief This function registers a LogHandler that will be called for a specific logging level.
	 *
	 * @param level - LogLevel with possible values logNONE, logERROR,
	 *                logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *                logDEBUG
	 *
	 * @param handler - Pointer to an instance of a class whose base 
	 *                  class is LogHandler.
	 */
	static void registerHandler(LogLevel level, LogHandler* handler);


	/* @cond For C API support ONLY */
	static void registerHandler(GMSEC_LogHandler* handler);
	/* @endcond */


	/* @cond For C API support ONLY */
	static void registerHandler(GMSEC_LogLevel level, GMSEC_LogHandler* handler);
	/* @endcond */


	/**
	 * @fn static const char* toString(LogLevel level)
	 *
	 * @brief This function converts the log level number to a string value
	 *
	 * @param level - LogLevel with possible values logNONE, logERROR,
	 *                logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *                logDEBUG
	 *
	 * @return static const char* pointing to possibilities "NONE,"
	 *         "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG,"
	 *         or "INVALID"
	 */
	static const char* toString(LogLevel level);


	/**
	 * @fn static LogLevel fromString(const char* level)
	 *
	 * @brief This function converts the string value to the log level number
	 *
	 * @param const char* level -- function expects possibilities "NONE,"
	 *        "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," or "DEBUG"
	 *
	 * @return LogLevel with possible values logNONE, logERROR,
	 *         logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *         logDEBUG
	 */
	static LogLevel fromString(const char* level);
};


} //namespace util
} //namespace api
} //namespace gmsec


#ifdef LOG_LEVEL_DEFINE
	#error "Attempting to redefine the GMSEC API Log levels."
#endif
#define LOG_LEVEL_DEFINE


#define GMSEC_LOG(level) \
	if ((level <= logNONE) && (level >= logNLEVEL)) ; \
	else if (level > ::gmsec::api::util::Log::getReportingLevel()) ;  \
	else ::gmsec::api::util::LoggerStream(__FILE__ , __LINE__).get(level)


#define GMSEC_ERROR     GMSEC_LOG(logERROR)
#define GMSEC_SECURE    GMSEC_LOG(logSECURE)
#define GMSEC_WARNING   GMSEC_LOG(logWARNING)
#define GMSEC_INFO      GMSEC_LOG(logINFO)
#define GMSEC_VERBOSE   GMSEC_LOG(logVERBOSE)
#define GMSEC_DEBUG     GMSEC_LOG(logDEBUG)

#endif

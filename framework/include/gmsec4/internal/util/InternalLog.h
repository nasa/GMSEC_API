/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalLog.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API_INTERNAL_LOG_H
#define GMSEC_API_INTERNAL_LOG_H

#include <gmsec4_defs.h>           // For LogLevel and LogEntry

#include <gmsec4/util/LogHandler.h>
#include <gmsec4/util/wdllexp.h>

#include <iostream>
#include <string>


namespace gmsec
{
	namespace util
	{
		class Log;      // for API 3.x support; remove later
	}

namespace api
{
namespace internal
{
	class DefaultHandler;
	class InternalConfig;


/**
 * @class Log
 *
 * @brief This class defines the logging interface.
 *
 */
class GMSEC_API InternalLog
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
	 * @fn static void registerHandler(gmsec::api::util::LogHandler *handler)
	 *
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 *
	 * @param handler - Pointer to an instance of a class whose base 
	 *                  class is LogHandler.
	 */
	static void registerHandler(gmsec::api::util::LogHandler* handler);


	/**
	 * @fn static void registerHandler(LogLevel level, gmsec::api::util::LogHandler* handler)
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
	static void registerHandler(LogLevel level, gmsec::api::util::LogHandler* handler);


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
	 * @fn static void fromString(const char* level)
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


	/* @cond For C API support ONLY */
	static void registerHandler(GMSEC_LogHandler* handler);
	/* @endcond */

	/* @cond For C API support ONLY */
	static void registerHandler(GMSEC_LogLevel level, GMSEC_LogHandler* handler);
	/* @endcond */


	/**
	 * @fn static std::string prepareLogMessage(const LogEntry& entry)
	 *
	 * @brief Convenience method for generating the output string for a logged message entry.
	 *
	 * @param entry - the LogEntry to use when generating the output string
	 *
	 * @returns A log message string.
	 */
	static std::string prepareLogMessage(const gmsec::api::util::LogEntry& entry);

private:
	friend class gmsec::util::Log;
	friend class gmsec::api::internal::InternalConfig;

	/**
	 * @fn static void setDefaultStream(std::ostream* s)
	 *
	 * @brief This function allows the user to change the default stream from stderr.
	 *
	 * @param s - a pointer to an std::ostream object to use as a destination for logging output.
	 */
	static void setDefaultStream(std::ostream* s);


	static LogLevel s_maxLevel;
};


inline LogLevel InternalLog::getReportingLevel()
{
	return InternalLog::s_maxLevel;
}


} //namespace internal
} //namespace api
} //namespace gmsec


// Convenience methods
//
gmsec::api::internal::DefaultHandler& getDefaultHandler();
gmsec::api::util::LogHandler*         getHandler(LogLevel level);


#endif

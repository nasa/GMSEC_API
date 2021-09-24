/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Log.h
 *
 *  @brief This file contains the implementation of the logging framework for
 *  the GMSEC API
 *
**/

#ifndef gmsec_util_Log_h
#define gmsec_util_Log_h


#define IGNORE_GMSEC_API_4_0_TYPES

#include <gmsec_defs.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>

#include <gmsec4/util/Log.h>

#include <sstream>
#include <iostream>
#include <cstring>


namespace gmsec
{
namespace util
{

typedef GMSEC_LOG_ENTRY LogEntry;


/** @class LogHandler
 *
 * @brief This class is the abstract base class for the logger handler callbacks.
 *
 * @sa Log
**/
class GMSEC_API LogHandler
{
public:

	/** @fn ~LogHandler()
	 * @brief Base class needs virtual destructor
	 */
	virtual ~LogHandler() { }

	/** @fn OnMessage(const LogEntry &entry)
	 * @brief This function is called by the API in response to a log message. A class derrived from
	 * gmsec::util::LogHandler needs to be registered with a Logger using registerHandler() in order
	 * to be called for a particular logging level.
	 *
	 * Because this class is user defined, it is not considered thread-safe unless made so by the user.
	 * The use of a gmsec::util::AutoMutex is suggested to encapsulate any sensitive code blocks.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnMessage(const LogEntry &entry) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the LogEntry that is passed into this function by the API.
	 * It is owned by the API and do not need to be managed by the client program. Also, it can
	 * not be stored by the client program beyond the scope of this callback function.
	 *
	 * @param entry - the received log entry
	 */
	virtual void CALL_TYPE OnMessage(const LogEntry &entry) = 0;
};


class LogStreamImp
{
public:
	std::ostringstream os;

	GMSEC_DEPRECATED LogStreamImp();
	~LogStreamImp();
};

class GMSEC_API LogStream
{
public:
	GMSEC_DEPRECATED LogStream();
	~LogStream();

	LogStream &operator<<(char x);
	LogStream &operator<<(int x);
	LogStream &operator<<(unsigned int x);
	LogStream &operator<<(long x);
	LogStream &operator<<(unsigned long x);
	LogStream &operator<<(long long x);
	LogStream &operator<<(unsigned long long x);

	LogStream &operator<<(float x);
	LogStream &operator<<(double x);
	LogStream &operator<<(const char *x);

	LogStream &operator<<(const void *x);
	LogStream &operator<<(const std::string &x);

	std::string getText();

	std::ostream &getStream();

private:
	LogStream(const LogStream &); // = delete;
	LogStream &operator=(const LogStream &); // = delete;

	LogStreamImp *ptr;
};


/** @class Logger
 *
 * @sa Log
**/
class GMSEC_API Logger
{
public:
	GMSEC_DEPRECATED Logger();
	GMSEC_DEPRECATED Logger(const char *file, int line);
	~Logger();
	std::ostream& Get(LogLevel level = logINFO);

private:
	Logger(const Logger&);
	Logger& operator =(const Logger&);
	LogEntry entry;
	LogHandler *handler;
	std::ostringstream os;
	
};


/** @class LoggerStream
 *
 * @sa LogStream
**/
class GMSEC_API LoggerStream
{
public:
	GMSEC_DEPRECATED LoggerStream();
	GMSEC_DEPRECATED LoggerStream(const char *file, int line);
	~LoggerStream();
	LogStream& Get(LogLevel level = logINFO);

private:
	LoggerStream(const LoggerStream&);
	LoggerStream& operator =(const LoggerStream&);
	LogEntry entry;
	LogHandler *handler;
	LogStream logStream;
};


/** @class Log
 *
 * @brief This class defines the logging interface
 *
**/
class GMSEC_API Log
{
public:

	/** @fn SetReportingLevel(LogLevel level)
	 * @brief This function sets the maximum logging level
	 *
	 * @param level - LogLevel with possible values logNONE, logERROR, 
	 *                logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *                logDEBUG
	 */
	GMSEC_DEPRECATED static void SetReportingLevel(LogLevel level);

	/** @fn GetReportingLevel()
	 * @brief This function returns the maximum logging level set by the library or
	 * the maximum logging level set by the user
	 *
	 * @return LogLevel with possible values logNONE, logERROR,
	 *         logSECURE, logWARNING, logINFO, logVERBOSE, or logDEBUG
	 */
	GMSEC_DEPRECATED static LogLevel GetReportingLevel();

	/** @fn RegisterHandler(LogHandler *handler)
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 *
	 * @param handler - Pointer to an instance of a class whose base 
	 *                  class is LogHandler.
	 */
	GMSEC_DEPRECATED static void RegisterHandler(LogHandler *handler);

	/** @fn RegisterHandler(LogLevel level, LogHandler *handler)
	 * @brief This function registers a LogHandler that will be called for a specific logging level.
	 *
	 * @param level - LogLevel with possible values logNONE, logERROR,
	 *                logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *                logDEBUG
	 *
	 * @param handler - Pointer to an instance of a class whose base 
	 *                  class is LogHandler.
	 *
	 */
	GMSEC_DEPRECATED static void RegisterHandler(LogLevel level, LogHandler *handler);

	/** @fn ToString(LogLevel level)
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
	GMSEC_DEPRECATED static const char* ToString(LogLevel level);

	/** @fn FromString(const char *level)
	 * @brief This function converts the string value to the log level number
	 *
	 * @param const char* level -- function expects possibilities "NONE,"
	 *        "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," or "DEBUG"
	 *
	 * @return LogLevel with possible values logNONE, logERROR,
	 *         logSECURE, logWARNING, logINFO, logVERBOSE, or
	 *         logDEBUG
	 */
	GMSEC_DEPRECATED static LogLevel FromString(const char *level);

	/** @fn SetDefaultStream(std::ostream *s)
	 * @brief This function allows the user to change the default stream from stderr.
	 * @param s - std::ostream* which points to new output stream to use.
	 */
	GMSEC_DEPRECATED static void SetDefaultStream(std::ostream *s);

	/*
	** these are for C API support ONLY
	*/
	GMSEC_DEPRECATED static void RegisterHandler(GMSEC_LOGGER_HANDLER *handler);
	GMSEC_DEPRECATED static void RegisterHandler(LogLevel level, GMSEC_LOGGER_HANDLER *handler);

private:
	// Log() {};
	// friend class Logger;
	static LogLevel fMaxLevel;
};


inline LogLevel Log::GetReportingLevel()
{
#if 0
	return fMaxLevel;
#else
	return gmsec::api::util::Log::getReportingLevel();
#endif
}


}	// namespace util
}	// namespace gmsec



#if 0
#ifdef LOG_LEVEL_DEFINE
#error "Attempting to redefine the log levels"
#endif
#define LOG_LEVEL_DEFINE
#endif

#ifdef GMSEC_BINCOMPAT_LOGGING
#define LOG_ACTIVE(level) \
	level < logNLEVEL && level < Log::GetReportingLevel()

#define LOG0(level)								\
	if ((level <= logNONE) && (level >= logNLEVEL)) ; 			\
	else if (level > gmsec::util::Log::GetReportingLevel()) ; 		\
	else gmsec::util::LoggerStream(__FILE__ , __LINE__).Get(level)
#else
#define LOG0(level)								\
	if ((level <= logNONE) && (level >= logNLEVEL)) ; 			\
	else if (level > gmsec::util::Log::GetReportingLevel()) ; 		\
	else gmsec::util::Logger(__FILE__ , __LINE__).Get(level)
#endif

#ifdef GMSEC_LOG_OSTREAM
#define LOG(level) LOG0(level).getStream()
#else
#define LOG(level) LOG0(level)
#endif

#define LOG_ERROR		LOG(logERROR)
#define LOG_SECURE		LOG(logSECURE)
#define LOG_WARNING		LOG(logWARNING)
#define LOG_INFO		LOG(logINFO)
#define LOG_VERBOSE		LOG(logVERBOSE)
#define LOG_DEBUG		LOG(logDEBUG)

#endif /* gmsec_util_Log_h */

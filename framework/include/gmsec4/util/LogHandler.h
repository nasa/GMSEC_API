/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogHandler.h
 *
 * @brief Defines the interface for the LogHandler callback.
 */

#ifndef GMSEC_API_UTIL_LOGHANDLER_H
#define GMSEC_API_UTIL_LOGHANDLER_H

#include <gmsec4_defs.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace util
{

/**
 * @struct LogEntry
 *
 * @brief The class that is passed to a LogHandler object when a message is logged.
 * The structure is defined as:
 *
 * @sa Log
 * @sa LogHandler
 */
struct LogEntry
{
    const char*    file;
    int            line;
    GMSEC_LogLevel level;
    GMSEC_TimeSpec time;
    const char*    message;
};


/**
 * @class LogHandler
 *
 * @brief This class is the abstract base class for the logger handler callbacks.
 *
 * @sa Log
 */
class GMSEC_API LogHandler
{
public:
	virtual ~LogHandler() {}


	/**
	 * @fn void onMessage(const LogEntry& entry) = 0
	 *
	 * @brief This function is called by the API in response to a log message. A class derived from
	 * gmsec::api::util::LogHandler needs to be registered with Log::registerHandler() in order
	 * to be called for a particular logging level.
	 *
	 * Because this class is user defined, it is not considered thread-safe unless made so by the
	 * user.  The use of a gmsec::util::AutoMutex is suggested to encapsulate any sensitive code
	 * blocks.
	 *
	 * The prototype for this funtion is:
	 * @code virtual void CALL_TYPE onMessage(const LogEntry &entry) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * @param entry - the received log entry
	 */
	virtual void CALL_TYPE onMessage(const LogEntry& entry) = 0;
};

} // namespace util
} // namespace api
} // namespace gmsec

#endif

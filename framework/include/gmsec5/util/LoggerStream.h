/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LoggerStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API5_UTIL_LOGGER_STREAM_H
#define GMSEC_API5_UTIL_LOGGER_STREAM_H

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{
	class InternalLoggerStream;
}
namespace util
{
	class LogStream;

/*
 * @cond
 * @class LoggerStream
 *
 * @brief Class that is referenced by LOG-style macros.
 */
class GMSEC_API LoggerStream
{
public:
    LoggerStream(const char* file = NULL, int line = 0);

    ~LoggerStream();

    LogStream& CALL_TYPE get(LogLevel level = LogLevel::logINFO);

private:
    LoggerStream(const LoggerStream&);
    LoggerStream& operator =(const LoggerStream&);

    gmsec::api5::internal::InternalLoggerStream* m_internal;
};
/** @endcond */


}  // namespace util
}  // namespace api5
}  // namespace gmsec
#endif

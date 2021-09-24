/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LoggerStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API_UTIL_LOGGER_STREAM_H
#define GMSEC_API_UTIL_LOGGER_STREAM_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
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
 * @desc Class that is referenced by LOG-style macros.
 */
class GMSEC_API LoggerStream
{
public:
    LoggerStream(const char* file = NULL, int line = 0);

    ~LoggerStream();

    LogStream& get(LogLevel level = logINFO);

private:
    LoggerStream(const LoggerStream&);
    LoggerStream& operator =(const LoggerStream&);

    gmsec::api::internal::InternalLoggerStream* m_internal;
};
/** @endcond */


}  // namespace util
}  // namespace api
}  // namespace gmsec
#endif

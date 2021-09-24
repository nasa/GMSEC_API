/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API_UTIL_LOG_STREAM_H
#define GMSEC_API_UTIL_LOG_STREAM_H

#include <gmsec4/util/wdllexp.h>

namespace gmsec
{
namespace api
{

namespace internal
{
	class InternalLogStream;
}

namespace util
{


/*
 * @cond
 * @class LogStream
 *
 * @desc Class used to direct data to an internal stream object using operator<<().
 *
 * @note This class is required to support API 4.x
 */
class GMSEC_API LogStream
{
public:
	LogStream();

	~LogStream();

	LogStream& operator<<(char x);
	LogStream& operator<<(int x);
	LogStream& operator<<(unsigned int x);
	LogStream& operator<<(long x);
	LogStream& operator<<(unsigned long x);
	LogStream& operator<<(long long x);
	LogStream& operator<<(unsigned long long x);

	LogStream& operator<<(float x);
	LogStream& operator<<(double x);
	LogStream& operator<<(const char* x);

	LogStream& operator<<(const void* x);

	const char* getText() const;

private:
	// defined, but not implemented
	LogStream(const LogStream&);
	LogStream& operator=(const LogStream&);

	internal::InternalLogStream* m_internal;
};
/** @endcond */


}  // namespace util
}  // namespace api
}  // namespace gmsec

#endif

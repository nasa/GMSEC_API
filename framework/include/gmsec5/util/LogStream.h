/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file LogStream.h
 *
 * @brief Contains the structs and classes that support logging.
 */

#ifndef GMSEC_API5_UTIL_LOG_STREAM_H
#define GMSEC_API5_UTIL_LOG_STREAM_H

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
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
 * @brief Class used to direct data to an internal stream object using operator<<().
 *
 * @note This class is required to support API 5.x
 */
class GMSEC_API LogStream
{
public:
	LogStream();

	~LogStream();

	LogStream& CALL_TYPE operator<<(char x);
	LogStream& CALL_TYPE operator<<(int x);
	LogStream& CALL_TYPE operator<<(unsigned int x);
	LogStream& CALL_TYPE operator<<(long x);
	LogStream& CALL_TYPE operator<<(unsigned long x);
	LogStream& CALL_TYPE operator<<(long long x);
	LogStream& CALL_TYPE operator<<(unsigned long long x);
	LogStream& CALL_TYPE operator<<(float x);
	LogStream& CALL_TYPE operator<<(double x);
	LogStream& CALL_TYPE operator<<(const char* x);
	LogStream& CALL_TYPE operator<<(const void* x);

	const char* CALL_TYPE getText() const;

private:
	// defined, but not implemented
	LogStream(const LogStream&);
	LogStream& operator=(const LogStream&);

	internal::InternalLogStream* m_internal;
};
/** @endcond */


}  // namespace util
}  // namespace api5
}  // namespace gmsec

#endif

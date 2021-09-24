
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_util_timeutil_h
#define gmsec_util_timeutil_h


#include <gmsec_defs.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{
namespace util
{

struct TimeBuffer
{
	double seconds;	// *integer* since 1970 Jan 1 0z
	int millis;	// 0 .. 999
	int nanos;	// 0 .. 1e9-1
};

/**
* @fn getTime_s ()
* @brief Returns the current time in (real) seconds since Jan 1, 1970.
* @note If buffer is non-zero, it will be populated.
*/
GMSEC_API double CALL_TYPE getTime_s(TimeBuffer *buffer = 0);

/**
* @fn formatTime_s (double time_s, char *buffer)
* @brief Formats a GMSEC time string YYYY-DDD-HH-MM-SS.sss
* @note buffer must be at least TIME_BUFSIZE characters
*/
static const int TIME_BUFSIZE = 32;
GMSEC_DEPRECATED GMSEC_API void CALL_TYPE formatTime_s(double time_s, char *buffer);

/**
* @fn formatTime_s (const TimeBuffer &b, char *buffer, int subs)
* @brief Formats a GMSEC time string YYYY-DDD-HH-MM-SS[.s*]
* @param subs How many digits of subseconds to format [0 .. 9].
* @note buffer must be at least TIME_BUFSIZE characters
*/
GMSEC_DEPRECATED GMSEC_API void CALL_TYPE formatTime_s(const TimeBuffer &t, char *buffer, int subs);

/**
* @fn getTime_s ()
* @brief Returns the current time in (real) seconds since Jan 1, 1970 as 
* represented by the GMSEC String
* @note If buffer is non-zero, it will be populated.
*/
GMSEC_DEPRECATED GMSEC_API double CALL_TYPE getCharTime_s(const char * timeString);


} // namespace util
} // namespace gmsec


#endif /* gmsec_util_timeutil_h */


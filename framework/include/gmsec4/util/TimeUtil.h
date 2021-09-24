/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file TimeUtil.h
 *
 * @brief Contains the functions that support the acquisition of current
 * system time, and the ability to format such into a GMSEC-style time
 * string.
 */


#ifndef GMSEC_API_UTIL_TIMEUTIL_H
#define GMSEC_API_UTIL_TIMEUTIL_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api
{
namespace util
{


class GMSEC_API TimeUtil
{
public:
	/**
	 * @fn int millisleep(unsigned int milliseconds)
	 *
	 * @brief Sleeps for the specified number of milliseconds (at least 1).
	 */
	static void CALL_TYPE millisleep(unsigned int milliseconds);


	/**
	 * @fn GMSEC_TimeSpec getCurrentTime()
	 *
	 * @brief Returns the current time in seconds since Jan 1, 1970.
	 */
	static GMSEC_TimeSpec CALL_TYPE getCurrentTime();


	/**
	 * @fn getCurrentTime_s(GMSEC_TimeSpec* ts = 0)
	 *
	 * @brief Returns the current time in (real) seconds since Jan 1, 1970.
	 *
	 * @note If timespec buffer is non-zero, it will be populated.
	 */
	static double CALL_TYPE getCurrentTime_s(GMSEC_TimeSpec* ts = 0);


	/**
	 * @fn GMSEC_TimeSpec getCharTime(const char* timeString)
	 *
	 * @brief Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
	 */
	static GMSEC_TimeSpec CALL_TYPE getCharTime(const char* timeString);


	/**
	 * @fn double getCharTime_s(const char* timeString)
	 *
	 * @brief Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
	 */
	 static double CALL_TYPE getCharTime_s(const char* timeString);


	/**
	 * @fn void CALL_TYPE formatTime(const GMSEC_TimeSpec& time_s, char* buffer)
	 *
	 * @brief Formats the given TimeSpec into a GMSEC time string with the format of YYYY-DDD-HH-MM-SS.sss
	 *
	 * @param ts     - a GMSEC_TimeSpec containing time information
	 * @param buffer - the array where to store the formatted time string.
	 *
	 * @note buffer must be at least GMSEC_TIME_BUFSIZE characters
	 */
	static void CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, char* buffer);


	/**
	 * @fn void formatTime(const GMSEC_TimeSpec& time_s, int subs, char* buffer)
	 *
	 * @brief Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.
	 *
	 * @param ts     - a GMSEC_TimeSpec containing time information
	 * @param subs   - the number of digits to allow for subseconds [0 .. 9].
	 * @param buffer - the array where to store the formatted time string.
	 *
	 * @note buffer must be at least GMSEC_TIME_BUFSIZE characters
	 */
	static void CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, int subs, char* buffer);


	// @cond
	// Deprecated since v4.2 - nevertheless we keep for binary compatibility reasons
	static void CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, char* buffer, int subs);
	// @endcond


	/**
	 * @fn GMSEC_TimeSpec diffTime(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
	 *
	 * @brief Computes the time-delta between the two given times.
	 *
	 * @return A GMSEC_TimeSpec that contains the time-delta.
	 */
	static GMSEC_TimeSpec CALL_TYPE diffTime(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


private:
	static void CALL_TYPE formatTimeAux(const GMSEC_TimeSpec& ts, char* buffer, const char* timeFormat, int subs);
};


/**
 * @fn bool operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Compares the two given GMSEC_TimeSpec values for equality.
 *
 * @return Returns true of the time values are equal; false otherwise.
 *
 * @note This method has been deprecated.
 */
GMSEC_API bool CALL_TYPE operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


/**
 * @fn bool operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Reports back as to whether the first time value is chronologically greater than the second time value.
 *
 * @return Returns true if the first time value is greater; false otherwise.
 *
 * @note This method has been deprecated.
 */
GMSEC_API bool CALL_TYPE operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


/**
 * @fn bool operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Reports back as to whether the first time value is chronologically less than the second time value.
 *
 * @return Returns true if the first time value is less; false otherwise.
 *
 * @note This method has been deprecated.
 */
GMSEC_API bool CALL_TYPE operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);

} // namespace util
} // namespace api
} // namespace gmsec


/**
 * @fn bool operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Compares the two given GMSEC_TimeSpec values for equality.
 *
 * @return Returns true of the time values are equal; false otherwise.
 */
GMSEC_API bool CALL_TYPE operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


/**
 * @fn bool operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Reports back as to whether the first time value is chronologically greater than the second time value.
 *
 * @return Returns true if the first time value is greater; false otherwise.
 */
GMSEC_API bool CALL_TYPE operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


/**
 * @fn bool operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
 *
 * @brief Reports back as to whether the first time value is chronologically less than the second time value.
 *
 * @return Returns true if the first time value is less; false otherwise.
 */
GMSEC_API bool CALL_TYPE operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


#endif

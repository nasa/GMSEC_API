/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file timeutil.h
 *
 *  @brief This file contains C-binding time utility functions.
 */

#ifndef GMSEC_API_C_TIME_UTIL_H
#define GMSEC_API_C_TIME_UTIL_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @fn void timeUtilMillisleep(unsigned int milliseconds)
 *
 * @brief Sleeps for the specified number of milliseconds (at least 1).
 */
GMSEC_API void timeUtilMillisleep(unsigned int milliseconds);


/**
 * @fn GMSEC_TimeSpec timeUtilGetCurrentTime()
 *
 * @desc Returns the current time, in seconds, since Jan 1 1970 00:00:00 GMT.
 */
GMSEC_API GMSEC_TimeSpec timeUtilGetCurrentTime();


/**
 * @fn double timeUtilGetCurrentTime_s(GMSEC_TimeSpec* ts)
 *
 * @brief Returns the current time in (real) seconds since Jan 1, 1970.
 *
 * @note If timespec buffer is non-NULL, it will be populated.
 */
GMSEC_API double timeUtilGetCurrentTime_s(GMSEC_TimeSpec* ts);


/**
 * @fn GMSEC_TimeSpec timeUtilGetCharTime(const char* timeString)
 *
 * @brief Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
 *
 * @param[in] timeString - String containing GMSEC formatted time (e.g. YYYY-DDD-HH-MM-SS[.sss])
 */
GMSEC_API GMSEC_TimeSpec timeUtilGetCharTime(const char* timeString);


/**
 * @fn GMSEC_TimeSpec timeUtilGetCharTime_s(const char* timeString)
 *
 * @brief Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
 *
 * @param[in] timeString - String containing GMSEC formatted time (e.g. YYYY-DDD-HH-MM-SS[.sss])
 */
GMSEC_API double timeUtilGetCharTime_s(const char* timeString);


/**
 * @fn void timeUtilFormatTime(GMSEC_TimeSpec time, GMSEC_Time buffer)
 *
 * @brief Converts the given time (in seconds) to a string with the format of YYYY-DDD-HH:MM:SS.mmm
 *
 * @param[in]  time   - the time
 * @param[out] buffer - buffer where to store the string
 *
 * @sa GMSEC_LogEntry
 */
GMSEC_API void timeUtilFormatTime(const GMSEC_TimeSpec time, GMSEC_Time buffer);


/**
 * @fn void timeUtilFormatTimeSubs(const GMSEC_TimeSpec time, int subs, GMSEC_Time buffer)
 *
 * @brief Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.
 *
 * @param[in]  time   - the time
 * @param[in]  subs   - the number of digits to allow for subseconds [0 .. 9].
 * @param[out] buffer - buffer where to store the string
 *
 * @sa GMSEC_LogEntry
 */
GMSEC_API void timeUtilFormatTimeSubs(const GMSEC_TimeSpec time, int subs, GMSEC_Time buffer);


/**
 * @fn void timeUtilDiffTime(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
 *
 * @brief Computes the time-delta between the two given times.
 *
 * @return A GMSEC_TimeSpec that contains the time-delta.
 */
GMSEC_API GMSEC_TimeSpec timeUtilDiffTime(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilEqualToconst GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
 *
 * @brief Compares the two given GMSEC_TimeSpec values for equality.
 *
 * @return Returns true of the time values are equal; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilEqualTo(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilGreaterThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
 *
 * @brief Reports back as to whether the first time value is chronologically greater than the second time value.
 *
 * @return Returns true if the first time value is greater; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilGreaterThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilLesserThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
 *
 * @brief Reports back as to whether the first time value is chronologically less than the second time value.
 *
 * @return Returns true if the first time value is less; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilLesserThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0);


#ifdef __cplusplus
}   // extern "C"
#endif

#endif

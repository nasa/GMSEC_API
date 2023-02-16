/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file timeutil.h
 *
 *  @brief This file contains C-binding time utility functions.
 */

#ifndef GMSEC_API5_C_TIME_UTIL_H
#define GMSEC_API5_C_TIME_UTIL_H

#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


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
 * @brief Returns the current time, in seconds, since Jan 1 1970 00:00:00 GMT.
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
 * @fn GMSEC_TimeSpec timeUtilConvertTimeString(const char* timeString, GMSEC_Status status)
 *
 * @brief Converts the given time string into a %GMSEC_TimeSpec.
 *
 * @param[in]  timeString - A time string int the format of YYYY-DDD-HH-MM-SS[.sss]
 * @param[out] status     - The result of the operation
 */
GMSEC_API GMSEC_TimeSpec timeUtilConvertTimeString(const char* timeString, GMSEC_Status status);


/**
 * @fn GMSEC_TimeSpec timeUtilConvertTimeString_s(const char* timeString, GMSEC_Status status)
 *
 * @brief Converts the given time string, in the format of YYYY-DDD-HH-MM-SS[.sss], into
 * a double value representing the number of seconds since Jan 1, 1970.
 *
 * @param[in]  timeString - A time string int the format of YYYY-DDD-HH-MM-SS[.sss]
 * @param[out] status     - The result of the operation
 */
GMSEC_API double timeUtilConvertTimeString_s(const char* timeString, GMSEC_Status status);


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
GMSEC_API void timeUtilFormatTime(GMSEC_TimeSpec time, GMSEC_Time buffer);


/**
 * @fn void timeUtilFormatTimeSubs(GMSEC_TimeSpec time, int subs, GMSEC_Time buffer)
 *
 * @brief Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.
 *
 * @param[in]  time   - the time
 * @param[in]  subs   - the number of digits to allow for subseconds [0 .. 9].
 * @param[out] buffer - buffer where to store the string
 *
 * @sa GMSEC_LogEntry
 */
GMSEC_API void timeUtilFormatTimeSubs(GMSEC_TimeSpec time, int subs, GMSEC_Time buffer);


/**
 * @fn void timeUtilDiffTime(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
 *
 * @brief Computes the time-delta between the two given times.
 *
 * @return A GMSEC_TimeSpec that contains the time-delta.
 */
GMSEC_API GMSEC_TimeSpec timeUtilDiffTime(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilEqualTo(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
 *
 * @brief Compares the two given GMSEC_TimeSpec values for equality.
 *
 * @return Returns true of the time values are equal; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilEqualTo(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilGreaterThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
 *
 * @brief Reports back as to whether the first time value is chronologically greater than the second time value.
 *
 * @return Returns true if the first time value is greater; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilGreaterThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0);


/**
 * @fn GMSEC_BOOL timeUtilLesserThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
 *
 * @brief Reports back as to whether the first time value is chronologically less than the second time value.
 *
 * @return Returns true if the first time value is less; false otherwise.
 */
GMSEC_API GMSEC_BOOL timeUtilLesserThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0);


#ifdef __cplusplus
}   // extern "C"
#endif

#endif

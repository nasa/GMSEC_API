/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file timeutil.cpp
 *
 *  @brief This file contains C-binding time utility functions.
 */

#include <gmsec4/c/util/timeutil.h>

#include <gmsec4/util/TimeUtil.h>


void CALL_TYPE timeUtilMillisleep(unsigned int milliseconds)
{
	gmsec::api::util::TimeUtil::millisleep(milliseconds);
}


GMSEC_TimeSpec CALL_TYPE timeUtilGetCurrentTime()
{
	return gmsec::api::util::TimeUtil::getCurrentTime();
}


double CALL_TYPE timeUtilGetCurrentTime_s(GMSEC_TimeSpec* ts)
{
	return gmsec::api::util::TimeUtil::getCurrentTime_s(ts);
}


GMSEC_TimeSpec CALL_TYPE timeUtilGetCharTime(const char* timeString)
{
	return gmsec::api::util::TimeUtil::getCharTime(timeString);
}


double CALL_TYPE timeUtilGetCharTime_s(const char* timeString)
{
	return gmsec::api::util::TimeUtil::getCharTime_s(timeString);
}


void CALL_TYPE timeUtilFormatTime(const GMSEC_TimeSpec time, GMSEC_Time buffer)
{
	gmsec::api::util::TimeUtil::formatTime(time, buffer);
}


void CALL_TYPE timeUtilFormatTimeSubs(const GMSEC_TimeSpec time, int subs, GMSEC_Time buffer)
{
	gmsec::api::util::TimeUtil::formatTime(time, subs, buffer);
}


GMSEC_TimeSpec CALL_TYPE timeUtilDiffTime(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
{
	return gmsec::api::util::TimeUtil::diffTime(time1, time0);
}


GMSEC_BOOL CALL_TYPE timeUtilEqualTo(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
{
	return (time1 == time0 ? GMSEC_TRUE : GMSEC_FALSE);
}


GMSEC_BOOL CALL_TYPE timeUtilGreaterThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
{
	return (time1 > time0 ? GMSEC_TRUE : GMSEC_FALSE);
}


GMSEC_BOOL CALL_TYPE timeUtilLesserThan(const GMSEC_TimeSpec time1, const GMSEC_TimeSpec time0)
{
	return (time1 < time0 ? GMSEC_TRUE : GMSEC_FALSE);
}

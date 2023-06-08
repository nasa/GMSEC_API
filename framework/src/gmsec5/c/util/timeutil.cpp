/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file timeutil.cpp
 *
 *  @brief This file contains C-binding time utility functions.
 */

#include <gmsec5/c/util/timeutil.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/TimeUtil.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void CALL_TYPE timeUtilMillisleep(unsigned int milliseconds)
{
	TimeUtil::millisleep(milliseconds);
}


GMSEC_TimeSpec CALL_TYPE timeUtilGetCurrentTime()
{
	return TimeUtil::getCurrentTime();
}


double CALL_TYPE timeUtilGetCurrentTime_s(GMSEC_TimeSpec* ts)
{
	return TimeUtil::getCurrentTime_s(ts);
}


GMSEC_TimeSpec CALL_TYPE timeUtilConvertTimeString(const char* timeString, GMSEC_Status status)
{
	GMSEC_TimeSpec spec;
	Status         result;

	try
	{
		spec = TimeUtil::convertTimeString(timeString);
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return spec;
}


double CALL_TYPE timeUtilConvertTimeString_s(const char* timeString, GMSEC_Status status)
{
	double time = 0;;
	Status result;

	try
	{
		time = TimeUtil::convertTimeString_s(timeString);
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return time;
}


void CALL_TYPE timeUtilFormatTime(GMSEC_TimeSpec time, GMSEC_Time buffer)
{
	TimeUtil::formatTime(time, buffer);
}


void CALL_TYPE timeUtilFormatTimeSubs(GMSEC_TimeSpec time, int subs, GMSEC_Time buffer)
{
	TimeUtil::formatTime(time, subs, buffer);
}


GMSEC_TimeSpec CALL_TYPE timeUtilDiffTime(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
{
	return TimeUtil::diffTime(time1, time0);
}


GMSEC_BOOL CALL_TYPE timeUtilEqualTo(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
{
	return (time1 == time0 ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
}


GMSEC_BOOL CALL_TYPE timeUtilGreaterThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
{
	return (time1 > time0 ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
}


GMSEC_BOOL CALL_TYPE timeUtilLesserThan(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0)
{
	return (time1 < time0 ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
}

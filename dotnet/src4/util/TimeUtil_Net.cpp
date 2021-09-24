/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <util/TimeUtil_Net.h>
#include <util/TimeSpec_Net.h>

#include <GMSEC_Exception_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/util/TimeUtil.h>


using namespace GMSEC::API;
using namespace GMSEC::API::UTIL;
using namespace System;


TimeSpec^ TimeUtil::GetCurrentTime()
{
	GMSEC_TimeSpec ts = gmsec::api::util::TimeUtil::getCurrentTime();

	TimeSpec^ curTime = gcnew TimeSpec();

	curTime->seconds     = ts.seconds;
	curTime->nanoseconds = ts.nanoseconds;

	return curTime;
}


String^ TimeUtil::FormatTime(TimeSpec^ spec)
{
	return TimeUtil::FormatTime(spec, 3);
}


String^ TimeUtil::FormatTime(TimeSpec^ spec, int subseconds)
{
	THROW_EXCEPTION_IF_NULLPTR(spec, StatusClass::OTHER_ERROR, StatusCode::INVALID_CALLBACK, "TimeSpec is null");

	if (subseconds < 0 || subseconds > 9)
	{
		throw gcnew GMSEC_Exception(StatusClass::OTHER_ERROR, StatusCode::OTHER_ERROR_CODE, "Subseconds not within range of [0..9]");
	}

	char* buffer = new char[GMSEC_TIME_BUFSIZE + subseconds];

	GMSEC_TimeSpec ts;

	ts.seconds     = spec->seconds;
	ts.nanoseconds = spec->nanoseconds;

	gmsec::api::util::TimeUtil::formatTime(ts, subseconds, buffer);

	String^ timestr = gcnew String(buffer);

	delete [] buffer;

	return timestr;
}

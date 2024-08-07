/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/util/TimeUtil.h>

#include <gmsec5/internal/StringUtil.h>

#ifdef GMSEC_VC6
#include <gmsec5/util/Mutex.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <sys/time.h>
#endif

#include <ctime>
#include <sys/timeb.h>

#include <sstream>

#include <cmath>  // for pow()


using namespace gmsec::api5::util;


void TimeUtil::millisleep(unsigned int milliseconds)
{
    if (milliseconds < 1)
	{
        milliseconds = 1;
	}

#if defined (WIN32)

    Sleep(milliseconds);

#else

    const int micros_per_second = 1000 * 1000;

    int seconds = 0;
    int micros = milliseconds * 1000;
    if (micros >= micros_per_second)
    {
        seconds = micros / micros_per_second;
        micros %= micros_per_second;
    }

    struct timeval timeout = { seconds, micros };
    int code = select(0, 0, 0, 0, &timeout);
    if (code)
    {
        // TODO : log warning
    }

#endif /* WIN32 */
}


GMSEC_TimeSpec TimeUtil::getCurrentTime()
{
#ifdef WIN32

#ifdef GMSEC_VC6
	struct timeb timebuffer;
	ftime(&timebuffer);
#else
	struct _timeb timebuffer;
	_ftime_s(&timebuffer);
#endif /* GMSEC_VC6 */

	GMSEC_TimeSpec ts = { (long) timebuffer.time, (long) timebuffer.millitm * 1000000 };
	return ts;

#else /* not WIN32 */

	struct timeval tv;
	gettimeofday(&tv, 0);

	GMSEC_TimeSpec ts = { tv.tv_sec, tv.tv_usec * 1000 };
	return ts;

#endif
}


double TimeUtil::getCurrentTime_s(GMSEC_TimeSpec* ts)
{
#ifdef WIN32
	#ifdef GMSEC_VC6

    struct timeb timebuffer;
    ftime(&timebuffer);

	#else

    struct _timeb timebuffer;
    _ftime_s(&timebuffer);

	#endif /* GMSEC_VC6 */

    long   millis  = timebuffer.millitm;
    double seconds = timebuffer.time + static_cast<double>(millis) / 1E3;

    if (ts != NULL)
    {
        ts->seconds     = static_cast<long>(timebuffer.time);
        ts->nanoseconds = static_cast<long>(millis * 1E6);
    }

#else /* not WIN32 */

    struct timeval tv;
    gettimeofday(&tv, 0);

    double seconds = tv.tv_sec + tv.tv_usec / 1E6;

	if (ts != NULL)
	{
		ts->seconds     = tv.tv_sec;
		ts->nanoseconds = tv.tv_usec * 1E3;
	}

#endif

	return seconds;
}


GMSEC_TimeSpec TimeUtil::convertTimeString(const char* timeString)
{
	double theTime = StringUtil::getTimeFromString(timeString);
	long   seconds = static_cast<long>(theTime);   // discard fractional seconds
	long   nanos   = static_cast<long>((theTime - seconds) * 1E9);

	GMSEC_TimeSpec ts = { seconds, nanos };

	return ts;
}


double TimeUtil::convertTimeString_s(const char* timeString)
{
	GMSEC_TimeSpec ts = TimeUtil::convertTimeString(timeString);

    double seconds = static_cast<double>(ts.seconds) + static_cast<double>(ts.nanoseconds) / 1E9;

	return seconds;
}


void TimeUtil::formatTime(const GMSEC_TimeSpec& ts, char* buffer)
{
	const char* timeFormat = "%04d-%03d-%02d:%02d:%02d.%03d";

	formatTimeAux(ts, buffer, timeFormat, 3);
}


void TimeUtil::formatTime(const GMSEC_TimeSpec& ts, int subs, char* buffer)
{
	if (subs == 0)
	{
		const char* timeFormat = "%04d-%03d-%02d:%02d:%02d";
		formatTimeAux(ts, buffer, timeFormat, 0);
	}
	else if (subs == 3 || (subs < 0 || subs > 9))
	{
		formatTime(ts, buffer);
	}
	else
	{
		const char* baseTimeFormat = "%04d-%03d-%02d:%02d:%02d.";

		std::ostringstream timeFormat;
		timeFormat << baseTimeFormat << "%0" << subs << "d";

		formatTimeAux(ts, buffer, timeFormat.str().c_str(), subs);
	}
}


GMSEC_TimeSpec TimeUtil::diffTime(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
{
	GMSEC_TimeSpec delta = {0, 0};

	delta.seconds     = time1.seconds - time0.seconds;
	delta.nanoseconds = time1.nanoseconds - time0.nanoseconds;

	if (delta.nanoseconds < 0)
	{
		delta.seconds     -= 1;
		delta.nanoseconds += 1000000000L;
	}

	return delta;
}


void TimeUtil::formatTimeAux(const GMSEC_TimeSpec& ts, char* buffer, const char* timeFormat, int subs)
{
	long   precision = (1000000000L / (long) std::pow(10.0, (double) subs));
	time_t seconds   = (time_t) ts.seconds;
	long   subsecs   = ts.nanoseconds / precision;

	struct tm newtime;

#ifdef WIN32

#ifdef GMSEC_VC6

	static Mutex mutex;
	AutoMutex hold(mutex);
	newtime = *gmtime_r(&seconds, &newtime);

#else

	gmtime_s(&newtime, &seconds);

#endif // GMSEC_VC6


#else // non-windows

	gmtime_r(&seconds, &newtime);

#endif

	StringUtil::stringFormat(buffer, GMSEC_TIME_BUFSIZE + (subs > 3 ? subs-3 : 0), timeFormat,
	                         newtime.tm_year+1900,
	                         newtime.tm_yday+1,
	                         newtime.tm_hour,
	                         newtime.tm_min,
	                         newtime.tm_sec,
	                         subsecs);
}


bool operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
{
	return time1.seconds == time0.seconds && time1.nanoseconds == time0.nanoseconds;
}


bool operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
{
	if (time1 == time0)
		return false;

	GMSEC_TimeSpec delta = gmsec::api5::util::TimeUtil::diffTime(time1, time0);

	if ((delta.seconds < 0) || (delta.seconds == 0 && delta.nanoseconds < 0))
		return false;

	return true;
}


bool operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0)
{
	if (time1 == time0)
		return false;

	return !(time1 > time0);
}

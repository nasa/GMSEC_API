
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifdef __hpux
#define _REENTRANT
#endif

#ifndef WIN32
#include <sys/time.h>
#endif

// #include <stdio.h>
#include <time.h>
#include <sys/timeb.h>

#include <gmsec/util/timeutil.h>
#include <gmsec/internal/strutil.h>

#ifdef GMSEC_VC6
#include <gmsec/util/Mutex.h>
#endif


/**
 * \file timeutil.cpp
 * \brief Provide time utility functions.
 */


namespace gmsec {
namespace util {


double getTime_s(TimeBuffer *buffer)
{

#ifdef WIN32
#ifdef GMSEC_VC6
	struct timeb timebuffer;
	ftime(&timebuffer);
#else
	struct _timeb timebuffer;
	_ftime_s(&timebuffer);
#endif /* GMSEC_VC6 */

	int millis = timebuffer.millitm;
	double seconds = timebuffer.time + millis / 1000.0;

	if (buffer != NULL)
	{
		buffer->seconds = (double) timebuffer.time;
		buffer->millis = millis;
		buffer->nanos = 1000000 * millis;
	}

#else /* not WIN32 */

	struct timeval tv;
	gettimeofday(&tv, 0);

	double seconds = tv.tv_sec + tv.tv_usec / 1e6;

	if (buffer != NULL)
	{
		buffer->seconds = tv.tv_sec;
		buffer->millis = int(tv.tv_usec / 1000);
		buffer->nanos = 1000 * tv.tv_usec;
	}

#endif

	return seconds;
}



void formatTime_s(double time_s, char *buffer)
{
	time_t t = (time_t) time_s;

	int millis = int(1000 * (time_s - t));

	struct tm newtime;

#ifdef WIN32
#ifdef GMSEC_VC6
	static Mutex mutex;
	AutoMutex hold(mutex);
	newtime = *gmtime_r(&t,&newtime);
#else
	gmtime_s(&newtime, &t);
#endif /* GMSEC_VC6 */
#else
	gmtime_r(&t, &newtime);
#endif

	stringFormat(buffer, TIME_BUFSIZE, "%04d-%03d-%02d:%02d:%02d.%03d",
	         newtime.tm_year+1900,
	         newtime.tm_yday+1,
	         newtime.tm_hour,
	         newtime.tm_min,
	         newtime.tm_sec,
	         millis);
}

double getCharTime_s(const char * timeString)
{
	return getTimeFromString(timeString);
}

} // namespace util
} // namespace gmsec


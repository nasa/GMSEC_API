/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Mutex.cpp
 *
 *  This file contains the Mutex class for threads.
 *
*/

#include <gmsec4/util/Mutex.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdThread.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/SystemUtil.h>

#ifdef WIN32
#include <windows.h>
#endif


using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using std::string;

static const int DEBUG = 0;


static std::string errorInfo(int code)
{
	std::string tmp;
	SystemUtil::getErrorString(code, tmp);

	char raw[256] = {0};
	rawbuf buffer(raw, sizeof(raw));
	std::ostream out(&buffer);

	out << code << ": " << tmp << std::ends;

	tmp = raw;

	return tmp;
}



Mutex::Mutex()
	:
	fDepth(0)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "+ Mutex:" << this;
	}

#ifdef WIN32

	fHandle = CreateMutex(0, 0, NULL);
	if (!fHandle)
	{
		GMSEC_WARNING << "CreateMutex failed: " << errorInfo(GetLastError()).c_str();
	}
	else if (DEBUG)
	{
		GMSEC_DEBUG << "CreateMutex:" << this << " => fHandle=" << fHandle;
	}

#else

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	int code = pthread_mutex_init(&fHandle, &attr);
	pthread_mutexattr_destroy(&attr);
	if (code)
	{
		GMSEC_WARNING << "Mutex:" << this << ": pthread_mutex_init => " << errorInfo(code).c_str();
	}

#endif
}


Mutex::~Mutex()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "~Mutex:" << this << ": thread " << StdThread::getCurrentID() << " depth=" << fDepth;
	}

#ifdef WIN32

	if (fDepth > 1)
	{
		--fDepth;
	}
	else if (fDepth == 1 && fHandle)
	{
		if (!ReleaseMutex(fHandle))
		{
			GMSEC_ERROR << "ReleaseMutex during destructor with depth=" << fDepth << " failed: " << errorInfo(GetLastError()).c_str();
		}

		if (!CloseHandle(fHandle))
		{
			GMSEC_ERROR << "~Mutex:" << this << ": fHandle=" << fHandle << ": CloseHandle failed: " << errorInfo(GetLastError()).c_str();
		}

		fDepth  = 0;
		fHandle = NULL;
	}

#else

	if (fDepth > 0)
	{
		if (fDepth > 1)
		{
			GMSEC_WARNING << "~Mutex:" << this << ": depth=" << fDepth;
		}
		--fDepth;
		int code = pthread_mutex_unlock(&fHandle);
		if (code)
		{
			GMSEC_WARNING << "~Mutex:" << this << ": pthread_mutex_unlock => " << errorInfo(code).c_str();
		}
	}

	int code = pthread_mutex_destroy(&fHandle);
	if (code)
	{
		GMSEC_WARNING << "~Mutex:" << this << ": pthread_mutex_destroy => " << errorInfo(code).c_str();
	}

#endif
}


void Mutex::enterMutex()
{
	if (DEBUG)
	{
		GMSEC_VERBOSE << "Mutex:" << this << ": thread " << StdThread::getCurrentID() << ": entering";
	}

#ifdef WIN32

	int code = WaitForSingleObject(fHandle, INFINITE);
	if(code == WAIT_FAILED)
	{
		GMSEC_ERROR << "enterMutex:WaitForSingleObject failed: " << errorInfo(GetLastError()).c_str();
	}

#else

	int code = pthread_mutex_lock(&fHandle);
	if (code)
	{
		GMSEC_WARNING << "Mutex:" << this << ": pthread_mutex_lock => " << errorInfo(code).c_str();
	}

#endif

	++fDepth;
	if (DEBUG)
	{
		GMSEC_DEBUG << "Mutex:" << this << ": thread " << StdThread::getCurrentID() << " entered depth=" << fDepth;
	}
}


void Mutex::leaveMutex()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Mutex:" << this << ": thread " << StdThread::getCurrentID() << ": leaving depth=" << fDepth;
	}

	if (fDepth < 1)
		return;

	--fDepth;

#ifdef WIN32

	if(!ReleaseMutex(fHandle))
	{
		GMSEC_ERROR << "leaveMutex:ReleaseMutex failed: " << errorInfo(GetLastError()).c_str();
	}

#else

	int code = pthread_mutex_unlock(&fHandle);
	if (code)
	{
		GMSEC_WARNING << "Mutex:" << this << ": pthread_mutex_unlock => " << errorInfo(code).c_str();
	}

#endif

	if (DEBUG)
	{
		GMSEC_DEBUG << "Mutex:" << this << ": thread " << StdThread::getCurrentID() << " left depth=" << fDepth;
	}
}


void Mutex::decrDepth()
{
	if (fDepth > 0)
	{
		--fDepth;
	}
	else
	{
		GMSEC_WARNING << "Mutex:" << this << ": thread " << StdThread::getCurrentID() << " decrDepth depth=" << fDepth;
	}
}


void Mutex::incrDepth()
{
	++fDepth;
}


AutoMutex::AutoMutex(Mutex &m, bool acquire)
	: mutex(m),
	owned(false)
{
	if (acquire)
	{
		enter();
	}
}


AutoMutex::~AutoMutex()
{
	leave();
}


void AutoMutex::enter()
{
	if (!owned)
	{
		mutex.enterMutex();
	}
	owned = true;
}


void AutoMutex::leave()
{
	if (owned)
	{
		mutex.leaveMutex();
	}
	owned = false;
}

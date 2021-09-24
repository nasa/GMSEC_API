
/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec/util/Mutex.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <gmsec/util/Thread.h>
#include <gmsec/util/sysutil.h>
#include <gmsec/internal/rawbuf.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using std::string;

static const int DEBUG = 0;


static string errorInfo(int code)
{
	string tmp;
	getErrorString(code, tmp);

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
		LOG_DEBUG << "+ Mutex:" << this;
	}

#ifdef WIN32

	fHandle = CreateMutex(0, 0, NULL);
	if (!fHandle)
	{
		LOG_WARNING << "CreateMutex failed: " << errorInfo(GetLastError());
	}
	else if (DEBUG)
	{
		LOG_DEBUG << "CreateMutex:" << this << " => fHandle=" << fHandle;
	}

#else

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	int code = pthread_mutex_init(&fHandle, &attr);
	pthread_mutexattr_destroy(&attr);
	if (code)
	{
		LOG_WARNING << "Mutex:" << this << ": pthread_mutex_init => " << errorInfo(code);
	}

#endif
}


Mutex::~Mutex()
{
	if (DEBUG)
	{
		LOG_DEBUG << "~Mutex:" << this << ": thread " << Thread::getCurrentID() << " depth=" << fDepth;
	}

#ifdef WIN32

	if (fDepth > 1)
	{
		fDepth--;
	}
	if (fDepth > 0)
	{
		if (!ReleaseMutex(fHandle))
		{
			LOG_ERROR << "ReleaseMutex during destructor with depth=" << fDepth << " failed: " << errorInfo(GetLastError());
		}
	}

	if (!CloseHandle(fHandle))
	{
		LOG_ERROR << "~Mutex:" << this << ": fHandle=" << fHandle << ": CloseHandle failed: " << errorInfo(GetLastError());
	}

#else

	if (fDepth > 0)
	{
		if (fDepth > 1)
		{
			LOG_WARNING << "~Mutex:" << this << ": depth=" << fDepth;
		}
		--fDepth;
		int code = pthread_mutex_unlock(&fHandle);
		if (code)
		{
			LOG_WARNING << "~Mutex:" << this << ": pthread_mutex_unlock => " << errorInfo(code);
		}
	}

	int code = pthread_mutex_destroy(&fHandle);
	if (code)
	{
		LOG_WARNING << "~Mutex:" << this << ": pthread_mutex_destroy => " << errorInfo(code);
	}

#endif
}


void Mutex::enterMutex()
{
	if (DEBUG)
	{
		LOG_VERBOSE << "Mutex:" << this << ": thread " << Thread::getCurrentID() << ": entering";
	}

#ifdef WIN32

	int code = WaitForSingleObject(fHandle, INFINITE);
	if(code == WAIT_FAILED)
	{
		LOG_ERROR << "enterMutex:WaitForSingleObject failed: " << errorInfo(GetLastError());
	}

#else

	int code = pthread_mutex_lock(&fHandle);
	if (code)
	{
		LOG_WARNING << "Mutex:" << this << ": pthread_mutex_lock => " << errorInfo(code);
	}

#endif

	++fDepth;
	if (DEBUG)
	{
		LOG_DEBUG << "Mutex:" << this << ": thread " << Thread::getCurrentID() << " entered depth=" << fDepth;
	}
}


void Mutex::leaveMutex()
{
	if (DEBUG)
	{
		LOG_DEBUG << "Mutex:" << this << ": thread " << Thread::getCurrentID() << ": leaving depth=" << fDepth;
	}

	if (fDepth < 1)
		return;

	--fDepth;

#ifdef WIN32

	if(!ReleaseMutex(fHandle))
	{
		LOG_ERROR << "leaveMutex:ReleaseMutex failed: " << errorInfo(GetLastError());
	}

#else

	int code = pthread_mutex_unlock(&fHandle);
	if (code)
	{
		LOG_WARNING << "Mutex:" << this << ": pthread_mutex_unlock => " << errorInfo(code);
	}

#endif

	if (DEBUG)
	{
		LOG_DEBUG << "Mutex:" << this << ": thread " << Thread::getCurrentID() << " left depth=" << fDepth;
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
		LOG_WARNING << "Mutex:" << this << ": thread " << Thread::getCurrentID() << " decrDepth depth=" << fDepth;
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
		enter();
}


AutoMutex::~AutoMutex()
{
	leave();
}


void AutoMutex::enter()
{
	if (!owned)
		mutex.enterMutex();
	owned = true;
}


void AutoMutex::leave()
{
	if (owned)
		mutex.leaveMutex();
	owned = false;
}


/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file InternalMutex.cpp
 *
 *  This file contains the Mutex class for threads.
 */

#include <gmsec4/internal/util/InternalMutex.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/StdThread.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <string>


using namespace gmsec::api::internal;
using namespace gmsec::api::util;


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


InternalMutex::InternalMutex()
	: m_depth(0)
{
#ifdef WIN32

	m_handle = CreateMutex(0, 0, NULL);

	if (!m_handle)
	{
		int code = GetLastError();
		GMSEC_ERROR << "Failed to create Mutex; reason => " << errorInfo(code).c_str();
		throw Exception(OTHER_ERROR, INITIALIZATION_ERROR, code, errorInfo(code).c_str());
	}

#else

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	int code = pthread_mutex_init(&m_handle, &attr);
	pthread_mutexattr_destroy(&attr);
	if (code)
	{
		GMSEC_ERROR << "Failed to create Mutex; reason => " << errorInfo(code).c_str();
		throw Exception(OTHER_ERROR, INITIALIZATION_ERROR, code, errorInfo(code).c_str());
	}

#endif
}


InternalMutex::~InternalMutex()
{
#ifdef WIN32

	if (m_depth > 1)
	{
		--m_depth;
	}
	else if (m_depth == 1 && m_handle)
	{
		if (!ReleaseMutex(m_handle))
		{
			GMSEC_ERROR << "Failed to release mutex [" << this << "]; error => " << errorInfo(GetLastError()).c_str();
		}

		if (!CloseHandle(m_handle))
		{
			GMSEC_ERROR << "Failed to destroy mutex [" << this << "]; error => " << errorInfo(GetLastError()).c_str();
		}

		m_depth  = 0;
		m_handle = NULL;
	}

#else

	if (m_depth > 0)
	{
		if (m_depth > 1)
		{
			GMSEC_WARNING << "Destroying mutex [" << this << "] that is still locked [depth = " << m_depth << "]; will first try to unlock";
		}

		--m_depth;

		int code = pthread_mutex_unlock(&m_handle);

		if (code)
		{
			GMSEC_ERROR << "Failed to unlock mutex [" << this << "]; error => " << errorInfo(code).c_str();
		}
	}

	int code = pthread_mutex_destroy(&m_handle);

	if (code)
	{
		GMSEC_ERROR << "Failed to destroy mutex [" << this << "]; error => " << errorInfo(code).c_str();
	}

#endif
}


void InternalMutex::enterMutex()
{
#ifdef WIN32

	int code = WaitForSingleObject(m_handle, INFINITE);
	if (code == WAIT_FAILED)
	{
		throw Exception(OTHER_ERROR, OTHER_ERROR_CODE, code, errorInfo(GetLastError()).c_str());
	}

#else

	int code = pthread_mutex_lock(&m_handle);
	if (code)
	{
		throw Exception(OTHER_ERROR, OTHER_ERROR_CODE, code, errorInfo(code).c_str());
	}

#endif

	++m_depth;
}


void InternalMutex::leaveMutex()
{
	if (m_depth < 1)
		return;

	--m_depth;

#ifdef WIN32

	if(!ReleaseMutex(m_handle))
	{
		GMSEC_ERROR << "Unable to release mutex [" << this << "]: " << errorInfo(GetLastError()).c_str();
	}

#else

	int code = pthread_mutex_unlock(&m_handle);
	if (code)
	{
		GMSEC_ERROR << "Unable to unlock mutex [" << this << "]: " << errorInfo(code).c_str();
	}

#endif
}


#ifdef WIN32

HANDLE& InternalMutex::getHandle()
{
	return m_handle;
}

#else

pthread_mutex_t& InternalMutex::getHandle()
{
	return m_handle;
}

#endif


int InternalMutex::getDepth() const
{
	return m_depth;
}


void InternalMutex::incrDepth()
{
	++m_depth;
}


void InternalMutex::decrDepth()
{
	if (m_depth > 0)
	{
		--m_depth;
	}
}



InternalAutoMutex::InternalAutoMutex(Mutex &m, bool acquire)
	: mutex(m),
	  owned(false)
{
	if (acquire)
	{
		enter();
	}
}


InternalAutoMutex::~InternalAutoMutex()
{
	leave();
}


void InternalAutoMutex::enter()
{
	if (!owned)
	{
		mutex.enterMutex();
	}
	owned = true;
}


void InternalAutoMutex::leave()
{
	if (owned)
	{
		mutex.leaveMutex();
	}
	owned = false;
}

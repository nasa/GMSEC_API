
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file Thread.cpp
 *
 *  This file contains the Thread class.
*/

#include <gmsec/util/Thread.h>
#include <gmsec/util/sysutil.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/rawbuf.h>

#ifdef WIN32
#include <windows.h>
#endif

using namespace gmsec::util;


static const int IS_DEBUG = 0;



static std::string errorInfo(int code, const char * action = 0)
{
	std::string tmp;
	getErrorString(code, tmp);

	char raw[256] = {0};
	rawbuf buffer(raw, sizeof(raw));
	std::ostream out(&buffer);

	out << code << ": " << tmp << std::ends;

	tmp = raw;

	if (action)
		std::cerr << "Thread.cpp:" << action << ' ' << tmp << '\n';

	return tmp;
}


static bool isBigEndian()
{
	long x = 1;
	char * p = (char *) &x;
	return p[0] == 0;
}


static Thread::id_t handleToID(const Thread::handle_t in)
{
	Thread::id_t out = 0;

	size_t count = sizeof(out);

	const GMSEC_U8 * from = (GMSEC_U8 *) &in;
	GMSEC_U8 * to = (GMSEC_U8 *) &out;

	if (sizeof(in) > sizeof(out))
	{
		// will copy the start of the pthread_t
		count = sizeof(in);
	}
	else if (sizeof(in) < sizeof(out))
	{
		// place in low order bytes
		if (isBigEndian())
			to += sizeof(out) - sizeof(in);
	}

	copyBytes(to, from, count);

	return out;
}


#if 0

/* If we really want to support an exact representation */

#include <gmsec/util/Buffer.h>

static std::string handleToString(const Thread::handle_t in)
{
	std::string out;

	const GMSEC_U8 * raw = (GMSEC_U8 *) &in;
	DataBuffer buffer(raw, sizeof(in), false);
	convertBlobToBase16(buffer, out);

	return out;
}

#endif


void *Thread::process0(void *arg)
{
	Thread *object = reinterpret_cast<Thread*>(arg);
	if (object)
	{
		object->process();
	}
#ifndef WIN32
	pthread_exit(NULL);
#endif
	return 0;
}



void Thread::process()
{
	fID = getCurrentID();
	LOG_VERBOSE << "Thread.process:" << this << ":id=" << fID << ':' << fName;

	run();

	if (fLifeline.get())
	{
		if (IS_DEBUG)
		{
			LOG_DEBUG << "Thread::" << this << ':' << fName << ":process: releasing lifeline";
		}
		fLifeline.reset();
	}
}


Thread::Thread(bool selfReferential)
	:
	fHandle(0),
	fID(0),
	fJoined(false),
	fDetached(false),
	fLifeline(NULL)
{
	if (IS_DEBUG)
	{
		LOG_DEBUG << "Thread::" << this << ": constructor";
	}

	if (selfReferential)
		fLifeline.reset(this);
}

Thread::~Thread()
{
	if (IS_DEBUG)
	{
		LOG_DEBUG << "Thread:" << this << ":fID=" << fID << ':' << fName << ": destructor";
	}

#ifdef WIN32
	if (fHandle != 0)
	{
		//ExitThread(fID);
		CloseHandle(fHandle);
	}
#else /* WIN32 */
	if (fHandle != 0)
	{
		if (!fJoined && !fDetached)
		{
			int code = pthread_detach(fHandle);
			if (code)
			{
				LOG_ERROR << "~Thread:" << this << ':' << fName << ": pthread_detach => " << errorInfo(code, "~Thread/pthread_detach");
			}
#if GMSEC_THREAD_DESTRUCTOR_CANCEL
			code = pthread_cancel(fHandle);
			if (code)
			{
				LOG_ERROR << "~Thread:" << this << ':' << fName << ": pthread_cancel => " << errorInfo(code, "~Thread/pthread_cancel");
			}
#endif
		}
	}
#endif /* WIN32 */
}


int Thread::start()
{
#ifdef WIN32
	if (fHandle != 0)
	{
		//ExitThread(fID);
		CloseHandle(fHandle);
		fHandle = 0;
	}
#else
	if (fHandle != 0)
	{
		LOG_DEBUG << "Thread::start: fHandle=" << handleToID(fHandle) << ", cancelling\n";
		pthread_detach(fHandle);
		pthread_cancel(fHandle);
		fHandle = 0;
	}
#endif

#ifdef WIN32

	DWORD tmpID = 0;
	fHandle = CreateThread(
	                    THREAD_SUSPEND_RESUME & THREAD_TERMINATE ,		// pointer to thread security attributes
	                    0,												// initial thread stack size, in bytes
	                    (LPTHREAD_START_ROUTINE) &Thread::process0,	// pointer to thread function
	                    this,											// argument for new thread
	                    0, // CREATE_SUSPENDED ,						// creation flags
	                    &tmpID										// pointer to returned thread identifier
	                );

	if (tmpID)
		return GMSEC_TRUE;
	else
		return GMSEC_FALSE;

#else

	int err;

	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	if (err)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_attr_init => " << errorInfo(err, "start/pthread_attr_init");
		return 0;
	}

#ifndef __hpux
	err = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	if (err)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_attr_setinheritsched => " << errorInfo(err, "start/pthread_setinheritsched");
	}
#endif

	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (err)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_attr_setdetachstate => " << errorInfo(err, "start/pthread_attr_setdetachstate");
	}

	err = pthread_create(&fHandle, &attr, &Thread::process0, this);
	if (err)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_create => " << errorInfo(err, "start/pthread_create");
	}

	// free the attr (no longer needed)
	pthread_attr_destroy(&attr);

	if (err == 0)
		return GMSEC_TRUE;
	else
		return GMSEC_FALSE;

#endif
}


bool Thread::isCurrentContext()
{
#ifdef WIN32

	DWORD tmp = GetCurrentThreadId();

	return fID == tmp;

#else

	pthread_t tmp = pthread_self();

	return pthread_equal(fHandle, tmp);

#endif
}


Thread::id_t Thread::getID()
{
	return fID;
}


Thread::id_t Thread::getCurrentID()
{
	id_t id;

#ifdef WIN32

	id = (id_t) GetCurrentThreadId();

#else

	handle_t tmp = pthread_self();
	id = handleToID(tmp);

#endif

	return id;
}


int Thread::join()
{
	if (fJoined || fDetached)
	{
		LOG_VERBOSE << "Thread::" << this << ':' << fName << ": join: already joined/detached";
		//Change this to return successfully (return code 1) since if the thread is joined
		//we're at the state we want to be and if we're detached the target thread has already
		//completed and the thread that called join should continue. -MEH
		return 1;
	}

#ifdef WIN32

	WaitForSingleObject(fHandle, INFINITE);

#else

	void *nothing = 0;
	int code = pthread_join(fHandle, &nothing);
	if (code)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_join => " << errorInfo(code, "pthread_join");
	}

#endif /* WIN32 */

	fJoined = true;

	return 1;
}


int Thread::detach()
{
	if (fDetached || fJoined)
	{
		LOG_VERBOSE << "Thread::" << this << ':' << fName << ": detach: already joined/detached";
		return 0;
	}

	fDetached = true;

#ifdef WIN32
	// nothing
#else
	int code = pthread_detach(fHandle);
	if (code)
	{
		LOG_ERROR << "Thread:" << this << ':' << fName << ": pthread_detach => " << errorInfo(code, "pthread_detach");
	}
#endif

	return 1;
}


void Thread::attach(gshared_ptr<Thread> &mine)
{
	if (fLifeline.get())
		mine = fLifeline;
	else
	{
		LOG_INFO << "Thread::" << this << ':' << fName << ": attach: not attachable";
	}
}


void Thread::yield()
{
#ifdef WIN32
	Sleep(0);
#else
	sched_yield();
#endif
}


void Thread::setName(const char * name)
{
	fName = name;
}


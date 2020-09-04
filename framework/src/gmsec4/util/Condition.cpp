/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Condition.cpp
 *
 *  @brief This file contains a class for cross platform condition variable support
 */

#ifndef WIN32
#include <errno.h>
#endif

#ifdef GMSEC_VC6
#define _WIN32_WINNT 0x0400
#endif

#include <gmsec4/util/Condition.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>

#include <gmsec4/internal/SystemUtil.h>
#include <gmsec4/internal/Rawbuf.h>



static const int DEBUG = 0;


static std::string errorInfo(int code)
{
	std::string tmp;
	gmsec::api::util::SystemUtil::getErrorString(code, tmp);

	char raw[256] = {0};
	gmsec::api::util::rawbuf buffer(raw, sizeof(raw));
	std::ostream out(&buffer);

	out << code << ": " << tmp << std::ends;

	tmp = raw;

	return tmp;
}


namespace gmsec {
namespace api {
namespace util {

Condition::Condition(Mutex& m)
	: status(INVALID),
	  ownsMutex(false),
	  mutex(&m)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition:" << this << ": external Mutex " << &m;
	}

#if defined (WIN32)

	waiters_count_ = 0;
	was_broadcast_ = 0;
	sema_ = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
	InitializeCriticalSection(&waiters_count_lock_);
	waiters_done_ = CreateEvent(NULL, FALSE, FALSE, NULL);  // unnamed

#else

	int code = pthread_cond_init(&cv, NULL);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_init => " << errorInfo(code).c_str();
	}

#endif
}


Condition::Condition()
	: status(INVALID),
	  ownsMutex(true),
	  mutex(new Mutex())
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition[" << this << "] owned Mutex " << mutex;
	}

#if defined (WIN32)

	waiters_count_ = 0;
	was_broadcast_ = 0;
	InitializeCriticalSection(&waiters_count_lock_);
	sema_ = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);

	waiters_done_ = CreateEvent(NULL, FALSE, FALSE, NULL);  // unnamed

#else

	int code = pthread_cond_init(&cv, NULL);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_init => " << errorInfo(code).c_str();
	}

#endif
}


Condition::~Condition()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "~Condition:" << this << ':';
	}

	if (ownsMutex)
	{
		if (DEBUG)
		{
			GMSEC_DEBUG << "~Condition:" << this << ": destroying owned mutex " << mutex;
		}
		delete mutex;
	}
	else if (mutex->fDepth > 0)
	{
		GMSEC_WARNING << "~Condition:" << this << ": unowned Mutex " << mutex << " depth=" << mutex->fDepth;
	}

#if defined (WIN32)

	CloseHandle(sema_);
	CloseHandle(waiters_done_);
	DeleteCriticalSection(&waiters_count_lock_);

#else

	int code = pthread_cond_destroy(&cv);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_destroy => " << errorInfo(code).c_str();
	}

#endif
}


Mutex &Condition::getMutex()
{
	return *mutex;
}


int Condition::wait()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition:" << this << ": wait";
	}

	if (mutex->fDepth < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " wait but mutex depth=" << mutex->fDepth;
	}

#if defined (WIN32)

	// Avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);
	waiters_count_++;
	LeaveCriticalSection(&waiters_count_lock_);

	// This call atomically releases the mutex and waits on the
	// semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
	// are called by another thread.
	mutex->decrDepth();
	SignalObjectAndWait(mutex->fHandle, sema_, INFINITE, FALSE);

	// Reacquire lock to avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);

	// We're no longer waiting...
	waiters_count_--;

	// Check to see if we're the last waiter after <pthread_cond_broadcast>.
	int last_waiter = was_broadcast_ && waiters_count_ == 0;

	LeaveCriticalSection(&waiters_count_lock_);

	// If we're the last waiter thread during this particular broadcast
	// then let all the other threads proceed.
	if (last_waiter)
	{
		// This call atomically signals the <waiters_done_> event and waits until
		// it can acquire the <external_mutex>.  This is required to ensure fairness.
		SignalObjectAndWait(waiters_done_, mutex->fHandle, INFINITE, FALSE);
		mutex->incrDepth();
	}
	else
	{
		// Always regain the external mutex since that's the guarantee we
		// give to our callers.
		mutex->enterMutex();
	}

#else

	int code = pthread_cond_wait(&cv, &mutex->fHandle);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_wait => " << errorInfo(code).c_str();
	}

#endif

	return status;
}


int Condition::wait(long timeout)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition[" << this << "] wait(" << timeout << ')';
	}

	if (mutex->fDepth < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " wait(" << timeout << " but mutex depth=" << mutex->fDepth;
	}

	if (timeout < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " wait(" << timeout << " <= 0)";
		status = TIMEOUT;
		return status;
	}

#if defined (WIN32)

	// Avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);
	waiters_count_++;
	LeaveCriticalSection(&waiters_count_lock_);

	// This call atomically releases the mutex and waits on the
	// semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
	// are called by another thread.
	mutex->decrDepth();
	DWORD dwWaitResult = SignalObjectAndWait(mutex->fHandle, sema_, timeout, FALSE);
	if (dwWaitResult != WAIT_OBJECT_0)
	{
		status = TIMEOUT;
	}

	// Reacquire lock to avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);

	// We're no longer waiting...
	waiters_count_--;

	// Check to see if we're the last waiter after <pthread_cond_broadcast>.
	int last_waiter = was_broadcast_ && waiters_count_ == 0;

	LeaveCriticalSection(&waiters_count_lock_);

	// If we're the last waiter thread during this particular broadcast
	// then let all the other threads proceed.
	if (last_waiter)
	{
		// This call atomically signals the <waiters_done_> event and waits until
		// it can acquire the <external_mutex>.  This is required to ensure fairness.
		SignalObjectAndWait(waiters_done_, mutex->fHandle, INFINITE, FALSE);
		mutex->incrDepth();
	}
	else
	{
		// Always regain the external mutex since that's the guarantee we
		// give to our callers.
		mutex->enterMutex();
	}

#else   // not WIN32

	GMSEC_TimeSpec ts = TimeUtil::getCurrentTime();

	double tmp_ns = double(ts.nanoseconds) + (timeout * 1E6);

	if (tmp_ns < 1E9)
	{
		ts.nanoseconds = (long) tmp_ns;
	}
	else
	{
		long delta_s = long(tmp_ns / 1E9);

		tmp_ns -= (delta_s * 1E9);

		ts.seconds     += delta_s;
		ts.nanoseconds  = long(tmp_ns);
	}

	struct timespec abstime = { (time_t) ts.seconds, ts.nanoseconds };

	int code = pthread_cond_timedwait(&cv, &mutex->fHandle, &abstime);

//DMW BEGIN
	if (code == 0 && status == TIMEOUT)
	{
		GMSEC_WARNING << "DMW -- good status from pthread_cond_timedwait, yet status is TIMEOUT";
	}
//DMW END
	if (code == ETIMEDOUT)
	{
		status = TIMEOUT;
	}
	else if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_timedwait => " << errorInfo(code).c_str();
	}

#endif

	return status;
}


int Condition::waitUntil(const GMSEC_TimeSpec& until)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition[" << this << "] waituntil(s=" << until.seconds << ", ns=" << until.nanoseconds << ')';
	}

	if (mutex->fDepth < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " waituntil but mutex depth=" << mutex->fDepth;
	}

#if defined (WIN32)

	GMSEC_TimeSpec now;
	gmsec::api::util::TimeUtil::getCurrentTime_s(&now);

	long delta_s = 0;
	if (until.seconds > now.seconds)
	{
		delta_s = until.seconds - now.seconds;
	}

	long delta_ns = until.nanoseconds - now.nanoseconds;
	if (delta_ns < 0)
	{
		delta_ns = 0;
	}

	long timeout_ms = (delta_s * 1000) + (delta_ns / 1000000);

	return wait(timeout_ms);

#else   // WIN32

	struct timespec abstime =
	{
		(time_t) until.seconds,
		until.nanoseconds
	};
	int code = pthread_cond_timedwait(&cv, &mutex->fHandle, &abstime);
//DMW BEGIN
	if (code == 0 && status == TIMEOUT)
	{
		GMSEC_WARNING << "DMW -- good status from pthread_cond_timedwait, yet status is TIMEOUT";
	}
//DMW END
	if (code == ETIMEDOUT)
		status = TIMEOUT;
	else if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_timedwait => " << errorInfo(code).c_str();
	}

#endif

	return status;
}


void Condition::signal(int result)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition[" << this << "] Signal(" << result << ')';
	}

	if (mutex->fDepth < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " Signal(" << result << ") but mutex depth=" << mutex->fDepth;
	}

	status = result;

#if defined (WIN32)

	EnterCriticalSection(&waiters_count_lock_);
	int have_waiters = waiters_count_ > 0;
	LeaveCriticalSection(&waiters_count_lock_);

	// If there aren't any waiters, then this is a no-op.
	if (have_waiters)
		ReleaseSemaphore(sema_, 1, 0);

#else

	int code = pthread_cond_signal(&cv);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_signal => " << errorInfo(code).c_str();
	}

#endif
}


void Condition::broadcast(int result)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "Condition:" << this << ": Broadcast(" << result << ')';
	}

	if (mutex->fDepth < 1)
	{
		GMSEC_WARNING << "Condition:" << this << " Broadcast(" << result << ") but mutex depth=" << mutex->fDepth;
	}

	status = result;

#if defined (WIN32)

	// This is needed to ensure that <waiters_count_> and <was_broadcast_> are
	// consistent relative to each other.
	EnterCriticalSection(&waiters_count_lock_);
	int have_waiters = 0;

	if (waiters_count_ > 0)
	{
		// We are broadcasting, even if there is just one waiter...
		// Record that we are broadcasting, which helps optimize
		// <pthread_cond_wait> for the non-broadcast case.
		was_broadcast_ = 1;
		have_waiters = 1;
	}

	if (have_waiters)
	{
		// Wake up all the waiters atomically.
		ReleaseSemaphore(sema_, waiters_count_, 0);

		LeaveCriticalSection(&waiters_count_lock_);

		// Wait for all the awakened threads to acquire the counting
		// semaphore.
		WaitForSingleObject(waiters_done_, INFINITE);
		// This assignment is okay, even without the <waiters_count_lock_> held
		// because no other waiter threads can wake up to access it.
		was_broadcast_ = 0;
	}
	else
		LeaveCriticalSection(&waiters_count_lock_);

#else

	int code = pthread_cond_broadcast(&cv);
	if (code)
	{
		GMSEC_WARNING << "Condition:" << this << ": pthread_cond_broadcast => " << errorInfo(code).c_str();
	}

#endif
}

} // namespace util
} // namespace api
} // namespace gmsec

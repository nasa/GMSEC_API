/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file InternalCondition.cpp
 *
 *  @brief This file contains a class for cross platform condition variable support
 */

#include <gmsec4/internal/util/InternalCondition.h>
#include <gmsec4/internal/util/MutexBuddy.h>

#include <gmsec4/internal/SystemUtil.h>
#include <gmsec4/internal/Rawbuf.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Condition.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>

#ifndef WIN32
#include <errno.h>
#endif


using namespace gmsec::api::internal;
using namespace gmsec::api::util;


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


InternalCondition::InternalCondition(Mutex& m)
	: status(Condition::INVALID),
	  ownsMutex(false),
	  mutex(&m)
{
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
		GMSEC_ERROR << "Failed to create Condition; reason => " << errorInfo(code).c_str();
		throw Exception(OTHER_ERROR, INITIALIZATION_ERROR, code, errorInfo(code).c_str());
	}

#endif
}


InternalCondition::InternalCondition()
	: status(Condition::INVALID),
	  ownsMutex(true),
	  mutex(new Mutex())
{
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
		GMSEC_ERROR << "Failed to create Condition; reason => " << errorInfo(code).c_str();
		throw Exception(OTHER_ERROR, INITIALIZATION_ERROR, code, errorInfo(code).c_str());
	}

#endif
}


InternalCondition::~InternalCondition()
{
	if (ownsMutex)
	{
		delete mutex;
	}
	else if (MutexBuddy::getInternal(*mutex).getDepth() > 0)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex in use (i.e. is locked) [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
	}

#if defined (WIN32)

	CloseHandle(sema_);
	CloseHandle(waiters_done_);
	DeleteCriticalSection(&waiters_count_lock_);

#else

	int code = pthread_cond_destroy(&cv);
	if (code)
	{
		GMSEC_WARNING << "Condition [" << this << "] could not be destroyed; reason => " << errorInfo(code).c_str();
	}

#endif
}


Mutex& InternalCondition::getMutex()
{
	return *mutex;
}


int InternalCondition::wait()
{
	if (MutexBuddy::getInternal(*mutex).getDepth() < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex is not locked [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
	}

#if defined (WIN32)

	// Avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);
	waiters_count_++;
	LeaveCriticalSection(&waiters_count_lock_);

	// This call atomically releases the mutex and waits on the
	// semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
	// are called by another thread.
	MutexBuddy::getInternal(*mutex).decrDepth();
	SignalObjectAndWait(MutexBuddy::getInternal(*mutex).getHandle(), sema_, INFINITE, FALSE);

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
		SignalObjectAndWait(waiters_done_, MutexBuddy::getInternal(*mutex).getHandle(), INFINITE, FALSE);
		MutexBuddy::getInternal(*mutex).incrDepth();
	}
	else
	{
		// Always regain the external mutex since that's the guarantee we
		// give to our callers.
		mutex->enterMutex();
	}

#else

	int code = pthread_cond_wait(&cv, &MutexBuddy::getInternal(*mutex).getHandle());

	if (code != 0)
	{
		GMSEC_ERROR << "Failed to wait on Condition [" << this << "]; reason => " << errorInfo(code).c_str();
		return Condition::INVALID;
	}

#endif

	// Return status set by signaller/broadcaster.
	return getStatus();
}


int InternalCondition::wait(long timeout)
{
	if (MutexBuddy::getInternal(*mutex).getDepth() < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex is not locked [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
	}

	if (timeout < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] wait timeout cannot be less than 1 ms";
		return Condition::TIMEOUT;
	}

#if defined (WIN32)

	// Avoid race conditions.
	EnterCriticalSection(&waiters_count_lock_);
	waiters_count_++;
	LeaveCriticalSection(&waiters_count_lock_);

	// This call atomically releases the mutex and waits on the
	// semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
	// are called by another thread.
	MutexBuddy::getInternal(*mutex).decrDepth();
	DWORD dwWaitResult = SignalObjectAndWait(MutexBuddy::getInternal(*mutex).getHandle(), sema_, timeout, FALSE);
	if (dwWaitResult != WAIT_OBJECT_0)
	{
		setStatus(Condition::TIMEOUT);
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
		SignalObjectAndWait(waiters_done_, MutexBuddy::getInternal(*mutex).getHandle(), INFINITE, FALSE);
		MutexBuddy::getInternal(*mutex).incrDepth();
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

	int code = pthread_cond_timedwait(&cv, &MutexBuddy::getInternal(*mutex).getHandle(), &abstime);

	if (code == ETIMEDOUT)
	{
		return Condition::TIMEOUT;
	}
	else if (code != 0)
	{
		GMSEC_WARNING << "Condition [" << this << "] timed wait failed; reason => " << errorInfo(code).c_str();
		return Condition::INVALID;
	}

#endif

	// Return status set by signaler/broadcaster.
	return getStatus();
}


int InternalCondition::waitUntil(const GMSEC_TimeSpec& until)
{
	if (MutexBuddy::getInternal(*mutex).getDepth() < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex is not locked [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
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

	int code = pthread_cond_timedwait(&cv, &MutexBuddy::getInternal(*mutex).getHandle(), &abstime);

	if (code == ETIMEDOUT)
	{
		return Condition::TIMEOUT;
	}
	else if (code != 0)
	{
		GMSEC_WARNING << "Condition [" << this << "] timed wait failed; reason => " << errorInfo(code).c_str();
		return Condition::INVALID;
	}

#endif

	// Return status set by signaller/broadcaster.
	return getStatus();
}


void InternalCondition::signal(int result)
{
	setStatus(result);

	if (MutexBuddy::getInternal(*mutex).getDepth() < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex is not locked [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
	}

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
		GMSEC_WARNING << "Condition [" << this << "] failed to deliver signal; reason => " << errorInfo(code).c_str();
	}

#endif
}


void InternalCondition::broadcast(int result)
{
	setStatus(result);

	if (MutexBuddy::getInternal(*mutex).getDepth() < 1)
	{
		GMSEC_WARNING << "Condition [" << this << "] mutex is not locked [depth = " << MutexBuddy::getInternal(*mutex).getDepth() << "]";
	}

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
		GMSEC_WARNING << "Condition [" << this << "] failed to broadcast signal; reason => " << errorInfo(code).c_str();
	}

#endif
}

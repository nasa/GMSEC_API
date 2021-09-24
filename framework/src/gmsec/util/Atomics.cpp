/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/**
 *  @file Atomics.cpp
 *
*/


#include <gmsec/util/Atomics.h>
#include <gmsec/internal/InternalAtomics.h>

namespace gmsec
{
namespace util
{

AtomicInteger::AtomicInteger(int initialValue)
	:
	value(initialValue)
{
}

AtomicInteger::~AtomicInteger()
{

}

int AtomicInteger::addAndGet(int addition)
{
	AutoMutex hold(mutex);
	value += addition;
	return value;
}

bool AtomicInteger::compareAndSet(int expect, int update)
{
	AutoMutex hold(mutex);
	if(value == expect){
		value = update;
		return true;
	}else{
		return false;
	}	
}

int AtomicInteger::decrementAndGet()
{
	AutoMutex hold(mutex);
	return --value;
}

double AtomicInteger::doubleValue()
{
	AutoMutex hold(mutex);
	return (double) value;
}	

float AtomicInteger::floatValue()
{
	AutoMutex hold(mutex);
	return (float) value;
}	

int AtomicInteger::get()
{
	AutoMutex hold(mutex);
	return value;
}

int AtomicInteger::getAndAdd(int delta)
{
	AutoMutex hold(mutex);
	int old = value;
	value += delta;
	return old;
}

int AtomicInteger::getAndDecrement()
{
	AutoMutex hold(mutex);
	int old = value;
	value--;
	return old;
}	

int AtomicInteger::getAndIncrement()
{
	AutoMutex hold(mutex);
	int old = value;
	value++;
	return old;
}

int AtomicInteger::getAndSet(int newValue)
{
	AutoMutex hold(mutex);
	int old = value;
	value = newValue;
	return old;
}

int AtomicInteger::incrementAndGet()
{
	AutoMutex hold(mutex);
	return ++value;
}

long AtomicInteger::longValue()
{
	AutoMutex hold(mutex);
	return (long) value;
}

void AtomicInteger::set(int newValue)
{
	AutoMutex hold(mutex);
	value = newValue;
}

AtomicBoolean::AtomicBoolean(bool initialValue)
	:
	value(initialValue)
{
}

AtomicBoolean::~AtomicBoolean()
{

}

bool AtomicBoolean::compareAndSet(bool expect, bool update)
{
	AutoMutex hold(mutex);
	if(value == expect){
		value = update;
		return true;
	}else{
		return false;
	}
}

bool AtomicBoolean::get()
{
	AutoMutex hold(mutex);
	return value;
}

bool AtomicBoolean::getAndSet(bool newValue)
{
	AutoMutex hold(mutex);
	bool old = value;
	value = newValue;
	return old;
}

void AtomicBoolean::set(bool newValue)
{
	AutoMutex hold(mutex);
	value = newValue;
}

AtomicLong::AtomicLong(long initialValue)
	:
	value(initialValue)
{
}

AtomicLong::~AtomicLong()
{

}

long AtomicLong::addAndGet(long addition)
{
	AutoMutex hold(mutex);
	value += addition;
	return value;
}

bool AtomicLong::compareAndSet(long expect, long update)
{
	AutoMutex hold(mutex);
	if(value == expect){
		value = update;
		return true;
	}else{
		return false;
	}	
}

long AtomicLong::decrementAndGet()
{
	AutoMutex hold(mutex);
	return --value;
}

double AtomicLong::doubleValue()
{
	AutoMutex hold(mutex);
	return (double) value;
}	

long AtomicLong::get()
{
	AutoMutex hold(mutex);
	return value;
}

long AtomicLong::getAndAdd(long delta)
{
	AutoMutex hold(mutex);
	long old = value;
	value += delta;
	return old;
}

long AtomicLong::getAndDecrement()
{
	AutoMutex hold(mutex);
	long old = value;
	value--;
	return old;
}	

long AtomicLong::getAndIncrement()
{
	AutoMutex hold(mutex);
	long old = value;
	value++;
	return old;
}

long AtomicLong::getAndSet(long newValue)
{
	AutoMutex hold(mutex);
	long old = value;
	value = newValue;
	return old;
}

long AtomicLong::incrementAndGet()
{
	AutoMutex hold(mutex);
	return ++value;
}

void AtomicLong::set(long newValue)
{
	AutoMutex hold(mutex);
	value = newValue;
}

}  // util namespace
}  // gmsec namespace


GMSEC_I32 gmsec::util::syncAddAndFetch(volatile GMSEC_I32 *p, GMSEC_I32 n)
{
	return gmsec::internal::syncAddAndFetch(p, n);
}

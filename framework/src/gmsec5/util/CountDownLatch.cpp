/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Condition.cpp
 *
 *  @brief This file contains a cross-platform CountDownLatch that functions the same
 *	as the Java class of the same name.
 */

#include <gmsec5/util/CountDownLatch.h>

#include <gmsec5/util/Mutex.h>


using namespace gmsec::api5::util;


CountDownLatch::CountDownLatch(int c)
	: count(c)
{
}


CountDownLatch::~CountDownLatch()
{
}


void CountDownLatch::await()
{
	AutoMutex aMutex(condition.getMutex(), true);

	if (count > 0)
	{
		condition.wait();
	}
}


bool CountDownLatch::await(int timeout)
{
	AutoMutex aMutex(condition.getMutex(), true);

	if (count > 0)
	{
		int returnCondition = condition.wait(timeout);
		if (returnCondition == Condition::TIMEOUT)
		{
			return false;
		}
	}

	return true;
}


void CountDownLatch::countDown()
{
	AutoMutex aMutex(condition.getMutex(), true);
	if (count > 0)
	{
		--count;
		if (count == 0)
		{
			condition.broadcast(Condition::USER);
		}
	}
}


int CountDownLatch::getCount()
{
	return count;
}

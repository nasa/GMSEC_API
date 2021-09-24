/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file Condition.cpp
 *
 *  @author Matthew Handy
 *  @date   November 9, 2009
 *
 *  @brief This file contains a cross-platform CountDownLatch that functions the same
 *	as the Java class of the same name.
 *
 *
*/

#include <gmsec/util/CountDownLatch.h>

namespace gmsec
{
namespace util
{

CountDownLatch::CountDownLatch(int c)
	:
	count(c)
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
		condition.Wait();
	}
}

bool CountDownLatch::await(int timeout)
{
	AutoMutex aMutex(condition.getMutex(), true);

	if (count > 0)
	{
		int returnCondition = condition.Wait(timeout);
		if (returnCondition == gmsec::util::Condition::TIMEOUT)
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
		count--;
		if (count == 0)
		{
			condition.Broadcast(gmsec::util::Condition::USER);
		}
	}
}

int CountDownLatch::getCount()
{
	return count;
}

}
}


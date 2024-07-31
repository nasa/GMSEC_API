/*
 * Copyright 2007-2024 United States Government as represented by the
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

#include <gmsec5/util/Condition.h>

#include <gmsec5/internal/util/InternalCondition.h>


using namespace gmsec::api5::util;
using namespace gmsec::api5::internal;


Condition::Condition(Mutex& m)
	: m_internal(new InternalCondition(m))
{
}


Condition::Condition()
	: m_internal(new InternalCondition())
{
}


Condition::~Condition()
{
	delete m_internal;
}


Mutex& Condition::getMutex()
{
	return m_internal->getMutex();
}


int Condition::wait()
{
	return m_internal->wait();
}


int Condition::wait(long timeout)
{
	return m_internal->wait(timeout);
}


int Condition::waitUntil(const GMSEC_TimeSpec& until)
{
	return m_internal->waitUntil(until);
}


void Condition::signal(int result)
{
	m_internal->signal(result);
}


void Condition::broadcast(int result)
{
	m_internal->broadcast(result);
}

/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Mutex.cpp
 *
 *  This file contains the Mutex class for threads.
 */

#include <gmsec5/util/Mutex.h>

#include <gmsec5/internal/util/InternalMutex.h>


using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


Mutex::Mutex()
	: m_internal(new InternalMutex())
{
}


Mutex::~Mutex()
{
	delete m_internal;
	m_internal = 0;
}


void Mutex::enterMutex()
{
	m_internal->enterMutex();
}


void Mutex::leaveMutex()
{
	m_internal->leaveMutex();
}



AutoMutex::AutoMutex(Mutex& m, bool acquire)
	: m_internal(new gmsec::api5::internal::InternalAutoMutex(m, acquire))
{
}


AutoMutex::~AutoMutex()
{
	delete m_internal;
}


void AutoMutex::enter()
{
	m_internal->enter();
}


void AutoMutex::leave()
{
	m_internal->leave();
}

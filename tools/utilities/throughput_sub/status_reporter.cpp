/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file status_reporter.cpp
 * 
 */

#include "status_reporter.h"
#include "throughput_sub.h"

using namespace gmsec::api::util;


status_reporter::status_reporter(int monitorRate, throughput_sub* receiver)
	: m_monitorRateMs(monitorRate),
	  m_receiver(receiver)
{
}


status_reporter::~status_reporter()
{
}


bool status_reporter::run()
{
	bool done = false;
	while(!done)
	{
		TimeUtil::millisleep(m_monitorRateMs);

		size_t numBitsReceived = m_receiver->getNumBitsReceived();

		GMSEC_INFO << "\nReceived " << numBitsReceived / 1024 << " kb per " << m_monitorRateMs << " ms";
	}

	return true;
}

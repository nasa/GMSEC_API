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

#include "throughput_pub.h"

using namespace gmsec::api;
using namespace gmsec::api::util;


status_reporter::status_reporter(int dataSize, int monitorRate, throughput_pub* publisher)
	: m_dataSize(dataSize),
	  m_monitorRateMs(monitorRate),
	  m_publisher(publisher)
{
}


status_reporter::~status_reporter()
{
}


bool status_reporter::run()
{
	bool done = false;
	int lastNumPublished = 0;
	while(!done)
	{
		TimeUtil::millisleep(m_monitorRateMs);

		int numPublished            = m_publisher->getNumPublished();
		int publishedDuringInterval = numPublished - lastNumPublished;

		GMSEC_INFO << "Published " << (publishedDuringInterval * m_dataSize) / 1024 
		           << " kb per " << m_monitorRateMs << " ms (" << publishedDuringInterval << " messages)";

		lastNumPublished = numPublished;
	}

	return true;
}

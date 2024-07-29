/*
 * Copyright 2007-2024 United States Government as represented by the
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

#include <numeric>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


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

	int count = m_publisher->getRunningAverageCount();
	int intervals = -1;

	std::vector<int> runningAverageMsgs;
	std::vector<int> runningAverageBits;

	std::vector<int> sessionAverageMsgs;
	std::vector<int> sessionAverageBits;

	while(!done)
	{
		TimeUtil::millisleep(m_monitorRateMs);

		int numPublished            = m_publisher->getNumPublished();
		int publishedDuringInterval = numPublished - lastNumPublished;
		int bits					= (publishedDuringInterval * m_dataSize) / 1024;

		if (static_cast<int>(runningAverageMsgs.size()) <= count)
		{
			runningAverageMsgs.push_back(publishedDuringInterval);
		}
		else
		{
			runningAverageMsgs[intervals++ % count] = publishedDuringInterval;
		}

		if (static_cast<int>(runningAverageBits.size()) <= count)
		{
			runningAverageBits.push_back(bits);
		}
		else
		{
			runningAverageBits[intervals % count] = bits;
		}

		sessionAverageMsgs.push_back(publishedDuringInterval);
		sessionAverageBits.push_back(bits);

		GMSEC_INFO	<< "\nPublished " << bits
					<< " kb per " << m_monitorRateMs << " ms (" << publishedDuringInterval << " messages)\n"
					<< "Running Average (last " << count << " intervals): " 
					<< "Published " << avg(runningAverageBits)
					<< " kb per " << m_monitorRateMs << " ms (" << avg(runningAverageMsgs) << " messages)\n"
					<< "Session Average: " << "Published " << avg(sessionAverageBits)
					<< " kb per " << m_monitorRateMs << " ms (" << avg(sessionAverageMsgs) << " messages)\n";

		lastNumPublished = numPublished;
	}

	return true;
}

double status_reporter::avg(const std::vector<int>& values)
{
	return 1.0 * std::accumulate(values.begin(), values.end(), 0LL) / values.size();
}

/*
 * Copyright 2007-2022 United States Government as represented by the
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

#include <numeric>

using namespace gmsec::api5::util;


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
	bool waitingToReceive = true; //sub might start before pub and record 0 bits while waiting.
								  //don't accumulate intervals until sub starts receiving messages
	int receivedDuringInterval = 0;
	int droppedDuringInterval = 0;

	int count = m_receiver->getRunningAverageCount();
	int intervals = -1;

	std::vector<int> runningAverageMsgs;
	std::vector<int> runningAverageBits;
	std::vector<int> runningAverageDropped;

	std::vector<int> sessionAverageMsgs;
	std::vector<int> sessionAverageBits;
	std::vector<int> sessionAverageDropped;

	while(!done)
	{
		TimeUtil::millisleep(m_monitorRateMs);

		size_t numBitsReceived = m_receiver->getNumBitsReceived();
		receivedDuringInterval = m_receiver->getNumMsgsReceived();
		droppedDuringInterval = m_receiver->getNumMsgsDropped();
		int bits = static_cast<int>(numBitsReceived) / 1024;

		if (bits) waitingToReceive = false;

		if (!waitingToReceive && static_cast<int>(runningAverageMsgs.size()) <= count)
		{
			runningAverageMsgs.push_back(receivedDuringInterval);
		}
		else if (!waitingToReceive)
		{
			runningAverageMsgs[intervals++ % count] = receivedDuringInterval;
		}

		if (!waitingToReceive && static_cast<int>(runningAverageBits.size()) <= count)
		{
			runningAverageBits.push_back(bits);
		}
		else if (!waitingToReceive)
		{
			runningAverageBits[intervals % count] = bits;
		}

		if (!waitingToReceive && static_cast<int>(runningAverageDropped.size()) <= count)
		{
			runningAverageDropped.push_back(droppedDuringInterval);
		}
		else if (!waitingToReceive)
		{
			runningAverageDropped[intervals % count] = droppedDuringInterval;
		}

		if (!waitingToReceive)
		{
			sessionAverageMsgs.push_back(receivedDuringInterval);
			sessionAverageBits.push_back(bits);
			sessionAverageDropped.push_back(droppedDuringInterval);
		}

		GMSEC_INFO	<< "\nReceived " << bits
					<< " kb per " << m_monitorRateMs << " ms (" << receivedDuringInterval << " messages)\n"
					<< droppedDuringInterval << " messages dropped"
					<< "\n\nRunning Average (last " << count << " intervals):"
					<< "\nReceived " << avg(runningAverageBits)
					<< " kb per " << m_monitorRateMs << " ms (" << avg(runningAverageMsgs) << " messages)\n"
					<< avg(runningAverageDropped) << " messages dropped"
					<< "\n\nSession Average:"
					<< "\nReceived " << avg(sessionAverageBits)
					<< " kb per " << m_monitorRateMs << " ms (" << avg(sessionAverageMsgs) << " messages)\n"
					<< avg(sessionAverageDropped) << " messages dropped";
	}

	return true;
}

double status_reporter::avg(const std::vector<int>& values)
{
	if (values.size() < 1) return 0.0;
	return 1.0 * std::accumulate(values.begin(), values.end(), 0LL) / values.size();
}

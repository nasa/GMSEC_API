/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file status_reporter.h
 * 
 */

#ifndef STATUS_REPORTER_H
#define STATUS_REPORTER_H

#include <gmsec5_defs.h>

#include <vector>

class throughput_pub;

class status_reporter
{
public:
	status_reporter(int dataSize, int monitorRate, throughput_pub* publisher);
	~status_reporter();
	bool run();
private:
	int m_dataSize;
	int m_monitorRateMs;
	throughput_pub* m_publisher;

	double avg(const std::vector<int>& values);
};

#endif

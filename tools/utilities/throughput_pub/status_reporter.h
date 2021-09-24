/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4_defs.h>

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
	int m_lastNumPublished;
	throughput_pub* m_publisher;
};

#endif

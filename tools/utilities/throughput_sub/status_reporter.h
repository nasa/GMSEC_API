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

class throughput_sub;

class status_reporter
{
public:
	status_reporter(int monitorRate, throughput_sub* publisher);
	~status_reporter();
	bool run();
private:
	int m_monitorRateMs;
	throughput_sub* m_receiver;
};

#endif

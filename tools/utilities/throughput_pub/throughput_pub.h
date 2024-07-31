/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file throughput_pub.h
 * 
 */

#ifndef GMPUB_THROUGHPUT_H
#define GMPUB_THROUGHPUT_H

#include "../Utility.h"

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>

#include <string>

// Forward Declarations
class status_reporter;


class throughput_pub : public gmsec::api5::util::Utility
{
public:
	throughput_pub(const gmsec::api5::Config& config);

	~throughput_pub();

	void usage(const std::string& programName);

	int getNumPublished();

	int getRunningAverageCount();

	bool run();

private:
	gmsec::api5::Connection*      conn;
	gmsec::api5::util::StdThread* reportingThread;
	int                           iteration;
	int							  runningAverageCount; //number of intervals to count in running average
};

void runReporter(gmsec::api5::util::StdSharedPtr<status_reporter> reporter);

#endif

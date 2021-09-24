/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>

#include <string>

// Forward Declarations
class status_reporter;


class throughput_pub : public gmsec::api::util::Utility
{
public:
	throughput_pub(const gmsec::api::Config& config);

	~throughput_pub();

	void usage(const std::string& programName);

	int getNumPublished();

	bool run();

private:
	gmsec::api::mist::ConnectionManager* connMgr;
	gmsec::api::util::StdThread*         reportingThread;
	int                                  iteration;
};

void runReporter(gmsec::api::util::StdSharedPtr<status_reporter> reporter);

#endif

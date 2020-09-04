/*
 * Copyright 2007-2018 United States Government as represented by the
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

namespace gmsec
{
namespace api
{
	// Forward Declarations
	class Config;
	class Connection;
}
}

class status_reporter;

using namespace gmsec::api;
using namespace gmsec::api::util;

class throughput_pub : public Utility
{
public:
	throughput_pub(const Config& config);

	~throughput_pub();

	void usage(const std::string& programName);

	int getNumPublished();

	bool run();

private:
	Connection* connection;
	StdThread*  reportingThread;
	int         iteration;
};

void runReporter(StdSharedPtr<status_reporter> reporter);

#endif

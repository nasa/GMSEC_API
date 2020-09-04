/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file throughput_sub.h
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

class throughput_sub : public Utility
{
public:
	throughput_sub(const Config& config);

	~throughput_sub();

	void usage(const std::string& programName);

	int getNumMsgsDropped();

	size_t getNumBitsReceived();

	bool run();

private:
	Connection* connection;
	StdThread*  reportingThread;
	size_t      bytesReceived;
	int         droppedMsgCount;
	mutable     gmsec::api::util::Mutex mutex;
};

void runReporter(StdSharedPtr<status_reporter> reporter);

#endif

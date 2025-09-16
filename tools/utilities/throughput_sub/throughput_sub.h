/*
 * Copyright 2007-2025 United States Government as represented by the
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

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>

#include <string>

// Forward declaration(s)
class status_reporter;


class throughput_sub : public gmsec::api5::util::Utility
{
public:
	throughput_sub(const gmsec::api5::Config& config);

	~throughput_sub();

	void usage(const std::string& programName);

	int getNumMsgsDropped();

	int getNumMsgsReceived();

	size_t getNumBitsReceived();

	int getRunningAverageCount();

	bool run();

private:
	gmsec::api5::Connection*         conn;
	gmsec::api5::util::StdThread*    reportingThread;
	size_t                           bytesReceived;
	int                              droppedMsgCount;    //number of dropped messages since the last get call
	int								 receivedMsgCount;	 //number of received messages since the last get call
	int								 runningAverageCount;//number of intervals to count in running average
	mutable gmsec::api5::util::Mutex mutex;
};

void runReporter(gmsec::api5::util::StdSharedPtr<status_reporter> reporter);

#endif

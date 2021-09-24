/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file throughput_sub.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will listen for this publish program.
 * gmsub should be run before throughput_sub.
 * 
 */

#include "throughput_sub.h"
#include "status_reporter.h"

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


throughput_sub::throughput_sub(const Config& config)
	: Utility(config),
	  connMgr(0),
	  reportingThread(0),
	  bytesReceived(0),
	  droppedMsgCount(0),
	  mutex()
{
	/* Initialize utility */
	initialize();
}


throughput_sub::~throughput_sub()
{
	if (connMgr)
	{
		try
		{
			connMgr->cleanup();
		}
		catch (const Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete connMgr;
	}

	ConnectionManager::shutdownAllMiddlewares();
}


void throughput_sub::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* monitor-rate=<period of time (milliseconds) to report monitored throughput>"
	          << "\n"
	          << std::endl;
}


bool throughput_sub::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the ConnectionManager
		connMgr = new ConnectionManager(getConfig());

		//o Connect
		connMgr->initialize();

		//o Get information from the command line
		std::string subject     = get("SUBJECT", "GMSEC.TEST.PUBLISH");
		int         monitorRate = get("MONITOR-RATE", 1000); // Time to wait between reporting the achieved throughput

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Subscribing to subject '" << subject.c_str() << "'";

		(void) connMgr->subscribe(subject.c_str());

		GMSEC_INFO << "Reporting throughput status every " << monitorRate << " milliseconds";

		StdSharedPtr<status_reporter> reporter(new status_reporter(monitorRate, this));
		reportingThread = new StdThread(&runReporter, reporter);

		//o Receive message(s)
		bool done = false;
		int iteration = 0;
		reportingThread->start();
		while (!done)
		{
			Message* message = connMgr->receive(GMSEC_WAIT_FOREVER);

			{
				AutoMutex lock(mutex);

				bytesReceived += message->getBinaryField("DATA").getLength();
			}

			// Check to see if any messages have been dropped based on the COUNT field
			if (iteration++ != message->getIntegerValue("COUNT"))
			{
				droppedMsgCount += 1;
			}

			connMgr->release(message);
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int throughput_sub::getNumMsgsDropped()
{
	return droppedMsgCount;
}


size_t throughput_sub::getNumBitsReceived()
{
	// Need to reset the number of bytes received every so often so that it does not grow in an unbounded fashion and cause an overflow
	AutoMutex lock(mutex);

	size_t rcvd = bytesReceived;
	bytesReceived = 0;

	return rcvd;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	throughput_sub tps(config);

	tps.addToConfigFromFile();

	if (!tps.areOptionsValid(argc))
	{
		tps.usage("throughput_sub");
		return 1;
	}

	return tps.run() ? 0 : 1;
}


void runReporter(StdSharedPtr<status_reporter> reporter)
{
	try
	{
		reporter.get()->run();
	}
	catch (const std::exception& e)
	{
		GMSEC_ERROR << "Reporting thread terminated with error: " << e.what();
	}
}

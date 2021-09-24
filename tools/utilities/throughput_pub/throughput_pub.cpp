/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file throughput_pub.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will listen for this publish program.
 * gmsub should be run before throughput_pub.
 * 
 */

#include "throughput_pub.h"
#include "status_reporter.h"


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


throughput_pub::throughput_pub(const Config& config)
	: Utility(config),
	  connMgr(0),
	  reportingThread(0),
	  iteration(0)
{
	/* Initialize utility */
	initialize();
}


throughput_pub::~throughput_pub()
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


void throughput_pub::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* data-size=<number of bytes to package into messages>"
	          << "\n"
	          << "\n\t\t* monitor-rate=<period of time (milliseconds) to report monitored throughput>"
	          << "\n"
	          << std::endl;
}


bool throughput_pub::run()
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
		std::string subject = get("SUBJECT", "GMSEC.TEST.PUBLISH");
		int dataSize        = get("DATA-SIZE", 1000);    // Size of the binary packet to add to the GMSEC Message
		int monitorRate     = get("MONITOR-RATE", 1000); // Time to wait between reporting the achieved throughput

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Using subject '" << subject.c_str() << "'";
		GMSEC_INFO << "Attempting to publish GMSEC Messages containing a binary array of size: " << dataSize << " as quickly as possible";
		GMSEC_INFO << "Reporting throughput status every " << monitorRate << " milliseconds";

		StdSharedPtr<status_reporter> reporter(new status_reporter(dataSize, monitorRate, this));
		reportingThread = new StdThread(&runReporter, reporter);

		//o Publish message(s)
		bool done = false;
		unsigned char* data = new unsigned char[dataSize];

		//o Create message
		Message message(subject.c_str(), Message::PUBLISH);

		//o Add fields to message
		message.addField("COUNT", (GMSEC_I32) iteration);
		message.addField("DATA", (GMSEC_BIN) data, dataSize);

		if (getConfig().getBooleanValue("ENCRYPT", false))
		{
			message.addField("SEC-ENCRYPT", true);
		}

		reportingThread->start();
		while (!done)
		{
			message.addField("COUNT", (GMSEC_I32) iteration++);

			//o Publish Message
			connMgr->publish(message);
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int throughput_pub::getNumPublished()
{
	return iteration;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	throughput_pub tpp(config);

	tpp.addToConfigFromFile();

	if (!tpp.areOptionsValid(argc))
	{
		tpp.usage("throughput_pub");
		return 1;
	}

	return tpp.run() ? 0 : 1;
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

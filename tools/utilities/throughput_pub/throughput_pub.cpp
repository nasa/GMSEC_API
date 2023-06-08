/*
 * Copyright 2007-2023 United States Government as represented by the
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

#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


throughput_pub::throughput_pub(const Config& config)
	: Utility(config),
	  conn(0),
	  reportingThread(0),
	  iteration(0)
{
	/* Initialize utility */
	initialize();
}


throughput_pub::~throughput_pub()
{
	if (conn)
	{
		try
		{
			conn->disconnect();
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete conn;
	}

	Connection::shutdownAllMiddlewares();
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
			  << "\n\t\t* running-average-count=<Number of monitor intervals to consider when making running average>"
	          << "\n"
	          << std::endl;
}


bool throughput_pub::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		conn = new Connection(getConfig());

		//o Connect
		conn->connect();

		//o Get information from the command line
		std::string subject	= get("SUBJECT", "GMSEC.TEST.PUBLISH");
		int dataSize		= get("DATA-SIZE", 1000);    // Size of the binary packet to add to the GMSEC Message
		int monitorRate		= get("MONITOR-RATE", 1000); // Time to wait between reporting the achieved throughput
		runningAverageCount	= get("RUNNING-AVERAGE-COUNT", 5); // Number of monitor intervals to consider when making running average

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Using subject '" << subject.c_str() << "'";
		GMSEC_INFO << "Attempting to publish GMSEC Messages containing a binary array of size: " << dataSize << " as quickly as possible";
		GMSEC_INFO << "Reporting throughput status every " << monitorRate << " milliseconds";

		StdSharedPtr<status_reporter> reporter(new status_reporter(dataSize, monitorRate, this));
		reportingThread = new StdThread(&runReporter, reporter);

		//o Publish message(s)
		bool done = false;
		std::vector<GMSEC_U8> data(dataSize);

		//o Create message
		Message message;
		message.setSubject(subject.c_str());

		//o Add fields to message
		message.addField("COUNT", static_cast<GMSEC_I32>(iteration));
		message.addField("DATA", data.data(), data.size());

		reportingThread->start();
		while (!done)
		{
			message.addField("COUNT", static_cast<GMSEC_I32>(iteration++));

			//o Publish Message
			conn->publish(message);
		}
	}
	catch (const GmsecException& e)
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


int throughput_pub::getRunningAverageCount()
{
	return runningAverageCount;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		throughput_pub tpp(config);

		if (!tpp.areOptionsValid(argc))
		{
			tpp.usage("throughput_pub");
			return 1;
		}

		tpp.addToConfigFromFile();

		return tpp.run() ? 0 : 1;
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		return 1;
	}
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

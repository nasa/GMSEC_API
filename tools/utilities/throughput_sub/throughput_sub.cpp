/*
 * Copyright 2007-2017 United States Government as represented by the
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

#include "../example.h"
#include "throughput_sub.h"
#include "status_reporter.h"

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


throughput_sub::throughput_sub(Config &c)
	: config(c),
	  connection(0),
	  reportingThread(0),
	  bytesReceived(0),
	  droppedMsgCount(0),
	  mutex()
{
	/* Initialize config */
	example::initialize(c);
}


throughput_sub::~throughput_sub()
{
	if (connection)
	{
		try
		{
			connection->disconnect();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool throughput_sub::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Get information from the command line
		std::string subject = example::get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
		int monitorRate  = example::get(config, "MONITOR-RATE", 1000); // Time to wait between reporting the achieved throughput

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Subscribing to subject '" << subject.c_str() << "'";

		connection->subscribe(subject.c_str());

		GMSEC_INFO << "Reporting throughput status every " << monitorRate << " milliseconds";

		StdSharedPtr<status_reporter> reporter(new status_reporter(monitorRate, this));
		reportingThread = new StdThread(&runReporter, reporter);

		//o Receive message(s)
		bool done = false;
		int iteration = 0;
		reportingThread->start();
		while (!done)
		{
			Message* message = connection->receive();

			AutoMutex lock(mutex);
			bytesReceived += message->getBinaryField("DATA").getLength();
			lock.leave();

			// Check to see if any messages have been dropped based on the COUNT field
			if (iteration++ != message->getIntegerValue("COUNT"))
			{
				droppedMsgCount += 1;
			}
		}
	}
	catch (Exception& e)
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
	lock.leave();

	return rcvd;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("throughput_sub");
		return -1;
	}

	throughput_sub(config).run();
}


void runReporter(StdSharedPtr<status_reporter> reporter)
{
	try
	{
		reporter.get()->run();
	}
	catch (std::exception& e)
	{
		GMSEC_ERROR << "Reporting thread terminated with error: " << e.what();
	}
}

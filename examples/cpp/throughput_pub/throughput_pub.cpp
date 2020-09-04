/*
 * Copyright 2007-2017 United States Government as represented by the
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

#include "../example.h"
#include "throughput_pub.h"
#include "status_reporter.h"

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


throughput_pub::throughput_pub(Config &c)
	: config(c),
	  connection(0),
	  reportingThread(0),
	  iteration(0)
{
	/* Initialize config */
	example::initialize(c);
}


throughput_pub::~throughput_pub()
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


bool throughput_pub::run()
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
		int dataSize        = example::get(config, "DATA-SIZE", 0);          // Size of the binary packet to add to the GMSEC Message
		int monitorRate  = example::get(config, "MONITOR-RATE", 1000); // Time to wait between reporting the achieved throughput

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

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

		if (config.getBooleanValue("ENCRYPT", false))
		{
			message.addField("SEC-ENCRYPT", true);
		}

		reportingThread->start();
		while (!done)
		{
			message.addField("COUNT", (GMSEC_I32) iteration++);

			//o Publish Message
			connection->publish(message);
		}
	}
	catch (Exception& e)
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

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("throughput_pub");
		return -1;
	}

	throughput_pub(config).run();
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

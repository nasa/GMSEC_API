/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file subscribe_async.cpp
 * 
 * This file contains a simple example outlining how to asynchronously subscribe
 * to and retrieve messages from the middleware bus. Asynchronous receipt
 * operations essentially mean that a Callback subclass is implemented, that the
 * corresponding Callback object is registered to a subscription, and that
 * messages are received asynchronously after starting the AutoDispatcher.
 *
 * This example program is intended to be run before starting the 'publish'
 * example program.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper function
void initializeLogging(Config& config);

//o Globals
bool receivedMessage = false;

//o Create a subclass of Callback and implement the onMessage function
// The onMessage function will determine what action(s) the program takes
// when receiving messages asynchronously
class ExampleCallback: public ConnectionManagerCallback
{
public:
	virtual void CALL_TYPE onMessage(ConnectionManager& connection, const Message& message)
	{
		GMSEC_INFO << "[ExampleCallback::onMessage] Received:\n" << message.toXML();
		receivedMessage = true;
	}
};

int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which
	// is used to pass configuration options into objects such
	// as Connection objects, ConnectionManager objects, Subscribe
	// and Publish calls, Message objects, etc.
	Config config(argc, argv);

	initializeLogging(config);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create a ConnectionManager object
		// This is the linchpin for all communications between the
		// GMSEC API and the middleware server
		ConnectionManager connMgr(config);

		//o Open the connection to the middleware
		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		//o Output middleware client library version
		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Set up a subscription to listen for Messages with the topic
		// "GMSEC.TEST.PUBLISH" which are published to the middleware
		// bus and register the callback object to the subscription
		// Subscription subject wildcard syntax:
		// * - Matches any one token separated by periods in a subject
		// > - Reading left to right, matches everything up to and ONE
		//     or more tokens in a subject
		// + - Reading left to right, matches everything up to and ZERO 
		//     or more tokens in a subject
		// For more information on wildcards, please see the GMSEC API
		// User's Guide
		GMSEC_INFO << "Subscribing to the topic: " << DEFAULT_SUBSCRIPTION_SUBJECT;
		ExampleCallback cb;
		connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, &cb);

		//o Start the AutoDispatcher to begin asynchronously processing
		// messages
		connMgr.startAutoDispatch();

		//o Because the handling of messages is occurring asynchronously
		// in another thread, we will need to 'busy wait' here until a
		// message is received
		while (receivedMessage == 0)
		{
			TimeUtil::millisleep(100);
		}

		//o Clean up
		connMgr.stopAutoDispatch();
		connMgr.cleanup();
	}
	catch (Exception e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}

void initializeLogging(Config& config)
{
	// If it was not specified in the command-line arguments, set
	// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the 
	// program report output on the terminal/command line
	const char* logLevel  = config.getValue("LOGLEVEL");
	const char* logFile   = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}


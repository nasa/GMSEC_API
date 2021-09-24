/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file connection_state.cpp
 * 
 * This file contains an example which demonstrates how to utilize
 * EventCallbacks to track the state of the GMSEC Connection and its connection
 * to the middleware.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

//o Helper function(s)
void initializeLogging(Config& config);


class ConnectionStateCallback : public ConnectionManagerEventCallback
{
public:
	void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	{
		// Perform some action based on the type of event that occurred
		switch (event)
		{
		case Connection::CONNECTION_SUCCESSFUL_EVENT:
			GMSEC_INFO << "[onEvent]: Connected to the middleware server";
			break;

		case Connection::CONNECTION_BROKEN_EVENT:
			GMSEC_INFO << "[onEvent]: Connection to the middleware lost or terminated";
			break;

		case Connection::CONNECTION_RECONNECT_EVENT:
			GMSEC_INFO << "[onEvent]: Attempting to re-establish the connection to the middleware";
			break;

		default:
			GMSEC_INFO << "[onEvent]: " << status.get();
			break;
		}
	}
};


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	ConnectionStateCallback cb;	

	//o Load the command-line input into a GMSEC Config object
	Config config(argc, argv);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Register the event callback with the connection to catch
		// connection state changes, including:
		// 1. Connection successfully established to middleware
		// 2. Connection broken from middleware
		// 3. Reconnecting to the middleware
		//
		// Refer to the documentation for the Connection class for other events that
		// can be monitored.
		//
		connMgr.registerEventCallback(Connection::CONNECTION_SUCCESSFUL_EVENT, &cb);
		connMgr.registerEventCallback(Connection::CONNECTION_BROKEN_EVENT, &cb);
		connMgr.registerEventCallback(Connection::CONNECTION_RECONNECT_EVENT, &cb);

		//o Connect
		connMgr.initialize();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr.getLibraryVersion();

		// Assuming that the user provided proper reconnection syntax
		// for the corresponding middleware they are using, one could
		// kill the middleware server at this point, then start it back
		// up to demonstrate the EventCallback.onEvent() function
		// triggering

		//o Wait for user input to end the program
		GMSEC_INFO << "Waiting for Connection events to occur; press <Enter> to exit the program";
		std::string enter;
		std::getline(std::cin, enter);

		//o Clean up the ConnectionManager before exiting the program
		connMgr.cleanup();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}


void initializeLogging(Config& config)
{
	const char* logLevel = config.getValue("LOGLEVEL");
	const char* logFile  = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}

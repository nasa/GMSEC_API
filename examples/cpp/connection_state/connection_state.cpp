/*
 * Copyright 2007-2017 United States Government as represented by the
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

//o Helper functions
void initializeLogging(Config& config);
void populateMessage(Message& message, int i);

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
class ConnectionStateCallback : public ConnectionManagerEventCallback
{
public:
	void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	{
		// Perform some action based on the type of event that occurred
		if (event == Connection::CONNECTION_SUCCESSFUL_EVENT)
		{
			GMSEC_INFO << "Connected to the middleware server";
		}
		else if (event == Connection::CONNECTION_BROKEN_EVENT)
		{
			GMSEC_INFO << "Connection to the middleware lost or terminated";
		}
		else if (event == Connection::CONNECTION_RECONNECT_EVENT)
		{
			GMSEC_INFO << "Attempting to reestablish the connection to the middleware";
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
	// TODO: Once available, replace this statement with usage of
	// ConnectionManager::getAPIVersion (See RTC 4798)
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Connect
		connMgr.initialize();

		//o Register the event callback with the connection to catch
		// connection state changes, including:
		// 1. Connection successfully established to middleware
		// 2. Connection broken from middleware
		// 3. Reconnecting to the middleware
		connMgr.registerEventCallback(Connection::CONNECTION_SUCCESSFUL_EVENT, &cb);
		connMgr.registerEventCallback(Connection::CONNECTION_BROKEN_EVENT, &cb);
		connMgr.registerEventCallback(Connection::CONNECTION_RECONNECT_EVENT, &cb);

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr.getLibraryVersion();

		// Assuming that the user provided proper reconnection syntax
		// for the corresponding middleware they are using, one could
		// kill the middleware server at this point, then start it back
		// up to demonstrate the EventCallback.onEvent() function
		// triggering

		//o Wait for user input to end the program
		std::string enter;
		GMSEC_INFO << "Waiting for Connection events to occur, press <enter> to exit the program";
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


void populateMessage(Message& message, int value)
{
	// Note: Since C++ is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
	message.addField("CHAR-FIELD", (GMSEC_CHAR) 'c');
	message.addField("BOOL-FIELD-TRUE", true);
	message.addField("BOOL-FIELD-FALSE", false);
	message.addField("I8-FIELD", (GMSEC_I8) value);
	message.addField("I16-FIELD", (GMSEC_I16) value);
	message.addField("I32-FIELD", (GMSEC_I32) value);
	message.addField("I64-FIELD", (GMSEC_I64) value);
	message.addField("U8-FIELD", (GMSEC_U8) value);
	message.addField("U16-FIELD", (GMSEC_U16) value);
	message.addField("U32-FIELD", (GMSEC_U32) value);
	message.addField("U64-FIELD", (GMSEC_U64) value);
	message.addField("STRING-FIELD", "This is a test");
	message.addField("F32-FIELD", GMSEC_F32(1 + 1. / value));
	message.addField("F64-FIELD", GMSEC_F64(1 + 1. / value));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);
}


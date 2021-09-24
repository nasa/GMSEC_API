/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request.cpp
 * 
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message. This example program is intended to
 * be run after starting up either the 'reply' or 'reply_async' example program.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::mist;

const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper functions
void initializeLogging(Config& config);


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which is used to
	// pass configuration options into objects such as Connections,
	// ConnectionManagers, Subscribe and Publish function calls, Messages,
	// etc.
	Config config(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	config.addValue("gmsec-msg-content-validate-all", "false");

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the ConnectionManager
		ConnectionManager connMgr(config);

		//o Open the connection to the middleware
		connMgr.initialize();

		//o Output middleware client library version
		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Issuing a request using the DEFAULT_REQUEST_SUBJECT '" << DEFAULT_REQUEST_SUBJECT << "'";

		//o Create message
		Message requestMsg(DEFAULT_REQUEST_SUBJECT, Message::REQUEST);

		//o Add fields to message
		requestMsg.addField("QUESTION", "Is there anyone out there?");
		requestMsg.addField("COMPONENT", "request");

		//o Display XML representation of request message
		GMSEC_INFO << "Sending request message:\n" << requestMsg.toXML();

		//o Send Request Message
		// Timeout periods:
		// -1 - Wait forever
		//  0 - Return immediately
		// >0 - Time in milliseconds before timing out
		Message* replyMsg = connMgr.request(requestMsg, 5000, -1);

		// Example error handling for calling request() with a timeout
		if (replyMsg)
		{
			// Display the XML string representation of the reply
			GMSEC_INFO << "Received replyMsg:\n" << replyMsg->toXML();

			//o Destroy the replyMsg message
			connMgr.release(replyMsg);
		}
		else
		{
			GMSEC_WARNING << "[timeout] Failed to receive a response message";
		}

		//o Disconnect from the middleware and clean up the Connection
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

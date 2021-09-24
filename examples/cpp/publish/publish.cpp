/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file publish.cpp
 * 
 * This file contains an example describing how to create and publish a
 * message to the middleware bus using the lowest level function calls
 * available for such operations.
 *
 * This example program is intended to be run after starting the 'subscribe'
 * or 'subscribe_async' example program.
 */

#include "gmsec4_cpp.h"

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::mist;

const char* EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper functions
void initializeLogging(Config& config);
void populateMessage(Message& message);


int main (int argc, char* argv[])
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

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
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

		//o Create a message
		// Disclaimer: This message is not based off of a Message
		// Definition outlined by the GMSEC Interface
		// Specification Document (ISD).  It is meant to be an example
		// to demonstrate the various capabilities of the GMSEC Message
		// and Field classes. The GMSEC Team recommends that you map
		// your data into the Messages defined in the GMSEC ISD, as
		// doing so will make your software "GMSEC Compliant" resulting
		// in plug-and-play type functionality with other GMSEC
		// compliant software.
		Message message(EXAMPLE_MESSAGE_SUBJECT, Message::PUBLISH);
		populateMessage(message);

		//o Publish the message to the middleware bus
		connMgr.publish(message);

		//o Display the XML string representation of the Message for
		// the sake of review
		GMSEC_INFO << "Published message:\n" << message.toXML();

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();
	}
	catch (const Exception& e)
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


void populateMessage(Message& message)
{
	// Add Fields of varying data types to the Message
	int value = 1;

	// Note: Since C++ is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
	// Note: If a field of the same name is added to an existing message,
	// the value passed when calling addField will overwrite the existing
	// value.
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


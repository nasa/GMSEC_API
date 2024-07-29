/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file subscribe_subject_filter.cpp
 * 
 * This file contains a simple example outlining how to subscribe
 * to and retrieve messages from the middleware bus while using subject
 * exclusion filtering to reduce the scope of a wildcarded subject subscription.
 *
 * This example program is intended to be run before starting the
 * 'publish_subject_filter' example program.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>
#include <string>
#include <iostream>

using namespace gmsec::api5;


int main(int argc, char** argv)
{
	// Set up connection configuration options
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subscription topics for the example program
	const char* ALL_MESSAGES_TOPIC;
	const char* HB_MESSAGES_TOPIC;
	int level = config.getIntegerValue("gmsec-schema-level", static_cast<GMSEC_I32>(Specification::SchemaLevel::LEVEL_0));

	switch ( static_cast<Specification::SchemaLevel>(level) )
	{
	case Specification::SchemaLevel::LEVEL_0:
		ALL_MESSAGES_TOPIC = "C2MS.>";
		HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+";
		break;

	case Specification::SchemaLevel::LEVEL_1:
		ALL_MESSAGES_TOPIC = "C2MS-PIPE.>";
		HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
		break;

	case Specification::SchemaLevel::LEVEL_2:
	default:
		ALL_MESSAGES_TOPIC = "GMSEC.>";
		HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+";
		break;
	}

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Establish connection to the GMSEC Bus.
		conn->connect();
		
		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Subscribe to receive all messages using the GMSEC specification
		GMSEC_INFO << "Subscribe to: " << ALL_MESSAGES_TOPIC;
		conn->subscribe(ALL_MESSAGES_TOPIC);

		// Add specific message topic to the Connection's exclusion filter
		GMSEC_INFO << "Filtering out: " << HB_MESSAGES_TOPIC;
		conn->excludeSubject(HB_MESSAGES_TOPIC);


		// Receive first message (LOG) sent by publisher
		GMSEC_INFO << "Waiting to receive a message...";
		Message* rcvdMsg = conn->receive(GMSEC_WAIT_FOREVER);

		GMSEC_INFO << "Received LOG message:\n" << rcvdMsg->toXML();

		Message::destroy(rcvdMsg);


		// Although a second message (HB) is sent by the publisher, it will not
		// be delivered to this Connection object
		GMSEC_INFO << "Waiting to receive other message(s)...";
		rcvdMsg = conn->receive(5000);

		if (rcvdMsg == NULL)
		{
			GMSEC_INFO << "As expected, a timeout occurred (i.e. no HB message received)";
		}
		else
		{
			GMSEC_WARNING << "Received unexpected message:\n" << rcvdMsg->toXML();

			// Acknowledge processing the message
			rcvdMsg->acknowledge();

			// Destroy the message
			Message::destroy(rcvdMsg);
		}

		// Disconnect from the GMSEC Bus.
		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		// If here, the most likely case is the middleware could not be
		// deduced, or a failure occurred when attempting to connect to
		// the GMSEC Bus.
		GMSEC_ERROR << "GmsecException: " << e.what();
	}

	conn.reset();

	Connection::shutdownAllMiddlewares();
}

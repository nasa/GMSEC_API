/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe.cpp
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>

using namespace gmsec::api5;


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages as they are received from the bus
	config.addValue("gmsec-msg-content-validate-recv", "true");

	// Deduce subscription topic for the example program
	const char* subscription_topic;
	int level = config.getIntegerValue("gmsec-schema-level", static_cast<GMSEC_I32>(Specification::SchemaLevel::LEVEL_0));

	switch ( static_cast<Specification::SchemaLevel>(level) )
	{
	case Specification::SchemaLevel::LEVEL_0:
		subscription_topic = "C2MS.>";
		break;

	case Specification::SchemaLevel::LEVEL_1:
		subscription_topic = "C2MS-PIPE.>";
		break;

	case Specification::SchemaLevel::LEVEL_2:
	default:
		subscription_topic = "GMSEC.>";
		break;
	}

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Subscribe to receive message
		conn->subscribe(subscription_topic);
		GMSEC_INFO << "Subscribed to topic: " << subscription_topic;

		// Define timeout period, or use GMSEC_WAIT_FOREVER to block
		// indefinitely to receive a message.
		GMSEC_U32 timeout = 5000;   // 5 seconds

		// Attempt to receive a message
		Message* msg = conn->receive(timeout);

		if (msg != NULL)
		{
			// Received a message!
			GMSEC_INFO << "Received Message:\n" << msg->toXML();

			// Free resources associated with the message.
			Message::destroy(msg);
		}
		else
		{
			// Time out!
			GMSEC_WARNING << "Time out; no message was received";
		}

		// Disconnect from the GMSEC Bus, and terminate subscriptions.
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

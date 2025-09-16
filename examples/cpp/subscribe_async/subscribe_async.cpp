/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.cpp
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus using an asynchronous callback.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


// Define custom callback to receive messages
//
class MyCallback : public Callback
{
public:
	MyCallback() : gotMsg(false) {}

	void CALL_TYPE onMessage(Connection& conn, const Message& msg)
	{
		gotMsg = true;

		GMSEC_INFO << "MyCallback::onMessage --\n" << msg.toXML();

		// Acknowledge processing the message
		msg.acknowledge();
	}

	bool gotMessage() const
	{
		return gotMsg;
	}

private:
	bool gotMsg;
};


// Define custom callback to receive events
//
// Although not required, an event callback can assist with determining
// whether we have a connection to the GMSEC Bus, or in the case of this
// example program, whether the Auto-Dispatcher thread has terminated
// due to some error.
//
class MyEventCallback : public EventCallback
{
public:
	MyEventCallback() : error(false) {}

	void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::Event event)
	{
		// Check event type and/or error status of the Status object.

		if (event == Connection::Event::DISPATCHER_ERROR_EVENT)
		{
			// Auto-dispatcher error has occurred; we are done!
			GMSEC_ERROR << "MyEventCallback::onEvent -- " << status.getReason();
			error = true;
		}
		else if (status.hasError())
		{
			// Soft-error has occurred
			GMSEC_WARNING << "MyEventCallback::onEvent -- " << status.getReason();
		}
		else
		{
			// Nominal event has occurred
			GMSEC_INFO << "MyEventCallback::onEvent -- " << status.getReason();
		}
	}

	bool haveError() const
	{
		return error;
	}

private:
	bool error;
};


int main(int argc, char** argv)
{
	// Set up connection configuration options from the command line.
	Config config(argc, argv);

	// Validate incoming messages
	config.addValue("gmsec-msg-content-validate-recv", "true");

	// Initialize log level for output
	initializeLogLevel(config);

	// Declare custom callback and custom event callback
	MyCallback      mcb;
	MyEventCallback ecb;

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

		// Register custom event callback
		conn->registerEventCallback(Connection::Event::ALL_EVENTS, &ecb);

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Subscribe to receive message via a callback
		conn->subscribe(subscription_topic, &mcb);
		GMSEC_INFO << "Subscribed to topic: " << subscription_topic;

		// Start the auto-dispatcher so we can receive messages via the callback
		conn->startAutoDispatch();

		// Normally an application would have something important to do;
		// here we just idle until a message arrives. Should an error
		// occur, we bail.
		GMSEC_INFO << "Waiting to receive a message...";
		while (!mcb.gotMessage() && !ecb.haveError())
		{
			TimeUtil::millisleep(1000);
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

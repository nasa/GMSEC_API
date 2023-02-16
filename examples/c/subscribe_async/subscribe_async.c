/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.c
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus using an asynchronous callback.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


// Define custom callback to receive messages
//
GMSEC_BOOL g_gotMsg = GMSEC_FALSE;

void myCallback(GMSEC_Connection conn, GMSEC_Message msg)
{
	g_gotMsg = GMSEC_TRUE;

	GMSEC_INFO("myCallback:\n%s", messageToXML(msg, NULL));

	// Acknowledge processing the message
	messageAcknowledge(msg, NULL);
}

GMSEC_BOOL gotMessage()
{
	return g_gotMsg;
}


// Define custom callback to receive events
//
// Although not required, an event callback can assist with determining
// whether we have a connection to the GMSEC Bus, or in the case of this
// example program, whether the Auto-Dispatcher thread has terminated
// due to some error.
//
GMSEC_BOOL g_error = GMSEC_FALSE;

void myEventCallback(GMSEC_Connection conn, GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	// Check event type and/or error status of the Status object.

	if (event == GMSEC_DISPATCHER_ERROR_EVENT)
	{
		// Auto-dispatcher error has occurred; we are done!
		GMSEC_ERROR("myEventCallback -- %s", statusGetReason(status));
		g_error = GMSEC_TRUE;
	}
	else if (statusHasError(status) == GMSEC_TRUE)
	{
		// Soft-error has occurred
		GMSEC_WARNING("myEventCallback -- %s", statusGetReason(status));
	}
	else
	{
		// Nominal event has occurred
		GMSEC_INFO("myEventCallback -- %s", statusGetReason(status));
	}
}

GMSEC_BOOL haveError()
{
	return g_error;
}


// Helper function
//
void checkStatus(const char* operation, GMSEC_Status status);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options from the command line.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subscription topic for the example program
	const char* subscription_topic;
	GMSEC_I32 level = configGetIntegerValueOrDefault(config, "gmsec-schema-level", (GMSEC_I32) GMSEC_SCHEMA_LEVEL_0, NULL);

	switch (level)
	{
	case GMSEC_SCHEMA_LEVEL_0:
		subscription_topic = "C2MS.>";
		break;

	case GMSEC_SCHEMA_LEVEL_1:
		subscription_topic = "C2MS-PIPE.>";
		break;

	case GMSEC_SCHEMA_LEVEL_2:
	default:
		subscription_topic = "GMSEC.>";
		break;
	}

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Register custom Event Callback
	connectionRegisterEventCallback(conn, GMSEC_ALL_EVENTS, &myEventCallback, status);
	checkStatus("connectionRegisterEventCallback", status);

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Subscribe to receive message via a callback
	connectionSubscribeWithCallback(conn, subscription_topic, &myCallback, status);
	checkStatus("connectionSubscribe", status);
	GMSEC_INFO("Subscribed to topic: %s", subscription_topic);

	// Start the auto-dispatcher so we can receive messages via the callback
	connectionStartAutoDispatch(conn, status);
	checkStatus("connectionStartAutoDispatch", status);

	// Normally an application would have something important to do;
	// here we just idle until a message arrives. Should an error
	// occur, we bail.
	GMSEC_INFO("Waiting to receive a message...");
	while (gotMessage() == GMSEC_FALSE && haveError() == GMSEC_FALSE)
	{
		timeUtilMillisleep(1000);
	}

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
	connectionDisconnect(conn, NULL);

	return 0;
}


void checkStatus(const char* operation, GMSEC_Status status)
{
	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("%s failed; reason: %s", operation, statusGet(status));
		exit(-1);
	}
}

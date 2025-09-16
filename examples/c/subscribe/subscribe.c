/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe.c
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


void checkStatus(const char* operation, GMSEC_Status status);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to the bus
	configAddValue(config, "gmsec-msg-content-validate-recv", "true", NULL);

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

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Subscribe to receive message
	connectionSubscribe(conn, subscription_topic, status);
	checkStatus("connectionSubscribe", status);
	GMSEC_INFO("Subscribe to topic: %s", subscription_topic);

	// Define timeout period, or use GMSEC_WAIT_FOREVER to block
	// indefinitely to receive a message.
	GMSEC_U32 timeout = 5000;   // 5 seconds

	// Attempt to receive a message
	GMSEC_Message msg = connectionReceive(conn, timeout, status);
	checkStatus("connectionReceive", status);

	if (msg != NULL)
	{
		// Received a message!
		GMSEC_INFO("Received Message:\n%s", messageToXML(msg, NULL));

		// Acknowledge processing the message
		messageAcknowledge(msg, NULL);

		// Free resources associated with the message.
		messageDestroy(&msg);
	}
	else
	{
		// Time out!
		GMSEC_WARNING("Time out; no message was received");
	}

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
	connectionDisconnect(conn, NULL);

	// Clean up of resources
	connectionDestroy(&conn);
	configDestroy(&config);
	statusDestroy(&status);

	connectionShutdownAllMiddlewares();

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

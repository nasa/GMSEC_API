/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file subscribe_subject_filter.c
 * 
 * This file contains a simple example outlining how to subscribe
 * to and retrieve messages from the middleware bus while using subject
 * exclusion filtering to reduce the scope of a wildcarded subject subscription.
 *
 * This example program is intended to be run before starting the
 * 'publish_subject_filter' example program.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


// Helper function
void checkStatus(const char* operation, GMSEC_Status status);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subscription topics for the example program
	const char* ALL_MESSAGES_TOPIC;
	const char* HB_MESSAGES_TOPIC;
	GMSEC_I32 level = configGetIntegerValueOrDefault(config, "gmsec-schema-level", (GMSEC_I32) GMSEC_SCHEMA_LEVEL_0, NULL);

	switch (level)
	{
	case GMSEC_SCHEMA_LEVEL_0:
		ALL_MESSAGES_TOPIC = "C2MS.>";
		HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+";
		break;

	case GMSEC_SCHEMA_LEVEL_1:
		ALL_MESSAGES_TOPIC = "C2MS-PIPE.>";
		HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+";
		break;

	case GMSEC_SCHEMA_LEVEL_2:
	default:
		ALL_MESSAGES_TOPIC = "GMSEC.>";
		HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+";
		break;
	}

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);
		
	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Subscribe to receive all messages using the GMSEC specification
	GMSEC_INFO("Subscribe to: %s", ALL_MESSAGES_TOPIC);
	GMSEC_SubscriptionInfo info1 = connectionSubscribe(conn, ALL_MESSAGES_TOPIC, status);
	checkStatus("connectionSubscribe", status);

	// Add specific message topic to the Connection's exclusion filter
	GMSEC_INFO("Filtering out: %s", HB_MESSAGES_TOPIC);
	connectionExcludeSubject(conn, HB_MESSAGES_TOPIC, status);
	checkStatus("connectionExcludeSubject", status);


	// Receive first message (LOG) sent by publisher
	GMSEC_INFO("Waiting to receive a message...");
	GMSEC_Message rcvdMsg = connectionReceive(conn, GMSEC_WAIT_FOREVER, status);
	checkStatus("connectionReceive", status);

	GMSEC_INFO("Received LOG message:\n%s", messageToXML(rcvdMsg, NULL));

	// Acknowledge processing the message
	messageAcknowledge(rcvdMsg, NULL);

	// Destroy the message
	messageDestroy(&rcvdMsg);


	// Although a second message (HB) is sent by the publisher, it will not
	// be delivered to this Connection object
	GMSEC_INFO("Waiting to receive other message(s)...");
	rcvdMsg = connectionReceive(conn, 5000, status);
	checkStatus("connectionReceive", status);

	if (rcvdMsg == NULL)
	{
		GMSEC_INFO("As expected, a timeout occurred (i.e. no HB message received)");
	}
	else
	{
		GMSEC_WARNING("Received unexpected message:\n%s", messageToXML(rcvdMsg, NULL));

		// Acknowledge processing the message
		messageAcknowledge(rcvdMsg, NULL);

		// Destroy the message
		messageDestroy(&rcvdMsg);
	}

	// Disconnect from the GMSEC Bus.
	connectionUnsubscribe(conn, &info1, NULL);
	connectionDisconnect(conn, NULL);

	// Cleanup resources
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

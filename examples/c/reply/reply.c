/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file reply.c
 *
 * @brief This example program provides a basic demonstration of synchronously
 * receiving a Request Directive message from the GMSEC Bus, and then issuing
 * a Response Directive message (should one be requested).
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


void checkStatus(const char* operation, GMSEC_Status status);
void setStandardFields(GMSEC_MessageFactory factory, const char* component);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to and as they are received from the bus
    configAddValue(config, "gmsec-msg-content-validate-all", "true", NULL);

	// Define subscription topic for receiving request message(s).
	const char* subscription_topic;
	GMSEC_I32 level = configGetIntegerValueOrDefault(config, "gmsec-schema-level", (GMSEC_I32) GMSEC_SCHEMA_LEVEL_0, NULL);

	switch (level)
	{
	case GMSEC_SCHEMA_LEVEL_0:
		subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
		break;

	case GMSEC_SCHEMA_LEVEL_1:
		subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
		break;

	case GMSEC_SCHEMA_LEVEL_2:
	default:
		subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY";
		break;
	}

	// Create connection instance
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	setStandardFields(connectionGetMessageFactory(conn, NULL), "REPLY");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Set up our subscription for receiving a request message.
	connectionSubscribe(conn, subscription_topic, status);
	checkStatus("connectionSubscribe", status);
	GMSEC_INFO("Subscribed to topic: %s", subscription_topic);

	// Timeout was arbitrarily chosen as GMSEC_WAIT_FOREVER so that
	// we block indefinitely waiting to receive a request message.
	// It can be set to other time values (e.g. 1000 is 1 second) so
	// the application does not block indefinitely waiting for a
	// request message.
	GMSEC_I32 timeout = GMSEC_WAIT_FOREVER;

	// Block for a Request Directive message to arrive.
	GMSEC_Message reqMsg = connectionReceive(conn, timeout, status);
	checkStatus("connectionReceive", status);

	// Check if we have received a message.
	if (reqMsg != NULL)
	{
		// Output in XML the received message.
		GMSEC_INFO("Received Request Message:\n%s", messageToXML(reqMsg, NULL));

		// Check if a response message should be returned.
		if (messageHasField(reqMsg, "RESPONSE", NULL) == GMSEC_TRUE &&
		    messageGetBooleanValue(reqMsg, "RESPONSE", NULL) == GMSEC_TRUE)
		{
			// Create a Response Directive message using the MessageFactory.
			GMSEC_MessageFactory factory = connectionGetMessageFactory(conn, NULL);
			GMSEC_Message        rspMsg  = messageFactoryCreateMessage(factory, "RESP.DIR", status);

			checkStatus("messageFactoryCreateMessage", status);

			// Insert required and optional fields into the Response Directive
			// using values derived from the incoming Request Directive.
			if (messageHasField(reqMsg, "COMPONENT", NULL) == GMSEC_TRUE)
			{
				GMSEC_Field destComp = stringFieldCreate("DESTINATION-COMPONENT", messageGetStringValue(reqMsg, "COMPONENT", NULL), GMSEC_TRUE, NULL);
				messageAddField(rspMsg, destComp, NULL);
				fieldDestroy(&destComp);
			}
			if (messageHasField(reqMsg, "REQUEST-ID", NULL) == GMSEC_TRUE)
			{
				messageAddField(rspMsg, messageGetField(reqMsg, "REQUEST-ID", NULL), NULL);
			}

			// Set the response status for the request.
			messageSetFieldValueI32(rspMsg, "RESPONSE-STATUS", GMSEC_SUCCESSFUL_COMPLETION, NULL);

			// Send the Response Directive message.
			connectionReply(conn, reqMsg, rspMsg, status);
			checkStatus("connectionReply", status);

			// Output in XML the message that has been sent.
			GMSEC_INFO("Sent Response Message:\n%s", messageToXML(rspMsg, NULL));

			// Acknowledge processing the message
			messageAcknowledge(rspMsg, NULL);

			// Destroy response message
			messageDestroy(&rspMsg);
		}

		// Release resources associated with the received message.
		messageDestroy(&reqMsg);
	}
	else
	{
		// This else-block is reached if a reuqest message is not received.
		// When the timeout period is set to GMSEC_WAIT_FOREVER, then this
		// block would never be reached.
		GMSEC_WARNING("Time out; no Request Directive message received");
	}

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
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
	if (statusHasError(status))
	{
		GMSEC_ERROR("%s failed; reason: %s", operation, statusGet(status));
		exit(-1);
	}
}


// Convenience function for associating standard (i.e. common) fields
// with the MessageFactory which will be included automatically within
// all messages created using the factory.
void setStandardFields(GMSEC_MessageFactory factory, const char* component)
{
	GMSEC_Field fields[8] = {0};
	size_t      numFields = sizeof(fields) / sizeof(GMSEC_Field);
	size_t      i;

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", component, GMSEC_TRUE, NULL);

	messageFactorySetStandardFields(factory, fields, numFields, NULL);

	for (i = 0; i < numFields; ++i)
	{
		fieldDestroy(&fields[i]);
	}
}

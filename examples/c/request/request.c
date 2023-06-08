/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request.c
 *
 * @brief This example program provides a basic demonstration of issuing
 * a synchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message.
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

	// Validate messages before beint sent to and as they are received from the bus
	configAddValue(config, "gmsec-msg-content-validate-all", "true", NULL);

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	setStandardFields(connectionGetMessageFactory(conn, NULL), "REQUEST");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Create a Request Directive message using the MessageFactory.
	// Set both required and optional fields within the message.
	GMSEC_MessageFactory factory = connectionGetMessageFactory(conn, NULL);
	GMSEC_Message        reqMsg  = messageFactoryCreateMessage(factory, "REQ.DIR", status);

	checkStatus("messageFactoryCreateMessage", status);

	GMSEC_Field destComp = stringFieldCreate("DESTINATION-COMPONENT", "REPLY", GMSEC_TRUE, NULL);
	messageAddField(reqMsg, destComp, NULL);   // to whom is the request intended for
	fieldDestroy(&destComp);

	messageAddStringField(reqMsg, "DIRECTIVE-STRING", "Turn on the lights", NULL);  // operation to perform
	messageAddBooleanField(reqMsg, "RESPONSE", GMSEC_TRUE, NULL);                   // a response message is requested
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 0, NULL);

	// Output in XML what we intend to send
	GMSEC_INFO("Sending Request Message:\n%s", messageToXML(reqMsg, NULL));

	// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
	// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
	// prevent re-sending request message should a timeout occur.
	GMSEC_I32 timeout   = 5000;
	GMSEC_I32 republish = 5000;

	// Issue Request Directive.
	// Note, this is a blocking call (i.e. a synchronous operation).
	GMSEC_Message rspMsg = connectionRequest(conn, reqMsg, timeout, republish, status);
	checkStatus("connectionRequest", status);

	// We have returned from the previous blocking call; check if we have a message.
	if (rspMsg != NULL)
	{
		GMSEC_INFO("Received Response Directive message:\n%s", messageToXML(rspMsg, NULL));

		// Acknowledge processing the response message
		messageAcknowledge(rspMsg, NULL);

		// Release resources associated with the received message.
		messageDestroy(&rspMsg);
	}
	else
	{
		// This else-block is reached if a response message is not received.
		// If the republish period is set to a time period other than
		// GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
		GMSEC_WARNING("Time out; no Response Directive message received");
	}

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
	connectionDisconnect(conn, NULL);

	// Cleanup resources
	messageDestroy(&reqMsg);
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

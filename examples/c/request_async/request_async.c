/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request_async.c
 *
 * @brief This example program provides a basic demonstration of issuing
 * an asynchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message(s) via a
 * ReplyCallback.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


void checkStatus(const char* operation, GMSEC_Status status);

void setStandardFields(GMSEC_MessageFactory factory, const char* component);


GMSEC_BOOL g_ResponseComplete = GMSEC_FALSE;

void onReply(GMSEC_Connection conn, GMSEC_Message reqMsg, GMSEC_Message repMsg)
{
	GMSEC_INFO("onReply:\n%s", messageToXML(repMsg, NULL));

	if (messageHasField(repMsg, "RESPONSE-STATUS", NULL) == GMSEC_TRUE)
	{
		GMSEC_I32 status = messageGetI32Value(repMsg, "RESPONSE-STATUS", NULL);

		switch (status)
		{
		case GMSEC_ACKNOWLEDGEMENT:
		case GMSEC_WORKING_KEEP_ALIVE:
			g_ResponseComplete = GMSEC_FALSE;
			break;

		case GMSEC_SUCCESSFUL_COMPLETION:
		case GMSEC_FAILED_COMPLETION:
		case GMSEC_INVALID_REQUEST:
		case GMSEC_FINAL_MESSAGE:
			g_ResponseComplete = GMSEC_TRUE;
			break;
		}
	}
	else
	{
		g_ResponseComplete = GMSEC_TRUE;
	}

	// Acknowledge processing the response message
	messageAcknowledge(repMsg, NULL);
}

void onEvent(GMSEC_Connection conn, GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("onEvent: %s, event: %d", statusGet(status), event);
	}
	else
	{
		GMSEC_INFO("onEvent: %s, event: %d", statusGet(status), event);
	}
}

GMSEC_BOOL isResponseComplete()
{
	return g_ResponseComplete;
}


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

	// IMPORTANT! This config option must be set to receive multiple response messages
	configAddValue(config, "mw-multi-resp", "true", NULL);

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	setStandardFields(connectionGetMessageFactory(conn, NULL), "REQUEST-ASYNC");

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

	GMSEC_Field destComp = stringFieldCreate("DESTINATION-COMPONENT", "REPLY-MULTI", GMSEC_TRUE, NULL);
	messageAddField(reqMsg, destComp, NULL);   // to whom is the request intended for
	fieldDestroy(&destComp);

	messageAddStringField(reqMsg, "DIRECTIVE-STRING", "Turn on the lights", NULL);    // operation to perform
	messageAddBooleanField(reqMsg, "RESPONSE", GMSEC_TRUE, NULL);                     // a response message is requested
	messageAddU16Field(reqMsg, "REQUEST-ID", (GMSEC_U16) 0, NULL);

	// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
	// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
	// prevent re-sending request message should a timeout occur.
	GMSEC_I32 timeout   = 5000;
	GMSEC_I32 republish = 5000;

	// Issue Request Directive.
	// Note, this is a non-blocking call (i.e. an asynchronous operation).
	connectionRequestWithCallback(conn, reqMsg, timeout, &onReply, &onEvent, republish, NULL);

	checkStatus("connectionRequestWithCallback", status);

	// Output in XML what we have sent
	GMSEC_INFO("Sending Request Directive Message:\n%s", messageToXML(reqMsg, NULL));
	GMSEC_INFO("Use reply_multi example program to issue response messages");

	// Normally an application would continue with other operation, however for
	// demonstration purposes, we will loop until it has been verified that a
	// response message has been received.
	while (isResponseComplete() == GMSEC_FALSE)
	{
		GMSEC_INFO("Waiting to receive response message...");

		timeUtilMillisleep(1000);
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

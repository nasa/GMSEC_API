/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* EXAMPLE_SUBSCRIPTION_SUBJECT = "GMSEC.>";
const char* FILTERED_MESSAGE_SUBJECT     = "GMSEC.TEST.FILTERED";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Message message;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which is used to
	// pass configuration options into objects such as Connections,
	// ConnectionManagers, Subscribe and Publish function calls, Messages,
	// etc.
	config = configCreateWithArgs(argc, argv);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create a ConnectionManager object
	// This is the linchpin for all communications between the
	// GMSEC API and the middleware server
	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	//o Open the connection to the middleware
	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client library version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Set up a subscription to listen for Messages with the topic
	// "GMSEC.TEST.PUBLISH" which are published to the middleware
	// bus
	// Subscription subject wildcard syntax:
	// * - Matches any one token separated by periods in a subject
	// > - Reading left to right, matches everything up to and ONE
	//     or more tokens in a subject
	// + - Reading left to right, matches everything up to and ZERO
	//     or more tokens in a subject
	// For more information on wildcards, please see the GMSEC API
	// User's Guide
	GMSEC_INFO("Subscribing to the topic: %s", EXAMPLE_SUBSCRIPTION_SUBJECT);
	connectionManagerSubscribe(connMgr, EXAMPLE_SUBSCRIPTION_SUBJECT, status);
	checkStatus(status);

	//o Add a specific message subject to the Connection's exclusion
	// filter
	GMSEC_INFO("Adding a filter rule for the topic: %s", FILTERED_MESSAGE_SUBJECT);
	connectionManagerExcludeSubject(connMgr, FILTERED_MESSAGE_SUBJECT, status);
	checkStatus(status);

	//o Call receive() to synchronously retrieve a message that has
	// been received by the middleware client libraries
	// Timeout periods:
	// -1 - Wait forever
	//  0 - Return immediately
	// >0 - Time in milliseconds before timing out
	GMSEC_INFO("Waiting to receive a Message");
	message = connectionManagerReceive(connMgr, -1, status);
	checkStatus(status);

	if(message != NULL)
	{
		GMSEC_INFO("Received message:\n%s", messageToXML(message, status));
		connectionManagerRelease(connMgr, message, NULL);
	}

	GMSEC_INFO("Waiting 5 seconds to demonstrate that a second message will not be received");
	message = connectionManagerReceive(connMgr, 5000, status);
	checkStatus(status);

	if(message != NULL)
	{
		GMSEC_ERROR("Unexpectedly received a filtered message:\n%s", messageToXML(message, status));
		connectionManagerRelease(connMgr, message, NULL);
	}

	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	connectionManagerDestroy(&connMgr);
	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}

void initializeLogging(GMSEC_Config config, GMSEC_Status status)
{
	const char* logLevel = configGetValue(config, "LOGLEVEL", status);
	const char* logFile = configGetValue(config, "LOGFILE", status);

	if(!logLevel)
	{
		configAddValue(config, "LOGLEVEL", "INFO", status);
	}
	if(!logFile)
	{
		configAddValue(config, "LOGFILE", "STDOUT", status);
	}
}

void checkStatus(GMSEC_Status status)
{
	if(!statusIsError(status))
	{
		return;
	}

	GMSEC_ERROR(statusGet(status));
	exit(-1);
}

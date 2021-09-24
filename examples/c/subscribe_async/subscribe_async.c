/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.c
 *
 * This file contains a simple example outlining how to asynchronously subscribe
 * to and retrieve messages from the middleware bus. Asynchronous receipt
 * operations essentially mean that a Callback subclass is implemented, that the
 * corresponding Callback object is registered to a subscription, and that
 * messages are received asynchronously after starting the AutoDispatcher.
 *
 * This example program is intended to be run before starting the 'publish'
 * example program.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>


const char* DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
int millisleep(unsigned ms);

//o Globals
GMSEC_BOOL receivedMessage = GMSEC_FALSE;

void exampleCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg)
{
	GMSEC_INFO("[ExampleCallback::onMessage] Received:\n%s", messageToXML(msg, NULL));
	receivedMessage = GMSEC_TRUE;
}

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which
	// is used to pass configuration options into objects such
	// as Connection objects, ConnectionManager objects, Subscribe
	// and Publish calls, Message objects, etc.
	config = configCreateWithArgs(argc, argv);

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
	// bus and register the callback object to the subscription
	// Subscription subject wildcard syntax:
	// * - Matches any one token separated by periods in a subject
	// > - Reading left to right, matches everything up to and ONE
	//     or more tokens in a subject
	// + - Reading left to right, matches everything up to and ZERO
	//     or more tokens in a subject
	// For more information on wildcards, please see the GMSEC API
	// User's Guide
	GMSEC_INFO("Subscribing to the topic: %s", DEFAULT_SUBSCRIPTION_SUBJECT);
	connectionManagerSubscribeWithCallback(connMgr, DEFAULT_SUBSCRIPTION_SUBJECT, &exampleCallback, status);
	checkStatus(status);

	//o Start the AutoDispatcher to begin asynchronously processing
	// messages
	connectionManagerStartAutoDispatch(connMgr, status);
	checkStatus(status);

	//o Because the handling of messages is occurring asynchronously
	// in another thread, we will need to 'busy wait' here until a
	// message is received
	while(receivedMessage == 0)
	{
		timeUtilMillisleep(100);
	}

	//o Clean up
	connectionManagerStopAutoDispatch(connMgr, status);
	checkStatus(status);
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

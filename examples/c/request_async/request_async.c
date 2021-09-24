/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_async.c
 *
 * This file contains an example demonstrating how to asynchronously handle the
 * receipt of reply message(s) after issuing a request.
 * Asynchronous request operations essentially mean that a Callback subclass is
 * implemented, then a corresponding Callback object is supplied when invoking
 * request. Reply messages that are then received asynchronously and handled by
 * the Callback object.
 *
 * This example program is intended to be run after either starting up the
 * 'reply' or 'reply_async' example program.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>


const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
int millisleep(unsigned ms);

//o Globals
GMSEC_BOOL receivedReply = GMSEC_FALSE;

void exampleReplyCallback(GMSEC_ConnectionMgr connection, const GMSEC_Message request, const GMSEC_Message reply)
{
	//o Display XML representation of reply message
	GMSEC_INFO("[ExampleReplyCallback::onReply]\n%s", messageToXML(reply, NULL));
	receivedReply = GMSEC_TRUE;
}

void exampleEventCallback(GMSEC_ConnectionMgr connection, const GMSEC_Status status, const GMSEC_ConnectionEvent event)
{
	if(statusIsError(status))
	{
		GMSEC_ERROR("[ExampleReplyCallback::onError] %s, event=%s", statusGet(status), event);
	}
}

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Message requestMsg;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Output GMSEC API version
	GMSEC_INFO(connectionGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	//o Connect
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o output connection middleware version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Create request message
	requestMsg = messageCreate(DEFAULT_REQUEST_SUBJECT, GMSEC_REQUEST, status);
	checkStatus(status);

	//o Add fields to request message
	messageAddStringField(requestMsg, "QUESTION", "Does the request/reply functionality still work?", status);
	checkStatus(status);
	messageAddStringField(requestMsg, "COMPONENT", "request_async", status);
	checkStatus(status);

	//o Display XML representation of request message
	GMSEC_INFO("Requesting:\n%s", messageToXML(requestMsg, status));

	connectionManagerRequestWithCallback(connMgr, requestMsg, -1, &exampleReplyCallback, &exampleEventCallback, -1, status);
	checkStatus(status);

	GMSEC_INFO("Waiting for response...");

	while(receivedReply == GMSEC_FALSE)
	{
		timeUtilMillisleep(100);
	}

	if (receivedReply)
	{
		GMSEC_INFO("Response Received!");
	}
	else
	{
		GMSEC_WARNING("No response received");
	}

	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	messageDestroy(&requestMsg);
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

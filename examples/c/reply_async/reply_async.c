/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_async.c
 *
 * This file contains an example demonstrating how to asynchronously reply to a
 * request message.
 *
 * Asynchronous receipt operations essentially mean that a Callback subclass is
 * implemented, then the corresponding Callback object is registered to a
 * subscription, and that messages are received asynchronously after starting
 * the AutoDispatcher. In this case, replies are created and sent asynchronously
 * when receiving request messages.
 *
 * This example program is intended to be run before starting up either the
 * 'request' or 'request_async' example program.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
int millisleep(unsigned ms);

//o Globals
GMSEC_BOOL replySent = GMSEC_FALSE;

void exampleCallback(GMSEC_Connection connMgr, const GMSEC_Message message)
{
	GMSEC_Status status = statusCreate();
	GMSEC_Message reply;
	const char* component = "UNKNOWN";
	const char* status_code;
	char* reply_subject;

	//o Display the XML representation of the received message
	GMSEC_INFO("[ExampleCallback:onMessage] Received:\n%s", messageToXML(message, status));

	//o Construct Reply subject.
	component = 0;

	component = messageGetStringValue(message, "COMPONENT", status);

	if (statusIsError(status))
	{
		GMSEC_WARNING("COMPONENT field is not available: %s", statusGet(status));
	}

	//o Set Status Code to indicate Successful Completion.
	// The GMSEC Interface Specification Document defines 6
	// unique STATUS-CODE values:
	// 1 - Acknowledgement
	// 2 - Working/keep alive
	// 3 - Successful completion
	// 4 - Failed completion
	// 5 - Invalid request
	// 6 - Final message
	// If an asynchronous requestor is awaiting a reply, the
	// ReplyCallback in use will remain active for multiple
	// messages, so long as the messages it receives contain
	// a STATUS-CODE of either 1 or 2.
	status_code = "3";

	//o Create the reply subject.
	// See API Interface Specificaiton Document for
	// more information on Reply Message subjects.
	// Generally speaking, they are composed
	// accordingly:
	// [Spec].[Mission].[Satellite ID].
	// [Message Type].[Component Name].[Status Code]
	reply_subject = malloc(sizeof(char)*50);
	snprintf(reply_subject, sizeof(char)*50, "GMSEC.MISSION.SAT_ID.RESP.REPLY_ASYNC.%s", status_code);

	reply = messageCreate(reply_subject, GMSEC_REPLY, status);
	checkStatus(status);

	//o Add fields to the reply message
	messageAddStringField(reply, "COMPONENT", component, status);
	checkStatus(status);
	messageAddStringField(reply, "ANSWER", "Sure looks like it!", status);
	checkStatus(status);

	//o Display XML representation of reply message
	GMSEC_INFO("Prepared Reply:\n%s", messageToXML(reply, status));

	//o Send Reply
	connectionManagerReply(connMgr, message, reply, status);
	checkStatus(status);

	messageDestroy(&reply);
	statusDestroy(&status);
	free(reply_subject);

	replySent = GMSEC_TRUE;
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

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Output GMSEC API version
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	//o Connect
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o output connection middleware version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Subscribe
	connectionManagerSubscribeWithCallback(connMgr, DEFAULT_REQUEST_SUBJECT, &exampleCallback, status);
	checkStatus(status);

	//o Start the AutoDispatcher to begin async message receipt
	connectionManagerStartAutoDispatch(connMgr, status);
	checkStatus(status);

	//o Loop while waiting for the asynchronous response until done
	while(replySent == GMSEC_FALSE)
	{
		timeUtilMillisleep(100);
	}

	//o Wait for a few moments to ensure that the asynchronous reply
	// transaction went through with the middleware before closing
	// the connection and exiting the process
	{
		int i;
		for(i = 0; i < 5; i++)
		{
			timeUtilMillisleep(100);
		}
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

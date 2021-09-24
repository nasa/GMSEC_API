/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request.c
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message. This example program is intended to
 * be run after starting up either the 'reply' or 'reply_async' example program.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper functions
void checkStatus(GMSEC_Status status);
void initializeLogging(GMSEC_Config config, GMSEC_Status status);

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Message requestMsg;
	GMSEC_Message replyMsg;

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

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the ConnectionManager
	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	//o Open the connection to the middleware
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client library version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Output information
	GMSEC_INFO("Issuing a request using the DEFAULT_REQUEST_SUBJECT '%s'", DEFAULT_REQUEST_SUBJECT);

	//o Create message
	requestMsg = messageCreate(DEFAULT_REQUEST_SUBJECT, GMSEC_REQUEST, status);
	checkStatus(status);
	//o Add fields to message
	messageAddStringField(requestMsg, "QUESTION", "Is there anyone out there?", status);
	checkStatus(status);
	messageAddStringField(requestMsg, "COMPONENT", "request", status);
	checkStatus(status);
	//o Display XML representation of request message
	GMSEC_INFO("Sending request message:\n%s", messageToXML(requestMsg, status));

	//o Send Request Message
	// Timeout periods:
	// -1 - Wait forever
	//  0 - Return immediately
	// >0 - Time in milliseconds before timing out
	replyMsg = connectionManagerRequest(connMgr, requestMsg, -1, -1, status);
	checkStatus(status);
	// Example error handling for calling request() with a timeout
	if(replyMsg)
	{
		// Display the XML string representation of the reply
		GMSEC_INFO("Received replyMsg:\n%s", messageToXML(replyMsg, status));
		checkStatus(status);
		//o Destroy the replyMsg message
		connectionManagerRelease(connMgr, &replyMsg, status);
		checkStatus(status);
	}

	//o Disconnect from the middleware and clean up the Connection
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	//o Destroy all variables created
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

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_open_resp.c
 *
 * This file contains an example demonstrating how to issue a request message
 * and handle a coinciding reply message when using the open-response
 * configuration option. This example program is intended to be run after
 * starting up the 'reply_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality. Note that by setting the
 * configuration option 'GMSEC-REQ-RESP' to 'open-resp' in the requester
 * automatically turns on the 'mw-expose-resp' option.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
const char* OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Message requestMsg;
	GMSEC_Message replyMsg;

	char* reply_subject;

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

	//o Ensure that the open-response is enabled
 	// Note: Other subscribing applications should set the configuration
 	// option 'mw-expose-resp' to 'true' in order to receive exposed replies
 	// By setting the configuration option 'GMSEC-REQ-RESP' to 'open-resp'
 	// here, it automatically enables the 'mw-expose-resp' option.
	configAddValue(config, "GMSEC-REQ-RESP", "OPEN-RESP", status);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	// If it was not specified in the command-line arguments, set LOGLEVEL
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

	//o Subscribe to the bus in preparation to receive the
	// open-response message (Because it will not be routed
	// to the reqeust() call)
	reply_subject = malloc(sizeof(char)*50);
	snprintf(reply_subject, sizeof(char)*50, "%s%s", OPEN_RESP_REPLY_SUBJECT, ".*");
	connectionManagerSubscribe(connMgr, reply_subject, status);
	checkStatus(status);

	//o Output information
	GMSEC_INFO("Issuing a request using the subject '%s'", OPEN_RESP_REQUEST_SUBJECT);

	//o Create message
	requestMsg = messageCreate(OPEN_RESP_REQUEST_SUBJECT, GMSEC_REQUEST, status);
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
	replyMsg = connectionManagerRequest(connMgr, requestMsg, 1000, GMSEC_REQUEST_REPUBLISH_NEVER, status);
	checkStatus(status);

	// Example error handling for calling request() with a timeout
	if (replyMsg)
	{
		// Display the XML string representation of the reply
		GMSEC_INFO("Received replyMsg:\n%s", messageToXML(replyMsg, status));

		//o Destroy the replyMsg message
		connectionManagerRelease(connMgr, &replyMsg, status);
		checkStatus(status);
	}

	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	messageDestroy(&requestMsg);
	free(reply_subject);
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

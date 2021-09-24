/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply.cpp
 *
 * This file contains an example demonstrating how to issue a reply to a
 * request message.
 *
 * This example program is intended to be run before starting up the
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
	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client library version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	connectionManagerSubscribe(connMgr, DEFAULT_REQUEST_SUBJECT, status);
	checkStatus(status);
	//o Call receive() to synchronously retrieve a message that has
	// been received by the middleware client libraries
	// Timeout periods:
	// -1 - Wait forever
	//  0 - Return immediately
	// >0 - Time in milliseconds before timing out
	requestMsg = connectionManagerReceive(connMgr, -1, status);
	checkStatus(status);

	// Example error handling for calling receive() with a timeout
	if (requestMsg)
	{
		//o Display the XML representation of the received message
		GMSEC_INFO("Received a message\n%s", messageToXML(requestMsg, status));

		//o Double-check the Message type to ensure that it is a request
		if(messageGetKind(requestMsg, status) == GMSEC_REQUEST)
		{
			GMSEC_Message replyMsg;
			const char* status_code;
			char reply_subject[50];

			//o Get the name of the component who issued the request
			const char* component = 0;

			//o Construct a Reply message
			{
				GMSEC_Field compField = messageGetStringField(requestMsg, "COMPONENT", status);
				component = fieldGetStringValue(compField, status);
				checkStatus(status);
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

			//o Set the reply subject.
			// See API Interface Specificaiton Document for
			// more information on Reply Message subjects.
			// Generally speaking, they are composed
			// accordingly:
			// [Spec].[Mission].[Satellite ID].
			// [Message Type].[Component Name].[Status Code]
			snprintf(reply_subject, 50, "GMSEC.MISSION.SAT_ID.RESP.REPLY.%s", status_code);

			//o Create reply message
			replyMsg = messageCreate(reply_subject, GMSEC_REPLY, status);
			checkStatus(status);

			//o Add fields to the reply message
			messageAddStringField(replyMsg, "ANSWER", "Yup, I'm here!", status);
			checkStatus(status);
			messageAddStringField(replyMsg, "COMPONENT", component, status);
			checkStatus(status);

			//o Display XML representation of the reply message
			GMSEC_INFO("Prepared Reply:\n%s", messageToXML(replyMsg, status));
			checkStatus(status);

			//o Send Reply
			connectionManagerReply(connMgr, requestMsg, replyMsg, status);
			checkStatus(status);

			//o Destroy reply
			messageDestroy(&replyMsg);
		}
		connectionManagerRelease(connMgr, &requestMsg, status);
		checkStatus(status);
	}

	//o Disconnect from the middleware and clean up the Connection
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	//o Destroy all variables created
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

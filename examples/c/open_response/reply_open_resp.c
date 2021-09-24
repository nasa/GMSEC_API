/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_open_resp.c
 *
 * This file contains an example demonstrating how to utilize the open
 * response functionality to expose reply messages to normal subscribers.
 *
 * This example program is intended to be run before executing the
 * 'request_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality.
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
int millisleep(unsigned ms);

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

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Ensure that the open-response is enabled
	configAddValue(config, "GMSEC-REQ-RESP", "OPEN-RESP", status);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	//o Output GMSEC API version
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	//o Open the connection to the middleware
	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client/wrapper version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	connectionManagerSubscribe(connMgr, OPEN_RESP_REQUEST_SUBJECT, status);
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
	if(requestMsg)
	{
		//o Display the XML representation of the received message
		GMSEC_INFO("Received a message\n%s", messageToXML(requestMsg, status));

		//o Double-check the Message type to ensure that it is a request
		if(messageGetKind(requestMsg, status) == GMSEC_REQUEST)
		{
			GMSEC_Message replyMsg;
			char* reply_subject;
			const char* status_code;

			//o Get the name of the component who issued the request
			const char* component = 0;

			//o Construct a Reply message
			{
				const GMSEC_Field compField = messageGetStringField(requestMsg, "COMPONENT", status);
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
			reply_subject = malloc(sizeof(char)*50);
			snprintf(reply_subject, sizeof(char)*50, "%s.%s", OPEN_RESP_REPLY_SUBJECT, status_code);

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

			//o Send Reply
			connectionManagerReply(connMgr, requestMsg, replyMsg, status);
			checkStatus(status);

			//o Wait for a moment to ensure that the reply is sent out
			{
				int i;
				for(i = 0; i < 10; i++)
				{
					timeUtilMillisleep(100);
				}
			}

			messageDestroy(&replyMsg);
			free(reply_subject);
		}

		connectionManagerRelease(connMgr, &requestMsg, status);
		checkStatus(status);
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

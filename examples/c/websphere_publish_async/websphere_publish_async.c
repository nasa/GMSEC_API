/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file websphere_publish_async.c
 *
 * This file contains an example which demonstrates how to utilize the
 * WebSphere MQ asynchronous publish functionality, as well as how to check on
 * the status of publish operations (Since the client does not wait to receive
 * the status of each operation in order to achieve asynchronicity) by
 * registering an EventCallback with the ConnectionManager.
 *
 * It is recommended that users execute the program GMSEC_API/bin/gmsub to
 * receive the messages that this example publishes.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
void populateMessage(GMSEC_Message message, int i, GMSEC_Status status);

//o Globals
GMSEC_BOOL eventFired = GMSEC_FALSE;

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
void asyncStatusCheckCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	// Print the status of publish operations.  This includes counts
	// for successes, warnings, and errors.
	GMSEC_INFO(statusGetReason(status));

	if (statusIsError(status))
	{
		GMSEC_ERROR("The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: %d", statusGetCustomCode(status));
	}

	eventFired = GMSEC_TRUE;
}


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Message message;
	int count;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	config = configCreateWithArgs(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Toggle the WebSphere MQ client libraries-level asynchronous publish
	// functionality on
	configAddValue(config, "MW-ASYNC-PUBLISH", "true", status);

	//o Toggle the periodic WebSphere MQ asynchronous publish status report
	// functionality on.  The GMSEC API Connection will periodically fire
	// off an Event which will trigger the onEvent function of an
	// EventCallback registered to the event labeled,
	// Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT
	configAddValue(config, "MW-ASYNC-STATUS-CHECK", "true", status);

	//o Additionally, the "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL"
	// configuration option can be used to instruct the GMSEC Connection
	// on how frequently (in number of publish operations) it should
	// periodically fire the WSMQ_ASYNC_STATUS_CHECK_EVENT. By default, it
	// will fire once every 100 messages.
	// For the sake of this example, we will use 500 messages.
	configAddValue(config, "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "500", status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	//o Connect
	connectionManagerInitialize(connMgr, status);

	//o Register the event callback with the connection to catch
	// the WebSphere asynchronous publish status events which are
	// eminated from the API
	connectionManagerRegisterEventCallback(connMgr, GMSEC_WSMQ_ASYNC_STATUS_CHECK_EVENT, asyncStatusCheckCallback, status);
	checkStatus(status);

	//o Output middleware version
	GMSEC_INFO("Middleware version = %s", connectionManagerGetLibraryVersion(connMgr, status));

	GMSEC_INFO("Publishing messages using the subject: %s", EXAMPLE_MESSAGE_SUBJECT);

	//o Create a GMSEC Message object
	message = messageCreate(EXAMPLE_MESSAGE_SUBJECT, GMSEC_PUBLISH, status);

	//o Publish message as quickly as possible
	// (i.e. No sleep operation between each publish operation)
	count = 0;
	while (!eventFired)
	{
		//o Populate the Message with fields, increment a
		// counter so that a publisher can track how many
		// messages were published (if they are interested)
		populateMessage(message, count++, status);

		//o Publish the message to the middleware bus
		connectionManagerPublish(connMgr, message, status);

		// Note: We do not recommend printing the output of a
		// message when publishing it while using the WebSphere
		// async publish functionality, as it is
		// counter-intuitive to take take up CPU resources
		// performing I/O operations when attempting to achieve
		// high message throughput rates. As such, if you want
		// to analyze the messages published by this program,
		// we recommend you execute GMSEC_API/bin/gmsub to
		// receive the messages.
	}

	GMSEC_INFO("Event detected, ending example demonstration");

	//o Clean up the ConnectionManager before exiting the program
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	connectionManagerDestroy(&connMgr);
	messageDestroy(&message);
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


void populateMessage(GMSEC_Message message, int value, GMSEC_Status status)
{
	// Note: Since C is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
	// Note: If a field of the same name is added to an existing message,
	// the value passed when calling addField will overwrite the existing
	// value.
	messageAddCharField(message, "CHAR-FIELD", 'c', status);
	checkStatus(status);
	messageAddBooleanField(message, "BOOL-FIELD-TRUE", GMSEC_TRUE, status);
	checkStatus(status);
	messageAddBooleanField(message, "BOOL-FIELD-FALSE", GMSEC_FALSE, status);
	checkStatus(status);
	messageAddI8Field(message, "I8-FIELD", (GMSEC_I8) value, status);
	checkStatus(status);
	messageAddI16Field(message, "I16-FIELD", (GMSEC_I16) value, status);
	checkStatus(status);
	messageAddI32Field(message, "I32-FIELD", (GMSEC_I32) value, status);
	checkStatus(status);
	messageAddI64Field(message, "I64-FIELD", (GMSEC_I64) value, status);
	checkStatus(status);
	messageAddU8Field(message, "U8-FIELD", (GMSEC_U8) value, status);
	checkStatus(status);
	messageAddU16Field(message, "U16-FIELD", (GMSEC_U16) value, status);
	checkStatus(status);
	messageAddU32Field(message, "U32-FIELD", (GMSEC_U32) value, status);
	checkStatus(status);
	messageAddU64Field(message, "U64-FIELD", (GMSEC_U64) value, status);
	checkStatus(status);
	messageAddStringField(message, "STRING-FIELD", "This is a test", status);
	checkStatus(status);
	messageAddF32Field(message, "F32-FIELD", (GMSEC_F32) (1 + 1. / value), status);
	checkStatus(status);
	messageAddF64Field(message, "F64-FIELD", (GMSEC_F64) (1 + 1. / value), status);
	checkStatus(status);
	messageAddBinaryField(message, "BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7, status);
	checkStatus(status);
}

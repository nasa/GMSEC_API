/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file message_bins.c
 *
 * This file contains an example describing how to aggregate multiple messages
 * into a singular message sent to the middleware server.  This functionality
 * is useful for cutting down on the number of messages sent across the
 * middleware bus, if there is an application in the system which is publishing
 * an exorbitantly large number of messages.
 *
 * It is recommended that you execute the application GMSEC_API/bin/gmsub
 * prior to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* EXAMPLE_MESSAGE_SUBJECT     = "GMSEC.AGGREGATE.PUBLISH";
const char* EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void populateMessage(GMSEC_Message message, int count, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

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

	config = configCreateWithArgs(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	configAddValue(config, "gmsec-msg-content-validate-all", "false", NULL);

	//o Enable Message Binning
	configAddValue(config, "GMSEC-USE-MSG-BINS", "true", status);
	//o Specify the number of messages to be aggregated prior to publishing
	// the aggregate message to the middleware server (This applies to all
	// of the messages which match the subject(s) provided in the
	// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
	// Note: The aggregate message will be sent to the middleware server
	// immediately upon this many messages being published, regardless of
	// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
	configAddValue(config, "GMSEC-MSG-BIN-SIZE", "10", status);

	//o Specify a timeout (in milliseconds) for the aggregate message to be
	// sent to the middleware server
	// Note: The aggregate message will be sent to the middleware server
	// after this period of time if the message bin does not fill up (per
	// the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
	configAddValue(config, "GMSEC-MSG-BIN-TIMEOUT", "5000", status);

	//o Specify the subjects to aggreate messages for.
	// Note: Subscription wildcard syntax can be used here, and has been
	// supported since GMSEC API version 4.3.
	configAddValue(config, "GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH", status);

	//o Specify any subjects that should be excluded from being aggregated
	// This is useful if a wildcard subscription is provided in one of the
	// GMSEC-MSG-BIN-SUBJECT-N parameters.
	configAddValue(config, "GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", EXAMPLE_BIN_EXCLUDE_SUBJECT, status);

	initializeLogging(config, status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	{
		int i;
		//o Create a message
		GMSEC_Message message = messageCreate(EXAMPLE_MESSAGE_SUBJECT, GMSEC_PUBLISH, status);
		checkStatus(status);

		for(i = 0; i < 5; i++)
		{
			populateMessage(message, i+1, status);

			//o Publish the message to the middleware bus
			connectionManagerPublish(connMgr, message, status);
			checkStatus(status);

			//o Display the XML string representation of the Message for
			// the sake of review
			GMSEC_INFO("Published message: %s", messageToXML(message, status));
		}

		messageDestroy(&message);
	}

	{
		//o Create a message
		GMSEC_Message message = messageCreate(EXAMPLE_BIN_EXCLUDE_SUBJECT, GMSEC_PUBLISH, status);
		checkStatus(status);

		populateMessage(message, 1, status);

		//o Publish the message to the middleware bus
		// Note: When calling publish(), if a message does NOT match
		// one of the subjects to be aggregated, it will be immediately
		// published
		connectionManagerPublish(connMgr, message, status);
		checkStatus(status);

		//o Display the XML string representation of the Message for
		// the sake of review
		GMSEC_INFO("Published message: %s", messageToXML(message, status));

		messageDestroy(&message);
	}

	//o Disconnect from the middleware and clean up the Connection
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

void populateMessage(GMSEC_Message message, int count, GMSEC_Status status){
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
	messageAddI8Field(message, "I8-FIELD", (GMSEC_I8) count, status);
	checkStatus(status);
	messageAddI16Field(message, "I16-FIELD", (GMSEC_I16) count, status);
	checkStatus(status);
	messageAddI32Field(message, "I32-FIELD", (GMSEC_I32) count, status);
	checkStatus(status);
	messageAddI64Field(message, "I64-FIELD", (GMSEC_I64) count, status);
	checkStatus(status);
	messageAddU8Field(message, "U8-FIELD", (GMSEC_U8) count, status);
	checkStatus(status);
	messageAddU16Field(message, "U16-FIELD", (GMSEC_U16) count, status);
	checkStatus(status);
	messageAddU32Field(message, "U32-FIELD", (GMSEC_U32) count, status);
	checkStatus(status);
	messageAddU64Field(message, "U64-FIELD", (GMSEC_U64) count, status);
	checkStatus(status);
	messageAddStringField(message, "STRING-FIELD", "This is a test", status);
	checkStatus(status);
	messageAddF32Field(message, "F32-FIELD", (GMSEC_F32) (1 + 1. / count), status);
	checkStatus(status);
	messageAddF64Field(message, "F64-FIELD", (GMSEC_F64) (1 + 1. / count), status);
	checkStatus(status);
	messageAddBinaryField(message, "BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7, status);
	checkStatus(status);
}

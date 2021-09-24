/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file publish.c
 *
 * This file contains an example describing how to create and publish a
 * message to the middleware bus using the lowest level function calls
 * available for such operations.
 *
 * This example program is intended to be run after starting the 'subscribe'
 * or 'subscribe_async' example program.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void populateMessage(GMSEC_Message message, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

int main (int argc, char* argv[])
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

	//o Create a ConnectionManager object
	// This is the linchpin for all communications between the
	// GMSEC API and the middleware server
	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	//o Open the connection to the middleware
	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client library version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Create a message
	// Disclaimer: This message is not based off of a Message
	// Definition outlined by the GMSEC Interface
	// Specification Document (ISD).  It is meant to be an example
	// to demonstrate the various capabilities of the GMSEC Message
	// and Field classes. The GMSEC Team recommends that you map
	// your data into the Messages defined in the GMSEC ISD, as
	// doing so will make your software "GMSEC Compliant" resulting
	// in plug-and-play type functionality with other GMSEC
	// compliant software.
	message = messageCreate(EXAMPLE_MESSAGE_SUBJECT, GMSEC_PUBLISH, status);
	populateMessage(message, status);
	checkStatus(status);

	//o Publish the message to the middleware bus
	connectionManagerPublish(connMgr, message, status);
	checkStatus(status);

	//o Display the XML string representation of the Message for
	// the sake of review
	GMSEC_INFO("Published message: %s", messageToXML(message, status));

	//o Disconnect from the middleware and clean up the Connection
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	//o Destroy all variables created
	messageDestroy(&message);
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

void populateMessage(GMSEC_Message message, GMSEC_Status status)
{
	// Add Fields of varying data types to the Message
	int value = 1;

	// Note: Since C is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
	// Note: If a field of the same name is added to an existing message,
	// the value passed when calling addField will overwrite the existing
	// value.
	messageAddCharField(message, "CHAR-FIELD", 'c', status);
	messageAddBooleanField(message, "BOOL-FIELD-TRUE", GMSEC_TRUE, status);
	messageAddBooleanField(message, "BOOL-FIELD-FALSE", GMSEC_FALSE, status);
	messageAddI8Field(message, "I8-FIELD", (GMSEC_I8) value, status);
	messageAddI16Field(message, "I16-FIELD", (GMSEC_I16) value, status);
	messageAddI32Field(message, "I32-FIELD", (GMSEC_I32) value, status);
	messageAddI64Field(message, "I64-FIELD", (GMSEC_I64) value, status);
	messageAddU8Field(message, "U8-FIELD", (GMSEC_U8) value, status);
	messageAddU16Field(message, "U16-FIELD", (GMSEC_U16) value, status);
	messageAddU32Field(message, "U32-FIELD", (GMSEC_U32) value, status);
	messageAddU64Field(message, "U64-FIELD", (GMSEC_U64) value, status);
	messageAddStringField(message, "STRING-FIELD", "This is a test", status);
	messageAddF32Field(message, "F32-FIELD", (GMSEC_F32) (1 + 1. / value), status);
	messageAddF64Field(message, "F64-FIELD", (GMSEC_F64) (1 + 1. / value), status);
	messageAddBinaryField(message, "BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7, status);
	checkStatus(status);
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

/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file connection_state.c
 *
 * This file contains an example which demonstrates how to utilize
 * EventCallbacks to track the state of the GMSEC Connection and its connection
 * to the middleware.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

//o Helper functions
void checkStatus(GMSEC_Status status);
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void populateMessage(GMSEC_Message message, int count, GMSEC_Status status);

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
void connectionStateCallback(GMSEC_ConnectionMgr connMgr, GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	// Perform some action based on the type of event that occurred
	if (event == GMSEC_CONN_SUCCESSFUL_EVENT)
	{
		GMSEC_INFO("Connected to the middleware server");
	}
	else if (event == GMSEC_CONN_BROKEN_EVENT)
	{
		GMSEC_INFO("Connection to the middleware lost or terminated");
	}
	else if (event == GMSEC_CONN_RECONNECT_EVENT)
	{
		GMSEC_INFO("Attempting to reestablish the connection to the middleware");
	}
}

int main (int argc, char* argv[])
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
	config = configCreateWithArgs(argc, argv);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// TODO: Once available, replace this statement with usage of
	// ConnectionManager::getAPIVersion (See RTC 4798)
	GMSEC_INFO(connectionGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	//o Connect
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Register the event callback with the connection to catch
	// connection state changes, including:
	// 1. Connection successfully established to middleware
	// 2. Connection broken from middleware
	// 3. Reconnecting to the middleware
	connectionManagerRegisterEventCallback(connMgr, GMSEC_CONN_SUCCESSFUL_EVENT, connectionStateCallback, status);
	checkStatus(status);
	connectionManagerRegisterEventCallback(connMgr, GMSEC_CONN_BROKEN_EVENT, connectionStateCallback, status);
	checkStatus(status);
	connectionManagerRegisterEventCallback(connMgr, GMSEC_CONN_RECONNECT_EVENT, connectionStateCallback, status);
	checkStatus(status);

	//o Output middleware version
	GMSEC_INFO("Middleware version = %s", connectionManagerGetLibraryVersion(connMgr, status));

	// Assuming that the user provided proper reconnection syntax
	// for the corresponding middleware they are using, one could
	// kill the middleware server at this point, then start it back
	// up to demonstrate the EventCallback.onEvent() function
	// triggering

	//o Wait for user input to end the program
	GMSEC_INFO("Waiting for Connection events to occur, press <enter> to exit the program");
	getchar();

	//o Clean up the ConnectionManager before exiting the program
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

void populateMessage(GMSEC_Message message, int count, GMSEC_Status status)
{
	// Note: Since C is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
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

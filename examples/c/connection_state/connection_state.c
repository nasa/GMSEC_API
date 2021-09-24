/*
 * Copyright 2007-2021 United States Government as represented by the
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


void connectionStateCallback(GMSEC_ConnectionMgr connMgr, GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	switch (event)
	{
	case GMSEC_CONN_SUCCESSFUL_EVENT:
		GMSEC_INFO("[event]: Connected to the middleware server");
		break;

	case GMSEC_CONN_BROKEN_EVENT:
		GMSEC_INFO("[event]: Connection to the middleware lost or terminated");
		break;

	case GMSEC_CONN_RECONNECT_EVENT:
		GMSEC_INFO("[event]: Attempting to reestablish the connection to the middleware");
		break;

	default:
		GMSEC_INFO("[event]: %s", statusGet(status));
		break;
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
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the Connection
	connMgr = connectionManagerCreate(config, status);
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

	//o Connect
	connectionManagerInitialize(connMgr, status);
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

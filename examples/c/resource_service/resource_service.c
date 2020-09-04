/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file resource_service.c
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
 * to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* RSRC_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.RESOURCE_SERVICE.RSRC";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connManager;
	GMSEC_Field* headerFields;
	size_t numFields = 4;
	GMSEC_Message rsrcMsg;
	size_t interval_s;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connManager = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connManager, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connManager, status));

	//o Create all of the GMSEC Message header Fields which will
	// be used by all GMSEC Messages
	headerFields = malloc(sizeof(GMSEC_Field) * numFields);

	headerFields[0] = f32FieldCreate("HEADER-VERSION", (GMSEC_F32) 2010.0, status);
	checkStatus(status);
	headerFields[1] = stringFieldCreate("MISSION-ID", "GMSEC", status);
	checkStatus(status);
	headerFields[2] = stringFieldCreate("FACILITY", "GMSEC Lab", status);
	checkStatus(status);
	headerFields[3] = stringFieldCreate("COMPONENT", "heartbeat_service", status);
	checkStatus(status);

	//o Use setStandardFields to define a set of header fields for
	// all messages which are created or published on the
	// ConnectionManager using the following functions:
	// createLogMessage, publishLog, createHeartbeatMessage,
	// startHeartbeatService, createResourceMessage,
	// publishResourceMessage, or startResourceMessageService
	connectionManagerSetStandardFields(connManager, headerFields, numFields, status);
	checkStatus(status);

	//o Create and publish a Resource message using
	// createResourceMessage() and publish()
	//
	// Note: This is useful for applications which may need to add
	// additional Fields to the Resource Messages which are not
	// currently added by the GMSEC API
	rsrcMsg = connectionManagerCreateResourceMessage(connManager, RSRC_MESSAGE_SUBJECT, 1, 10, status);
	checkStatus(status);
	GMSEC_INFO("Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n%s", messageToXML(rsrcMsg, NULL));
	connectionManagerPublish(connManager, rsrcMsg, status);
	checkStatus(status);
	messageDestroy(&rsrcMsg);


	//o Kick off the Resource Service -- This will publish resource
	// messages automatically every X seconds, where X is the second
	// parameter provided to the startResourceMessageService() function.
	// If an interval is not provided, the service will default to
	// publishing a message every 60 seconds.
	interval_s = 30;
	GMSEC_INFO("Starting the Resource Message service, a message will be published every %d seconds", interval_s);
	connectionManagerStartResourceMessageService(connManager, RSRC_MESSAGE_SUBJECT, interval_s, 1, 10, status);
	checkStatus(status);

	//o Wait for user input to end the program
	GMSEC_INFO("Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program");
	getchar();

	//o Stop the Heartbeat Service
	connectionManagerStopResourceMessageService(connManager, status);
	checkStatus(status);

	//o Cleanup
	connectionManagerCleanup(connManager, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	{
		size_t i;
		for(i = 0; i < numFields; i ++)
		{
			fieldDestroy(&headerFields[i]);
		}
		free(headerFields);
	}
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

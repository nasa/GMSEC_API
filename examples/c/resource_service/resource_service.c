/*
 * Copyright 2007-2021 United States Government as represented by the
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

const char* RSRC_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.RESOURCE-SERVICE.RSRC";
const int   RSRC_PUBLISH_RATE    = 5; // in seconds

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

int main(int argc, char* argv[])
{
	GMSEC_Status        status = statusCreate();
	GMSEC_Config        config;
	GMSEC_ConnectionMgr connManager;
	unsigned int        version;
	size_t              numFields;
	GMSEC_Field*        headerFields;
	GMSEC_Message       rsrcMsg;

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
	version = specificationGetVersion(connectionManagerGetSpecification(connManager, NULL), NULL);

	switch (version)
	{
	case 201400: numFields = 4; break;
	case 201600: numFields = 3; break;
	default:     numFields = 5; break;
	}
	headerFields = malloc(sizeof(GMSEC_Field) * numFields);

	headerFields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", NULL);
	headerFields[1] = stringFieldCreate("FACILITY", "MY-FACILITY", NULL);
	headerFields[2] = stringFieldCreate("COMPONENT", "RESOURCE-SERVICE", NULL);

	if (version == 201400)
	{
		headerFields[3] = stringFieldCreate("MSG-ID", "MY-MSG-ID", NULL);
	}
	else if (version >= 201800)
	{
		headerFields[3] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", NULL);
		headerFields[4] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", NULL);
	}

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
	GMSEC_INFO("Starting the Resource Message service, a message will be published every %d seconds", RSRC_PUBLISH_RATE);
	connectionManagerStartResourceMessageService(connManager, RSRC_MESSAGE_SUBJECT, RSRC_PUBLISH_RATE, 1, 10, status);
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

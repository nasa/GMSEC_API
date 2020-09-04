/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file heartbeat_service.c
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 * to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* HB_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.C2CX.HB";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connManager;
	size_t hNumFields = 3;
	GMSEC_Field* headerFields;
	size_t hbNumFields;
	GMSEC_Field* hbStandardFields;

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
	headerFields = malloc(sizeof(GMSEC_Field) * hNumFields);

	headerFields[0] = stringFieldCreate("MISSION-ID", "GMSEC", status);
	checkStatus(status);
	headerFields[1] = stringFieldCreate("FACILITY", "GMSEC Lab", status);
	checkStatus(status);
	headerFields[2] = stringFieldCreate("COMPONENT", "heartbeat_service", status);
	checkStatus(status);

	//o Use setStandardFields to define a set of header fields for
	// all messages which are created or published on the
	// ConnectionManager using the following functions:
	// createLogMessage, publishLog, createHeartbeatMessage,
	// startHeartbeatService, createResourceMessage,
	// publishResourceMessage, or startResourceMessageService
	connectionManagerSetStandardFields(connManager, headerFields, hNumFields, status);
	checkStatus(status);

	// Note: Fields are immutable, so plan appropriately if you wish
	// to re-use variable names!
	{
		GMSEC_Message hbMsg;
		int version;
		//o Create all of the GMSEC Message header Fields which
		// will be used by all GMSEC HB Messages
		hbNumFields = 3;
		hbStandardFields = malloc(sizeof(GMSEC_Field) * hbNumFields);

		//o Determine which version of the GMSEC message specification
		// the ConnectionManager was initialized with and add
		// the correctly typed Fields to the Message
		version = specificationGetVersion(connectionManagerGetSpecification(connManager, status), status);
		if (version == 201600)
		{
			hbStandardFields[0] = u16FieldCreate("PUB-RATE", (GMSEC_U16) 30, status);
			checkStatus(status);
			hbStandardFields[1] = u16FieldCreate("COUNTER",  (GMSEC_U16) 1, status);
			checkStatus(status);
		}
		else if (version == 201400)
		{
			hbStandardFields[0] = i16FieldCreate("PUB-RATE", (GMSEC_U16) 30, status);
			checkStatus(status);
			hbStandardFields[1] = i16FieldCreate("COUNTER",  (GMSEC_U16) 1, status);
			checkStatus(status);
		}

		//o Note: COMPONENT-STATUS is an optional field used to
		// denote the operating status of the component, the
		// values are as follows:
		// 0 - Debug
		// 1 - Normal / Green
		// 2 - Warning / Yellow
		// 3 - Orange
		// 4 - Error / Red
		hbStandardFields[2] = i16FieldCreate("COMPONENT-STATUS", (GMSEC_I16) 0, status);
		checkStatus(status);

		//o Create and publish a Heartbeat message using
		// createLogMessage() and publish()
		//
		// Note: This is useful for applications which may need
		// to create proxy heartbeats on behalf of a subsystem,
		// as creating multiple ConnectionManagers can consume
		// more memory than necessary.  In this case, extra
		// logic would need to be added to handle the timing of
		// the publications.
		hbMsg = connectionManagerCreateHeartbeatMessage(connManager, HB_MESSAGE_SUBJECT, hbStandardFields, hbNumFields, status);
		checkStatus(status);
		GMSEC_INFO("Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n%s", messageToXML(hbMsg, status));
		connectionManagerPublish(connManager, hbMsg, status);
		checkStatus(status);

		//o Kick off the Heartbeat Service -- This will publish
		// heartbeat messages automatically every X seconds,
		// where Xis the value which was provided for PUB-RATE
		// Note: If PUB-RATE was not provided, it will default
		// to 30 seconds per automatic Heartbeat publication
		GMSEC_INFO("Starting the Heartbeat service, a message will be published every %s seconds", fieldGetStringValue(hbStandardFields[0], status));
		connectionManagerStartHeartbeatService(connManager, HB_MESSAGE_SUBJECT, hbStandardFields, hbNumFields, status);
		checkStatus(status);

		messageDestroy(&hbMsg);
	}

	{
		//o Use setHeartbeatServiceField to change the state of the
		// COMPONENT-STATUS Field to indicate that the component has
		// transitioned from a startup/debug state to a running/green
		// state.
		GMSEC_Field componentStatusField = i16FieldCreate("COMPONENT-STATUS", (GMSEC_I16) 1, status);
		checkStatus(status);
		connectionManagerSetHeartbeatServiceField(connManager, componentStatusField, status);
		checkStatus(status);
		fieldDestroy(&componentStatusField);
	}

	//o Wait for user input to end the program
	GMSEC_INFO("Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program");
	getchar();

	connectionManagerStopHeartbeatService(connManager, status);
	checkStatus(status);

	connectionManagerCleanup(connManager, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	configDestroy(&config);
	statusDestroy(&status);
	fieldDestroy(&(headerFields[0]));
	fieldDestroy(&(headerFields[1]));
	fieldDestroy(&(headerFields[2]));
	free(headerFields);
	fieldDestroy(&(hbStandardFields[0]));
	fieldDestroy(&(hbStandardFields[1]));
	fieldDestroy(&(hbStandardFields[2]));
	free(hbStandardFields);

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

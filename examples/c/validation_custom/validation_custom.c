/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.c
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 */

#include <gmsec4_c.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif


const char* HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM";
const char* ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB";


//o Helper functions
void checkStatus(GMSEC_Status status);

void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void setupStandardFields(unsigned int specVersion);
void clearStandardFields();


void customMessageValidator(const GMSEC_Message msg, GMSEC_Status status)
{
	//o Get message type and subtype
	const char* type;
	const char* subtype;

	if (messageHasField(msg, "MESSAGE-TYPE", NULL) == GMSEC_TRUE)
	{
		type = messageGetStringValue(msg, "MESSAGE-TYPE", NULL);
	}
	if (messageHasField(msg, "C2CX-SUBTYPE", NULL) == GMSEC_TRUE)
	{
		subtype = messageGetStringValue(msg, "C2CX-SUBTYPE", NULL);
	}
	else if (messageHasField(msg, "MESSAGE-SUBTYPE", NULL) == GMSEC_TRUE)
	{
		subtype = messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL);
	}

	//o Ensure we have a Heartbeat message and it contains the PUB-RATE field
	if ((strncmp(type, "MSG", strlen("MSG")) == 0) &&
	    (strncmp(subtype, "HB", strlen("HB")) == 0) &&
	    (messageHasField(msg, "PUB-RATE", NULL) == GMSEC_TRUE))
	{
		GMSEC_I64 pubRate = messageGetIntegerValue(msg, "PUB-RATE", status);

		if (statusIsError(status) == GMSEC_FALSE && ((pubRate < 10) || (pubRate > 60)))
		{
			statusSet(status, GMSEC_API_MSG_ERROR, GMSEC_API_VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value", 0);
		}
	}
	else
	{
		statusSet(status, GMSEC_API_MSG_ERROR, GMSEC_API_INVALID_MSG, "Non-Heartbeat message received", 0);
	}
}


int main(int argc, char* argv[])
{
	GMSEC_Status        status  = NULL;
	GMSEC_Config        config  = NULL;
	GMSEC_ConnectionMgr connMgr = NULL;
	GMSEC_Message       msg     = NULL;
	unsigned int        specVersion;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	status = statusCreate();
	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

	//o Enable Message validation via connection configuration
	configAddValue(config, "gmsec-msg-content-validate-send", "true", NULL);

	GMSEC_INFO("API version: %s", connectionManagerGetAPIVersion());

	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	GMSEC_INFO("Middleware version: %s", connectionManagerGetLibraryVersion(connMgr, NULL));

	//o Register custom message validator
	connectionManagerRegisterMessageValidator(connMgr, customMessageValidator, NULL);

	//o Get the version of the message specification we are using
	specVersion = specificationGetVersion(connectionManagerGetSpecification(connMgr, NULL), NULL);

	//o Set up standard/common fields used with all messages
	setupStandardFields(specVersion);

	//o Create Heartbeat Message
	//o Note: Message subject and schema ID vary depending on the specification in use
	const char* subject  = (specVersion > C_GMSEC_ISD_2018_00 ? HB_MSG_SUBJECT : ALT_HB_MSG_SUBJECT);
	const char* schemaID = (specVersion > C_GMSEC_ISD_2018_00 ? "MSG.HB" : "MSG.C2CX.HB");

	msg = mistMessageCreate(subject, schemaID, connectionManagerGetSpecification(connMgr, NULL), status);
	checkStatus(status);

	//o Add PUB-RATE field with illegal value
	mistMessageSetValue(msg, "PUB-RATE", "5", status);
	checkStatus(status);

	//o For very old specifications, we need to add a MSG-ID field
	if (specVersion <= C_GMSEC_ISD_2014_00)
	{
		messageAddStringField(msg, "MSG-ID", "12345", status);
		checkStatus(status);
	}

	//o Attempt to publish malformed message
	GMSEC_INFO("Attempting to publish malformed message...");
	connectionManagerPublish(connMgr, msg, status);

	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_INFO("This is an expected error:\n%s", statusGet(status));
	}
	else
	{
		GMSEC_WARNING("Was expecting an error");
	}

	//o Fix PUB-RATE field with legal value
	mistMessageSetValue(msg, "PUB-RATE", "15", status);
	checkStatus(status);

	//o Publish a good message
	GMSEC_INFO("Attempting to publish a good message...");
	connectionManagerPublish(connMgr, msg, status);

	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_WARNING("Unexpected error:\n%s", statusGet(status));
	}
	else
	{
		GMSEC_INFO("Message published!");
	}

	//o Disconnect from the middleware and clean up the Connection
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	//o Clear standard/common fields used with all messages
	clearStandardFields();

	messageDestroy(&msg);
	connectionManagerDestroy(&connMgr);
	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}


void checkStatus(GMSEC_Status status)
{
	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("Fatal Error: %s", statusGet(status));
		exit(-1);
	}
}


void initializeLogging(GMSEC_Config config, GMSEC_Status status)
{
	const char* logLevel = configGetValue(config, "LOGLEVEL", status);
	const char* logFile  = configGetValue(config, "LOGFILE", status);

	if (!logLevel)
	{
		configAddValue(config, "LOGLEVEL", "INFO", status);
	}
	if (!logFile)
	{
		configAddValue(config, "LOGFILE", "STDOUT", status);
	}
}


void setupStandardFields(unsigned int specVersion)
{
	size_t numFields = (specVersion <= C_GMSEC_ISD_2016_00 ? 6 : 8);
	size_t i;

	GMSEC_Field* definedFields = malloc(sizeof(GMSEC_Field) * numFields);

	definedFields[0] = stringFieldCreate("MISSION-ID", "MISSION", NULL);
	definedFields[1] = stringFieldCreate("CONSTELLATION-ID", "CONSTELLATION", NULL);
	definedFields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "SATELLITE", NULL);
	definedFields[3] = stringFieldCreate("SAT-ID-LOGICAL", "SATELLITE", NULL);
	definedFields[4] = stringFieldCreate("FACILITY", "GMSEC-LAB", NULL);
	definedFields[5] = stringFieldCreate("COMPONENT", "VALIDATION-CUSTOM", NULL);

	if (specVersion >= C_GMSEC_ISD_2018_00)
	{
		definedFields[6] = stringFieldCreate("DOMAIN1", "DOMAIN1", NULL);
		definedFields[7] = stringFieldCreate("DOMAIN2", "DOMAIN2", NULL);
	}

	mistMessageSetStandardFields(definedFields, numFields, NULL);

	for (i = 0; i < numFields; ++i)
	{
		fieldDestroy(&definedFields[i]);
	}

	free(definedFields);
}


void clearStandardFields()
{
	mistMessageClearStandardFields();
}

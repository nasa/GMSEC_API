/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file log_message.c
 *
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant LOG message to the middleware bus using the Messaging
 * Interface Standardization Toolkit (MIST) namespace ConnectionManager class
 * function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* GMSEC_SPEC_VERSION  = "201600";
const char* LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connManager;
	size_t numFields = 4;
	GMSEC_Field* definedFields;
	GMSEC_Message logMsg;
	char eventTimeBuf[GMSEC_TIME_BUFSIZE];

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

	//o Set the GMSEC message specification version to be used to determine
	// what the structure of messages is for verification and the
	// construction of MistMessages
	configAddValue(config, "GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION, status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connManager = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connManager, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connManager, status));

	//o Begin the steps necessary to create a GMSEC-compliant LOG
	// message using the ConnectionManager

	//o Create all of the GMSEC Message header Fields which will
	// be used by all GMSEC Messages
	//
	// Note: Since these Fields contain variable values which are
	// based on the context in which they are used, they cannot be
	// automatically populated using MistMessage.
	definedFields = malloc(sizeof(GMSEC_Message) * numFields);

	definedFields[0] = stringFieldCreate("MISSION-ID", "MISSION", status);
	checkStatus(status);
	// Note: SAT-ID-PHYSICAL is an optional header Field, according
	// to the GMSEC ISD.
	definedFields[1] = stringFieldCreate("SAT-ID-PHYSICAL", "SPACECRAFT", status);
	checkStatus(status);
	definedFields[2] = stringFieldCreate("FACILITY", "GMSEC Lab", status);
	checkStatus(status);
	definedFields[3] = stringFieldCreate("COMPONENT", "log_message", status);
	checkStatus(status);

	//o Use setStandardFields to define a set of header fields for
	// all messages which are created or published on the
	// ConnectionManager using the following functions:
	// createLogMessage, publishLog, createHeartbeatMessage,
	// startHeartbeatService, createResourceMessage,
	// publishResourceMessage, or startResourceMessageService
	connectionManagerSetStandardFields(connManager, definedFields, numFields, status);
	checkStatus(status);

	//o Use MistMessage to construct a GMSEC LOG message based off
	// of the latest XML templates provided with the GMSEC API.
	// This will automatically populate the Message with all of the
	// Fields which have specific values defined for them in the XML
	// template files.  For more information on which Fields have
	// values defined for them, please review the XML template files
	// located in GMSEC_API/templates.
	//
	// Note: The second parameter is an identifier for the type of
	// message to construct.
	logMsg = mistMessageCreate(LOG_MESSAGE_SUBJECT, "MSG.LOG", connectionManagerGetSpecification(connManager, status), status);
	checkStatus(status);

	//o Add the LOG-specific fields to the LOG message
	//
	// Note: Since these Fields contain variable values which are
	// based on the context in which they are used, they cannot be
	// automatically populated using MistMessage.
	timeUtilFormatTime(timeUtilGetCurrentTime(), eventTimeBuf);

	messageAddI16Field(logMsg, "SEVERITY", (GMSEC_I16) 1, status);
	checkStatus(status);
	mistMessageSetValue(logMsg, "MSG-TEXT", "Creating an example GMSEC LOG Message", status);
	checkStatus(status);
	mistMessageSetValue(logMsg, "OCCURRENCE-TYPE", "SYS", status);
	checkStatus(status);
	mistMessageSetValue(logMsg, "SUBCLASS", "AST", status);
	checkStatus(status);
	mistMessageSetValue(logMsg, "EVENT-TIME", eventTimeBuf, status);
	checkStatus(status);

	//o Add the standard fields to the LOG message
	connectionManagerAddStandardFields(connManager, logMsg, status);
	checkStatus(status);

	connectionManagerPublish(connManager, logMsg, status);
	checkStatus(status);

	GMSEC_INFO("Published LOG message:\n%s", messageToXML(logMsg, status));

	connectionManagerCleanup(connManager, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	messageDestroy(&logMsg);
	{
		int i;
		for(i = 0; i < numFields; i++)
		{
			fieldDestroy(&(definedFields[i]));
		}
	}
	free(definedFields);
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

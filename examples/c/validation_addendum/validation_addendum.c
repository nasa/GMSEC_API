/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_addendum.cpp
 *
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif



//o Note: The name 'EXAMPLE' in this subject represents the name of the message
// specification which is being used.  If using message definitions outlined in
// the GMSEC Interface Specification Document, this would instead be, "GMSEC."
const char* EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
char* addTimeToString(const char* preface);


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
	size_t numFields = 4;
	GMSEC_Field* definedFields;
	GMSEC_Message message;
	GMSEC_Message badMessage;
	char* timestring;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

	//o Enable Message validation.  This parameter is "false" by default.
	configAddValue(config, "GMSEC-MSG-CONTENT-VALIDATE", "true", NULL);

	//o Tell the API that there is an additional layer of message schema to
	// validate (The 'EXAMPLE' message definitions).  This value is set to
	// 0 (Only 'GMSEC' specification validation) by default.
	//
	// Note: These levels for validation are determined by the "LEVEL-X"
	// attributes defined in the .DIRECTORY.xml file contained in the XML
	// templates directory.  In thise case, Level-0 means GMSEC and Level-1
	// means EXAMPLE.
	//
	// Note: The GMSEC team envisions using message specifications in a
	// layered hierarchical fashion.  For example, the "GMSEC" message
	// specification would be 'layer 0', followed by an organization-level
	// message specification at 'layer 1' which builds upon the message
	// specification outlined in the GMSEC ISD.  This would be followed by
	// a mission-level message specification at 'layer 2' and so on.
	configAddValue(config, "GMSEC-SCHEMA-LEVEL", "1", NULL);

	//o Tell the API where to find all of the schema files.
	//
	// Note: This example only demonstrates a simple usage case of this
	// functionality.  When using this functionality, if the intent is
	// to use any of the GMSEC message definitions, then ALL of the XML
	// template files must be contained in the same directory.
	// e.g. GMSEC_API/templates/2016.00 (Or the location defined in
	// GMSEC-SCHEMA-PATH)
	configAddValue(config, "GMSEC-SCHEMA-PATH", "templates", NULL);

	//o Since this example relies on the 2016.00 version of the templates,
	//  we indicate such within the configuration object.
	configAddValue(config, "GMSEC-SPECIFICATION-VERSION", "201600", NULL);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	definedFields = malloc(sizeof(GMSEC_Field) * numFields);

	definedFields[0] = stringFieldCreate("MISSION-ID", "MISSION", status);
	checkStatus(status);
	definedFields[1] = stringFieldCreate("SAT-ID-PHYSICAL", "SPACECRAFT", status);
	checkStatus(status);
	definedFields[2] = stringFieldCreate("FACILITY", "GMSEC Lab", status);
	checkStatus(status);
	definedFields[3] = stringFieldCreate("COMPONENT", "log_message", status);
	checkStatus(status);

	connectionManagerSetStandardFields(connMgr, definedFields, numFields, status);
	checkStatus(status);

	//o Create a Message using a subject defined in the XML template
	// outlining our example addendum message definitions
	message = mistMessageCreate(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connectionManagerGetSpecification(connMgr, status), status);
	checkStatus(status);

	//o Add all of the necessary Fields to our message
	messageAddU16Field(message, "NUM-OF-EVENTS", (GMSEC_U16) 2, status);
	checkStatus(status);
	timestring = addTimeToString("AOS occurred at: ");
	mistMessageSetValue(message, "EVENT.1", timestring, status);
	free(timestring);
	checkStatus(status);
	timestring = addTimeToString("Telemetry downlink began at: ");
	mistMessageSetValue(message, "EVENT.2", timestring, status);
	free(timestring);
	checkStatus(status);

	connectionManagerAddStandardFields(connMgr, message, status);
	checkStatus(status);

	//o Publish the message to the middleware bus
	connectionManagerPublish(connMgr, message, status);
	checkStatus(status);

	//o Display the XML string representation of the Message for
	// the sake of review
	GMSEC_INFO("Published message:\n%s", messageToXML(message, NULL));

	//o Setup a new message without some of the Required Fields and
	// attempt to publish it (i.e. Trigger a validation failure)
	badMessage = mistMessageCreate(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connectionManagerGetSpecification(connMgr, NULL), status);
	connectionManagerPublish(connMgr, badMessage, status);

	if(statusIsError(status))
	{
		GMSEC_ERROR("This error is expected:\n%s", statusGet(status));
	}

	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	connectionManagerDestroy(&connMgr);
	messageDestroy(&message);
	messageDestroy(&badMessage);
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

char* addTimeToString(const char* preface)
{
	char* result = malloc(strlen(preface) + 22);
	char theTime[22];
	timeUtilFormatTime(timeUtilGetCurrentTime(), theTime);
	snprintf(result, strlen(preface) + 22, "%s%s", preface, theTime);
	return result;
}

/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file specification_addendum.c
 * 
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Helper functions
void checkStatus(const char* operation, GMSEC_Status status);
void setStandardFields(GMSEC_MessageFactory factory, const char* component);
char* addTimeToString(const char* preface);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options from the command line.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);


	// Enable Message validation. This setting is "false" by default.
	configAddValue(config, "GMSEC-MSG-CONTENT-VALIDATE-SEND", "true", NULL);


	// Tell the API that there is an additional layer of message schema to
	// validate (The 'EXAMPLE' message definitions). By default, this value
	// is set to 0 (only 'C2MS' specification validation).
	//
	// Note: These levels for validation are determined by the "LEVEL-X"
	// attributes defined in the DIRECTORY.xml file contained in the XML
	// templates directory. In the case of this example, Level-0 implies
	// C2MS, and Level-1 is our example extension.
	//
	// Note: The GMSEC team envisions using message specifications in a
	// layered hierarchical fashion. For example, the "C2MS" message
	// specification would be 'layer 0', followed by an organization-level
	// message specification at 'layer 1' which builds upon the message
	// specification outlined in C2MS.  This would be followed by a mission
	// level message specification at 'layer 2' (e.g. GMSEC) and so on.
	configAddValue(config, "GMSEC-SCHEMA-LEVEL", "1", NULL);


	// Tell the API where to find all of the schema files.
	//
	// Note: This example only demonstrates a simple usage case of this
	// functionality. When using this functionality, if the intent is
	// to use any of the GMSEC message definitions, then ALL of the XML
	// template files must be contained in the same directory.
	// e.g. GMSEC_API/templates/2019.00 (or the location defined in
	// GMSEC-SCHEMA-PATH)
	configAddValue(config, "GMSEC-SCHEMA-PATH", "templates", NULL);


	// Since this example relies on the 2019.00 version of the templates,
	// we indicate such within the configuration object.
	configAddValue(config, "GMSEC-SPECIFICATION-VERSION", "201900", NULL);


	// Create connection instance using custom configuration.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	setStandardFields(connectionGetMessageFactory(conn, NULL), "SPECIFICATION-ADDENDUM");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Output information regarding the API version and connection.
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));


	// Create a LOG Message using a subject defined in the XML template
	// outlining our example addendum message definitions.
	GMSEC_Message logMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", status);
	checkStatus("messageFactoryCreateMessage", status);

	// Add all of the required fields to the LOG message.
	messageAddU16Field(logMsg, "NUM-OF-EVENTS", (GMSEC_U16) 2, NULL);

	char* event = addTimeToString("AOS occurred at: ");
	messageAddStringField(logMsg, "EVENT.1.DESC", event, NULL);
	free(event);

	event = addTimeToString("Telemetry downlink began at: ");
	messageAddStringField(logMsg, "EVENT.2.DESC", event, NULL);
	free(event);

	// Publish the LOG message to the GMSEC Bus.
	connectionPublish(conn, logMsg, status);
	checkStatus("connectionPublish", status);

	// Display the XML string representation of the LOG message just published.
	GMSEC_INFO("Published custom LOG Message:\n%s", messageToXML(logMsg, NULL));


	// Setup a new LOG message without the NUM-OF-EVENTS required field and
	// attempt to publish it (i.e. trigger a validation failure)
	GMSEC_Message badLogMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", status);
	checkStatus("messageFactoryCreateMessage", status);

	connectionPublish(conn, badLogMsg, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_INFO("This validation error is expected for the incomplete LOG message:\n%s", statusGet(status));
	}
	else
	{
		GMSEC_WARNING("Surprisingly was able to publish an invalid message: %s", messageToXML(badLogMsg, NULL));
	}

	// Disconnect from the GMSEC Bus.
	connectionDisconnect(conn, NULL);

	// Cleanup resources
	messageDestroy(&badLogMsg);
	messageDestroy(&logMsg);
	connectionDestroy(&conn);
	configDestroy(&config);
	statusDestroy(&status);

	connectionShutdownAllMiddlewares();

	return 0;
}


void checkStatus(const char* operation, GMSEC_Status status)
{
	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("%s failed; reason: %s", operation, statusGet(status));
		exit(-1);
	}
}


// Convenience function for associating standard (i.e. common) fields
// with the MessageFactory which will be included automatically within
// all messages created using the factory.
void setStandardFields(GMSEC_MessageFactory factory, const char* component)
{
	GMSEC_Field fields[8] = {0};
	size_t      numFields = sizeof(fields) / sizeof(GMSEC_Field);
	size_t      i;

	fields[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	fields[1] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[2] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	fields[3] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST", GMSEC_TRUE, NULL);
	fields[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	fields[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	fields[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	fields[7] = stringFieldCreate("COMPONENT", component, GMSEC_TRUE, NULL);

	messageFactorySetStandardFields(factory, fields, numFields, NULL);

	for (i = 0; i < numFields; ++i)
	{
		fieldDestroy(&fields[i]);
	}
}


char* addTimeToString(const char* preface)
{
	GMSEC_Time theTime;
	timeUtilFormatTime(timeUtilGetCurrentTime(), theTime);

	size_t bufLen = strlen(preface) + strlen(theTime) + 1;
	char*  event  = malloc(bufLen);

	assert(event != NULL);

	snprintf(event, bufLen - 1, "%s%s", preface, theTime);

	return event;
}

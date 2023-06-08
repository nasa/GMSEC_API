/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file publish_subject_filter.c
 * 
 * This file contains an example publishing a few messages with different
 * subjects in order to demonstrate the subject filtering functionality of the
 * API. 
 *
 * This example program is intended to be run after starting the
 * 'subscribe_subject_filter' example program.
 */


#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


// Helper function
void checkStatus(const char* operation, GMSEC_Status status);
void setStandardFields(GMSEC_MessageFactory factory, const char* component);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	setStandardFields(connectionGetMessageFactory(conn, NULL), "PUBLISH-SUBJECT-FILTER");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);
		
	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Create a valid Log message using the MessageFactory.
	GMSEC_Message logMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "LOG", status);
	checkStatus("messageFactoryCreateMessage", status);

	// Add required fields
	messageAddStringField(logMsg, "SUBCLASS", "INFO", NULL);
	messageAddStringField(logMsg, "OCCURRENCE-TYPE", "TEST", NULL);
	messageAddI16Field(logMsg, "SEVERITY", (GMSEC_I16) 1, NULL);
	messageAddStringField(logMsg, "MSG-TEXT", "Hello subscriber!", NULL);

	// And the current (event) time
	GMSEC_Time theTime;
	timeUtilFormatTime(timeUtilGetCurrentTime(), theTime);
	messageAddStringField(logMsg, "EVENT-TIME", theTime, NULL);

	// Create a valid Heartbeat message using the MessageFactory.
	GMSEC_Message hbMsg = messageFactoryCreateMessage(connectionGetMessageFactory(conn, NULL), "HB", status);

	GMSEC_INFO("Publishing two messages -- Log message will be received by the subscriber,"
		       " the Heartbeat message will be filtered out");

	// Publish the Log message
	connectionPublish(conn, logMsg, status);
	checkStatus("connectionPublish", status);
	GMSEC_INFO("Published LOG message:\n%s", messageToXML(logMsg, NULL));

	// Delay a bit
	timeUtilMillisleep(2000);

	// Publish the Heartbeat message
	connectionPublish(conn, hbMsg, status);
	checkStatus("connectionPublish", status);
	GMSEC_INFO("Published HB message:\n%s", messageToXML(hbMsg, NULL));

	// Disconnect from the GMSEC Bus.
	connectionDisconnect(conn, NULL);

	// Cleanup resources
	messageDestroy(&hbMsg);
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
		GMSEC_INFO("%s failed; reason: %s", operation, statusGet(status));
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

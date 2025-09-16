/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file custom_validation.c
 *
 * @brief This example program demonstrates how to implement additional
 * message validation logic in addition to that which the GMSEC API
 * provides.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


void checkStatus(const char* operation, GMSEC_Status status);
void setStandardFields(GMSEC_MessageFactory factory, const char* component);


// Define a custom message validator
void customMessageValidator(GMSEC_Message msg, GMSEC_Status status)
{
	// Check if we have a Heartbeat message, and if so, then check if
	// the PUB-RATE has an acceptable value.
	if (messageHasField(msg, "MESSAGE-SUBTYPE", NULL) == GMSEC_TRUE &&
	    strncmp(messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL), "HB", 2) == 0)
	{
		if (messageHasField(msg, "PUB-RATE", NULL) == GMSEC_TRUE)
		{
			GMSEC_I32 pubRate = messageGetI32Value(msg, "PUB-RATE", NULL);

			if (pubRate < 10 || pubRate > 60)
			{
				statusSet(status, GMSEC_MSG_ERROR, GMSEC_VALUE_OUT_OF_RANGE,
				           "PUB-RATE field does not have a valid value", 0);
			}
		}
		else
		{
			statusSet(status, GMSEC_MSG_ERROR, GMSEC_MISSING_REQUIRED_FIELD,
			          "PUB-RATE field not found", 0);
		}
	}
}


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to the bus
	configAddValue(config, "gmsec-msg-content-validate-send", "true", NULL);

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	GMSEC_MessageFactory factory = connectionGetMessageFactory(conn, NULL);
	setStandardFields(factory, "CUSTOM-VALIDATION");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Register custom message validator
	messageFactoryRegisterMessageValidator(factory, &customMessageValidator, NULL);
	checkStatus("messageFactoryRegisterMessageValidator", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Create a Heartbeat message using the MessageFactory.
	// Set both required and optional fields within the message.
	GMSEC_Message hbMsg = messageFactoryCreateMessage(factory, "HB", NULL);

	// Publish the message without PUB-RATE (expecting custom validation error)
	GMSEC_INFO("Publishing Message:\n%s", messageToXML(hbMsg, NULL));
	connectionPublish(conn, hbMsg, status);
	assert(statusHasError(status) == GMSEC_TRUE);
	GMSEC_INFO("This error is expected: %s", statusGet(status));

	// Publish the message with invalid PUB-RATE value (expecting custom validation error)
	messageSetFieldValueU32(hbMsg, "PUB-RATE", (GMSEC_U32) 5, NULL);

	GMSEC_INFO("Publishing Message:\n%s", messageToXML(hbMsg, NULL));
	connectionPublish(conn, hbMsg, status);
	assert(statusHasError(status) == GMSEC_TRUE);
	GMSEC_INFO("This error is expected: %s", statusGet(status));

	// Publish the message with valid PUB-RATE value (expecting no validation error)
	messageSetFieldValueU32(hbMsg, "PUB-RATE", (GMSEC_U32) 10, NULL);

	GMSEC_INFO("Publishing Message:\n%s", messageToXML(hbMsg, NULL));
	connectionPublish(conn, hbMsg, status);
	assert(statusHasError(status) == GMSEC_FALSE);
	GMSEC_INFO("Message is valid!");

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
	connectionDisconnect(conn, NULL);

	// Cleanup resources
	messageDestroy(&hbMsg);
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

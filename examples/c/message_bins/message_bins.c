/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_bins.c
 *
 * @brief This file contains an example describing how to aggregate multiple
 * messages into a singular message sent to the middleware server. This
 * functionality is useful for reducing the number of messages sent across
 * the * middleware bus, if there is an application which is publishing an
 * exorbitantly large number of (small) messages.
 *
 * It is recommended to execute the application GMSEC_API/bin/gmsub prior
 * to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdlib.h>


void checkStatus(const char* operation, GMSEC_Status status);

void setStandardFields(GMSEC_MessageFactory factory, const char* component);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subject and excluded subject for message(s) that will be binned and for those that will not
	const char* msg_bin_subject;
	const char* msg_bin_exclude_subject;
	GMSEC_I32 level = configGetIntegerValueOrDefault(config, "gmsec-schema-level", (GMSEC_I32) GMSEC_SCHEMA_LEVEL_0, NULL);

	switch (level)
	{
	case GMSEC_SCHEMA_LEVEL_0:
		msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
		break;

	case GMSEC_SCHEMA_LEVEL_1:
		msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
		break;

	case GMSEC_SCHEMA_LEVEL_2:
	default:
		msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
		break;
	}

	// Validate messages before being sent to the bus
	configAddValue(config, "gmsec-msg-content-validate-send", "true", NULL);

	// Configure to enable message binning
	configAddValue(config, "gmsec-use-msg-bins", "true", NULL);

	// Specify the number of messages to be aggregated prior to publishing
	// the aggregate message to the middleware server. This applies to all
	// of the messages which match the subject(s) provided in the
	// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
	// Note: The aggregate message will be sent to the middleware server
	// immediately upon this many messages being published, regardless of
	// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
	configAddValue(config, "gmsec-msg-bin-size", "10", NULL);

	// Specify a timeout (in milliseconds) for the aggregate message to be
	// sent to the GMSEC Bus.
	// Note: The aggregate message will be sent to the GMSEC Bus after this
	// period of time if the message bin does not fill up (per the value
	// provided for gmsec-msg-bin-size) prior to this timeout
	configAddValue(config, "gmsec-msg-bin-timeout", "5000", NULL);

	// Specify the message subject(s) of messages that should be aggregated.
	// Subscription wildcard syntax can be used here.
	// Note: If this option is not specified, then ALL messages will be
	// aggregated (except those that are excluded).
	configAddValue(config, "gmsec-msg-bin-subject-1", msg_bin_subject, NULL);

	// Specify any message subject(s) that should be excluded from being
	// aggregated.
	// This is useful if a wildcard subscription is provided in one of the
	// gmsec-msg-bin-subject-N parameters.
	configAddValue(config, "gmsec-msg-bin-exclude-subject-1", msg_bin_exclude_subject, NULL);

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Set up standard fields within the MessageFactory associated
	// with the connection object.
	GMSEC_MessageFactory factory = connectionGetMessageFactory(conn, NULL);
	setStandardFields(factory, "MESSAGE-BINS");

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Create a Log messages using the MessageFactory.
	// Set both required and optional fields within the message.
	GMSEC_Message logMsg1 = messageFactoryCreateMessage(factory, "LOG", NULL);
	GMSEC_Message logMsg2 = messageFactoryCreateMessage(factory, "LOG", NULL);

	messageAddStringField(logMsg1, "SUBCLASS", "AAA", NULL);
	messageAddStringField(logMsg1, "OCCURRENCE-TYPE", "PLOT", NULL);
	messageAddI16Field(logMsg1, "SEVERITY", (GMSEC_I16) 1, NULL);
	messageAddStringField(logMsg1, "MSG-TEXT", "Message will be binned", NULL);

	messageAddStringField(logMsg2, "SUBCLASS", "TLM", NULL);
	messageAddStringField(logMsg2, "OCCURRENCE-TYPE", "DATA", NULL);
	messageAddI16Field(logMsg2, "SEVERITY", (GMSEC_I16) 1, NULL);
	messageAddStringField(logMsg2, "MSG-TEXT", "Message will NOT be binned", NULL);

	int i;
	for (i = 0; i < 5; ++i)
	{
		GMSEC_Time theTime;
		timeUtilFormatTime(timeUtilGetCurrentTime(), theTime);

		messageAddStringField(logMsg1, "EVENT-TIME", theTime, NULL);
		messageAddStringField(logMsg2, "EVENT-TIME", theTime, NULL);

		// Publish the first message; it will be aggregated and published at a later time
		connectionPublish(conn, logMsg1, status);
		checkStatus("connectionPublish", status);
		GMSEC_INFO("Publishing AAA Log Message %d (will be aggregated):\n%s", (i+1), messageToXML(logMsg1, NULL));

			// Publish the second message; it will NOT be aggregated and will be published immediately
		connectionPublish(conn, logMsg2, status);
		checkStatus("connectionPublish", status);
		GMSEC_INFO("Publishing TLM Log Message %d (will be NOT aggregated):\n%s", (i+1), messageToXML(logMsg2, NULL));

		// Add processing delay
		timeUtilMillisleep(100);
	}

	// Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
	GMSEC_INFO("Waiting for binned messages to be published...");
	timeUtilMillisleep(6000);

	// Disconnect from the GMSEC Bus, and terminate subscriptions.
	// Note: Any remaining messages in message bins will be published
	// before disconnecting from the GMSEC Bus, thus ensuring no message
	// loss occurs.
	connectionDisconnect(conn, NULL);

	// Cleanup resources
	messageDestroy(&logMsg1);
	messageDestroy(&logMsg2);
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

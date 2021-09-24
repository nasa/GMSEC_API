/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file product_message.c
 *
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 * Messaging Interface Standardization Toolkit (MIST) namespace
 * ConnectionManager class function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

const char* PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

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
	GMSEC_ProductFile externalFile;
	GMSEC_ProductFile binaryFile;
	GMSEC_Message productMessage;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

	//o Enable Message validation.  This parameter is "false" by default.
	configAddValue(config, "GMSEC-MSG-CONTENT-VALIDATE", "true", status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connManager = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connManager, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connManager, status));

	//o Create all of the GMSEC Message header Fields which will
	// be used by all GMSEC Messages
	//
	// Note: Since these Fields contain variable values which are
	// based on the context in which they are used, they cannot be
	// automatically populated using MistMessage.
	definedFields = malloc(sizeof(GMSEC_Field) * numFields);

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

	//o Create a ProductFile object with the product name,
	// description, version, file format, and the URI
	externalFile = productFileCreate("External File", "External File Description", "1.0.0", "TXT", "//hostname/dir/filename", status);
	checkStatus(status);

	{
		size_t fSize = 8;
		unsigned char filePayload[8];
		unsigned char idx;
		for(idx = 0; idx < fSize; idx++)
		{
			filePayload[idx] = idx;
		}

		//o Create a ProductFile object with the product name,
		// description, version, format, binary array, and file size
		binaryFile = productFileCreateUsingData("File as Binary", "Binary File Description", "1.0.0", "BIN", filePayload, fSize, status);
		checkStatus(status);
	}

	//o Create a Product File Message with the subject,
	// RESPONSE-STATUS Field value, Message type (publish, request,
	// or reply), PROD-TYPE Field value, PROD-SUBTYPE Field value,
	// and pass it the Specification object from the Connection
	// Manager
	productMessage = productFileMessageCreateWithSpec(PROD_MESSAGE_SUBJECT, GMSEC_SUCCESSFUL_COMPLETION, GMSEC_PUBLISH, "AUTO", "DM", connectionManagerGetSpecification(connManager, status), status);
	checkStatus(status);
	productFileMessageAddProductFile(productMessage, externalFile, status);
	checkStatus(status);
	productFileMessageAddProductFile(productMessage, binaryFile, status);
	checkStatus(status);

	connectionManagerAddStandardFields(connManager, productMessage, status);
	checkStatus(status);

	connectionManagerPublish(connManager, productMessage, status);
	checkStatus(status);

	GMSEC_INFO("Published DEV message:\n%s", messageToXML(productMessage, status));

	connectionManagerCleanup(connManager, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	messageDestroy(&productMessage);
	productFileDestroy(&externalFile);
	productFileDestroy(&binaryFile);
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

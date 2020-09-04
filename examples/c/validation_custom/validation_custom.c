/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file validation_custom.c
 *
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 *
 * Note: This example focuses on adding additional validation upon the receipt
 * of a message.  It almost goes without saying that additional logic can be
 * added to a program prior to invoking the publish() function without having
 * to do anything special.
 */

#include <gmsec4_c.h>
#include <gmsec4/c/mist/product_file_iterator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);
void setupStandardFields(GMSEC_ConnectionMgr connMgr, GMSEC_Status status);
GMSEC_Message createProductFileMessage(const GMSEC_ConnectionMgr connMgr, const char* filePath, GMSEC_Status status);
GMSEC_BOOL isProdMsg(GMSEC_Message message, GMSEC_Status status);


void validationCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message message)
{
	GMSEC_Status status = statusCreate();
	//o Run the message through the GMSEC API-supplied validation
	specificationValidateMessage(connectionManagerGetSpecification(connMgr, status), message, status);
	checkStatus(status);

	//o In this example scenario, we are expecting to receive a
	// GMSEC PROD message containing a URI to a location on the disk
	// where a product file has been placed for retrieval.  In this
	// case, we want to validate that the location on the disk is
	// in an area which we would expect (i.e. Something that the
	// team has agreed upon prior to operational deployment).
	//
	// By validating this URI, we ensure that no malicious users
	// have infiltrated the system and somehow modified the messages
	// to cause us to retrieve a file from an unknown location.

	//o Start by checking to ensure that this is a PROD message
	if(isProdMsg(message, status))
	{
		GMSEC_ProductFileIterator prodIter;
		GMSEC_Message prodMessage = productFileMessageCreateUsingData(messageToXML(message, NULL), status);
		checkStatus(status);

		//o Extract the Product File URI location(s) from the
		// message using a ProductFileIterator
		prodIter = productFileMessageGetIterator(prodMessage, status);
		checkStatus(status);

		while(productFileIteratorHasNext(prodIter, status))
		{
			const char* prodUri;
			GMSEC_ProductFile prodFile = productFileIteratorNext(prodIter, status);
			checkStatus(status);

			//o Check to ensure that the URI contains "//hostname/dir"
			prodUri = productFileGetURI(prodFile, status);
			checkStatus(status);
			if(strstr(prodUri, "//hostname/dir") == NULL)
			{
				const char* msgXML = messageToXML(message, NULL);
				size_t buff_size = strlen(msgXML) + 50;
				char* errorMsg = malloc(buff_size);
				snprintf(errorMsg, buff_size, "Received an invalid PROD Message (bad URI):\n%s", msgXML);
				statusSet(status, GMSEC_API_MIST_ERROR, GMSEC_API_MESSAGE_FAILED_VALIDATION, errorMsg, 0);
				GMSEC_ERROR(statusGet(status));
				statusReset(status);
				free(errorMsg);
			}
		}

		messageDestroy(&prodMessage);

		if (statusIsError(status) == GMSEC_FALSE)
		{
			GMSEC_INFO("Received a valid message:\n%s", messageToXML(message, NULL));
		}
	}

	statusDestroy(&status);
}



int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;
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

	// TODO: Once available, replace this statement with usage of
	// ConnectionManager::getAPIVersion (See RTC 4798)
	GMSEC_INFO(connectionGetAPIVersion());

	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Set up the ValidationCallback and subscribe
	connectionManagerSubscribeWithCallback(connMgr, PROD_MESSAGE_SUBJECT, validationCallback, status);
	checkStatus(status);

	//o Start the AutoDispatcher
	connectionManagerStartAutoDispatch(connMgr, status);
	checkStatus(status);

	//o Create and publish a simple Product File Message
	setupStandardFields(connMgr, status);

	productMessage = createProductFileMessage(connMgr, "//hostname/dir/filename", status);

	//o Publish the message to the middleware bus
	connectionManagerPublish(connMgr, productMessage, status);
	checkStatus(status);

	messageDestroy(&productMessage);

	productMessage = createProductFileMessage(connMgr, "//badhost/dir/filename", status);

	connectionManagerPublish(connMgr, productMessage, status);
	checkStatus(status);

	messageDestroy(&productMessage);

	connectionManagerStopAutoDispatch(connMgr, status);
	checkStatus(status);

	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	connectionManagerDestroy(&connMgr);
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

void setupStandardFields(GMSEC_ConnectionMgr connMgr, GMSEC_Status status)
{
	size_t numFields = 4;
	GMSEC_Field* definedFields = malloc(sizeof(GMSEC_Field) * numFields);

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

	connectionManagerSetStandardFields(connMgr, definedFields, numFields, status);
	checkStatus(status);

	fieldDestroy(&definedFields[0]);
	fieldDestroy(&definedFields[1]);
	fieldDestroy(&definedFields[2]);
	fieldDestroy(&definedFields[3]);

	free(definedFields);
}

GMSEC_Message createProductFileMessage(const GMSEC_ConnectionMgr connMgr, const char* filePath, GMSEC_Status status)
{
	GMSEC_Message productMessage;
	GMSEC_ProductFile externalFile = productFileCreate("External File", "External File Description", "1.0.0", "TXT", filePath, status);
	checkStatus(status);

	productMessage = productFileMessageCreateWithSpec(PROD_MESSAGE_SUBJECT, GMSEC_SUCCESSFUL_COMPLETION, GMSEC_PUBLISH, "AUTO", "DM", connectionManagerGetSpecification(connMgr, status), status);
	checkStatus(status);
	productFileMessageAddProductFile(productMessage, externalFile, status);
	productFileDestroy(&externalFile);
	checkStatus(status);

	connectionManagerAddStandardFields(connMgr, productMessage, status);
	checkStatus(status);

	return productMessage;
}

GMSEC_BOOL isProdMsg(const GMSEC_Message message, GMSEC_Status status)
{
	GMSEC_BOOL result =
		strcmp(messageGetStringValue(message, "MESSAGE-TYPE", status), "MSG") == 0
		&& strcmp(messageGetStringValue(message, "MESSAGE-SUBTYPE", status), "PROD") == 0;

	return result;
}

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file mnemonic_message.c
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

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* GMSEC_SPEC_VERSION   = "201600";
const char* MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE";

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
	GMSEC_MnemonicSample mSample;
	size_t numSamples = 1;
	GMSEC_MnemonicSample* mnemonic_samples;
	GMSEC_Mnemonic mnemonic;
	GMSEC_Field statusField;
	GMSEC_Message mvalMessage;
	int version;

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

	//o Enable Message validation.  This parameter is "false" by default.
	configAddValue(config, "GMSEC-MSG-CONTENT-VALIDATE", "true", status);

	GMSEC_INFO(connectionManagerGetAPIVersion());

	connManager = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connManager, status);

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

	mSample = mnemonicSampleCreate("MS1", i32FieldCreate("MS1", 15, status), status);
	checkStatus(status);

	mnemonicSampleSetEUValue(mSample, f32FieldCreate("My EU", 15.0, status), status);
	checkStatus(status);

	mnemonicSampleSetFlags(mSample, 1, status);
	checkStatus(status);

	mnemonicSampleSetLimit(mSample, GMSEC_RED_HIGH, status);
	checkStatus(status);

	// Implicitly set limit enable/disable with setting of limit
	mnemonicSampleSetQuality(mSample, GMSEC_TRUE, status);
	checkStatus(status);

	mnemonicSampleSetStalenessStatus(mSample, GMSEC_FALSE, status);
	checkStatus(status);

	mnemonicSampleSetTextValue(mSample, "15", status);
	checkStatus(status);

	mnemonic_samples = malloc(sizeof(GMSEC_MnemonicSample) * numSamples);
	mnemonic_samples[0] = mSample;

	//o Add the Mnemonic values to a Mnemonic object
	mnemonic = mnemonicCreate("M1", mnemonic_samples, numSamples, status);
	checkStatus(status);
	statusField = i16FieldCreate("status", 5, status);
	checkStatus(status);
	mnemonicSetStatus(mnemonic, statusField, status);
	checkStatus(status);
	mnemonicSetUnits(mnemonic, "units", status);
	checkStatus(status);


	version = specificationGetVersion(connectionManagerGetSpecification(connManager, status), status);
	checkStatus(status);

	//o Construct an MVAL Message and add the Mnemonic values to it
	mvalMessage = mnemonicMessageCreateWithSpec(MVAL_MESSAGE_SUBJECT, "MSG.MVAL", connectionManagerGetSpecification(connManager, status), status);
	checkStatus(status);
	mnemonicMessageAddMnemonic(mvalMessage, mnemonic, status);
	checkStatus(status);

	//o If validating with the 2014 spec, the MSG-ID field is
	// required
	if (version == 201400)
	{
		mistMessageSetValue(mvalMessage, "MSG-ID", "MVAL Request MSG-ID would go here", status);
		checkStatus(status);
	}

	//o Add the header fields to the MVAL message
	connectionManagerAddStandardFields(connManager, mvalMessage, status);
	checkStatus(status);

	GMSEC_INFO("Publishing MVAL message:\n%s", messageToXML(mvalMessage, status));
	connectionManagerPublish(connManager, mvalMessage, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	messageDestroy(&mvalMessage);
	mnemonicDestroy(&mnemonic);
	{
		int i;
		for(i = 0; i < numSamples; i++)
		{
			mnemonicSampleDestroy(&(mnemonic_samples[i]));
		}
	}
	free(mnemonic_samples);
	fieldDestroy(&statusField);
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

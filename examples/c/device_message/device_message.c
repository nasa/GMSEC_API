/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file device_message.cpp
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

const char* DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEV.DEVICE_MESSAGE.DEV";

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
	GMSEC_Field paramVal;
	GMSEC_DeviceParam param;
	GMSEC_Device device1;
	GMSEC_Field devInfo;
	GMSEC_Field devNum;
	GMSEC_Message devMessage;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	config = configCreateWithArgs(argc, argv);

	initializeLogging(config, status);

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
	definedFields[3] = stringFieldCreate("COMPONENT", "device_message", status);
	checkStatus(status);

	//o Use setStandardFields to define a set of header fields for
	// all messages which are created or published on the
	// ConnectionManager using the following functions:
	// createLogMessage, publishLog, createHeartbeatMessage,
	// startHeartbeatService, createResourceMessage,
	// publishResourceMessage, or startResourceMessageService
	connectionManagerSetStandardFields(connManager, definedFields, numFields, status);
	checkStatus(status);

	paramVal = i32FieldCreate("DEVICE.1.PARAM.1.VALUE", (GMSEC_I32) 79, status);
	checkStatus(status);
	param = deviceParamCreate("DEV parameter 1", "parameter 1 timestamp", paramVal, status);
	checkStatus(status);

	device1 = deviceCreate("device 1", GMSEC_RED, status);
	checkStatus(status);
	deviceSetGroup(device1, "group", status);
	checkStatus(status);
	deviceSetRole(device1, "role", status);
	checkStatus(status);
	deviceSetModel(device1, "model", status);
	checkStatus(status);
	deviceSetSerial(device1, "1138", status);
	checkStatus(status);
	deviceSetVersion(device1, "1.4.5.2.3.4.5", status);
	checkStatus(status);
	devInfo = i16FieldCreate("info", 5, status);
	deviceSetInfo(device1, devInfo, status);
	checkStatus(status);
	devNum = i16FieldCreate("num", 5, status);
	deviceSetNumber(device1, devNum, status);
	checkStatus(status);
	deviceAddParam(device1, param, status);
	checkStatus(status);

	//o Construct an DEV Message and add the Device values to it
	devMessage = deviceMessageCreateWithSpec(DEV_MESSAGE_SUBJECT, connectionManagerGetSpecification(connManager, status), status);
	checkStatus(status);
	deviceMessageAddDevice(devMessage, device1, status);
	checkStatus(status);

	connectionManagerAddStandardFields(connManager, devMessage, status);
	checkStatus(status);

	connectionManagerPublish(connManager, devMessage, status);
	checkStatus(status);

	GMSEC_INFO("Published DEV message:\n%s", messageToXML(devMessage, status));

	connectionManagerCleanup(connManager, status);
	checkStatus(status);

	connectionManagerDestroy(&connManager);
	messageDestroy(&devMessage);
	deviceDestroy(&device1);
	fieldDestroy(&devInfo);
	fieldDestroy(&devNum);
	fieldDestroy(&(definedFields[0]));
	fieldDestroy(&(definedFields[1]));
	fieldDestroy(&(definedFields[2]));
	fieldDestroy(&(definedFields[3]));
	free(definedFields);
	deviceParamDestroy(&param);
	fieldDestroy(&paramVal);
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

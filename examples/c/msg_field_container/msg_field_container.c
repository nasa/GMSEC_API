/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file msg_field_container.c
 *
 *
 * This file contains an example demonstrating the creation of GMSEC Messages
 * using the two available container types for Field objects: Binary Tree Map
 * (STL map) or Hash Map (unordered map).
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

//o Helper functions
GMSEC_Config initializeLogging(GMSEC_Status status);
void createMsgUsingConfigFile(const char* filename, GMSEC_Status status);
void createMsgUsingConfigObject(GMSEC_Status status);
GMSEC_Message buildMessage(GMSEC_Config config, size_t numFields, GMSEC_Status status);
void displayMessage(GMSEC_Message msg, const char* description, GMSEC_Status status);
void checkStatus(GMSEC_Status status);

int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;

	if (argc <= 1)
	{
		printf("usage: %s <config-file>\n", argv[0]);
		printf("\nPlease see the GMSEC_API/example/cpp/message_field_container directory for an example xml configuration file.");
		return -1;
	}

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	config = initializeLogging(status);

	//o Demonstration of setting the configuration options via
	// configuration file
	createMsgUsingConfigFile(argv[1], status);

	//o Demonstration of hard-coding the configuration options
	// using a Config object
	createMsgUsingConfigObject(status);

	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}


void createMsgUsingConfigFile(const char* filename, GMSEC_Status status)
{
	GMSEC_ConfigFile cfgFile;
	GMSEC_Config cfg;
	int i;

	//o Load and parse configuration file
	cfgFile = configFileCreate();
	checkStatus(status);
	configFileLoad(cfgFile, filename, status);
	checkStatus(status);

	//o Retrieve config from file
	cfg = configFileLookupConfig(cfgFile, "config", status);
	checkStatus(status);
	configDestroy(&cfg);

	//o Loop through three different examples outlined in the configuration
	// file provided with this example
	for(i = 0; i < 3; ++i)
	{
		char* msgName;
		GMSEC_Message msg;
		const char* description;

		//o Form message name (e.g. msg1) to reference.
		msgName = malloc(sizeof(char)*10);
		snprintf(msgName, sizeof(char)*10, "msg%d", i+1);

		//o Parse the message from the configuration file and load it
		// into a GMSEC Message
		msg = configFileLookupMessage(cfgFile, msgName, status);
		checkStatus(status);
		free(msgName);

		//o Display the message
		description = 0;
		switch (i+1)
		{
			case 1: description = "Expecting TREE map (at size limit of 8 fields):"; break;
			case 2: description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
			case 3: description = "Expecting HASH map (size limit is ignored):"; break;
		}

		displayMessage(msg, description, status);

		messageDestroy(&msg);
	}

	configFileDestroy(&cfgFile);
}

void createMsgUsingConfigObject(GMSEC_Status status)
{
	GMSEC_Config cfg1, cfg2, cfg3, empty;
	GMSEC_Message msg1, msg2, msg3, msg4, msg5, msg6, msg7;

	cfg1 = configCreate();
	cfg2 = configCreate();
	cfg3 = configCreate();
	empty = configCreate();

	configAddValue(cfg1, "gmsec-msgfld-store-type", "tree", status);
	checkStatus(status);
	//o If the Message object is using a binary tree map to store fields,
	// the 'gmsec-msgfield-store-size' configuration option indicates how
	// many fields should be allowed to be stored before the Message
	// object converts to using a hash map (Default is 50).
	configAddValue(cfg1, "gmsec-msgfld-store-size", "10", status);
	checkStatus(status);

	configAddValue(cfg2, "gmsec-msgfld-store-type", "hash", status);
	checkStatus(status);

	configAddValue(cfg3, "gmsec-msgfld-store-type", "tree", status);
	checkStatus(status);
	configAddValue(cfg3, "gmsec-msgfld-store-size", "0", status);
	checkStatus(status);

	msg1 = buildMessage(cfg1,   9, status);   // uses bin-tree container map (under size limit)
	msg2 = buildMessage(cfg1,  10, status);   // uses bin-tree container map (at size limit)
	msg3 = buildMessage(cfg1,  11, status);   // uses hash container map (exceeds size limit)
	msg4 = buildMessage(cfg2,   5, status);   // default to using hash container map (size limit ignored)
	msg5 = buildMessage(cfg3,  55, status);   // always uses bin-tree container map
	msg6 = buildMessage(empty, 50, status);   // uses bin-tree container map (default size limit not exceeded)
	msg7 = buildMessage(empty, 51, status);   // uses hash container map (exceeds default size limit)

	displayMessage(msg1, "Message using TREE container map (under limit):", status);
	displayMessage(msg2, "Message using TREE container map (at max limit):", status);
	displayMessage(msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):", status);
	displayMessage(msg4, "Message using HASH container map (limit ignored):", status);
	displayMessage(msg5, "Message using TREE container map (limit ignored):", status);
	displayMessage(msg6, "Message using TREE container map (at limit):", status);
	displayMessage(msg7, "Message using HASH container map (limit exceeded):", status);

	messageDestroy(&msg1);
	messageDestroy(&msg2);
	messageDestroy(&msg3);
	messageDestroy(&msg4);
	messageDestroy(&msg5);
	messageDestroy(&msg6);
	messageDestroy(&msg7);

	configDestroy(&cfg1);
	configDestroy(&cfg2);
	configDestroy(&cfg3);
	configDestroy(&empty);
}

GMSEC_Message buildMessage(GMSEC_Config config, size_t numFields, GMSEC_Status status)
{
	GMSEC_Message msg = messageCreateWithConfig("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", GMSEC_PUBLISH, config, status);
	checkStatus(status);

	if (numFields > 0)
	{
		size_t i;
		// Add Fields
		for (i = 0; i < numFields; ++i)
		{
			char* oss = malloc(sizeof(char)*50);
			snprintf(oss, sizeof(char)*50, "FIELD-%s%lu", (i < 10 ? "00" : (i < 100 ? "0" : "")), i);

			messageAddU64Field(msg, oss, (GMSEC_U64) i, status);
			checkStatus(status);
			free(oss);
		}
	}

	return msg;
}

void displayMessage(GMSEC_Message msg, const char* description, GMSEC_Status status)
{
	const char* xml = messageToXML(msg, status);
	GMSEC_INFO("%s\n%s", description, xml);
}

GMSEC_Config initializeLogging(GMSEC_Status status)
{
	// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
	// object, it affects the logging level and output destination for all
	// logging macros in the entire API, regardless of whether that Config
	// object is kept around or not. In this case, we are creating a local
	// Config object just for the sake of initializing logging.
	GMSEC_Config config = configCreate();

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

	return config;
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

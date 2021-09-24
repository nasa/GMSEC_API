/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_field_iterator.c
 *
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>

	#define snprintf sprintf_s
#endif

const char* DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.>";

//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
const char* typeToString(GMSEC_FieldType type, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


void fieldIterationCallback(GMSEC_ConnectionMgr connection, const GMSEC_Message message)
{
	GMSEC_Status status = statusCreate();
	GMSEC_MessageFieldIterator iter;
	GMSEC_INFO("Received a message with subject: %s", messageGetSubject(message, status));

	GMSEC_INFO("Field Name (Field Type): Field Value");
	iter = messageGetFieldIterator(message, status);
	checkStatus(status);

	while(messageFieldIteratorHasNext(iter, status))
	{
		const GMSEC_Field field = messageFieldIteratorNext(iter, status);
		checkStatus(status);

		//o Extract the Field Name, Type, and Value (As
		// a string, to print)
		//
		// Note: 'getter' functions are also defined for
		// Integer, Unsigned Integer, and Double values.
		GMSEC_INFO("%s (%s): %s", fieldGetName(field, status), typeToString(fieldGetType(field, status), status), fieldGetStringValue(field, status));
		checkStatus(status);

		//o Field objects can also be converted to
		// specific Field types prior to retrieval of
		// the value contained in the Field.  This is
		// useful for ensuring that data types do not
		// lose any level of precision, but requires
		// a more intricate implementation.
	}
}


int main(int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_Config config;
	GMSEC_ConnectionMgr connMgr;

	if (argc <= 1)
	{
		printf("usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which
	// is used to pass configuration options into objects such
	// as Connection objects, ConnectionManager objects, Subscribe
	// and Publish calls, Message objects, etc.
	config = configCreateWithArgs(argc, argv);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config, status);

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO(connectionManagerGetAPIVersion());

	connMgr = connectionManagerCreate(config, status);
	checkStatus(status);

	GMSEC_INFO("Opening the connection to the middleware server");
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	GMSEC_INFO("Subscribing to the topic: %s", DEFAULT_SUBSCRIPTION_SUBJECT);
	connectionManagerSubscribeWithCallback(connMgr, DEFAULT_SUBSCRIPTION_SUBJECT, fieldIterationCallback, status);
	checkStatus(status);

	connectionManagerStartAutoDispatch(connMgr, status);
	checkStatus(status);

	//o Wait for user input to end the program
	GMSEC_INFO("Listening for Messages indefinitely, press <enter> to exit the program");
	getchar();

	//o Clean up
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

const char* typeToString(GMSEC_FieldType type, GMSEC_Status status)
{
	const char* ret_string;

	switch (type)
	{
	  case GMSEC_BOOL_TYPE:
		ret_string = "BOOL";
		break;

	  case GMSEC_BIN_TYPE:
		ret_string = "BIN";
		break;

	  case GMSEC_CHAR_TYPE:
		ret_string = "CHAR";
		break;

	  case GMSEC_I8_TYPE:
		ret_string = "I8";
		break;

	  case GMSEC_I16_TYPE:
		ret_string = "I16";
		break;

	  case GMSEC_I32_TYPE:
		ret_string = "I32";
		break;

	  case GMSEC_I64_TYPE:
		ret_string = "I64";
		break;

	  case GMSEC_F32_TYPE:
		ret_string = "F32";
		break;

	  case GMSEC_F64_TYPE:
		ret_string = "F64";
		break;

	  case GMSEC_STRING_TYPE:
		ret_string = "STRING";
		break;

	  case GMSEC_U8_TYPE:
		ret_string = "U8";
		break;

	  case GMSEC_U16_TYPE:
		ret_string = "U16";
		break;

	  case GMSEC_U32_TYPE:
		ret_string = "U32";
		break;

	  case GMSEC_U64_TYPE:
		ret_string = "U64";
		break;

	  default:
		{
			char oss[50];
			snprintf(oss, 50, "Unsupported Field TYPE: %d", type);
			statusSet(status, GMSEC_API_FIELD_ERROR, GMSEC_API_UNKNOWN_FIELD_TYPE, oss, 0);
		}

	}

	return ret_string;
}

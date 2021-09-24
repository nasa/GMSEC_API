/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file logging.c
 *
 * This file contains an example outlining the use of the GMSEC API logging
 * framework.
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

//o Helper functions
void publishTestMessage(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


//o Define a baseline LogHandler
// This will be used by the Log macros; The implementation of onMessage
// determines how messages will be logged to output
void baseHandler(const char* wai, const GMSEC_LogEntry* entry)
{
	char tempBuffer[50];
	timeUtilFormatTime(entry->time, tempBuffer);

	printf("[BaseHandler::onMessage] for: %s : %s [%s] [%s:%d] %s\n",
		wai, tempBuffer, logLevelToString(entry->level),
		entry->file, entry->line, entry->message);

	fflush(stdout);
}

// Different Handlers can be implemented for each logging level, if so desired
void errorHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("GMSEC_ERROR", entry);
}

void warningHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("GMSEC_WARNING", entry);
}

void infoHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("GMSEC_INFO", entry);
}

void verboseHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("GMSEC_VERBOSE", entry);
}

void debugHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("GMSEC_DEBUG", entry);
}

void anyHandler(const GMSEC_LogEntry* entry)
{
	baseHandler("ANY_HANDLER", entry);
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

	config = configCreateWithArgs(argc, argv);


	//o Register log handlers
	logRegisterHandlerByLevel(logERROR, &errorHandler);
	logRegisterHandlerByLevel(logWARNING, &warningHandler);
	logRegisterHandlerByLevel(logINFO, &infoHandler);
	logRegisterHandlerByLevel(logVERBOSE, &verboseHandler);
	logRegisterHandlerByLevel(logDEBUG, &debugHandler);

	//o Set logging reporting level
	logSetReportingLevel(logVERBOSE);
	GMSEC_VERBOSE("The log reporting level is now set to: %d", logGetReportingLevel());

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Create the ConnectionManager
	connMgr = connectionManagerCreateUsingValidation(config, GMSEC_FALSE, status);
	checkStatus(status);

	//o Connect
	connectionManagerInitialize(connMgr, status);
	checkStatus(status);

	//o Output middleware client library version
	GMSEC_INFO(connectionManagerGetLibraryVersion(connMgr, status));

	//o Publish a message
	publishTestMessage(connMgr, "GMSEC.TEST.PUBLISH", status);

	//o Disconnect from the middleware and clean up the Connection
	connectionManagerCleanup(connMgr, status);
	checkStatus(status);

	connectionManagerDestroy(&connMgr);

	//o Unregister log handlers
	logRegisterHandler((GMSEC_LogHandler*) NULL);

	//o Set log stream to stderr
	configAddValue(config, "LOGFILE", "STDERR", status);
	GMSEC_INFO("%s%s%s%s", "This message should go to stderr, not stdout.  ",
	     "For example, in bash test by running as:\n",
	     "./logging mw-id=bolt 2> testfile.txt\n",
	     "... and then check the contents of testfile.txt");

	//o Reset log stream to stdout
	configAddValue(config, "LOGFILE", "STDOUT", status);

	GMSEC_ERROR("This is an example error message.");
	GMSEC_WARNING("This is an example warning message.");
	GMSEC_VERBOSE("This is an example \"verbose\" message.");
	GMSEC_DEBUG("This is an example debug message which should not show.");

	// This last message cannot be shown right now because
	// Log::setReportingLevel(logVERBOSE), used above, does not
	// allow DEBUG messages to come out.
	GMSEC_VERBOSE("This is another example \"verbose\" message.");

	// Set logging reporting level to now allow DEBUG messages to be shown
	logSetReportingLevel(logDEBUG);
	if (logGetReportingLevel() == logDEBUG)
	{
		GMSEC_INFO("Changed reporting level to logDEBUG");
	}
	else
	{
		GMSEC_ERROR("Failed to change reporting level to logDEBUG");
	}

	// The DEBUG message below will be shown successfully, unlike the last
	// debug message.
	GMSEC_DEBUG("This is an example debug message which should show.");

	GMSEC_DEBUG("NONE reporting level, numerically, is %d", logLevelFromString("NONE"));
	GMSEC_DEBUG("ERROR reporting level, numerically, is %d", logLevelFromString("ERROR"));
	GMSEC_DEBUG("SECURE reporting level, numerically, is %d", logLevelFromString("SECURE"));
	GMSEC_DEBUG("WARNING reporting level, numerically, is %d", logLevelFromString("WARNING"));
	GMSEC_DEBUG("INFO reporting level, numerically, is %d", logLevelFromString("INFO"));
	GMSEC_DEBUG("VERBOSE reporting level, numerically, is %d", logLevelFromString("VERBOSE"));
	GMSEC_DEBUG("DEBUG reporting level, numerically, is %d", logLevelFromString("DEBUG"));

	//o Register general-purpose handler and test
	logRegisterHandler(&anyHandler);

	GMSEC_ERROR("NONE reporting level, numerically, is %d", logLevelFromString("NONE"));
	GMSEC_ERROR("ERROR reporting level, numerically, is %d", logLevelFromString("ERROR"));
	GMSEC_WARNING("WARNING reporting level, numerically, is %d", logLevelFromString("WARNING"));
	GMSEC_INFO("INFO reporting level, numerically, is %d", logLevelFromString("INFO"));
	GMSEC_VERBOSE("VERBOSE reporting level, numerically, is %d", logLevelFromString("VERBOSE"));
	GMSEC_DEBUG("DEBUG reporting level, numerically, is %d", logLevelFromString("DEBUG"));

	//o Unregister log handlers
	logRegisterHandler((GMSEC_LogHandler*) NULL);

	configDestroy(&config);
	statusDestroy(&status);
	return 0;
}

void publishTestMessage(GMSEC_ConnectionMgr connMgr, const char* subject, GMSEC_Status status){
	int i = 123;

	//o Create a Message object
	GMSEC_Message message = messageCreate(subject, GMSEC_PUBLISH, status);
	checkStatus(status);

	messageAddBooleanField(message, "F", GMSEC_FALSE, status);
	checkStatus(status);
	messageAddI32Field(message, "I", i, status);
	checkStatus(status);
	messageAddU16Field(message, "K", i, status);
	checkStatus(status);
	messageAddStringField(message, "S", "This is a test", status);
	checkStatus(status);
	messageAddF32Field(message, "D", (1 + 1.f/i), status);
	checkStatus(status);
	messageAddBinaryField(message, "X", (GMSEC_BIN) "JLMNOPQ", 7, status);
	checkStatus(status);

	connectionManagerPublish(connMgr, message, status);
	checkStatus(status);

	GMSEC_INFO("Sent:\n%s", messageToXML(message, status));

	messageDestroy(&message);
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

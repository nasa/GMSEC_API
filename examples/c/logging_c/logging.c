/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file logging.c
 *
 * A C example demonstration of GMSEC Logging Framework.
 *
 */

#include "../example.h"


typedef struct
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    message;
} logging_t;


void logging_c_printUsage()
{
	printf("\nusage: logging_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\titerations=<iterations>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


void logging_CommonHandler(const char* from, const GMSEC_LogEntry* entry)
{
	GMSEC_Time timeBuffer;

	timeUtilFormatTime(entry->time, timeBuffer);

	printf("On %s Message:\n", from);
	printf("%s [%s] [%s:%d] %s\n\n",
		timeBuffer, logLevelToString(entry->level), entry->file, entry->line, entry->message);

	fflush(stdout);
}


void logging_ErrorLogHandler(const GMSEC_LogEntry* entry)
{
	logging_CommonHandler("ERROR", entry);
}


void logging_WarningLogHandler(const GMSEC_LogEntry *entry)
{
	logging_CommonHandler("WARNING", entry);
}


void logging_InfoLogHandler(const GMSEC_LogEntry *entry)
{
	logging_CommonHandler("INFO", entry);
}


void logging_VerboseLogHandler(const GMSEC_LogEntry *entry)
{
	logging_CommonHandler("VERBOSE", entry);
}


void logging_DebugLogHandler(const GMSEC_LogEntry *entry)
{
	logging_CommonHandler("DEBUG", entry);
}


void logging_AnyLogHandler(const GMSEC_LogEntry *entry)
{
	logging_CommonHandler("ANY", entry);
}


GMSEC_BOOL logging_Initialize(logging_t *this, int argc, char *argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	/* Create and register log handlers */
	logRegisterHandlerByLevel(logERROR, logging_ErrorLogHandler);
	logRegisterHandlerByLevel(logWARNING, logging_WarningLogHandler);
	logRegisterHandlerByLevel(logINFO, logging_InfoLogHandler);
	logRegisterHandlerByLevel(logVERBOSE, logging_VerboseLogHandler);
	logRegisterHandlerByLevel(logDEBUG, logging_DebugLogHandler);

	logSetReportingLevel(logVERBOSE);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL logging_Run(logging_t* this)
{
	int         i = 0;
	int         value = 123;
	int         iterations;
	const char* subject = NULL;

	example_getStringDefault(this->config, "SUBJECT", "GMSEC.TEST.PUBLISH", &subject);

	iterations = example_getInteger(this->config, "ITERATIONS", 10);

	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));

	/* Create a Message */
	this->message = messageCreate(subject, GMSEC_PUBLISH, this->status);
	if (!example_check("messageCreate", this->status)) return GMSEC_FALSE;

	/* Add Fields */
	messageAddCharField(this->message, "CHAR-FIELD", 'c', NULL);
	messageAddBooleanField(this->message, "BOOL-FIELD-TRUE", GMSEC_TRUE, NULL);
	messageAddBooleanField(this->message, "BOOL-FIELD-FALSE", GMSEC_FALSE, NULL);
	messageAddI16Field(this->message, "I16-FIELD", (GMSEC_I16) value, NULL);
	messageAddI32Field(this->message, "I32-FIELD", (GMSEC_I32) value, NULL);
	messageAddU16Field(this->message, "U16-FIELD", (GMSEC_U16) value, NULL);
	messageAddU32Field(this->message, "U32-FIELD", (GMSEC_U32) value, NULL);
	messageAddStringField(this->message, "STRING-FIELD", "This is a test", NULL);
	messageAddF32Field(this->message, "F32-FIELD", (GMSEC_F32) (1 + 1./value), NULL);
	messageAddF64Field(this->message, "F64-FIELD", (GMSEC_F64) (1 + 1./value), NULL);
	messageAddBinaryField(this->message, "BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7, NULL);
	messageAddI32Field(this->message, "COUNT", (GMSEC_I32) iterations, NULL);

	for (i = 0; i < iterations; ++i)
	{
		printf("Setting log level to %u\n", logLevelFromString("VERBOSE"));

		logSetReportingLevel(logVERBOSE);

		GMSEC_INFO("The reporting level is set to %u", logGetReportingLevel());

		if (logIsLoggingEnabled(logINFO) == GMSEC_TRUE)
		{
			GMSEC_INFO("Logging is enabled for the logINFO level, as expected.");
		}
		else
		{
			GMSEC_ERROR("Logging is *not* enabled for the logINFO level; unexpected!");
		}

		/* Display XML representation of the message */
		GMSEC_INFO("Prepared Message:\n%s", messageToXML(this->message, NULL));

		/* Publish Message */
		connectionPublish(this->connection, this->message, this->status);
		if (!example_check("connectionPublish", this->status)) return GMSEC_FALSE;

		GMSEC_INFO("Published Message #%d", (i+1));

		GMSEC_ERROR("This is an example error message for iteration %d.\n", i);
		GMSEC_WARNING("This is an example warning message for iteration %d.\n", i);
		GMSEC_VERBOSE("This is an example \"verbose\" message for iteration %d.\n", i);
		GMSEC_DEBUG("This is an example debug message for iteration %d, which should not show.\n", i);

		/* This last message cannot be shown right now because
		 * logSetReportingLevel(logVERBOSE), used above, does not
		 * allow DEBUG messages to come out.
		 */
		GMSEC_VERBOSE("This is another example \"verbose\" message for iteration %d.\n", i);


		/* Set logging reporting level to now allow DEBUG messages to be shown */
		logSetReportingLevel(logDEBUG);
		if (logGetReportingLevel() == logDEBUG)
		{
			GMSEC_INFO("Changed reporting level to logDEBUG");
		}
		else
		{
			GMSEC_ERROR("Failed to change reporting level to logDEBUG");
		}
		GMSEC_DEBUG("This is an example debug message for iteration %d, which should show.\n", i);
	}

	GMSEC_DEBUG("NONE reporting level, numerically, is %u", logLevelFromString("NONE"));
	GMSEC_DEBUG("ERROR reporting level, numerically, is %u", logLevelFromString("ERROR"));
	GMSEC_DEBUG("SECURE reporting level, numerically, is %u", logLevelFromString("SECURE"));
	GMSEC_DEBUG("WARNING reporting level, numerically, is %u", logLevelFromString("WARNING"));
	GMSEC_DEBUG("INFO reporting level, numerically, is %u", logLevelFromString("INFO"));
	GMSEC_DEBUG("VERBOSE reporting level, numerically, is %u", logLevelFromString("VERBOSE"));
	GMSEC_DEBUG("DEBUG reporting level, numerically, is %u", logLevelFromString("DEBUG"));

	/* Register general-purpose handler and test */
	logRegisterHandler(logging_AnyLogHandler);

	GMSEC_ERROR("NONE reporting level, numerically, is %u", logLevelFromString("NONE"));
	GMSEC_ERROR("ERROR reporting level, numerically, is %u", logLevelFromString("ERROR"));
	GMSEC_WARNING("WARNING reporting level, numerically, is %u", logLevelFromString("WARNING"));
	GMSEC_INFO("INFO reporting level, numerically, is %u", logLevelFromString("INFO"));
	GMSEC_VERBOSE("VERBOSE reporting level, numerically, is %u", logLevelFromString("VERBOSE"));
	GMSEC_DEBUG("DEBUG reporting level, numerically, is %u", logLevelFromString("DEBUG"));
 
	return GMSEC_TRUE;
}


void logging_Cleanup(logging_t* this)
{
	logRegisterHandler(NULL);

	/* Destroy the Message */
	if (this->message != NULL)
	{
		messageDestroy(&this->message);
	}

	/* Destroy connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
	}

	/* Destroy the Config */
	configDestroy(&this->config);

	/* Destroy the Status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	logging_t example_log = {NULL};

	if (!logging_Initialize(&example_log, argc, argv)) 
	{
		return -1;
	}

	if (example_option_invalid(example_log.config, argc))
	{
		logging_c_printUsage();
		return -1;
	}

	logging_Run(&example_log);

	logging_Cleanup(&example_log);

	return 0;
}


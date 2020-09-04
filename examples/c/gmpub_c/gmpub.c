/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmpub.c
 *
 * A C example demonstration of GMSEC publisher functionality.
 */

#include "../example.h"


typedef struct
{
  GMSEC_Status     status;
  GMSEC_Config     config;
  GMSEC_Connection connection;
  GMSEC_Message    message;
  GMSEC_Field      field;
} gmpub_t;


void gmpub_printUsage()
{
	printf("\nusage: gmpub_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\titerations=<iterations>"
	       "\n\tinterval_ms=<interval (milliseconds)>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


GMSEC_BOOL gmpub_Initialize(gmpub_t* this, int argc, char* argv[])
{
	this->status     = statusCreate();
	this->config     = configCreateWithArgs(argc, argv);
	this->connection = NULL;
	this->message    = NULL;
	this->field      = NULL;

	/* Initialize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmpub_Publish(gmpub_t *this, int iteration, const char *subject)
{
	int         value = iteration + 1;
	const char* value_ptr = NULL;

	/* Create a Message */
	this->message = messageCreate(subject, GMSEC_PUBLISH, this->status);
	if (!example_check("messageCreate", this->status)) return GMSEC_FALSE;

	/* Add Fields */
	messageAddCharField(this->message, "CHAR-FIELD", 'c', this->status);
	if (!example_check("messageAddCharField", this->status)) return GMSEC_FALSE;

	messageAddBooleanField(this->message, "BOOL-FIELD-TRUE", GMSEC_TRUE, this->status);
	if (!example_check("messageAddBooleanField(TRUE)", this->status)) return GMSEC_FALSE;

	messageAddBooleanField(this->message, "BOOL-FIELD-FALSE", GMSEC_FALSE, this->status);
	if (!example_check("messageAddBooleanField(FALSE)", this->status)) return GMSEC_FALSE;

	messageAddI8Field(this->message, "I8-FIELD", (GMSEC_I8) value, this->status);
	if (!example_check("messageAddI8Field", this->status)) return GMSEC_FALSE;

	messageAddI16Field(this->message, "I16-FIELD", (GMSEC_I16) value, this->status);
	if (!example_check("messageAddI16Field", this->status)) return GMSEC_FALSE;

	messageAddI32Field(this->message, "I32-FIELD", (GMSEC_I32) value, this->status);
	if (!example_check("messageAddI32Field", this->status)) return GMSEC_FALSE;

	messageAddI64Field(this->message, "I64-FIELD", (GMSEC_I64) value, this->status);
	if (!example_check("messageAddI64Field", this->status)) return GMSEC_FALSE;

	messageAddU8Field(this->message, "U8-FIELD", (GMSEC_U8) value, this->status);
	if (!example_check("messageAddU8Field", this->status)) return GMSEC_FALSE;

	messageAddU16Field(this->message, "U16-FIELD", (GMSEC_U16) value, this->status);
	if (!example_check("messageAddU16Field", this->status)) return GMSEC_FALSE;

	messageAddU32Field(this->message, "U32-FIELD", (GMSEC_U32) value, this->status);
	if (!example_check("messageAddU32Field", this->status)) return GMSEC_FALSE;

	messageAddU64Field(this->message, "U64-FIELD", (GMSEC_U64) value, this->status);
	if (!example_check("messageAddU64Field", this->status)) return GMSEC_FALSE;

	messageAddStringField(this->message, "STRING-FIELD", "This is a test", this->status);
	if (!example_check("messageAddStringField", this->status)) return GMSEC_FALSE;

	messageAddF32Field(this->message, "F32-FIELD", (GMSEC_F32) (1 + 1. / value), this->status);
	if (!example_check("messageAddF32Field", this->status)) return GMSEC_FALSE;

	messageAddF64Field(this->message, "F64-FIELD", (GMSEC_F64) (1 + 1. / value), this->status);
	if (!example_check("messageAddF64Field", this->status)) return GMSEC_FALSE;

	messageAddBinaryField(this->message, "BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7, this->status);
	if (!example_check("messageAddBinaryField", this->status)) return GMSEC_FALSE;

	messageAddI32Field(this->message, "COUNT", (GMSEC_I32) iteration, this->status);
	if (!example_check("messageAddI32Field(COUNT)", this->status)) return GMSEC_FALSE;


  	value_ptr = configGetValue(this->config, "ENCRYPT", this->status);

	if (value_ptr != NULL)
	{
#ifndef WIN32
		if (strcasecmp(value_ptr, "true") == 0)
#else
		if (_stricmp(value_ptr, "true") == 0)
#endif
		{
			messageAddBooleanField(this->message, "SEC-ENCRYPT", GMSEC_TRUE, this->status);
			if (!example_check("messageAddBooleanField(SEC-ENCRYPT)", this->status)) return GMSEC_FALSE;
		}
	}

	/* Publish Message */
	connectionPublish(this->connection, this->message, this->status);
	if (!example_check("Publish", this->status)) return GMSEC_FALSE;

	/* Display XML representation of the message */
	GMSEC_INFO("Published:\n%s", messageToXML(this->message, NULL));

	messageDestroy(&(this->message));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmpub_Run(gmpub_t* this)
{
	const char* subject = NULL;
	const char* tmp     = NULL;
	int iterations      = example_getInteger(this->config, "ITERATIONS", 1);
	int interval_ms     = example_getInteger(this->config, "INTERVAL_MS", 1000);
	int i;

	example_getStringDefault(this->config, "SUBJECT", "GMSEC.TEST.PUBLISH", &subject);

	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the Connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	tmp = connectionGetLibraryVersion(this->connection, this->status);
	if (!example_check("connectionGetLibraryVersion", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Middleware version = %s",tmp);
	GMSEC_INFO("Using subject '%s'", subject);
	GMSEC_INFO("Publishing %d message(s)", iterations);

	for (i = 0; i < iterations; ++i)
	{
		if (!gmpub_Publish(this, i, subject)) return GMSEC_FALSE;

		if (i < iterations - 1)
		{
			example_millisleep(interval_ms);
		}
	}

	return GMSEC_TRUE;
}


void gmpub_Cleanup(gmpub_t* this)
{
	/* Destroy the Message */
	if (this->message != NULL)
	{
		messageDestroy(&(this->message));
	}

	/* Destroy the Connection */
	if (this->connection != NULL)
	{
		connectionDisconnect(this->connection, this->status);
		if (!example_check("connectionDisconnect", this->status)) return;

		connectionDestroy(&(this->connection));
	}

	/* Destroy the Config */
	configDestroy(&(this->config));

	/* Destroy the Status */
	statusDestroy(&(this->status));
}


int main(int argc, char* argv[])
{
	gmpub_t gmpub;

	if (gmpub_Initialize(&gmpub, argc, argv) == GMSEC_FALSE)
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	if (example_option_invalid(gmpub.config, argc) == GMSEC_TRUE)
	{
		gmpub_printUsage();
		return -1;
	}

	gmpub_Run(&gmpub);
	gmpub_Cleanup(&gmpub);
 
	return 0;
}

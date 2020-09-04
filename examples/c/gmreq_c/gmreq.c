/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmreq.c
 *
 *  A C example demonstation of the GMSEC request functionality.
 */

#include "../example.h"

#define GMREQ_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"
#define GMREQ_QUESTION        "Does the request/reply functionality still work?"


typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    request;
	GMSEC_Message    reply;
} gmreq_t;


void gmreq_printUsage()
{
	printf("\nusage: gmreq_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


GMSEC_BOOL gmreq_Initialize(gmreq_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmreq_Run(gmreq_t *this)
{
	const char* subject = NULL;
	int         msg_timeout_ms;

	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);

	if (this->connection == NULL)
	{
		return GMSEC_FALSE;
	}

	/* Output the middleware version */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, this->status));
 
	/* Get comfiguration options */
	example_getStringDefault(this->config, "SUBJECT", GMREQ_DEFAULT_SUBJECT, &subject);
	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);

	/* Create a Request */
	this->request = messageCreate(subject, GMSEC_REQUEST, this->status);
	if (!example_check("messageCreate(request)", this->status)) return GMSEC_FALSE;

	/* Add Fields */
	messageAddStringField(this->request, "question", GMREQ_QUESTION, this->status);
	if (!example_check("messageAddStringField(question)", this->status)) return GMSEC_FALSE;

	messageAddStringField(this->request, "COMPONENT", "GMREQ", this->status);
	if (!example_check("messageAddStringField(COMPONENT)", this->status)) return GMSEC_FALSE;

	/* Display XML representation of the request message */
	GMSEC_INFO("Build Request Message:\n%s", messageToXML(this->request, this->status));

	/* Send Request */
	this->reply = connectionRequest(this->connection, this->request, msg_timeout_ms, 0, this->status);
	if (!example_check("connectionRequest", this->status)) return GMSEC_FALSE;

	if (this->reply)
	{
		/* Display XML representation of received reply message */
		GMSEC_INFO("Received Reply Message:\n%s", messageToXML(this->reply, this->status));

		connectionRelease(this->connection, &this->reply, this->status);
	}
	else
	{
		GMSEC_WARNING("timeout! (no reply received)");
	}

	return GMSEC_FALSE;
}


void gmreq_Cleanup(gmreq_t* this)
{
	/* Destroy Reply */
	if (this->reply != NULL)
	{
		messageDestroy(&this->reply);
	}

	/* Destroy Request */
	if (this->request != NULL)
	{
		messageDestroy(&this->request);
	}

	/* Destroy connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
		if (!example_check("Disconnection", this->status)) return;
	}

	/* Destroy the Config */
	configDestroy(&this->config);

	/* Destroy Status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	gmreq_t gmreq = {NULL};

	if (!gmreq_Initialize(&gmreq, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmreq");
		return -1;
	}

	if (example_option_invalid(gmreq.config, argc))
	{
		gmreq_printUsage();
		return -1;
	}

	gmreq_Run(&gmreq);

	gmreq_Cleanup(&gmreq);

	return 0;
}

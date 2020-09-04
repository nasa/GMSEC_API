/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmreq_c_cb.c
 *
 * A C example demonstration of GMSEC callback requestor functionality.
 *
 */

#include "../example.h"

#define GMREQ_CB_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"


GMSEC_BOOL gReplyCallbackFlag = GMSEC_FALSE;


typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    request;
} gmreq_cb_t;


void gmreq_cb_printUsage()
{
	printf("\nusage: gmrpl_cb_c connectiontype=<middleware> "
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


GMSEC_BOOL gmreq_cb_Initialize(gmreq_cb_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


void reply_callback(GMSEC_Connection conn, GMSEC_Message request, GMSEC_Message reply)
{
	GMSEC_Status status = statusCreate();

	if (reply)
	{
		/* Display XML representation of the reply message */
		GMSEC_INFO("Received Reply Message:\n%s", messageToXML(reply, status));
	}

	/* NOTE: reply message is 'owned' by the framework and should not be freed */

	/* Destroy Status */
	statusDestroy(&status);

	/* flag that callback has been called */
	gReplyCallbackFlag = GMSEC_TRUE;
}


void event_callback(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_WARNING("Received Event: %s [event=%d]", statusGet(status), event);
	}
}


GMSEC_BOOL gmreq_cb_Run(gmreq_cb_t *this)
{
	const char* subject = NULL;
	int         msg_timeout_ms;
	int         deltaTime = 0;

	/* Get configuration options */
	example_getStringDefault(this->config, "SUBJECT", GMREQ_CB_DEFAULT_SUBJECT, &subject);
	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);

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

	/* Create request */
	this->request = messageCreate(subject, GMSEC_REQUEST, this->status);
	if (!example_check("messageCreate", this->status)) return GMSEC_FALSE;

	/* Add Fields */
	messageAddStringField(this->request, "QUESTION", "Does the request/reply functionality still work?", this->status);
	if (!example_check("messageAddStringField(QUESTION)", this->status)) return GMSEC_FALSE;

	messageAddStringField(this->request, "COMPONENT", "GMREQ", this->status);
	if (!example_check("messageAddStringField(COMPONENT)", this->status)) return GMSEC_FALSE;

	/* Display XML representation of the request message */
	GMSEC_INFO("Request Message:\n%s", messageToXML(this->request, this->status));

	/* Send Request */   
	connectionRequestWithCallback(this->connection, this->request, msg_timeout_ms, reply_callback, event_callback, 0, this->status);

	if (!example_check("RequestWCallback(connection)", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Waiting for response...");
	while (gReplyCallbackFlag == GMSEC_FALSE)
	{
		example_millisleep(10);

		if (msg_timeout_ms != GMSEC_WAIT_FOREVER)
		{
			deltaTime += 10;
			if (deltaTime > msg_timeout_ms)
			{
			  break;
			}
		}
	}

	if (gReplyCallbackFlag == GMSEC_TRUE)
	{
		GMSEC_INFO("Response Received!");
	}
	else
	{
		GMSEC_INFO("No response received");
	}

	return GMSEC_TRUE;
}


void gmreq_cb_Cleanup(gmreq_cb_t* this)
{
	/* Destroy the Request */
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
	gmreq_cb_t gmreq_cb = {NULL};

	if (!gmreq_cb_Initialize(&gmreq_cb, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmreq");
		return -1;
	}

	if (example_option_invalid(gmreq_cb.config, argc))
	{
		gmreq_cb_printUsage();
		return -1;
	}

	gmreq_cb_Run(&gmreq_cb);

	gmreq_cb_Cleanup(&gmreq_cb);

	return 0;
}

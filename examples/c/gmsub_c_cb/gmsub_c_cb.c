/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
* @file gmsub_c_cb.c
*
* A C example demonstration of GMSEC callback functionality for subscribers
* allowing for code to be executed when a message is received
*/

#include "../example.h"


typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    message;
	GMSEC_SubscriptionInfo* info;
} gmsub_cb_t;


void gmsub_cb_printUsage()
{
	printf("\nusage: gmsub_cb_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
	       "\n\tprog_timeout_s=<timeout period (seconds)>"
	       "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


GMSEC_BOOL gmsub_cb_Initialize(gmsub_cb_t* this, int argc, char *argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&this->config);

	return GMSEC_TRUE;
}


void publish_Callback(GMSEC_Connection conn, GMSEC_Message msg)
{
	GMSEC_Status status = statusCreate();

	if (msg)
	{
		/* Display XML representation of the message */
		GMSEC_INFO("publish_Callback Received Message:\n%s", messageToXML(msg, status));
	}

	/* Destroy Status */
	statusDestroy(&status);
}


GMSEC_BOOL gmsub_cb_Run(gmsub_cb_t* this)
{
	const char*   subject = NULL;
	const char*   temp_subject = NULL;
	int           msg_timeout_ms = GMSEC_WAIT_FOREVER;
	int           prog_timeout_s = GMSEC_WAIT_FOREVER;
	int           iterations = 0;
	int           count = 0;
	GMSEC_BOOL    done = GMSEC_FALSE;

	time_t        prevTime;
	time_t        nextTime;
	double        elapsedTime = 0;

	/* Get configurable options */
	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", msg_timeout_ms);
	prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", prog_timeout_s);
	iterations     = example_getInteger(this->config, "ITERATIONS", iterations);
	example_getStringDefault(this->config, "SUBJECT", "GMSEC.>", &subject);


	if (msg_timeout_ms == GMSEC_WAIT_FOREVER && prog_timeout_s != GMSEC_WAIT_FOREVER)
	{
		GMSEC_INFO("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
		return GMSEC_FALSE;
	}

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

	if (iterations > 0)
	{
		GMSEC_INFO("Waiting for up to %d", iterations);
	}

	/* Subscribe */
	this->info = (GMSEC_SubscriptionInfo*)malloc(2 * sizeof(GMSEC_SubscriptionInfo));
	GMSEC_INFO("Subscribing to %s", subject);
	this->info[0] = connectionSubscribeWithCallback(this->connection, subject, publish_Callback, this->status);
	if (!example_check("Subscribing...", this->status)) return GMSEC_FALSE;

	subject = "GMSEC.TERMINATE"; 

	GMSEC_INFO("Subscribing to %s", subject);
	this->info[1] = connectionSubscribeWithCallback(this->connection, subject, publish_Callback, this->status);
	if (!example_check("Subscribing...", this->status)) return GMSEC_FALSE;

	/* Listen */
	time(&prevTime);

	while (done != GMSEC_TRUE)
	{
		if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			GMSEC_INFO("Program Timeout Reached!");
			return GMSEC_FALSE;
		}

		this->message = connectionReceive(this->connection, msg_timeout_ms, this->status);
		example_check("connectionReceive", this->status);
		
		if (statusIsError(this->status) == GMSEC_TRUE)
		{
			return GMSEC_FALSE;
		}

		if (prog_timeout_s != GMSEC_WAIT_FOREVER)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (this->message == NULL)
		{
			GMSEC_INFO("timeout.");
			continue;
		}

		++count;

		if (iterations > 0 && count >= iterations)
		{
			done = GMSEC_TRUE;
		}

		connectionDispatch(this->connection, this->message, this->status);
		if (!example_check("connectionDispatch", this->status)) return GMSEC_FALSE;

		temp_subject = messageGetSubject(this->message, this->status);
		if (strcmp(temp_subject, "GMSEC.TERMINATE") == 0)
		{
			done = GMSEC_TRUE;
		}

		/* Destroy Message */
		connectionRelease(this->connection, &this->message, this->status);
	}

	return GMSEC_TRUE;
}


void gmsub_cb_Cleanup(gmsub_cb_t* this)
{
	int i;

	/* Destroy the message */
	if (this->message != NULL)
	{
		messageDestroy(&this->message);
	}

	for(i = 1; i >= 0; i--)
	{
		GMSEC_INFO("Unsubscribing from %s", subscriptionInfoGetSubject(this->info[i]));
		connectionUnsubscribe(this->connection, &(this->info[i]), this->status);
		if (!example_check("Unsubscribing...", this->status))
		{
			GMSEC_ERROR("Problem with Unsubscribing...");
		}
	}
	free(this->info);

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
	gmsub_cb_t gmsub_cb = {NULL};

	if (!gmsub_cb_Initialize(&gmsub_cb, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmreq");
		return -1;
	}

	if (example_option_invalid(gmsub_cb.config, argc))
	{
		gmsub_cb_printUsage();
		return -1;
	}

	gmsub_cb_Run(&gmsub_cb);

	gmsub_cb_Cleanup(&gmsub_cb);

	return 0;
}

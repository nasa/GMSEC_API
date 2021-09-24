
/*
 * Copyright 2007-2021 United States Government as represented by the
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
	GMSEC_STATUS_OBJECT status;
	GMSEC_CONFIG_OBJECT config;
	GMSEC_CONNECTION_OBJECT connection;
	GMSEC_MESSAGE_OBJECT message;
	GMSEC_FIELD_OBJECT field;
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


int gmsub_cb_Initialize(gmsub_cb_t *this, int argc, char *argv[])
{
	this->status = NULL;
	this->connection = NULL;	
	this->config = NULL;
	this->message = NULL;
	this->field = NULL;

	/* create status */
	gmsec_CreateStatus(&(this->status));
	
	/* create config */
	gmsec_CreateConfigParams(&(this->config), argc, argv, this->status);
	if (!example_check("CreateConfigParam", this->status))
	{
		fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
		return GMSEC_FALSE;
	}

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


void publish_Callback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg)
{
	GMSEC_STR xml = NULL;
	GMSEC_STATUS_OBJECT result = NULL;

	/* create result */
	gmsec_CreateStatus(&result);

	/* Dump Message */
	gmsec_MsgToXML(msg, &xml, result);
	example_check("MsgToXML(Dump Msg via callback)", result); 
	if (xml != NULL)
	{
		LOG_INFO("Message recieved, dumping via callback =>\n%s", xml);
		fflush(stdout);
	}

	/* Destroy Status */
	gmsec_DestroyStatus(&result);
}


int gmsub_cb_Run(gmsub_cb_t *this)
{
	GMSEC_STR subject;
	GMSEC_STR tmp = NULL;
	GMSEC_STR temp_subject;
	GMSEC_MESSAGE_OBJECT temp_msg;
	int msg_timeout_ms;
	int prog_timeout_s;
	int iterations;
	int count;
	int done;

	time_t prevTime;
	time_t nextTime;
	double elapsedTime = 0;

	/* Get Timeout */
	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS",
                                      EXAMPLE_MSG_TIMEOUT);

	prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S",
									   EXAMPLE_PROG_TIMEOUT);

	if(msg_timeout_ms == EXAMPLE_MSG_TIMEOUT && prog_timeout_s != EXAMPLE_PROG_TIMEOUT)
	{
		LOG_INFO("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
		//gmsub_cb_printUsage();
		return GMSEC_FALSE;
	}

	/* Output the GMSEC API version */
	LOG_INFO("%s", gmsec_GetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);

	if (this->connection == NULL)
		return GMSEC_FALSE;

	/* Output the middleware version */
	gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
	if (!example_check("Getting Library Version", this->status))
		return GMSEC_FALSE;

	if (tmp != NULL)
	{
		LOG_INFO("%s", tmp);
	}

	/* Get Iterations */
	iterations = example_getInteger(this->config, "ITERATIONS", 0);
	if (iterations > 0)
	{
		LOG_INFO("Waiting for up to %d", iterations);
	}
	/* Subscribe */
	subject = NULL;
	example_getStringDefault(this->config, "SUBJECT", "GMSEC.>", &subject);

	LOG_INFO("Subscribing to %s", subject);
	gmsec_SubscribeWCallback(this->connection, subject, publish_Callback, 
		this->status);
	if (!example_check("Subscribing...", this->status))
		return GMSEC_FALSE;

	subject = "GMSEC.TERMINATE"; 

	LOG_INFO("Subscribing to %s", subject);
	gmsec_SubscribeWCallback(this->connection, subject, publish_Callback,
		this->status);
	if (!example_check("Subscribing...", this->status))
		return GMSEC_FALSE;

	/* Listen */
	count = 0;
	done = 0;
	time(&prevTime);

	while(!done)
	{
		if(prog_timeout_s != EXAMPLE_PROG_TIMEOUT && elapsedTime >= prog_timeout_s)
		{
			LOG_INFO("Program Timeout Reached!");
			return GMSEC_FALSE;
		}

		gmsec_GetNextMsg(this->connection, &(this->message), msg_timeout_ms,
						 this->status);
		example_check("Get Next Message...", this->status);
		
		if(prog_timeout_s != EXAMPLE_PROG_TIMEOUT)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (gmsec_isStatusError(this->status))
		{
			if (gmsec_GetStatusCode(this->status) == GMSEC_TIMEOUT_OCCURRED)
				LOG_INFO("timeout occurred");
			else
				return GMSEC_FALSE;
		}

		if (gmsec_isMsgValid(this->message))
		{
			++count;
			if (iterations > 0 && count >= iterations)
				done = GMSEC_TRUE;

			gmsec_DispatchMsg(this->connection, this->message, this->status);
			if (!example_check("DispatchMsg", this->status)) return GMSEC_FALSE;

			temp_subject = NULL;
			gmsec_GetMsgSubject(this->message, &temp_subject, this->status);
			if (strcmp(temp_subject, "GMSEC.TERMINATE") == 0)
				done = GMSEC_TRUE;

			/* Destroy Message */
			temp_msg = this->message;
			this->message = NULL;

			gmsec_DestroyMessage(this->connection, temp_msg, this->status);

			if (!example_check("DestroyMessage", this->status))
				return GMSEC_FALSE;
		}
	}
	return GMSEC_TRUE;
}

void gmsub_cb_Cleanup(gmsub_cb_t *this)
{
	/* Destroy Field */
	if (this->field != NULL)
	{
		gmsec_DestroyField(&this->field, this->status);
		example_check("Final DestroyField", this->status);
	}

	/* Destroy the Request */
	if (this->message != NULL)
	{
		gmsec_DestroyMessage(this->connection, this->message, this->status);
		example_check("Final DestroyMessage", this->status);
	}

	/* Destroy connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
		if (!example_check("Disconnection", this->status)) return;
	} 

	/* Destroy the Config */
	gmsec_DestroyConfig(&(this->config), this->status);

	/* Destroy Status */
	gmsec_DestroyStatus(&this->status);
}

int main(int argc, char* argv[])
{
	gmsub_cb_t gmsub_cb;

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

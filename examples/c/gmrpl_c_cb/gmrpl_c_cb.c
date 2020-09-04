/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmrpl_c_cb.c
 *
 * A C example demonstration of GMSEC callback this->reply functionality. 
 */

#include "../example.h"

#define GMRPL_CB_DEFAULT_SUBJECT "GMSEC.>"


typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    message;
	GMSEC_SubscriptionInfo *info;
} gmrpl_cb_t;


typedef struct 
{
	GMSEC_Status  status;
	GMSEC_Message reply;
	char*         stringHolder; 
} gmrpl_cb_OnMessage_t;


const size_t MAX_STR_LEN = 255;


void gmrpl_cb_printUsage()
{
	printf("\nusage: gmrpl_c_cb connectiontype=<middleware> "
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


GMSEC_BOOL gmrpl_cb_Initialize(gmrpl_cb_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmrpl_cb_OnMessage_Run(GMSEC_Connection connection, GMSEC_Message message, gmrpl_cb_OnMessage_t* this)
{
	const char* componentValue = NULL;
	GMSEC_Field componentField = NULL;
	char        replySubject[256];

	if (!message)
	{
		GMSEC_ERROR("onMessage Received a NULL message");
		return GMSEC_FALSE;
	}

	if (messageGetKind(message, NULL) != GMSEC_REQUEST)
	{
		GMSEC_WARNING("onMessage Received a non-REQUEST message");
		return GMSEC_FALSE;
	}

	this->status = statusCreate();

	/* Display XML representation of the received message */
	GMSEC_INFO("onMessageCallback Received:\n%s", messageToXML(message, NULL));

	/* Get the COMPONENT field from the message */
	componentField = messageGetStringField(message, "COMPONENT", this->status);
	if (!example_check("messageGetStringField(COMPONENT)", this->status)) return GMSEC_FALSE;

	/* Get the COMPONENT field value */
	componentValue = stringFieldGetValue(componentField, NULL);

	/* Create Reply Subject */
	snprintf(replySubject, sizeof(replySubject) - 1, "GMSEC.MISSION.SAT_ID.RESP.%s.1", componentValue);

	/* Create a reply message */
	this->reply = messageCreate(replySubject, GMSEC_REPLY, NULL);

	/* Add fields to the reply message */
	messageAddStringField(this->reply, "ANSWER", "Sure looks like it!", NULL);
	messageAddStringField(this->reply, "COMPONENT", "GMRPL", NULL);

	/* Display the XML representation of the reply message */
	GMSEC_INFO("onMessageCallback Prepared Reply Message:\n%s", messageToXML(this->reply, NULL));

	/* Send Reply */
	connectionReply(connection, message, this->reply, this->status);
	if (!example_check("connectionReply", this->status)) return GMSEC_FALSE;

	return GMSEC_TRUE;
}


void gmrpl_cb_OnMessage_Cleanup(GMSEC_Connection connection, gmrpl_cb_OnMessage_t* this)
{
	/* Destroy Reply */
	if (this->reply != NULL)
	{
		messageDestroy(&this->reply);
	}

	/* Destroy status */
	statusDestroy(&this->status);

	/* Deallocate memory */
	if (this->stringHolder)
	{
		free(this->stringHolder);
	}
}


void CALL_TYPE onMessageCallback(GMSEC_Connection connection, const GMSEC_Message message)
{
	gmrpl_cb_OnMessage_t gmrpl_callback = {NULL};;

	if (!gmrpl_cb_OnMessage_Run(connection, message, &gmrpl_callback))
	{
		GMSEC_WARNING("gmrpl_cb_PublishOnMessage failed");
	}

	gmrpl_cb_OnMessage_Cleanup(connection, &gmrpl_callback);
}
 

GMSEC_BOOL gmrpl_cb_Run(gmrpl_cb_t* this)
{
	time_t     prevTime;
	time_t     nextTime;
	double     elapsedTime = 0; 
	GMSEC_BOOL done = GMSEC_FALSE;

	/* Get configurable values for the example program */
	const char* subject        = NULL;
	int         msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
	int         prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

	example_getStringDefault(this->config, "SUBJECT", GMRPL_CB_DEFAULT_SUBJECT, &subject);

	if (msg_timeout_ms == GMSEC_WAIT_FOREVER && prog_timeout_s != GMSEC_WAIT_FOREVER)
	{
		GMSEC_INFO("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
		return GMSEC_FALSE;
	}

	/* Add the configuration for threading */
	configAddValue(this->config, "isThreaded", "TRUE", this->status);
	if (!example_check("configAddValue(isThreaded)", this->status)) return GMSEC_FALSE;

	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);

	if (this->connection == NULL)
	{
		return GMSEC_FALSE;
	}

	/* Output the middleware version */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));

	/* Subscribe using subject and callback */
	this->info = (GMSEC_SubscriptionInfo*)malloc(2 * sizeof(GMSEC_SubscriptionInfo));
	GMSEC_INFO("Subscribing to %s (using callback)", subject);
	this->info[0] = connectionSubscribeWithCallback(this->connection, subject, onMessageCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Subscribing to GMSEC.TERMINATE (using callback)");
	this->info[1] = connectionSubscribeWithCallback(this->connection, "GMSEC.TERMINATE", onMessageCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback", this->status)) return GMSEC_FALSE;


	time(&prevTime);
 
	/* Listen */
	while (!done)
	{
		if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			GMSEC_INFO("Program Timeout Reached!");
			return GMSEC_FALSE;
		}

		this->message = connectionReceive(this->connection, msg_timeout_ms, this->status);
		if (!example_check("connectionReceive", this->status)) return GMSEC_FALSE;

		if (prog_timeout_s != GMSEC_WAIT_FOREVER)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (this->message == NULL)
		{
			GMSEC_INFO("timeout occurred");
			continue;
		}

		/* Received message! */

		/* Dispatch callbacks */
		connectionDispatch(this->connection, this->message, this->status);
		if (!example_check("DispatchMsg", this->status)) return GMSEC_FALSE;

		subject = messageGetSubject(this->message, NULL);

		if (strcmp(subject, "GMSEC.TERMINATE") == 0)
		{
			done = GMSEC_TRUE;
		}
       
		/* Destroy inbound message */
		connectionRelease(this->connection, &this->message, NULL);
	}

	return GMSEC_TRUE;
}


void gmrpl_cb_Cleanup(gmrpl_cb_t* this)
{
	int i;

	/* Destroy Message */
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
	}

	/* Destroy the Config */
	configDestroy(&this->config);

	/* Destroy Status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	gmrpl_cb_t gmrpl_cb = {NULL};

	if (!gmrpl_cb_Initialize(&gmrpl_cb, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmrpl_cb");
		return -1;
	}

	if (example_option_invalid(gmrpl_cb.config, argc))
	{
		gmrpl_cb_printUsage();
		return -1;
	}
  
	gmrpl_cb_Run(&gmrpl_cb);

	gmrpl_cb_Cleanup(&gmrpl_cb);

	return 0;
}

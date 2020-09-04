/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmrpl.c
 *
 *   A C example demonstration of GMSEC reply functionality.
 */

#include "../example.h"


/* @TODO: need to determine what the default subject should be.
 * #define GMRPL_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL" 
 */
#define GMRPL_DEFAULT_SUBJECT "GMSEC.>"


typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    message;
	GMSEC_Message    reply;
	GMSEC_SubscriptionInfo* info;
	char*            stringHolder;
} gmrpl_t;


const size_t MAX_STR_LEN = 255;


void gmrpl_printUsage()
{
	printf("\nusage: gmrpl_c connectiontype=<middleware> "
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


GMSEC_BOOL gmrpl_Initialize(gmrpl_t* this, int argc, char* argv[])
{
	this->status       = statusCreate();
	this->config       = configCreateWithArgs(argc, argv);
	this->stringHolder = (char*) malloc(MAX_STR_LEN);

	strncpy(this->stringHolder, "", MAX_STR_LEN - 1);

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmrpl_Run(gmrpl_t* this)
{
	const char* subject = NULL;
	time_t      prevTime;
	time_t      nextTime;
	double      elapsedTime = 0;
	GMSEC_BOOL  done = GMSEC_FALSE;

	/* Get configurable options for the example program */
	int msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
	int prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);
	example_getStringDefault(this->config, "SUBJECT", GMRPL_DEFAULT_SUBJECT, &subject);

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
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));

	/* Subscribe */
	this->info = (GMSEC_SubscriptionInfo*)malloc(2 * sizeof(GMSEC_SubscriptionInfo));
	GMSEC_INFO("Subscribing to %s", subject);
	this->info[0] = connectionSubscribe(this->connection, subject, this->status);
	if (!example_check("Subscribing...", this->status))
	{
		return GMSEC_FALSE;
	}

	subject = "GMSEC.TERMINATE";
	GMSEC_INFO("Subscribing to %s", subject);
	this->info[1] = connectionSubscribe(this->connection, subject, this->status);
	if (!example_check("Subscribing...", this->status))
	{
		return GMSEC_FALSE;
	}

	time(&prevTime);

	while (!done)
	{
		if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			GMSEC_INFO("Program Timeout Reached!");
			return GMSEC_FALSE;
		}


		this->message = connectionReceive(this->connection, msg_timeout_ms, this->status);

		if (prog_timeout_s != GMSEC_WAIT_FOREVER)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (!example_check("connectionReceive", this->status))
		{
			return GMSEC_FALSE;
		}
		else if (this->message == NULL)
		{
			GMSEC_WARNING("timeout occurred");
			continue;
		}

		/* Received message! */
		subject = messageGetSubject(this->message, NULL);

		if (strcmp(subject, "GMSEC.TERMINATE") == 0)
		{
			done = GMSEC_TRUE;
		}

		/* Display the XML representation of the received message */
		GMSEC_INFO("Received:\n%s", messageToXML(this->message, NULL));

		/* check if request message was received */
		if (messageGetKind(this->message, NULL) == GMSEC_REQUEST)
		{
			/* Get COMPONENT field from the message */
			char              replySubject[256];
			const char*       componentValue = NULL;
			const GMSEC_Field componentField = messageGetStringField(this->message, "COMPONENT", this->status);

			if (!example_check("messageGetStringField(COMPONENT)", this->status)) return GMSEC_FALSE;

			/* Get the COMPONENT value */
			componentValue = stringFieldGetValue(componentField, NULL);


			/* Create Reply Subject */
			snprintf(replySubject, sizeof(replySubject) - 1, "GMSEC.MISSION.SAT_ID.RESP.%s.1", componentValue);

			/* Create Reply */
			this->reply = messageCreate(replySubject, GMSEC_REPLY, this->status);
			if (!example_check("CreateMessage(reply)", this->status)) return GMSEC_FALSE;

			/* Add Fields */
			messageAddStringField(this->reply, "ANSWER", "Sure looks like it!", NULL);
			messageAddStringField(this->reply, "COMPONENT", "GMRPL", NULL);

			/* Display XML representation of the reply message */
			GMSEC_INFO("Prepared Reply Message:\n%s", messageToXML(this->reply, NULL));

			/* Issue the reply */
			connectionReply(this->connection, this->message, this->reply, this->status);
			if (!example_check("connectionReply", this->status)) return GMSEC_FALSE;
 
			GMSEC_INFO("Reply sent!");

			/* Destroy the reply message */
			messageDestroy(&this->reply);
       }

       /* Destroy Message */
       connectionRelease(this->connection, &this->message, this->status);
	}

	return GMSEC_TRUE;
}


void gmrpl_Cleanup(gmrpl_t* this)
{
	int i;

	/* Destroy Reply */
	if (this->reply != NULL)
	{
		messageDestroy(&this->reply);
	}

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

	/* Deallocate memory */
	free(this->stringHolder);
}


int main(int argc, char* argv[])
{
	gmrpl_t gmrpl = {NULL};

	if (!gmrpl_Initialize(&gmrpl, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmrpl");
		return -1;
	}

	if (example_option_invalid(gmrpl.config, argc))
	{
		gmrpl_printUsage();
		return -1;
	}

	gmrpl_Run(&gmrpl);

	gmrpl_Cleanup(&gmrpl);

	return 0;
}

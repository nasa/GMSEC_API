/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
* @file gmsub.c
*
*  A C example demonstration of GMSEC subscriber functionality.
*/

#include "../example.h"

typedef struct 
{
	GMSEC_Status     status;
	GMSEC_Config     config;	
	GMSEC_Connection connection;
	GMSEC_Message    message;
	GMSEC_SubscriptionInfo *info;
} gmsub_t;


void gmsub_printUsage()
{
	printf("\nusage: gmsub_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
       	"\n\tNote that the parameter 'connectiontype' is required. "
       	"\n\tThe rest of other parameters are optional.\n"
       	"\n\tserver=<server name> "
       	"(required if middleware is not bolt/MB locally)"	
       	"\n\tsubject=<subject name>"
       	"\n\tsubject.<N>=<subject name>"
       	"\n\t\tNote: N must be 1 or greater, and be consecutive."
       	"\n\t\tFor example: subject.1=GMSEC.A subject.2=GMSEC.B ..."
       	"\n\titerations=<iterations>"
       	"\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
       	"\n\tprog_timeout_s=<timeout period (seconds)>"
       	"\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s"
       	"\n\tloglevel=<log level>"
        "\n\tcfgfile=<config_filepath.xml>"
       	"\n\nFor more information, see API User's Guide\n");
}


GMSEC_BOOL gmsub_Initialize(gmsub_t *this, int argc, char *argv[])
{
	this->status     = statusCreate();
	this->config     = configCreateWithArgs(argc, argv);
	this->connection = NULL;
	this->message    = NULL;

	/* Initilaize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmsub_Run(gmsub_t* this, int subjectCount)
{
	const char*   subject     = NULL;
	int           count       = 0;
	GMSEC_BOOL    done        = GMSEC_FALSE;
	time_t        prevTime    = 0;
	time_t        nextTime    = 0;
	double        elapsedTime = 0; 

	/* get configurable options */
	int  iterations     = example_getInteger(this->config, "ITERATIONS", 0);
	int  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
	int  prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

	if (msg_timeout_ms == GMSEC_WAIT_FOREVER && prog_timeout_s != GMSEC_WAIT_FOREVER)
	{
		GMSEC_WARNING("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
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
	if (subjectCount == 0)
	{
		example_getStringDefault(this->config, "SUBJECT", "GMSEC.>", &subject);

		GMSEC_INFO("Subscribing to %s", subject);

		this->info = (GMSEC_SubscriptionInfo *)malloc(sizeof(GMSEC_SubscriptionInfo));
		this->info[0] = connectionSubscribe(this->connection, subject, this->status);
		if (!example_check("Subscribing...", this->status))
		{
			return GMSEC_FALSE;
		}
	}
	else
	{
		const char* str2 = "subject.";
		int         i    = 1;
		this->info = (GMSEC_SubscriptionInfo *)malloc((subjectCount+1) * sizeof(GMSEC_SubscriptionInfo));

		for(; i <= subjectCount; i++)
		{
			char buff[20] = {0};
			snprintf(buff, sizeof(buff) - 1, "%s%d", str2, i);

			if (example_getString(this->config, buff, &subject))
			{
				GMSEC_INFO("Subscribing to %s", subject);
				this->info[i-1] = connectionSubscribe(this->connection, subject, this->status);
				if (!example_check("Subscribing...", this->status))
				{
					return GMSEC_FALSE;
				}
			}
		}
		subject = "GMSEC.TERMINATE"; 

		GMSEC_INFO("Subscribing to %s", subject);
		this->info[i-1] = connectionSubscribe(this->connection, subject, this->status);
		if (!example_check("Subscribing...", this->status))
		{
			return GMSEC_FALSE;
		}
	}

	time(&prevTime);

	/* Listen */
	while (done == GMSEC_FALSE)
	{
		const char* msg_subject = NULL;

		if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			GMSEC_INFO("Program Timeout Reached!");
			return GMSEC_FALSE;
		}

		this->message = connectionReceive(this->connection, msg_timeout_ms, this->status);
		example_check("connectionReceive...", this->status);
		
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

		/* Received a message */

		/* Determine if we have received our maximum number of messages */
		++count;
		if (iterations > 0 && count >= iterations)
		{
			done = GMSEC_TRUE;
		}

		/* Get the subject associated with the message to see if we should terminate */
		msg_subject = messageGetSubject(this->message, this->status);
		if (strcmp(msg_subject, "GMSEC.TERMINATE") == 0)
		{
			done = GMSEC_TRUE;
		}

		/* Display XML representation of the message */
		GMSEC_INFO("Received:\n%s", messageToXML(this->message, this->status));

		/* Destroy Message */
		messageDestroy(&(this->message));
	}

	return GMSEC_TRUE;
}


void gmsub_Cleanup(gmsub_t* this, int subjectCount)
{
	/* Destory Message */
	if (this->message != NULL)
	{
		messageDestroy(&(this->message));
	}
	
	if(subjectCount == 0)
	{
		GMSEC_INFO("Unsubscribing from %s", subscriptionInfoGetSubject(this->info[0]));
		connectionUnsubscribe(this->connection, &(this->info[0]), this->status);
		if (!example_check("Unsubscribing...", this->status))
		{
			GMSEC_ERROR("Problem with Unsubscribing...");
		}
	}
	else
	{
		int i = subjectCount;
		for(; i >= 0; i--)
		{
			GMSEC_INFO("Unsubscribing from %s", subscriptionInfoGetSubject(this->info[i]));
			connectionUnsubscribe(this->connection, &(this->info[i]), this->status);
			if (!example_check("Unsubscribing...", this->status))
			{
				GMSEC_ERROR("Problem with Unsubscribing...");
			}
		}
	}
	free(this->info);

	/* Destory Connection */
	if (this->connection != NULL)
	{
		connectionDisconnect(this->connection, this->status);

		connectionDestroy(&(this->connection));
	}

	/* Destroy the Config */
	configDestroy(&(this->config));

	/* Destroy Status */
	statusDestroy(&(this->status));
}


int gmsub_DetermineSubCount(int argc, char* argv[])
{
	const char* substr = "subject.";
	int         subjectCount = 0;
	int         i = 0;

	for(; i < argc; ++i)
	{	
		char* arg = argv[i];

		if (strstr(arg,substr) != NULL)
		{
			++subjectCount;
		}
	}

	return subjectCount;
}


int main(int argc, char* argv[])
{
	gmsub_t gmsub;

	if (gmsub_Initialize(&gmsub, argc, argv) == GMSEC_FALSE)
	{
		fprintf(stderr, "\nFailure of initializing the gmsub");
		return -1;
	}

	if (example_option_invalid(gmsub.config, argc) == GMSEC_TRUE)
	{
		gmsub_printUsage();
		return -1;
	}

	gmsub_Run(&gmsub, gmsub_DetermineSubCount(argc, argv));
	gmsub_Cleanup(&gmsub, gmsub_DetermineSubCount(argc, argv));

	return 0;
}


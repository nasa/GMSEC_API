
/*
 * Copyright 2007-2021 United States Government as represented by the
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
	GMSEC_STATUS_OBJECT status;
	GMSEC_CONFIG_OBJECT config;	
	GMSEC_CONNECTION_OBJECT connection;
	GMSEC_MESSAGE_OBJECT message;
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

int gmsub_Initialize(gmsub_t *this, int argc, char *argv[])
{
	this->status = NULL;
	this->connection = NULL;
	this->config = NULL;
	this->message = NULL;

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

int gmsub_Run(gmsub_t *this, int subjectCount)
{
	GMSEC_STR tmp;
	GMSEC_STR xml = NULL;
	GMSEC_STR temp_subject;
	GMSEC_MESSAGE_OBJECT temp_msg;
	const char *subject = NULL;
	int iterations;
	int msg_timeout_ms;
	int prog_timeout_s;
	int done;
	int count;

	time_t prevTime;
	time_t nextTime;
	double elapsedTime = 0; 

	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", 
        EXAMPLE_MSG_TIMEOUT);

	prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S",
		EXAMPLE_PROG_TIMEOUT);
	
	if(msg_timeout_ms == EXAMPLE_MSG_TIMEOUT && prog_timeout_s != EXAMPLE_PROG_TIMEOUT)
	{
		LOG_INFO("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
		//gmsub_printUsage();
		return GMSEC_FALSE;
	}

	/* Output the GMSEC API version */
	LOG_INFO("%s", gmsec_GetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);

	if (this->connection == NULL)
		return GMSEC_FALSE;

	/* Output the middleware version */
	tmp = NULL;
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
	if (subjectCount == 0)
	{
		example_getStringDefault(this->config, "SUBJECT", "GMSEC.>", &subject);
		LOG_INFO("Subscribing to %s", subject);
		gmsec_Subscribe(this->connection, subject, this->status);
		if (!example_check("Subscribing...", this->status))
			return GMSEC_FALSE;
	}
	else
	{
		int i=1;
		char* str2 = "subject.";
		for(; i <= subjectCount; i++)
		{
			char buff[20];
			snprintf(buff, sizeof(buff) - 1, "%s%d", str2, i);
			if(example_getString(this->config, buff, &subject))
			{
				LOG_INFO("Subscribing to %s", subject);
				gmsec_Subscribe(this->connection, subject, this->status);
				if (!example_check("Subscribing...", this->status))
					return GMSEC_FALSE;
			}
		}
	}

	subject = "GMSEC.TERMINATE"; 

	LOG_INFO("Subscribing to %s", subject);
	gmsec_Subscribe(this->connection, subject, this->status);
	if (!example_check("Subscribing...", this->status))
		return GMSEC_FALSE;

	done = GMSEC_FALSE;
	count = 0;
	time(&prevTime);

	/* Listen */
	while (!done)
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

			xml = NULL;
			gmsec_MsgToXML(this->message, &xml, this->status);
			if (!example_check("MsgToXML(xml)", this->status)) return GMSEC_FALSE;
				LOG_INFO("received\n%s", xml); 
			fflush(stdout);

			temp_subject = NULL;
			gmsec_GetMsgSubject(this->message, &temp_subject, 
				this->status);
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

void gmsub_Cleanup(gmsub_t *this)
{
	/* Destory message */
	if (this->message != NULL)
	{
		gmsec_DestroyMessage(this->connection, this->message, this->status);
		example_check("Final DestroyMessage", this->status);
	}

	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
		if (!example_check("Disconnection", this->status)) return;
	}

	/* Destroy the Config */
	gmsec_DestroyConfig(&(this->config), this->status);

	/* Destroy Status */
	gmsec_DestroyStatus(&(this->status));
}

int gmsub_DetermineSubCount(int argc, char* argv[])
{
	const char* substr = "subject.";
	int subjectCount = 0;
	int i = 0;
	for(; i < argc; i++)
	{	
		char* arg = argv[i];
		if(strstr(arg,substr)!=NULL)
		{
			subjectCount++;
		}
	}
	return subjectCount;
}

int main(int argc, char* argv[])
{
	gmsub_t gmsub;

	if (!gmsub_Initialize(&gmsub, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the gmsub");
		return -1;
	}

	if (example_option_invalid(gmsub.config, argc))
	{
		gmsub_printUsage();
		return -1;
	}

	gmsub_Run(&gmsub, gmsub_DetermineSubCount(argc, argv));

	gmsub_Cleanup(&gmsub);

	return 0;
}


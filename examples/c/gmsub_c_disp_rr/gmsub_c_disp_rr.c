/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmsub_c_disp_rr.c
 *
 * A C example demonstration of GMSEC subscriber using the dispatcher.
 *
 * The callback is registered with the Connection.  A seperate thread, transmits a request
 * and waits for a reply.  The Dispatcher is then started, which launches in its own
 * thread.  We're looking for contention issues with the message queue, you should be
 * able to recieve messages while still waiting for a reply.
 *
 * Notes:
 *  - Uses the GNU Common C++ library for cross platform threading support
 */

#include "../example.h"


typedef struct
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_Message    request;
	GMSEC_SubscriptionInfo info;
} gmsub_c_disp_rr_t;


void gmsub_c_disp_rr_printUsage()
{
	printf("\nusage: gmsub_c_disp_rr connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
	       "\n\tprog_timeout_s=<timeout period (seconds)>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


void CALL_TYPE onMessageCallback(GMSEC_Connection conn, const GMSEC_Message msg)
{
	if (msg)
	{
		/* Display XML representation of the message */
		GMSEC_INFO("onMessage Received Message:\n%s", messageToXML(msg, NULL));
	}
}


void CALL_TYPE onReplyCallback(GMSEC_Connection conn, const GMSEC_Message request, const GMSEC_Message reply)
{
	if (reply)
	{
		/* Display XML representation of the reply message */
		GMSEC_INFO("onReply Received Message:\n%s", messageToXML(reply, NULL));
	}
}


void CALL_TYPE onEventCallback(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_WARNING("onEventCallback: %s [event=%d]", statusGet(status), event);
	}
}


GMSEC_BOOL gmsub_c_disp_rr_Initialize(gmsub_c_disp_rr_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	configAddValue(this->config, "isThreaded", "GMSEC_TRUE", this->status);
	if (!example_check("configAddValue(isThreaded)", this->status)) return GMSEC_FALSE;

	/* Initialize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmsub_c_disp_rr_Run(gmsub_c_disp_rr_t *this)
{
	const char* subject = NULL;
	int         msg_timeout_ms = GMSEC_WAIT_FOREVER;
	int         prog_timeout_s = GMSEC_WAIT_FOREVER;
	int         deltaTime = 0;

	example_getStringDefault(this->config, "SUBJECT", "GMSEC.TEST.PUBLISH.NOREPLY", &subject);

	msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", msg_timeout_ms);
	prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", prog_timeout_s);

	/* output GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the Connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));

	/* Register the callback for Publish and Subscribe */
	GMSEC_INFO("Subscribing to Publisher");

	this->info = connectionSubscribeWithCallback(this->connection, subject, onMessageCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback", this->status)) return GMSEC_FALSE; 

	/* Launch the auto dispatcher in a seperate thread */
	GMSEC_INFO("Starting AutoDispatch");
	connectionStartAutoDispatch(this->connection, this->status);
	if (!example_check("connectionStartAutoDispatch", this->status)) return GMSEC_FALSE;

	/* Create a request and reply callback, and make a request */
	GMSEC_INFO("Creating a request message");
	this->request = messageCreate(subject, GMSEC_REQUEST, this->status);
	if (!example_check("messageCreate(request)", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Sending the request");
	connectionRequestWithCallback(this->connection, this->request, msg_timeout_ms, onReplyCallback, onEventCallback, 0, this->status);
	if (!example_check("connectionRequestWithCallback", this->status)) return GMSEC_FALSE;
  
 	//Wait 
	if (prog_timeout_s == GMSEC_WAIT_FOREVER)
	{
		GMSEC_INFO("Press the enter key to exit");
		example_await_input();
	}
	else
	{
		while (deltaTime < prog_timeout_s)
		{
			example_millisleep(1000);
			deltaTime += 1;
		}
		GMSEC_INFO("Program timeout reached!");
	}

	GMSEC_INFO("Stopping AutoDispatch");
	connectionStopAutoDispatch(this->connection, this->status);
	example_check("StopAutoDispatch", this->status);

	return GMSEC_TRUE;
}


void gmsub_c_disp_rr_Cleanup(gmsub_c_disp_rr_t* this)
{
	/* Destory the request message */
	if (this->request != NULL)
	{
		messageDestroy(&this->request);
	}

	GMSEC_INFO("Unsubscribing from %s", subscriptionInfoGetSubject(this->info));
	connectionUnsubscribe(this->connection, &(this->info), this->status);
	if (!example_check("Unsubscribing...", this->status))
	{
		GMSEC_ERROR("Problem with Unsubscribing...");
	}

	/* Destroy the connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
	}

	/* Destroy the Config */
	configDestroy(&this->config);

	/* Destroy the status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	gmsub_c_disp_rr_t gmsub_disp_rr = {NULL};

	if (!gmsub_c_disp_rr_Initialize(&gmsub_disp_rr, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	if (example_option_invalid(gmsub_disp_rr.config, argc))
	{
		gmsub_c_disp_rr_printUsage();
		return -1;
	}

	gmsub_c_disp_rr_Run(&gmsub_disp_rr);

	gmsub_c_disp_rr_Cleanup(&gmsub_disp_rr);

	return 0;
}

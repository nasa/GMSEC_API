/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmsub_c_disp.c
 *
 * A C example demonstation of GMSEC subscriber using dispatcher.
 *
 */

#include "../example.h"


typedef struct
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Connection connection;
	GMSEC_SubscriptionInfo info;
} gmsub_c_disp_t;


void gmsub_c_disp_printUsage()
{
	printf("\nusage: gmsub_c_disp connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


void CALL_TYPE onMessage(GMSEC_Connection conn, GMSEC_Message msg)
{
	GMSEC_Status status = statusCreate();

	if (msg)
	{
		/* Display XML representation of the received message */
		GMSEC_INFO("onMessage Received Message:\n%s", messageToXML(msg, status));
	}

	GMSEC_INFO("Press the enter key to exit");
}


GMSEC_BOOL gmsub_c_disp_Initialize(gmsub_c_disp_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	configAddValue(this->config, "isThreaded", "GMSEC_TRUE", this->status);
	if (!example_check("CreateAddValue(isThreaded)", this->status)) return GMSEC_FALSE;

	/* Initialize the configuration */
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	return GMSEC_TRUE;
}


GMSEC_BOOL gmsub_c_disp_Run(gmsub_c_disp_t* this)
{
	const char* subject = NULL;

	example_getStringDefault(this->config, "SUBJECT", "GMSEC.TEST.PUBLISH", &subject);

	/* output GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the Connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, this->status));

	/* Subscribe */
	GMSEC_INFO("Subscribing to Publisher");
	this->info = connectionSubscribeWithCallback(this->connection, subject, onMessage, this->status);
  	if (!example_check("SubscribeWCallback", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Starting AutoDispatch");
	connectionStartAutoDispatch(this->connection, this->status);
	if (!example_check("StartAutoDispatch", this->status)) return GMSEC_FALSE;

	GMSEC_INFO("Press the enter key to exit");

	/* Wait for any input */
	example_await_input();

	GMSEC_INFO("Stopping AutoDispatch");
	connectionStopAutoDispatch(this->connection, this->status);
	example_check("StopAutoDispatch", this->status);

	return GMSEC_TRUE;
}


void gmsub_c_disp_Cleanup(gmsub_c_disp_t* this)
{
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

	/* Destroy the Status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	gmsub_c_disp_t gmsub_disp = {NULL};

	if (!gmsub_c_disp_Initialize(&gmsub_disp, argc, argv))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	if (example_option_invalid(gmsub_disp.config, argc))
	{
		gmsub_c_disp_printUsage();
		return -1;
	}

	gmsub_c_disp_Run(&gmsub_disp);

	gmsub_c_disp_Cleanup(&gmsub_disp);

	return 0;
}

/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmconfig.c
 *
 * A C example demonstration of the GMSEC publisher.  This example loads 
 * connection and message information from a standard config file.
 *  
 */

#include "../example.h"


typedef struct
{
  GMSEC_Status     status;
  GMSEC_Config     config;
  GMSEC_ConfigFile config_file;
  GMSEC_Connection connection;
  GMSEC_Message    message;
  GMSEC_SubscriptionInfo info;
} gmconfig_t;


int gmconfig_Initialize(gmconfig_t* this, const char* cfgFilename)
{
	this->status      = statusCreate();
	this->config_file = configFileCreate();
	this->config      = NULL;
	this->connection  = NULL;
	this->message     = NULL;

	/* Load the configuration file and parse it */
	configFileLoad(this->config_file, cfgFilename, this->status);
	if (!example_check("configFileLoad", this->status))
	{
		fprintf(stderr, "%s\n", statusGet(this->status));
		return GMSEC_FALSE;
	}

	/* Get the configuration from the file */
	this->config = configFileLookupConfig(this->config_file, "config1", this->status);
	if (!example_check("configFileLookupConfig", this->status))
	{
		fprintf(stderr, "%s\n", statusGet(this->status));
		return GMSEC_FALSE;
	}

	/* Initilaize the configuration */
	example_initialize(this->config);

	return GMSEC_TRUE;
}


int gmconfig_Run(gmconfig_t *this)
{
	int k = 0;
	const char* pattern = NULL;

	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Dump the configuration */
	GMSEC_INFO("Using configuration:\n%s", configToXML(this->config, this->status));

	/* Establish the Connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	GMSEC_INFO("Middleware version: %s", connectionGetLibraryVersion(this->connection, this->status));

	/* Lookup subject pattern by name */
	pattern = configFileLookupSubscription(this->config_file, "events", this->status);
	if (!example_check("configFileLookupSubscription", this->status)) 
	{
		return GMSEC_FALSE;
	}

	GMSEC_INFO("Subscribing to: %s", pattern);

	/* Subscribe on the connection */
	this->info = connectionSubscribe(this->connection, pattern, this->status);
	if (!example_check("connectionSubscribe", this->status)) return GMSEC_FALSE;

	/* Get Message from the configuration file */
	this->message = configFileLookupMessage(this->config_file, "msg1", this->status);
	if (!example_check("configFileLookupMessage", this->status)) return GMSEC_FALSE;

	for (k=0; k < 10; ++k)
	{
		/* Display XML representation of the message */
		GMSEC_INFO("Prepared:\n%s", messageToXML(this->message, this->status));

		/* Publish Message */
		connectionPublish(this->connection, this->message, this->status);
		if (!example_check("Publish(connection)", this->status)) return GMSEC_FALSE;

		GMSEC_INFO("Message published!");
	}

	return GMSEC_TRUE;
}


void gmconfig_Cleanup(gmconfig_t *this)
{
	/* Destroy the Config */
	if (this->config != NULL)
	{
		configDestroy(&this->config);
	}

	/* Destroy the ConfigFile */
	if (this->config_file != NULL)
	{
		configFileDestroy(&this->config_file);
	}

	/* Destroy the Message */
	if (this->message != NULL)
	{
		messageDestroy(&(this->message));
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

	/* Destroy the Status */
	statusDestroy(&(this->status));
}


int main(int argc, char* argv[])
{
	gmconfig_t gmconfig;

	if (argc <= 1)
	{
		printf("usage: gmconfig_c <filename>\n");
		return -1;
	}

	if (!gmconfig_Initialize(&gmconfig, argv[1]))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	gmconfig_Run(&gmconfig);
	gmconfig_Cleanup(&gmconfig);

	return 0;
}

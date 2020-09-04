/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file recorder_c.c
 *
 * A C example demonstration of various GMSEC Recorder functionality.
 *
 * Contained within this example:

 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Creating a connection using the Config object
 * - Connecting to the network
 * - Using the XML file to store and set various other program configurations
 * - Subscribing to specific subjects to receieve messages through a callback
 * - Starting the automatic message dispatching thread
 * - Creating a heartbeat message from an XML definition
 * * Displaying message XML
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * - Disconnecting from the network
 * - Message memory cleanup
 * - Connection cleanup
 *
 * Any item with a * next to it is specific to this file in the overall example.
 */

#include "../example.h"


unsigned int globalHeartbeatCount = 0;

typedef struct
{
	GMSEC_Status     status;
	GMSEC_ConfigFile configFile;
	GMSEC_Config     config;
	GMSEC_Config     programConfig;
	GMSEC_Connection connection;
	GMSEC_Message    heartbeatMessage;
	GMSEC_SubscriptionInfo* info;
} recorder_c_t;


/*
 * Callback Class for Subscriptions 
 */
void recorder_c_logCallback(GMSEC_Connection conn, GMSEC_Message logMsg)
{
	/* Display the XML representation of the log message */
	GMSEC_INFO("Received Log Message:\n%s", messageToXML(logMsg, NULL));

	/* Note: The message does not have to be cleaned as it is owned by the framework */
}


GMSEC_BOOL recorder_c_Initialize(recorder_c_t *this, const char* cfgFilename)
{
	this->status     = statusCreate();
	this->configFile = configFileCreate();

	/* Load the configuration file and parse it */
	configFileLoad(this->configFile, cfgFilename, this->status);
	if (!example_check("configFileLoad", this->status)) return GMSEC_FALSE;

	/* Look up the connection configuration from the config file */
	this->config = configFileLookupConfig(this->configFile, "connection-config", this->status);
	if (!example_check("configFileLookupConfig(connection-config)", this->status)) return GMSEC_FALSE;

	/* Initilaize the configuration */
	example_initialize(this->config);

	return GMSEC_TRUE;
}


GMSEC_BOOL recorder_c_CreateHeartBeat(recorder_c_t* this, GMSEC_I16* heartbeatHolder)
{
	GMSEC_Field pubRateField = NULL;

	*heartbeatHolder = 0;

	/* Find and load the config file heartbeat message definition */
	this->heartbeatMessage = configFileLookupMessage(this->configFile, "C2CX-HEARTBEAT-REC", this->status);
	if (!example_check("configFileLookupMessage(C2CX-HEARTBEAT-REC)", this->status)) return GMSEC_FALSE;

	/* Obtain the publish rate field, by name, from the heartbeat message */
	pubRateField = messageGetI16Field(this->heartbeatMessage, "PUB-RATE", this->status);
	if (example_check("messageGetI16Field(PUB-RATE)", this->status))
	{
		/* Obtain the value from the field */
		*heartbeatHolder = i16FieldGetValue(pubRateField, NULL);
  }
  else
  {
		/* Default value */
		*heartbeatHolder = 30;
  }

  return GMSEC_TRUE;
}


void recorder_c_Publish(recorder_c_t *this, GMSEC_I16 loopCountdown, GMSEC_I16 updateRate, GMSEC_I16 heartbeatHolder)
{
	GMSEC_I16  heartbeatCountdown = 0;
	GMSEC_Time timeBuffer;

	/* Output some general program information */
	GMSEC_INFO("Publishing for %d seconds.", loopCountdown);
	GMSEC_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatHolder);

	/* Call to create the thread that will automatically retrieve messages off the message queue */
	connectionStartAutoDispatch(this->connection, this->status);

	/* Start time */
	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("Start Time: %s", timeBuffer);

	/* Publishing loop */
	for (; loopCountdown > 0; --loopCountdown)
	{
		/* When the countdown reaches 0, we publish a heartbeat message and reset the counter */
		if (heartbeatCountdown < 1)
		{
			++globalHeartbeatCount;

			/* Update the message counter */
			messageAddI16Field(this->heartbeatMessage, "COUNTER", (GMSEC_I16) globalHeartbeatCount, NULL);

			/* Publish the heartbeat message */
			connectionPublish(this->connection, this->heartbeatMessage, this->status);
			if (!example_check("connectionPublish(heartbeatMessage)", this->status)) continue; 

			/* Output a heartbeat marker and reset the counter */
			GMSEC_INFO("Published Heartbeat");
			heartbeatCountdown = heartbeatHolder;
		}

		// Decrement the counters
		heartbeatCountdown -= updateRate;

		/* Sleep for 1 second */
		example_sleep(updateRate);
	}

	/* End time */
	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("End Time: %s", timeBuffer);
}


GMSEC_BOOL recorder_c_Run(recorder_c_t* this)
{
	GMSEC_I16 updateRate       = 1;
	GMSEC_I16 heartbeatPubRate = 30;
	GMSEC_I16 loopTimes        = 30;
	const char* subject        = NULL;

	/* Look up additional program configutation information */
	this->programConfig = configFileLookupConfig(this->configFile, "program-config", this->status);
	if (!example_check("configFileLookupConfig(program-config)", this->status)) return GMSEC_FALSE;

	/* Get program configurable values */
	updateRate = configGetIntegerValueWithDefault(this->programConfig, "update-rate", updateRate, this->status);
	loopTimes  = configGetIntegerValueWithDefault(this->programConfig, "loop-time", loopTimes, this->status);


	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	// output GMSEC API and Middleware info, just for display purposes
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));


	/* Create subscriptions from subscription templates in the configuration file using callbacks */
	this->info = (GMSEC_SubscriptionInfo *)malloc(2 * sizeof(GMSEC_SubscriptionInfo));
	
	subject = configFileLookupSubscription(this->configFile, "RECEIVE-LOG", this->status);
	if (!example_check("configFileLookupSubscription(RECEIVE-LOG)", this->status)) return GMSEC_FALSE;

	this->info[0] = connectionSubscribeWithCallback(this->connection, subject, recorder_c_logCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback(logCallback)", this->status)) return GMSEC_FALSE;

	subject = configFileLookupSubscription(this->configFile, "SEND-LOG", this->status);
	if (!example_check("configFileLookupSubscription(SEND-LOG)", this->status)) return GMSEC_FALSE;

	this->info[1] = connectionSubscribeWithCallback(this->connection, subject, recorder_c_logCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback(logCallback)", this->status)) return GMSEC_FALSE;


	/* Create Heartbeat Message */
	if (!recorder_c_CreateHeartBeat(this, &heartbeatPubRate))
	{
		return GMSEC_FALSE;
	}

	/* Main Publishing Loop */
	recorder_c_Publish(this, loopTimes, updateRate, heartbeatPubRate);

	return GMSEC_TRUE;
} 


void recorder_c_Cleanup(recorder_c_t* this)
{
	int i;
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

	if (this->connection != NULL)
	{
		/* Stop auto dispatcher */
		connectionStopAutoDispatch(this->connection, NULL);

  		/* Destroy the connection */
		example_closeConnection(this->connection, NULL);
	}

	/* Destroy the heartbeat message */
	if (this->heartbeatMessage != NULL)
	{
		messageDestroy(&this->heartbeatMessage);
	}

	/* Destroy the configuration file */
	if (this->configFile != NULL)
	{
		configFileDestroy(&this->configFile);
	}

	/* Destroy the config objects/variables */
	if (this->config != NULL)
	{
		configDestroy(&this->config);
	}

	/* Destroy the program config objects/variables */
	if (this->programConfig != NULL)
	{
		configDestroy(&this->programConfig);
	}

	/* Destroy the Status */
	if (this->status != NULL)
	{
		statusDestroy(&this->status);
	}
}


int main(int argc, char* argv[])
{
	recorder_c_t recorder = {NULL};

	/* Simple check for proper usage */
	if (argc < 2)
	{
		printf("Usage: %s <XML config filename>\n\n", argv[0]);
		exit(-1);
	}

	if (!recorder_c_Initialize(&recorder, argv[1]))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	recorder_c_Run(&recorder);

	recorder_c_Cleanup(&recorder);

	return 0;
}

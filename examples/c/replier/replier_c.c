/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file Replier_c.c
 *
 * A C example demonstration of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Creating a connection using the Config object
 * - Connecting to the network
 * - Using the XML file to store and set various other program configurations
 * - Subscribing to specific subjects to receieve messages through a callback
 * - Creating a heartbeat message from an XML definition
 * - Starting the automatic message dispatching thread
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 *   * Creating a response message from an XML definition upon receiving a request
 *   * Programatically altering a field in the response message
 *   * Using the Reply call to send a reply message
 * - Creating a log message from an XML definition
 * - Programatically altering a field in the log message
 * - Publishing the log message using the Publish call
 * - Disconnecting from the network
 * - Message memory cleanup
 * - Connection cleanup
 *
 * Any item with a * next to it is specific to this file in the overall example.
*/

#include "../example.h"


GMSEC_ConfigFile g_configFile = NULL;


typedef struct
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Config     programConfig;
	GMSEC_Connection connection;
	GMSEC_Message    heartbeatMessage;
	GMSEC_SubscriptionInfo info;
} replier_c_t;


typedef struct
{
	GMSEC_Status     status;
	GMSEC_Message    replyMessage;
	GMSEC_Message    logMessage;
	char*            stringHolder;
} replier_c_callback_t;


const size_t MAX_STR_LEN = 255;


void replier_c_cb_Initialize(replier_c_callback_t* this)
{
	this->status       = statusCreate();
	this->stringHolder = malloc(MAX_STR_LEN);

	strncpy(this->stringHolder, "", MAX_STR_LEN - 1);
}


void replier_c_cb_Run(replier_c_callback_t* this, GMSEC_Connection conn, GMSEC_Message request)
{
	GMSEC_Field   temp_field;
	GMSEC_Message temp_message;
	const char*   dir_keyword = NULL;
	const char*   subject = NULL;
	GMSEC_Time    timeBuffer;


	/* Obtain the DIRECTIVE-KEYWORD field from the message. It holds the request count. */
	temp_field = messageGetStringField(request, "DIRECTIVE-KEYWORD", this->status);
	if (!example_check("messageGetStringField(DIRECTIVE-KEYWORD)", this->status)) return;

	dir_keyword = stringFieldGetValue(temp_field, NULL);


	/* Copy and output the request number which has been placed in the DIRECTIVE-KEYWORD field */
	strncpy(this->stringHolder, dir_keyword, MAX_STR_LEN - 1);
	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("onMessage Received Request (%s): %s", this->stringHolder, timeBuffer);


	/* Lookup and load the config file directive response message definition */
	temp_message = configFileLookupMessage(g_configFile, "DIRECTIVE-RESPONSE", this->status);
	if (!example_check("LookupConfigFileMessage via callback", this->status)) return;


	/* Replace the DATA field with the request number we are responding to */
	messageAddStringField(temp_message, "DATA", this->stringHolder, this->status);
	if (!example_check("MsgAddField(DATA)", this->status)) return;


	/* Replace the TIME-COMPLETE field with the correct time */
	messageAddStringField(temp_message, "TIME-COMPLETED", timeBuffer, this->status);
	if (!example_check("MsgAddField(TIME-COMPLETED)", this->status)) return;


	/* Get the request message's subject */
	subject = messageGetSubject(request, NULL);


	/* Create reply message */
	this->replyMessage = messageCreate(subject, GMSEC_REPLY, NULL);


	/* copy fields from temporary message to reply message */
	messageCopyFields(temp_message, this->replyMessage, NULL);


	/* destroy temporary message */
	messageDestroy(&temp_message);


	/* Send the reply */
	connectionReply(conn, request, this->replyMessage, this->status);
	if (!example_check("connectionReply", this->status)) return;

	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("onMessage Reply Sent (%s): %s", this->stringHolder, timeBuffer);


	/* Look up and load the config file log message definition */
	this->logMessage = configFileLookupMessage(g_configFile, "LOG-REP", this->status);
	if (!example_check("configFileLookupMessage(LOG-REP)", this->status)) return;

	/* Fill the log message time */
	messageAddStringField(this->logMessage, "EVENT-TIME", timeBuffer, this->status);
	example_check("messageAddStringField(EVENT-TIME)", this->status);

	/* Fill the log message message text */
	messageAddStringField(this->logMessage, "MSG-TEXT", this->stringHolder, this->status);
	example_check("messageAddStringField(MSG-TEXT)", this->status);

	// Publish the log message
	connectionPublish(conn, this->logMessage, this->status);
	if (example_check("Publish via callback", this->status))
	{
		GMSEC_INFO("onMessage Published log message");
	}
}


void replier_c_cb_Cleanup(replier_c_callback_t* this, GMSEC_Connection conn)
{
	if (this->logMessage != NULL)
	{
		messageDestroy(&this->logMessage);
	}

	if (this->replyMessage != NULL)
	{
		messageDestroy(&this->replyMessage);
	}

	statusDestroy(&this->status);

	if (this->stringHolder)
	{
		free(this->stringHolder);
		this->stringHolder = NULL;
	}
}


/**
 *  Callback Class for Request-Reply
 */
void onMessageCallback(GMSEC_Connection conn, GMSEC_Message msg)
{
	if (msg != NULL && messageGetKind(msg, NULL) == GMSEC_REQUEST)
	{
		replier_c_callback_t replier_cb = {NULL};

		replier_c_cb_Initialize(&replier_cb);
		replier_c_cb_Run(&replier_cb, conn, msg);
		replier_c_cb_Cleanup(&replier_cb, conn);
	}
}


int replier_c_Initialize(replier_c_t* this, const char* cfg_file_name)
{
	this->status = statusCreate();
	g_configFile = configFileCreate();

	/* Load the configuration file and parse it */
	configFileLoad(g_configFile, cfg_file_name, this->status);
	if (!example_check("LoadConfigFile", this->status)) return GMSEC_FALSE;

	/* Get the configuration from the file */
	this->config = configFileLookupConfig(g_configFile, "connection-config", this->status);
	if (!example_check("LookupConfigFileConfig", this->status)) return GMSEC_FALSE;

	/* Initilaize the configuration */
	example_initialize(this->config);

	return GMSEC_TRUE;
}


int replier_c_CreateHeartbeat(replier_c_t* this, GMSEC_I16* heartbeatPubRate)
{
	GMSEC_Field pub_rate_field = NULL;

	/* Find and load the config file heartbeat message definition */
	this->heartbeatMessage = configFileLookupMessage(g_configFile, "C2CX-HEARTBEAT-REP", this->status);
	if (!example_check("configFileLookupMessage(C2CX-HEARTBEAT-REP)", this->status)) return GMSEC_FALSE;

	/* Obtain the publish rate field, by name, from the heartbeat message */
	pub_rate_field = messageGetI16Field(this->heartbeatMessage, "PUB-RATE", this->status);
	if (example_check("messageGetI16Field(PUB-RATE)", this->status))
	{
		/* Obtain the value from the field */
		*heartbeatPubRate = i16FieldGetValue(pub_rate_field, this->status);
		if (!example_check("i16FieldGetValue(PUB-RATE)", this->status)) return GMSEC_FALSE;
	}
	else
	{
		/* Default value */
		*heartbeatPubRate = 30;
	}

	return GMSEC_TRUE;
}


void replier_c_Publish(replier_c_t* this, GMSEC_I16 loopTime, GMSEC_I16 updateRate, GMSEC_I16 heartbeatPubRate)
{
	GMSEC_I16  heartbeatCount = 0;
	GMSEC_I16  heartbeatCountdown = 0;
	GMSEC_Time timeBuffer;

	/* Output some general program information */
	GMSEC_INFO("Publishing for %d seconds.", loopTime);
	GMSEC_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatPubRate);

	/* Call to create the thread that will automatically retrieve messages off the message queue */
	connectionStartAutoDispatch(this->connection, this->status);
	if (!example_check("connectionStartAutoDispatch", this->status)) return;

	/* Start time */
	timeUtilFormatTime(timeUtilGetCurrentTime(), timeBuffer);
	GMSEC_INFO("Start Time: %s", timeBuffer);

	/* Publishing loop */
	for (; loopTime > 0; --loopTime)
	{
		/* When the countdown reaches 0, we publish a heartbeat message and reset the counter */
		if (heartbeatCountdown < 1)
		{
			++heartbeatCount;

			/* Update the message counter */
			messageAddI16Field(this->heartbeatMessage, "COUNTER", (GMSEC_I16) heartbeatCount, this->status);
			example_check("messageAddI16Field(COUNTER)", this->status);

			/* Publish the heartbeat message */
			connectionPublish(this->connection, this->heartbeatMessage, this->status);
			example_check("Publish(heartbeatMessage)", this->status);

			/* Ouput a heartbeat marker and reset the counter */
			GMSEC_INFO("Published Heartbeat");
			heartbeatCountdown = heartbeatPubRate;
		}

		/* Decrement the counters */
		heartbeatCountdown -= updateRate;

		/* Sleep for 1 second */
		example_sleep(updateRate);
	}

	/* End time */
	timeUtilFormatTime(timeUtilGetCurrentTime(), timeBuffer);
	GMSEC_INFO("End Time: %s", timeBuffer);
}


GMSEC_BOOL replier_c_Run(replier_c_t* this)
{
	const char* subject = NULL;
	GMSEC_I16   heartbeatPubRate = 0;
	GMSEC_I16   loopTime = 30;
	GMSEC_I16   updateRate  = 1;


	/* Look up subscription subject/topic for DIRECTIVE-REQUEST */
	subject = configFileLookupSubscription(g_configFile, "DIRECTIVE-REQUEST", this->status);
	if (!example_check("configFileLookupSubscription(DIRECTIVE-REQUEST)", this->status)) return GMSEC_FALSE;

	/* Look up the program configutation information from the config file */
	this->programConfig = configFileLookupConfig(g_configFile, "program-config", this->status);
	if (!example_check("configFileLookupConfig(program-config)", this->status)) return GMSEC_FALSE;

	/* Get program configuration values, if available */
	updateRate = configGetIntegerValueWithDefault(this->programConfig, "update-rate", updateRate, this->status);
	if (!example_check("configGetIntegerValueWithDefault(update-rate)", this->status)) return GMSEC_FALSE;

	loopTime = configGetIntegerValueWithDefault(this->programConfig, "loop-time", loopTime, this->status);
	if (!example_check("configGetIntegerValueWithDefault(loop-time)", this->status)) return GMSEC_FALSE;


	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* output GMSEC API and Middleware info, just for display purposes */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));

	/* Create subscriptions from subscription templates in the config file */
	this->info = connectionSubscribeWithCallback(this->connection, subject, onMessageCallback, this->status);
	if (!example_check("connectionSubscribeWithCallback(onMessageCallback)", this->status)) return GMSEC_FALSE;


	/* Create Heartbeat Message */
	if (!replier_c_CreateHeartbeat(this, &heartbeatPubRate))
		return GMSEC_FALSE;


	/* Main Publishing Loop */
	replier_c_Publish(this, loopTime, updateRate, heartbeatPubRate);


	return GMSEC_TRUE;
}


void replier_c_Cleanup(replier_c_t* this)
{
	GMSEC_INFO("Unsubscribing from %s", subscriptionInfoGetSubject(this->info));
	connectionUnsubscribe(this->connection, &(this->info), this->status);
	if (!example_check("Unsubscribing...", this->status))
	{
		GMSEC_ERROR("Problem with Unsubscribing...");
	}
	
	/* Stop auto dispatcher and destroy the connection */
	if (this->connection != NULL)
	{
		connectionStopAutoDispatch(this->connection, this->status);
		if (!example_check("StopAutoDispatch", this->status)) return;

		example_closeConnection(this->connection, this->status);
		if (!example_check("Final Disconnection", this->status)) return;
	}

	/* Destroy the heartbeat message */
	if (this->heartbeatMessage != NULL)
	{
		messageDestroy(&this->heartbeatMessage);
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

	/* Destroy the configuration file */
	if (g_configFile != NULL)
	{
		configFileDestroy(&g_configFile);
	}
}


int main(int argc, char *argv[])
{
	replier_c_t replier = {NULL};

	/* Simple check for proper usage */
	if (argc < 2)
	{
		printf("Usage: %s <XML config filename>\n\n", argv[0]);
		exit(-1);
	}

	if (!replier_c_Initialize(&replier, argv[1]))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	replier_c_Run(&replier);

	replier_c_Cleanup(&replier);

	return 0;
}

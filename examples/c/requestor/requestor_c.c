/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file Requestor_c.c
 *
 * A C example demonstration of GMSEC Requestor functionality.
 *
 * Contained within this example:
 * 
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Creating a connection using the Config object
 * - Connecting to the network
 * - Using the XML file to store and set various other program configurations
 * - Creating a heartbeat message from an XML definition
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 *   * Creating a directive request message from an XML definition
 *   * Publishing the directive request message using the Request call
 *   * Using a callback class object to receieve a response from the request call
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


/* A global configuratoin file */
GMSEC_ConfigFile g_configFile = NULL;

typedef struct
{
	GMSEC_Status     status;
	GMSEC_Config     config;
	GMSEC_Config     programConfig;
	GMSEC_Connection connection;
	GMSEC_Message    heartbeatMessage;
	GMSEC_Message    directiveRequestMessage;
	char*            tempString;
} requestor_c_t;

typedef struct
{
	GMSEC_Status  status;
	GMSEC_Message logMessage;
	char*         stringHolder;
} requestor_c_callback_t;

const size_t MAX_STR_LEN = 255;


void requestor_c_cb_Initialize(requestor_c_callback_t* this)
{
	this->status       = statusCreate();
	this->stringHolder = malloc(MAX_STR_LEN);

	strncpy(this->stringHolder, "", MAX_STR_LEN - 1);
}


void requestor_c_cb_Run(requestor_c_callback_t* this, GMSEC_Connection conn, const GMSEC_Message reply)
{
	GMSEC_Field responseField = NULL;
	GMSEC_Field dataField = NULL;
	GMSEC_I16   response;
	const char* data = NULL;

	GMSEC_Time timeBuffer;
	example_getCurrentTime(&timeBuffer);

	/* Get the status of the response (successful, failed, ect) and act accordingly */
	responseField = messageGetI16Field(reply, "RESPONSE-STATUS", this->status);
	if (!example_check("messageGetI16Field(RESPONSE-STATUS)", this->status)) return;

	response = i16FieldGetValue(responseField, NULL);

	switch (response)
	{
		case 1:
			GMSEC_INFO("Acknowledgement");
			break;

		case 2:
			GMSEC_INFO("Working/keep alive");
			break;

		case 3:
			/* Obtain the DATA field which was filled with what request this reply was generated from */
			dataField = messageGetStringField(reply, "DATA", this->status);
			if (!example_check("messageGetStringField(DATA)", this->status)) return;

			data = stringFieldGetValue(dataField, NULL);
			strncpy(this->stringHolder, data, MAX_STR_LEN - 1);

			/* Output that we've recieved a reply */
			GMSEC_INFO("onReplyCallback Received Reply (%s): %s", data, timeBuffer);
			break;

		case 4:
			GMSEC_INFO("Failed completion");
			break;

		case 5:
			GMSEC_INFO("Invalid Request");
			break;

		default:
			break;
	}

	/* Lookup and load the config file log message definition */
	this->logMessage = configFileLookupMessage(g_configFile, "LOG-REQ", this->status);
	if (!example_check("configFileLookupMessage(LOG-REQ)", this->status)) return;

	/* Add fields to the log message */
	messageAddStringField(this->logMessage, "EVENT-TIME", timeBuffer, NULL);
	messageAddStringField(this->logMessage, "MSG-TEXT", this->stringHolder, NULL);

	// Publish the log message
	connectionPublish(conn, this->logMessage, this->status);
	if (example_check("connectionPublish", this->status))
	{
		GMSEC_INFO("onReplyCallback Published Log Message");
	}
}


void requestor_c_cb_Cleanup(requestor_c_callback_t* this)
{
	if (this->logMessage != NULL)
	{
		messageDestroy(&this->logMessage);
	}

	if (this->status)
	{
		statusDestroy(&this->status);
	}

	if (this->stringHolder)
	{
		free(this->stringHolder);
		this->stringHolder = NULL;
	}
}


/**
 * ReplyCallback for Request-Reply
 */
void onReplyCallback(GMSEC_Connection conn, const GMSEC_Message request, const GMSEC_Message reply)
{
	requestor_c_callback_t requestor_cb = {NULL};

	requestor_c_cb_Initialize(&requestor_cb);
	requestor_c_cb_Run(&requestor_cb, conn, reply);
	requestor_c_cb_Cleanup(&requestor_cb);
}

/**
 * EventCallback for Request-Reply
 */
void onEventCallback(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent event)
{
	if (statusIsError(status) == GMSEC_TRUE)
	{
		GMSEC_WARNING("onEventCallback: %s [event=%d]", statusGet(status), event);
	}
}


GMSEC_BOOL requestor_c_Initialize(requestor_c_t *this, const char* cfgFilename)
{
	g_configFile     = configFileCreate();
	this->status     = statusCreate();
	this->tempString = (char *) malloc(MAX_STR_LEN);

	/* Load the configuration file and parse it */  
	configFileLoad(g_configFile, cfgFilename, this->status);
	if (!example_check("configFileLoad", this->status)) return GMSEC_FALSE;

	this->config = configFileLookupConfig(g_configFile, "connection-config", this->status);
	if (!example_check("configFileLookupConfig(connection-config)", this->status)) return GMSEC_FALSE;

	/* Initilaize the configuration */
	example_initialize(this->config);

	return GMSEC_TRUE;
}


GMSEC_BOOL requestor_c_CreateHeartbeat(requestor_c_t* this, GMSEC_I16* heartbeatPubRate)
{
	GMSEC_Field pubRateField = NULL;

	/* Look up the config file heartbeat message definition */
	this->heartbeatMessage = configFileLookupMessage(g_configFile, "C2CX-HEARTBEAT-REQ", this->status);
	if (!example_check("configFileLookupMessage(C2CX-HEARTBEAT-REQ)", this->status)) return GMSEC_FALSE;

	/* Obtain the publish rate field, by name, from the heartbeat message */
	pubRateField = messageGetI16Field(this->heartbeatMessage, "PUB-RATE", this->status);

	if (example_check("messageGetI16Field(PUB-RATE)", this->status))
	{
		/* Obtain the value from the field */
		*heartbeatPubRate = i16FieldGetValue(pubRateField, NULL);
	}
	else
	{
		*heartbeatPubRate = 30;
	}

	return GMSEC_TRUE;
}


GMSEC_BOOL requestor_c_CreateDirective(requestor_c_t* this)
{
	/* Find and load the configuration file Directive Request Message definition */
	this->directiveRequestMessage = configFileLookupMessage(g_configFile, "DIRECTIVE-REQUEST", this->status);

	return example_check("configFileLookupMessage(DIRECTIVE-REQUEST)", this->status);
}


void requestor_c_Publish(requestor_c_t *this, GMSEC_I16 loopCountdown, GMSEC_I16 updateRate, GMSEC_I16 heartbeatPubRate, GMSEC_I16 directiveHolder)
{
	GMSEC_I16  requestCount = 0;
	GMSEC_I16  heartbeatCount = 0;
	GMSEC_I16  heartbeatCountdown = 0;
	GMSEC_I16  directiveCountdown = 0;
	GMSEC_Time timeBuffer;

	/* Output some general program information */
	GMSEC_INFO("Publishing for %d seconds.", loopCountdown);
	GMSEC_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatPubRate);
	GMSEC_INFO("Publishing Directive Request Messages every %d seconds.", directiveHolder);

	/* Start time */
	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("Start Time: %s", timeBuffer);

	/* Publishing loop */
	for (; loopCountdown > 0; --loopCountdown)
	{
		/* When the countdown reaches 0, we publish a directive message and reset the counter */
		if (directiveCountdown < 1)
    	{
			++requestCount;

			/* Alter the directive request to a unique string */
			snprintf(this->tempString, MAX_STR_LEN - 1, "%d", requestCount);

			messageAddStringField(this->directiveRequestMessage, "DIRECTIVE-KEYWORD", this->tempString, this->status);
			example_check("messageAddStringField(DIRECTIVE-KEYWORD)", this->status);

			/* Alter the directive request to a unique string */
			snprintf(this->tempString, MAX_STR_LEN - 1, "DIRECTIVE REQUEST MESSAGE %d", requestCount);

			messageAddStringField(this->directiveRequestMessage, "DIRECTIVE-STRING", this->tempString, this->status);
			example_check("messageAddStringField(DIRECTIVE-STRING)", this->status);

			/* Publish time */
			GMSEC_INFO("Publishing Directive Request Message (%d): %s", requestCount, timeBuffer);

			/* Send the request with the reply being send to the callback we created */
			connectionRequestWithCallback(this->connection,
			                              this->directiveRequestMessage,
			                              GMSEC_WAIT_FOREVER,
			                              onReplyCallback,
			                              onEventCallback,
			                              0,
			                              this->status);
			example_check("connectionRequestWithCallback", this->status);

			/* Reset the countdown */
			directiveCountdown = directiveHolder;
		}

		/* When the countdown reaches 0, we publish a heartbeat message and reset the counter */
		if (heartbeatCountdown < 1)
		{
			++heartbeatCount;

			/* Update the message counter */
			messageAddI16Field(this->heartbeatMessage, "COUNTER", (GMSEC_I16) heartbeatCount, this->status);
			example_check("messageAddI16Field(COUNTER)", this->status);

			/* Publish the heartbeat message */
			connectionPublish(this->connection, this->heartbeatMessage, this->status);
			example_check("connectionPublish(heartbeat)", this->status);

			// Ouput a heartbeat marker and reset the counter
			GMSEC_INFO("Published HeartBeat");
			heartbeatCountdown = heartbeatPubRate;
		}

		/* Decrement the counters */
		heartbeatCountdown -= updateRate;
		directiveCountdown -= updateRate;

		/* Sleep for 1 second */
		example_sleep(updateRate);
	}

	/* End time */
	example_getCurrentTime(&timeBuffer);
	GMSEC_INFO("End Time: %s", timeBuffer);
}


GMSEC_BOOL requestor_c_Run(requestor_c_t *this)
{
	GMSEC_I16 heartbeatPubRate = 30;
	GMSEC_I16 updateRate       = 1;
	GMSEC_I16 loopTimes        = 30;
	GMSEC_I16 sendRate         = 5;

	/* Look up additional program configutation information */
	this->programConfig = configFileLookupConfig(g_configFile, "program-config", this->status);
	if (!example_check("LookupConfigFileConfig(program-config)", this->status)) return GMSEC_FALSE;

	/* Get program configuration values */
	updateRate = configGetIntegerValueWithDefault(this->programConfig, "update-rate", updateRate, NULL);
	loopTimes  = configGetIntegerValueWithDefault(this->programConfig, "loop-time", loopTimes, NULL);
	sendRate   = configGetIntegerValueWithDefault(this->programConfig, "send-directive-rate", sendRate, NULL);


	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Establish the connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* output GMSEC API and Middleware info, just for display purposes */
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(this->connection, NULL));


	/* Create Heartbeat Message */
	if (!requestor_c_CreateHeartbeat(this, &heartbeatPubRate))
		return GMSEC_FALSE;

	/* Create Directive Request Message */
	if (!requestor_c_CreateDirective(this))
		return GMSEC_FALSE;

	/* Main Publishing Loop */
	requestor_c_Publish(this, loopTimes, updateRate, heartbeatPubRate, sendRate);

	return GMSEC_TRUE;
}


void requestor_c_Cleanup(requestor_c_t* this)
{
	if (this->tempString)
	{
		free(this->tempString);
	}

	/* Destroy the heartbeat message */
	if (this->heartbeatMessage != NULL)
	{
		messageDestroy(&this->heartbeatMessage);
	}

	/* Destroy the directive message */
	if (this->directiveRequestMessage!= NULL)
	{
		messageDestroy(&this->directiveRequestMessage);
	}

	/* Destroy the connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
	}

	/* Destroy the configuration file */
	if (g_configFile != NULL)
	{
		configFileDestroy(&g_configFile);
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
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	requestor_c_t requestor = {NULL};

	/* Simple check for proper usage */
	if (argc < 2)
	{
		printf("Usage: %s <XML config filename>\n\n", argv[0]);
		exit(-1);
	}

	if (!requestor_c_Initialize(&requestor, argv[1]))
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	requestor_c_Run(&requestor);

	requestor_c_Cleanup(&requestor);

	return 0;
}

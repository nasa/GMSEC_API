/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gm_msg_config.c
 *
 * A C example demonstration of the GMSEC interactive application.  It allows 
 * the user to select, display and publish one of several standard GMSEC 
 * messages.  This is for assisting GMSEC users in learning the format of GMSEC
 * standard messages as well as how to read them from a configuration file.
 * The configuration file used for the example code is the XML configuration 
 * file used by the GMSEC CTS which is 
 * "PublisherConfig-GMSEC_Message_Spec-2009_May.xml".
 *
 */

#include "../example.h"


static const char* gm_msg_config_types[] =
{ 
    "LOG", "ARCHIVE-MESSAGE-RETRIEVAL-REQUEST",
    "ARCHIVE-MESSAGE-RETRIEVAL-RESPONSE", "DIRECTIVE-REQUEST",
    "DIRECTIVE-RESPONSE", "C2CX-CONFIGURATION", "C2CX-CONTROL",
    "C2CX-DEVICE", "C2CX-HEARTBEAT", "C2CX-RESOURCE",
    "TELEMETRY-CCSDS-PACKET", "TELEMETRY-CCSDS-FRAME",
    "REPLAY-TELEMETRY-REQUEST", "REPLAY-TELEMETRY-RESPONSE",
    "MNEMONIC-VALUE-REQUEST", "MNEMONIC-VALUE-RESPONSE",
    "MNEMONIC-VALUE-DATA-MESSAGE", "ARCHIVE-MNEMONIC-VALUE-REQUEST",
    "ARCHIVE-MNEMONIC-VALUE-RESPONSE", "ARCHIVE-MNEMONIC-VALUE-DATA",
    "DATABASE-ATTRIBUTES-REQUEST", "DB-RESPONSE-LIMIT-SET",
    "DB-RESPONSE-TEXT-CONVERSION", "DB-RESPONSE-CAL-CURVE",
    "DB-RESPONSE-SHORT-DESCRIPTION", "DB-RESPONSE-LONG-DESCRIPTION",
    "DB-RESPONSE-LIST-OF-MNEMONICS", "COMMAND-REQUEST", "COMMAND-RESPONSE",
    "PRODUCT-REQUEST", "PRODUCT-RESPONSE", "PRODUCT-MSG"
};


typedef struct
{
  GMSEC_Status     status;
  GMSEC_Config     config;
  GMSEC_ConfigFile config_file;
  GMSEC_Connection connection;
  GMSEC_Message    message;
} gm_msg_config_t;


GMSEC_BOOL gm_msg_config_Initialize(gm_msg_config_t *this, const char* cfgFilename)
{
	this->status      = statusCreate();
	this->config_file = configFileCreate();
	this->config      = NULL;
	this->connection  = NULL;
	this->message     = NULL;

	/* Load the configuration file and parse it */
	configFileLoad(this->config_file, cfgFilename, this->status);
	if (!example_check("configFileLoad", this->status)) return GMSEC_FALSE;

	/* create the configuration  */
	this->config = configFileLookupConfig(this->config_file, "GMSEC-MESSAGE-BUS", this->status);
	if (!example_check("CreateConfig", this->status)) return GMSEC_FALSE;

	/* Initilaize the configuration */
	example_initialize(this->config);

	return GMSEC_TRUE;
}


void gm_msg_config_DisplayFormat(const char* format)
{
	GMSEC_INFO("\n===================================="
	           "====================================\n"
	           " -- Displaying Format for Message Type :  %s\n"
	           "===================================="
	           "====================================", format);
}


int gm_msg_config_Display(const char** message_string)
{
	int i = 0;
	int message_type = 0;
	int type_size = sizeof(gm_msg_config_types)/sizeof(gm_msg_config_types[0]);

	/* Display set of standard GMSEC messages to user. */
	printf("========================================================================\n");
	printf("Select the number of the desired GMSEC Message format to be displayed:\n");
	printf("========================================================================\n");

	/* Display the options */
	for (i = 0; i < type_size; i++)
	{
		printf("%2d.) %s\n", (i+1), gm_msg_config_types[i]);
	}
	message_type = example_input_integer(1, type_size);

	if (message_type == -1) 
	{
	return -1; 
	}

	*message_string = gm_msg_config_types[message_type - 1];

	return message_type;
}


GMSEC_BOOL gm_msg_config_Run(gm_msg_config_t* this)
{
	GMSEC_BOOL done = GMSEC_FALSE;


	/* Output the GMSEC API version */
	GMSEC_INFO("%s", connectionGetAPIVersion());

	/* Display the XML representaion of the configuration */
	GMSEC_INFO("Using configuration:\n%s", configToXML(this->config, this->status));

	/* Establish the Connection */
	this->connection = example_openConnection(this->config, this->status);
	if (this->connection == NULL) return GMSEC_FALSE;

	/* Output the middleware version */
	GMSEC_INFO("Middleware version: %s", connectionGetLibraryVersion(this->connection, this->status));

	while (!done)
	{
		int         message_type = 0;
		const char* message_string = NULL;

		/* Get the specific message type */
		message_type = gm_msg_config_Display(&message_string);

		if (message_type != -1)
		{
			GMSEC_MessageKind message_kind;

			/* Load the Message from ConfigFile */
			this->message = configFileLookupMessage(this->config_file, message_string, this->status);
			if (!example_check("configFileLookupMessage", this->status)) return GMSEC_FALSE;

			/* Get the message kind */
			message_kind = messageGetKind(this->message, this->status);

			gm_msg_config_DisplayFormat(message_string);

			/* Display XML representation of the message */
          	GMSEC_INFO("Prepared message:\n%s", messageToXML(this->message, this->status));

			switch (message_kind)
			{
				case GMSEC_PUBLISH:
					/* Publish Message */
					connectionPublish(this->connection, this->message, this->status);
					if (!example_check("connectionPublish", this->status)) return GMSEC_FALSE;
					GMSEC_INFO("Published!");
					break;

				case GMSEC_REQUEST:
					{
						GMSEC_Message* reply = connectionRequest(this->connection, this->message, 1000, 0, this->status);
					GMSEC_INFO("Request issued!");

						if (reply)
						{
							/* Display XML representation of the reply message */
							GMSEC_INFO("Received Reply:\n%s", messageToXML(reply, this->status));

							/* Destroy the reply message */
							connectionRelease(this->connection, reply, this->status);
						}
					}
					break;

				case GMSEC_REPLY:
					/* TODO: demonstrate example */
					break;
			}

			done = example_stopMenu("Display another message format");
		}
		else
		{
			done = GMSEC_TRUE;
		}
	}

	return GMSEC_TRUE;
}


void gm_msg_config_Cleanup(gm_msg_config_t *this)
{
	/* Destroy the Message */
	if (this->message != NULL)
	{
		messageDestroy(&this->message);
	}

	/* Destroy the connection */
	if (this->connection != NULL)
	{
		example_closeConnection(this->connection, this->status);
		if (!example_check("Disconnection", this->status)) return;
	}

	/* Destroy the Status */
	statusDestroy(&this->status);
}


int main(int argc, char* argv[])
{
	gm_msg_config_t gm_msg_config;

	if (argc <= 1)
	{
		printf("usage: gm_msg_config_c <filename.xml>\n");
		return -1;
	}

	if (!gm_msg_config_Initialize(&gm_msg_config, argv[1]))
	{
		fprintf(stderr, "\nFailure of initializing the example\n");
		return -1;
	}

	gm_msg_config_Run(&gm_msg_config);

	gm_msg_config_Cleanup(&gm_msg_config);

	return 0;
}

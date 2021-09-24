
/*
 * Copyright 2007-2021 United States Government as represented by the
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

static GMSEC_STR gm_msg_config_types[] = { 
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
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIGFILE_OBJECT config_file;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
} gm_msg_config_t;


int gm_msg_config_Initialize(gm_msg_config_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->config_file = NULL;
  this->message = NULL;

  /* create status */
  gmsec_CreateStatus(&(this->status));


  /* create the configuration file */
  gmsec_CreateConfigFile(&this->config_file, argv[1], this->status);
  if (!example_check("CreateConfigFile", this->status)) return GMSEC_FALSE;

  /* Load the configuration file and parse it */
  gmsec_LoadConfigFile(this->config_file, this->status);
  if (!example_check("LoadConfigFile", this->status)) return GMSEC_FALSE;

  /* create the configuration  */
  gmsec_CreateConfig(&this->config, this->status);
  if (!example_check("CreateConfig", this->status)) return GMSEC_FALSE;

  /* Get the configuration from the file */
  gmsec_LookupConfigFileConfig(this->config_file, "GMSEC-MESSAGE-BUS",
                               this->config, this->status);
  if (!example_check("LookupConfigFileConfig", this->status)) return GMSEC_FALSE;

  /* Initilaize the configuration */
  example_initialize(this->config);

  return GMSEC_TRUE;
}

void gm_msg_config_DisplayFormat(GMSEC_STR format)
{
  LOG_INFO("\n===================================="
           "====================================\n"
           " -- Displaying Format for Message Type :  %s\n"
           "===================================="
           "====================================", format);
}

int gm_msg_config_Display(GMSEC_STR *message_string)
{
  int i = 0;
  int message_type = 0;
  int type_size = sizeof(gm_msg_config_types)/sizeof(gm_msg_config_types[0]);

  /* Display set of standard GMSEC messages to user. */
  printf("===================================="
         "====================================\n");
  printf("Select the number of the desired GMSEC Message "
         "format to be displayed:\n");
  printf("===================================="
         "====================================\n");

  /* Display the options */
  for (i = 0; i < type_size; i++)
  {
     printf("%2d.) %s\n", (i+1), gm_msg_config_types[i]);
  }
  message_type = example_input_integer(1, type_size);

  if (message_type == -1) 
     return -1; 

  *message_string = gm_msg_config_types[message_type - 1];

  return message_type;
}


int gm_msg_config_Run(gm_msg_config_t *this)
{
   GMSEC_STR tmp = NULL;
   GMSEC_STR xml = NULL;

  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Dump the configuration */
  gmsec_ConfigToXML(this->config, &xml, this->status);
  if (!example_check("ConfigToXML", this->status)) return GMSEC_FALSE;
  if (xml)
  {
     LOG_INFO("received from the configuration =>\n%s", xml);
  }

  /* Establish the Connection */
  this->connection = example_openConnection(this->config, this->status);
  if (this->connection == NULL) return GMSEC_FALSE;

  /* Output the middleware version */
  gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;
  if (tmp)
  {
     LOG_INFO("%s", tmp);
  }

  int message_type = 0;
  GMSEC_STR message_string = NULL;

  int done = GMSEC_FALSE;
  while(!done)
  {
    message_type = 0;

    /* Create a Message */
    gmsec_CreateMessageDflt(this->connection, &this->message, this->status);
    if (!example_check("CreateMessageDflt", this->status)) return GMSEC_FALSE;

    /* Enter the specific message type; otherwise, exit it */
    message_type = gm_msg_config_Display(&message_string);
    if (message_type != -1)
    {
       /* Get Message from ConfigFile */
       gmsec_LookupConfigFileMessage(this->config_file, message_string,
                                     this->message, this->status);
       if (!example_check("LookupConfigFileMessage", this->status))
          return GMSEC_FALSE;

       gm_msg_config_DisplayFormat(message_string);

       /* Dump Message */
       xml = NULL;
       gmsec_MsgToXML(this->message, &xml, this->status);


       if (!example_check("MsgToXML", this->status)) return GMSEC_FALSE;

       if (xml)
       {
          LOG_INFO("prepared =>");
          printf("\n%s\n", xml);
       }

       /* Publish Message */
       gmsec_Publish(this->connection, this->message, this->status);
       if (!example_check("Publish", this->status)) return GMSEC_FALSE;
       LOG_INFO("published");

       /* Destroy the Message */
       GMSEC_MESSAGE_OBJECT temp_message = this->message;
       this->message = NULL; 
       gmsec_DestroyMessage(this->connection, temp_message, this->status);
       if (!example_check("DestroyMessage", this->status)) return GMSEC_FALSE;

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
      gmsec_DestroyMessage(this->connection, this->message, this->status);
      example_check("Final DestroyMessage", this->status);
  }

  /* Destroy the connection */
  if (this->connection != NULL)
  {
     example_closeConnection(this->connection, this->status);
     if (!example_check("Disconnection", this->status)) return;
  }

  /* Destroy the Status */
  gmsec_DestroyStatus(&this->status);
}


int main(int argc, char* argv[])
{
  gm_msg_config_t gm_msg_config;

  if (argc <= 1)
  {
     printf("usage: gm_msg_config_c <filename.xml>\n");
     return -1;
  }

  if (!gm_msg_config_Initialize(&gm_msg_config, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example\n");
     return -1;
  }

  gm_msg_config_Run(&gm_msg_config);

  gm_msg_config_Cleanup(&gm_msg_config);

  return 0;
}

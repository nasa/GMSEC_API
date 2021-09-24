
/*
 * Copyright 2007-2021 United States Government as represented by the
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

/* Global configuration file */
GMSEC_CONFIGFILE_OBJECT replier_c_configFile = NULL; 

typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIG_OBJECT programConfig;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT heartbeatMessage;
  GMSEC_FIELD_OBJECT tempField;
} replier_c_t;

typedef struct
{
  GMSEC_STATUS_OBJECT result;
  GMSEC_MESSAGE_OBJECT replyMessage;
  GMSEC_MESSAGE_OBJECT logMessage;
  GMSEC_FIELD_OBJECT tempField;
  char * stringHolder;
} replier_c_callback_t;

const size_t MAX_STR_LEN = 255;


void replier_c_cb_Initialize(replier_c_callback_t *this)
{
  this->result = NULL;
  this->tempField = NULL;
  this->replyMessage = NULL;
  this->logMessage = NULL;

  /* Create the status and field variables */
  gmsec_CreateStatus(&this->result);
  gmsec_CreateField(&this->tempField, this->result);

  /* Just create some temp memory for holding a string */
  this->stringHolder = (char *)malloc(MAX_STR_LEN);
  strncpy(this->stringHolder, "", MAX_STR_LEN - 1);
}


void replier_c_cb_Run(replier_c_callback_t *this,
                      GMSEC_CONNECTION_OBJECT conn,
                      GMSEC_MESSAGE_OBJECT msg)
{
  GMSEC_MSG_KIND msgKind;
  GMSEC_STR subject;
  GMSEC_STR tempString = NULL;

  /* We don't care about anything but request messages */
  gmsec_GetMsgKind(msg, &msgKind, this->result);
  if (msgKind != GMSEC_MSG_REQUEST)
    return;

  /* Obtain the DIRECTIVE-KEYWORD field from the message.  
   * It holds the request count. 
   */
  gmsec_MsgGetField(msg, "DIRECTIVE-KEYWORD", this->tempField, this->result);
  gmsec_GetFieldValueSTR(this->tempField, &tempString, this->result);
  example_check("GetFieldValueSTR(DIRECTIVE-KEYWORD)", this->result);

  /* Copy and output the request number which has been placed in the 
   * DIRECTIVE-KEYWORD field
   */
  strncpy(this->stringHolder, tempString, MAX_STR_LEN - 1);
  LOG_INFO("nReceived Request (%s): %s", this->stringHolder, 
           gmsec_MsgGetTime());

  /* Create a generic message container for the directive response message */
  gmsec_CreateMessageDflt(conn, &this->replyMessage, this->result);
  example_check("CreateMessageDflt via callback", this->result);

  /* Find and load the config file directive response message definition */
  gmsec_LookupConfigFileMessage(replier_c_configFile, "DIRECTIVE-RESPONSE", 
                                this->replyMessage, this->result);
  example_check("LookupConfigFileMessage via callback", this->result);

  // Replace the DATA field with the request number we are responding to
  gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->result);
  gmsec_SetFieldName(this->tempField, "DATA", this->result);
  gmsec_SetFieldValueSTR(this->tempField, this->stringHolder, this->result);
  gmsec_MsgAddField(this->replyMessage, this->tempField, this->result);
  example_check("MsgAddField(DATA)", this->result);

  /* Replace the TIME-COMPLETE field with the correct time */
  gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->result);
  gmsec_SetFieldName(this->tempField, "TIME-COMPLETED", this->result);
  gmsec_SetFieldValueSTR(this->tempField, gmsec_MsgGetTime(), this->result);
  gmsec_MsgAddField(this->replyMessage, this->tempField, this->result);
  example_check("MsgAddField(TIME-COMPLETED)", this->result);

  /* Set the reply subject to the request message's subject */
  gmsec_GetMsgSubject(msg,  &subject, this->result);
  gmsec_SetMsgSubject(this->replyMessage, subject, this->result);
  example_check("GetMsgSubject(subject)", this->result);

  /* Send the reply */
  gmsec_Reply(conn, msg, this->replyMessage, this->result);
  example_check("Reply(replyMessage)", this->result);

  LOG_INFO("Reply Sent (%s): %s", this->stringHolder, gmsec_MsgGetTime());

  /* Create a generic message container for the log message */
  gmsec_CreateMessageDflt(conn, &this->logMessage, this->result);
  example_check("CreateMessageDflt(logMessage)", this->result);

  /* Find and load the config file log message definition */
  gmsec_LookupConfigFileMessage(replier_c_configFile, "LOG-REP", this->logMessage, 
                                this->result);
  example_check("LookupConfigFileMessage(LOG-REP)", this->result);

  /* Fill the log message time */
  gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->result);
  gmsec_SetFieldName(this->tempField, "EVENT-TIME", this->result);
  gmsec_SetFieldValueSTR(this->tempField, gmsec_MsgGetTime(), this->result);
  gmsec_MsgAddField(this->logMessage, this->tempField, this->result);
  example_check("MsgAddField(EVENT-TIME)", this->result);

  /* Fill the log message message text */
  gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->result);
  gmsec_SetFieldName(this->tempField, "MSG-TEXT", this->result);
  gmsec_SetFieldValueSTR(this->tempField, this->stringHolder, this->result);
  gmsec_MsgAddField(this->logMessage, this->tempField, this->result);
  example_check("MsgAddField(MSG-TEXT)", this->result);

  // Publish the log message
  gmsec_Publish(conn, this->logMessage, this->result);
  if (example_check("Publish via callback", this->result));
  {
     LOG_INFO("published via callback");
  }

}

void replier_c_cb_Cleanup(replier_c_callback_t *this, 
                          GMSEC_CONNECTION_OBJECT conn)
{
  if (this->tempField != NULL)
     gmsec_DestroyField(&this->tempField, this->result);

  if (this->logMessage != NULL)
     gmsec_DestroyMessage(conn, this->logMessage, this->result);

  if (this->replyMessage != NULL)
     gmsec_DestroyMessage(conn, this->replyMessage, this->result);

  gmsec_DestroyStatus(&this->result);

  if (this->stringHolder)
     free(this->stringHolder);
}


/**
 *  Callback Class for Request-Reply
 */
void replier_c_RequestReplyCallback(GMSEC_CONNECTION_OBJECT conn,
                                    GMSEC_MESSAGE_OBJECT msg)
{
  replier_c_callback_t replier_cb;

  replier_c_cb_Initialize(&replier_cb);
  replier_c_cb_Run(&replier_cb, conn, msg);
  replier_c_cb_Cleanup(&replier_cb, conn);
}


int replier_c_Initialize(replier_c_t *this, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->programConfig = NULL;
  this->heartbeatMessage = NULL;
  this->tempField = NULL;

  /* Create status and field */
  gmsec_CreateStatus(&this->status);
  gmsec_CreateField(&this->tempField, this->status);

  /* create the configuration file */
  gmsec_CreateConfigFile(&replier_c_configFile, argv[1], this->status);
  if (!example_check("CreateConfigFile", this->status)) return GMSEC_FALSE;

  /* Load the configuration file and parse it */
  gmsec_LoadConfigFile(replier_c_configFile, this->status);
  if (!example_check("LoadConfigFile", this->status)) return GMSEC_FALSE;

  /* create the configuration  */
  gmsec_CreateConfig(&this->config, this->status);
  if (!example_check("CreateConfig", this->status)) return GMSEC_FALSE;

  /* Create the configuration object/variable for additional configuration */
  gmsec_CreateConfig(&this->programConfig, this->status);
  if (!example_check("CreateConfig(programConfig)", this->status))
     return GMSEC_FALSE;

  /* Get the configuration from the file */
  gmsec_LookupConfigFileConfig(replier_c_configFile, "connection-config",
                               this->config, this->status);
  if (!example_check("LookupConfigFileConfig", this->status)) return GMSEC_FALSE;

  /* Initilaize the configuration */
  example_initialize(this->config);

  return GMSEC_TRUE;
}


int replier_c_CreateHeartbeat(replier_c_t *this,
                                   GMSEC_I16 *heartbeatHolder)
{
  GMSEC_I16 tempHolder = 0;

  /* Create a generic message container for the heartbeat message */
  gmsec_CreateMessageDflt(this->connection, &this->heartbeatMessage,
                          this->status);
  if (!example_check("CreateMessageDflt(heartbeatMessage)", this->status))
     return GMSEC_FALSE;

  /* Find and load the config file heartbeat message definition */
  gmsec_LookupConfigFileMessage(replier_c_configFile, "C2CX-HEARTBEAT-REP",
                                this->heartbeatMessage, this->status);
  if (!example_check("LookupConfigFileMessage(C2CX-HEARTBEAT-REP)", this->status))
     return GMSEC_FALSE;

  /* Obtain the publish rate field, by name, from the heartbeat message */
  gmsec_MsgGetField(this->heartbeatMessage, "PUB-RATE", this->tempField,
                    this->status);
  if (example_check("MsgGetField(PUB-RATE)", this->status))
  {
     /* Obtain the value from the field */
     gmsec_GetFieldValueI16(this->tempField, &tempHolder, this->status);
     if (!example_check("GetFieldValueI16", this->status))
         return GMSEC_FALSE;
     *heartbeatHolder = tempHolder;
  }
  else
  {
    /* Default value */
    *heartbeatHolder = 30;
  }

  return GMSEC_TRUE;
}


void replier_c_Publish(replier_c_t *this,
                       GMSEC_I16 loopCountdown,
                       GMSEC_I16 updateRate,
                       GMSEC_I16 heartbeatHolder)
{
  GMSEC_I16 heartbeatCount = 0;
  GMSEC_I16 heartbeatCountdown = 0;

  /* Output some general program information */
  LOG_INFO("Publishing for %d seconds.", loopCountdown);
  LOG_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatHolder);

  /* Call to create the thread that will automatically retrieve messages off
   * the message queue
   */
  gmsec_StartAutoDispatch(this->connection, this->status);
  example_check("StartAutoDispatch", this->status);

  /* Start time */
  LOG_INFO("Start Time: %s", gmsec_MsgGetTime());

  /* Publishing loop */
  for (; loopCountdown > 0; loopCountdown--)
  {
    /* When the countdown reaches 0, we publish a heartbeat message and
     * reset the counter
     */
    if (heartbeatCountdown < 1)
    {
       heartbeatCount++;

       /* Update the message counter */
       gmsec_SetFieldType(this->tempField, GMSEC_TYPE_I16, this->status);
       gmsec_SetFieldName(this->tempField, "COUNTER", this->status);
       gmsec_SetFieldValueI16(this->tempField, heartbeatCount, this->status);
       gmsec_MsgAddField(this->heartbeatMessage, this->tempField, 
                         this->status);
       example_check("MsgAddField(COUNTER)", this->status);

       /* Publish the heartbeat message */
       gmsec_Publish(this->connection, this->heartbeatMessage, this->status);
       example_check("Publish(heartbeatMessage)", this->status);

       /* Ouput a heartbeat marker and reset the counter */
       LOG_INFO("Published Heartbeat");
       heartbeatCountdown = heartbeatHolder;
    }

    /* Decrement the counters */
    heartbeatCountdown -= updateRate;

    /* Sleep for 1 second */
    example_sleep(updateRate);
  }

  /* End time */
  LOG_INFO("End Time: %s", gmsec_MsgGetTime());
}


int replier_c_Run(replier_c_t *this)
{
  GMSEC_STR configValue = NULL;
  GMSEC_STR tempSubject = NULL;
  GMSEC_I16 heartbeatHolder= 0;
  GMSEC_I16 loopCountdown = 0;
  GMSEC_I16 updateRate = 0;

  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Establish the connection */
  this->connection = example_openConnection(this->config, this->status);
  if (this->connection == NULL) return GMSEC_FALSE;

  /* output GMSEC API and Middleware info, just for display purposes */
  gmsec_GetLibraryVersion(this->connection, &configValue, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;
  if (configValue)
  {
     LOG_INFO("%s", configValue);
  }

  /* Look up additional program configutation information */
  gmsec_LookupConfigFileConfig(replier_c_configFile, "program-config",
                                this->programConfig, this->status);
  if (!example_check("LookupConfigFileConfig(program-config)", this->status))
     return GMSEC_FALSE;

  /* Set program config values */
  gmsec_ConfigGetValue(this->programConfig, "update-rate", &configValue,
                       this->status);
  if (!example_check("ConfigGetValue(update-rate)", this->status)) return GMSEC_FALSE;

  /* Get updateRate */
  if (!example_strToI16(configValue, &updateRate)) return GMSEC_FALSE;

  gmsec_ConfigGetValue(this->programConfig, "loop-time", &configValue,
                       this->status);
  if (!example_check("ConfigGetValue(loop-time)", this->status)) return GMSEC_FALSE;

  /* Get loopCountdown */
  if (!example_strToI16(configValue, &loopCountdown)) return GMSEC_FALSE;

  /* Create subscriptions from subscription templates in the config file */
  gmsec_LookupConfigFileSubscription(replier_c_configFile, "DIRECTIVE-REQUEST",
                                     &tempSubject, this->status);
  if (!example_check("LookupConfigFileSubscription(DIRECTIVE-REQUEST)",
                     this->status))
     return GMSEC_FALSE;

  gmsec_SubscribeWCallback(this->connection, tempSubject,
                           replier_c_RequestReplyCallback,
                           this->status);
  if (!example_check("SubscribeWCallback(logCallback)", this->status))
     return GMSEC_FALSE;

  /* Create Heartbeat Message */
  if (!replier_c_CreateHeartbeat(this, &heartbeatHolder))
     return GMSEC_FALSE;

  /* Main Publishing Loop */
  replier_c_Publish(this, loopCountdown, updateRate, heartbeatHolder);

  return GMSEC_TRUE;
}


void replier_c_Cleanup(replier_c_t *this)
{
  /* Stop auto dispatcher */
  gmsec_StopAutoDispatch(this->connection, this->status);
  if (!example_check("StopAutoDispatch", this->status)) return;

  /* Destroy the field */
  if (this->tempField != NULL)
  {
     gmsec_DestroyField(&this->tempField, this->status);
     if (!example_check("Final DestroyField", this->status)) return;
  }

  /* Destroy the heartbeat message */
  if (this->heartbeatMessage != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->heartbeatMessage,
                          this->status);
     if (!example_check("Final DestroyMessage", this->status)) return;
  }

  /* Destroy the connection */
  if (this->connection != NULL)
  {
     example_closeConnection(this->connection, this->status);
     if (!example_check("Final Disconnection", this->status)) return;
  }

  /* Destroy the configuration file */
  if (replier_c_configFile != NULL)
  {
     gmsec_DestroyConfigFile(&replier_c_configFile, this->status);
     if (!example_check("Final DestroyConfigFile", this->status)) return;
  }

  /* Destroy the config objects/variables */
  if (this->config != NULL)
  {
     gmsec_DestroyConfig(&this->config, this->status);
     if (!example_check("Final DestroyConfig", this->status)) return;
  }

  /* Destroy the program config objects/variables */
  if (this->programConfig != NULL)
  {
     gmsec_DestroyConfig(&this->programConfig, this->status);
     if (!example_check("Final DestroyConfig(program)", this->status)) return;
  }

  /* Destroy the Status */
  gmsec_DestroyStatus(&this->status);
}


int main(int argc, char *argv[])
{
  replier_c_t replier;

  /* Simple check for proper usage */
  if (argc < 2)
  {
    printf("Usage: %s <XML config filename>\n\n", argv[0]);
    exit(-1);
  }

  if (!replier_c_Initialize(&replier, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  replier_c_Run(&replier);

  replier_c_Cleanup(&replier);

  return 0;
}

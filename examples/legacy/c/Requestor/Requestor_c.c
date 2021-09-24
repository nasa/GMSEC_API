
/*
 * Copyright 2007-2021 United States Government as represented by the
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
GMSEC_CONFIGFILE_OBJECT requestor_c_configFile = NULL;

typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIG_OBJECT programConfig;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT heartbeatMessage;
  GMSEC_MESSAGE_OBJECT directiveRequestMessage;
  GMSEC_FIELD_OBJECT tempField;
  char *tempString;
} requestor_c_t;

const size_t MAX_STR_LEN = 255;

typedef struct
{
  GMSEC_STATUS_OBJECT result;
  GMSEC_MESSAGE_OBJECT logMessage;
  GMSEC_FIELD_OBJECT tempField;
  char *stringHolder;
} requestor_c_callback_t;


void requestor_c_cb_Initialize(requestor_c_callback_t *this)
{
  this->result = NULL;
  this->tempField = NULL;
  this->logMessage = NULL;

  /* Create the status and field variables */
  gmsec_CreateStatus(&this->result);
  gmsec_CreateField(&this->tempField, this->result);

  /* Just create some temp memory for holding a string */
  this->stringHolder = (char *)malloc(MAX_STR_LEN);
  strncpy(this->stringHolder, "", MAX_STR_LEN - 1);
}


void requestor_c_cb_Run(requestor_c_callback_t *this,
                        GMSEC_CONNECTION_OBJECT conn,
                        GMSEC_MESSAGE_OBJECT msg)
{
  GMSEC_MSG_KIND msgKind;
  GMSEC_I16 tempStatus;
  GMSEC_STR tempString = NULL;

  /* We don't care about anything but reply messages */
  gmsec_GetMsgKind(msg, &msgKind, this->result);
  if (msgKind != GMSEC_MSG_REPLY)
    return;

  /* Get the status of the response (successful, failed, ect) 
   * and act accordingly
   */
  gmsec_MsgGetField(msg, "RESPONSE-STATUS", this->tempField, this->result);
  gmsec_GetFieldValueI16(this->tempField, &tempStatus, this->result);
  example_check("GetFieldValueI16(RESPONSE-STATUS)", this->result);

  switch (tempStatus)
  {
    case 1 :
      LOG_INFO("Acknowledgement");
      break;
    case 2 :
      LOG_INFO("Working/keep alive");
      break;
    case 3 :
      /* Obtain the DATA field which was filled with what request this 
       * reply was generated from
       */
      gmsec_MsgGetField(msg, "DATA", this->tempField, this->result);
      gmsec_GetFieldValueSTR(this->tempField, &tempString, this->result);
      example_check("GetFieldValueSTR(DATA)", this->result);
      strncpy(this->stringHolder, tempString, MAX_STR_LEN - 1);

      /* Output that we've recieved a reply */
      LOG_INFO("Recieved Reply (%s): %s", tempString, gmsec_MsgGetTime());
      break;
    case 4 :
      LOG_INFO("Failed completion");
      break;
    case 5 :
      LOG_INFO("Invalid Request");
      break;
    default :
      break;
  }

  /* Create a generic message container for the log message */
  gmsec_CreateMessageDflt(conn, &this->logMessage, this->result);
  example_check("CreateMessageDflt(logMessage)", this->result);

  /* Find and load the config file log message definition */
  gmsec_LookupConfigFileMessage(requestor_c_configFile, "LOG-REQ", this->logMessage, this->result);
  example_check("LookupConfigFileMessage(LOG-REQ)", this->result);

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
  if (example_check("Publish via callback", this->result))
  {
     LOG_INFO("published via callback");
  }
}


void requestor_c_cb_Cleanup(requestor_c_callback_t *this,
                            GMSEC_CONNECTION_OBJECT conn)
{
  if (this->tempField != NULL)
     gmsec_DestroyField(&this->tempField, this->result);

  if (this->logMessage != NULL)
     gmsec_DestroyMessage(conn, this->logMessage, this->result);

  gmsec_DestroyStatus(&this->result);

  if (this->stringHolder)
     free(this->stringHolder);
}


/**
 * Callback Class for Request-Reply
 */
void requestor_c_RequestReplyCallback(GMSEC_CONNECTION_OBJECT conn,
                                      GMSEC_MESSAGE_OBJECT msg)
{
  requestor_c_callback_t requestor_cb;

  requestor_c_cb_Initialize(&requestor_cb);
  requestor_c_cb_Run(&requestor_cb, conn, msg);
  requestor_c_cb_Cleanup(&requestor_cb, conn);
}


int requestor_c_Initialize(requestor_c_t *this, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->programConfig = NULL;
  this->heartbeatMessage = NULL;
  this->directiveRequestMessage = NULL;
  this->tempField = NULL;

  /* Create status and field */
  gmsec_CreateStatus(&this->status);
  gmsec_CreateField(&this->tempField, this->status);

  this->tempString = (char *) malloc(MAX_STR_LEN);

  /* Look up the configuration file value that contains the configuration 
   * for the middleware
   */
  gmsec_CreateConfigFile(&requestor_c_configFile, argv[1], this->status);
  if (!example_check("CreateConfigFile", this->status)) return GMSEC_FALSE;

  /* Load the configuration file and parse it */  
  gmsec_LoadConfigFile(requestor_c_configFile, this->status);
  if (!example_check("LoadConfigFile", this->status)) return GMSEC_FALSE;

  /* create config */
  gmsec_CreateConfig(&this->config, this->status);
  if (!example_check("CreateConfig(config)", this->status))
     return GMSEC_FALSE;

  gmsec_CreateConfig(&this->programConfig, this->status);
  if (!example_check("CreateConfig(programConfig)", this->status)) 
     return GMSEC_FALSE;

  gmsec_LookupConfigFileConfig(requestor_c_configFile, "connection-config", this->config, this->status);
  if (!example_check("LookupConfigFileConfig", this->status)) return GMSEC_FALSE;

  /* Initilaize the configuration */
  example_initialize(this->config);

  return GMSEC_TRUE;
}

int requestor_c_CreateHeartbeat(requestor_c_t *this,
                                    GMSEC_I16 *heartbeatHolder)
{
  GMSEC_I16 tempHolder = 0;

  /* Create a generic message container for the heartbeat message */
  gmsec_CreateMessageDflt(this->connection, &this->heartbeatMessage, this->status);
  if (!example_check("CreateMessageDflt(heartbeatMessage)", this->status))
     return GMSEC_FALSE;

  /* Find and load the config file heartbeat message definition */
  gmsec_LookupConfigFileMessage(requestor_c_configFile, "C2CX-HEARTBEAT-REQ",
                                this->heartbeatMessage, this->status);
  if (!example_check("LookupConfigFileMessage(C2CX-HEARTBEAT-REQ)", this->status))
     return GMSEC_FALSE;

  /* Obtain the publish rate field, by name, from the heartbeat message */
  gmsec_MsgGetField(this->heartbeatMessage, "PUB-RATE", this->tempField, this->status);
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
    *heartbeatHolder = 30;
  }

  return GMSEC_TRUE;
}


int requestor_c_CreateDirective(requestor_c_t *this)
{
  /* Create a generic message container for the Directive Request Message */
  gmsec_CreateMessageDflt(this->connection, &this->directiveRequestMessage,
                          this->status);
  if (!example_check("CreateMessageDflt(directive)", this->status))
     return GMSEC_FALSE;

  /* Find and load the configuration file Directive Request 
   * Message definition 
   */
  gmsec_LookupConfigFileMessage(requestor_c_configFile, "DIRECTIVE-REQUEST", this->directiveRequestMessage, this->status);
  if (!example_check("CreateMessageDflt(directive)", this->status))
     return GMSEC_FALSE;

  return GMSEC_TRUE;
}


void requestor_c_Publish(requestor_c_t *this, GMSEC_I16 loopCountdown,
                         GMSEC_I16 updateRate, GMSEC_I16 heartbeatHolder,
                         GMSEC_I16 directiveHolder)
{
  GMSEC_I16 requestCount = 0;
  GMSEC_I16 heartbeatCount = 0;
  GMSEC_I16 heartbeatCountdown = 0;
  GMSEC_I16 directiveCountdown = 0;

  /* Output some general program information */
  LOG_INFO("Publishing for %d seconds.", loopCountdown);
  LOG_INFO("Publishing Directive Request Messages every %d seconds.",
           directiveHolder);
  LOG_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatHolder);

  /* Start time */
  LOG_INFO("Start Time: %s", gmsec_MsgGetTime());

  /* Publishing loop */
  for (; loopCountdown > 0; loopCountdown--)
  {
    /* When the countdown reaches 0, we publish a directive message and
     * reset the counter
     */
    if (directiveCountdown < 1)
    {
       requestCount++;

      /* Alter the directive request to a unique string */
      snprintf(this->tempString, MAX_STR_LEN - 1, "%d", requestCount);
      gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->status);
      gmsec_SetFieldName(this->tempField, "DIRECTIVE-KEYWORD", this->status);
      gmsec_SetFieldValueSTR(this->tempField, this->tempString, this->status);
      gmsec_MsgAddField(this->directiveRequestMessage, this->tempField, this->status);
      example_check("MsgAddField(DIRECTIVE-KEYWORD)", this->status);

      /* Alter the directive request to a unique string */
      snprintf(this->tempString, MAX_STR_LEN - 1, "DIRECTIVE REQUEST MESSAGE %d", requestCount);
      gmsec_SetFieldType(this->tempField, GMSEC_TYPE_STR, this->status);
      gmsec_SetFieldName(this->tempField, "DIRECTIVE-STRING", this->status);
      gmsec_SetFieldValueSTR(this->tempField, this->tempString, this->status);
      gmsec_MsgAddField(this->directiveRequestMessage, this->tempField,
                        this->status);
      example_check("MsgAddField(DIRECTIVE-STRING)", this->status);

      /* Publish time */
      LOG_INFO("Publishing Directive Request Message (%d): %s", requestCount, gmsec_MsgGetTime());

      /* Send the request with the reply being send to the callback 
       * we created 
       */
      gmsec_RequestWCallback(this->connection, this->directiveRequestMessage,
                             GMSEC_WAIT_FOREVER,
                             requestor_c_RequestReplyCallback,
                             this->status);
      example_check("RequestWCallback(RequestReplyCallback)", this->status);

      /* Reset the countdown */
      directiveCountdown = directiveHolder;
    }

    /* When the countdown reaches 0, we publish a heartbeat message and
     *  reset the counter
     */
    if (heartbeatCountdown < 1)
    {
       heartbeatCount++;

       /* Update the message counter */
       gmsec_SetFieldType(this->tempField, GMSEC_TYPE_SHORT, this->status);
       gmsec_SetFieldName(this->tempField, "COUNTER", this->status);
       gmsec_SetFieldValueI16(this->tempField, heartbeatCount, this->status);
       gmsec_MsgAddField(this->heartbeatMessage, this->tempField, 
                         this->status);
       example_check("MsgAddField(COUNTER)", this->status);

       /* Publish the heartbeat message */
       gmsec_Publish(this->connection, this->heartbeatMessage, this->status);
       example_check("Publish(heartbeat)", this->status);

       // Ouput a heartbeat marker and reset the counter
       LOG_INFO("Published HeartBeat");
       heartbeatCountdown = heartbeatHolder;
    }

    /* Decrement the counters */
    heartbeatCountdown -= updateRate;
    directiveCountdown -= updateRate;

    /* Sleep for 1 second */
    example_sleep(updateRate);
  }

  /* End time */
  LOG_INFO("End Time: %s", gmsec_MsgGetTime());
}


int requestor_c_Run(requestor_c_t *this)
{
  GMSEC_STR configValue = NULL;
  GMSEC_I16 heartbeatHolder= 0;
  GMSEC_I16 loopCountdown = 0;
  GMSEC_I16 updateRate = 0;
  GMSEC_I16 directiveHolder = 0;

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
  gmsec_LookupConfigFileConfig(requestor_c_configFile, "program-config",
                                this->programConfig, this->status);
  if (!example_check("LookupConfigFileConfig(program-config)", this->status))
     return GMSEC_FALSE;

  /* Set program configuration values */
  gmsec_ConfigGetValue(this->programConfig, "update-rate", &configValue,
                       this->status);
  if (!example_check("LookupConfigFileConfig(program-config)", this->status))
     return GMSEC_FALSE;

  /* Get updateRate */
  if (!example_strToI16(configValue, &updateRate)) return GMSEC_FALSE;

  gmsec_ConfigGetValue(this->programConfig, "loop-time", &configValue,
                       this->status);
  if (!example_check("ConfigGetValue(loop-time)", this->status)) return GMSEC_FALSE;

  /* Get loopCountdown */
  if (!example_strToI16(configValue, &loopCountdown)) return GMSEC_FALSE;

  gmsec_ConfigGetValue(this->programConfig, "send-directive-rate",
                       &configValue, this->status);
  if (!example_check("ConfigGetValue(send-directive-rate)", this->status))
     return GMSEC_FALSE;
                       
  /* Get loopCountdown */
  if (!example_strToI16(configValue, &directiveHolder)) return GMSEC_FALSE;

  /* Create Heartbeat Message */
  if (!requestor_c_CreateHeartbeat(this, &heartbeatHolder))
     return GMSEC_FALSE;

  /* Create Directive Request Message */
  if (!requestor_c_CreateDirective(this))
     return GMSEC_FALSE;

  /* Main Publishing Loop */
  requestor_c_Publish(this, loopCountdown, updateRate, heartbeatHolder, 
                      directiveHolder);

  return GMSEC_TRUE;
}

void requestor_c_Cleanup(requestor_c_t *this)
{
  if (this->tempString)
     free(this->tempString);

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
     if (!example_check("Final DestroyMessage(heartbeat)", this->status))
        return;
  }

  /* Destroy the directive message */
  if (this->directiveRequestMessage!= NULL)
  {
     gmsec_DestroyMessage(this->connection, this->directiveRequestMessage,
                          this->status);
     if (!example_check("Final DestroyMessage(directive)", this->status)) 
        return;
  }

  /* Destroy the connection */
  if (this->connection != NULL)
  {
     example_closeConnection(this->connection, this->status);
     if (!example_check("Final Disconnection", this->status)) return;
  }

  /* Destroy the configuration file */
  if (requestor_c_configFile != NULL)
  {
     gmsec_DestroyConfigFile(&requestor_c_configFile, this->status);
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
  requestor_c_t requestor;

  /* Simple check for proper usage */
  if (argc < 2)
  {
    printf("Usage: %s <XML config filename>\n\n", argv[0]);
    exit(-1);
  }

  if (!requestor_c_Initialize(&requestor, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  requestor_c_Run(&requestor);
  requestor_c_Cleanup(&requestor);

  return 0;
}

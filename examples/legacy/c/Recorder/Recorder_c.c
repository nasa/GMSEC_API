
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file Recorder_c.c
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
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIG_OBJECT programConfig;
  GMSEC_CONFIGFILE_OBJECT configFile;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT heartbeatMessage;
  GMSEC_FIELD_OBJECT tempField;
} recorder_c_t;


/*
 * Callback Class for Subscriptions 
 */
void recorder_c_logCallback(GMSEC_CONNECTION_OBJECT connection,
                            GMSEC_MESSAGE_OBJECT log)
{
  GMSEC_STATUS_OBJECT result = NULL;
  GMSEC_STR xml = NULL;

  /* Create the status variable memory */
  gmsec_CreateStatus(&result);

  /* Display the message */
  gmsec_MsgToXML(log, &xml, result);
  example_check("MsgToXML(log)", result);
  if (xml)
  {
     LOG_INFO("%s", xml);
  } 

  /* Cleanup */
  gmsec_DestroyStatus(&result);

  /* Note:  The message does not have to be cleaned as it is 
   * owned by the framework 
   */
}

int recorder_c_Initialize(recorder_c_t *this, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->programConfig = NULL;
  this->configFile = NULL;
  this->heartbeatMessage = NULL;
  this->tempField = NULL;

  /* Create status and field */
  gmsec_CreateStatus(&this->status);
  gmsec_CreateField(&this->tempField, this->status);

  /* create the configuration file */
  gmsec_CreateConfigFile(&this->configFile, argv[1], this->status);
  if (!example_check("CreateConfigFile", this->status)) return GMSEC_FALSE;

  /* Load the configuration file and parse it */
  gmsec_LoadConfigFile(this->configFile, this->status);
  if (!example_check("LoadConfigFile", this->status)) return GMSEC_FALSE;

  /* create the configuration  */
  gmsec_CreateConfig(&this->config, this->status);
  if (!example_check("CreateConfig", this->status)) return GMSEC_FALSE;

  /* Get the configuration from the file */
  gmsec_LookupConfigFileConfig(this->configFile, "connection-config",
                               this->config, this->status);
  if (!example_check("LookupConfigFileConfig", this->status)) return GMSEC_FALSE;

  /* Initilaize the configuration */
  example_initialize(this->config);

  return GMSEC_TRUE;
}

int recorder_c_CreateHeartBeat(recorder_c_t *this, 
                                   GMSEC_I16 *heartbeatHolder)
{
  GMSEC_I16 tempHolder = 0;


  *heartbeatHolder = 0;

  /* Create a generic message container for the heartbeat message */
  gmsec_CreateMessageDflt(this->connection, &this->heartbeatMessage,
                          this->status);
  if (!example_check("CreateMessageDflt(heartbeatMessage)", this->status))
     return GMSEC_FALSE;

  /* Find and load the config file heartbeat message definition */
  gmsec_LookupConfigFileMessage(this->configFile, "C2CX-HEARTBEAT-REC", 
                                this->heartbeatMessage, this->status);
  if (!example_check("LookupConfigFileMessage(C2CX-HEARTBEAT-REC)", this->status))
     return GMSEC_FALSE;

  /* Obtain the publish rate field, by name, from the heartbeat message */
  gmsec_MsgGetField(this->heartbeatMessage, "PUB-RATE", this->tempField, 
                    this->status);
  if (example_check("MsgGetField(PUB-RATE)", this->status))
  {
     /* Obtain the value from the field */
     gmsec_GetFieldValueI16(this->tempField, &tempHolder, this->status);
     if (!example_check("GetFieldValueI16", this->status)) return GMSEC_FALSE;

     *heartbeatHolder = tempHolder;
  }
  else
  {
    /* Default value */
    *heartbeatHolder = 30;
  }

  return GMSEC_TRUE;
}


void recorder_c_Publish(recorder_c_t *this, 
                        GMSEC_I16 loopCountdown,
                        GMSEC_I16 updateRate,
                        GMSEC_I16 heartbeatHolder)
{
  GMSEC_I16 heartbeatCountdown = 0;

  /* Output some general program information */
  LOG_INFO("Publishing for %d seconds.", loopCountdown);
  LOG_INFO("Publishing Heartbeat Messages every %d seconds.", heartbeatHolder);

  /* Call to create the thread that will automatically retrieve messages off 
   * the message queue
   */
  gmsec_StartAutoDispatch(this->connection, this->status);

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
      globalHeartbeatCount++;

      /* Update the message counter */
      gmsec_SetFieldType(this->tempField, GMSEC_TYPE_I16, this->status);
      gmsec_SetFieldName(this->tempField, "COUNTER", this->status);
      gmsec_SetFieldValueI16(this->tempField, (GMSEC_I16) globalHeartbeatCount,
                             this->status);
      gmsec_MsgAddField(this->heartbeatMessage, this->tempField, this->status);
      example_check("MsgAddField(COUNTER)", this->status); 

      /* Publish the heartbeat message */
      gmsec_Publish(this->connection, this->heartbeatMessage, this->status);
      example_check("Publish(heartbeatMessage)", this->status); 

      /* Output a heartbeat marker and reset the counter */
      LOG_INFO("Published Heartbeat");
      heartbeatCountdown = heartbeatHolder;
    }

    // Decrement the counters
    heartbeatCountdown -= updateRate;

    /* Sleep for 1 second */
    example_sleep(updateRate);
  }

  /* End time */
  LOG_INFO("End Time: %s", gmsec_MsgGetTime());
}


int recorder_c_Run(recorder_c_t *this)
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

  // output GMSEC API and Middleware info, just for display purposes
  gmsec_GetLibraryVersion(this->connection, &configValue, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;
  if (configValue)
  {
     LOG_INFO("%s", configValue);
  }

  /* Create the configuration object/variable for additional configuration */
  gmsec_CreateConfig(&this->programConfig, this->status);
  if (!example_check("CreateConfig(programConfig)", this->status)) 
     return GMSEC_FALSE;

  /* Look up additional program configutation information */
  gmsec_LookupConfigFileConfig(this->configFile, "program-config",
                                this->programConfig, this->status);
  if (!example_check("LookupConfigFileConfig(program-config)", this->status))
     return GMSEC_FALSE;

  /* Set program config values */
  gmsec_ConfigGetValue(this->programConfig, "update-rate", &configValue,
                       this->status);
  if (!example_check("ConfigGetValue(update-rate)", this->status))
     return GMSEC_FALSE;

  /* Get updateRate */
  if (!example_strToI16(configValue, &updateRate)) return GMSEC_FALSE;

  gmsec_ConfigGetValue(this->programConfig, "loop-time", &configValue,
                       this->status);
  if (!example_check("ConfigGetValue(loop-time)", this->status)) return GMSEC_FALSE;

  /* Get loopCountdown */
  if (!example_strToI16(configValue, &loopCountdown)) return GMSEC_FALSE;

  /* Create subscriptions from subscription templates in the configuration
   * file using callbacks
   */
  gmsec_LookupConfigFileSubscription(this->configFile, "RECEIVE-LOG", 
                                     &tempSubject, this->status);
  if (!example_check("LookupConfigFileSubscription(RECEIVE-LOG)", this->status)) 
     return GMSEC_FALSE;

  gmsec_SubscribeWCallback(this->connection, tempSubject, recorder_c_logCallback,
                           this->status);
  if (!example_check("SubscribeWCallback(logCallback)", this->status))
     return GMSEC_FALSE;

  gmsec_LookupConfigFileSubscription(this->configFile, "SEND-LOG",
                                     &tempSubject, this->status);
  if (!example_check("LookupConfigFileSubscription(SEND-LOG)", this->status))
     return GMSEC_FALSE;

  gmsec_SubscribeWCallback(this->connection, tempSubject,
                           recorder_c_logCallback, this->status);
  if (!example_check("SubscribeWCallback(logCallback)", this->status))
     return GMSEC_FALSE;

  /* Create Heartbeat Message */
  if (!recorder_c_CreateHeartBeat(this, &heartbeatHolder))
     return GMSEC_FALSE;

  /* Main Publishing Loop */
  recorder_c_Publish(this, loopCountdown, updateRate, heartbeatHolder);

  return GMSEC_TRUE;
} 

void recorder_c_Cleanup(recorder_c_t *this)
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
  if (this->configFile != NULL)
  {
     gmsec_DestroyConfigFile(&this->configFile, this->status);
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
  recorder_c_t recorder;

  /* Simple check for proper usage */
  if (argc < 2)
  {
    printf("Usage: %s <XML config filename>\n\n", argv[0]);
    exit(-1);
  }

  if (!recorder_c_Initialize(&recorder, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  recorder_c_Run(&recorder);

  recorder_c_Cleanup(&recorder);

  return 0;
}

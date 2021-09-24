
/*
 * Copyright 2007-2021 United States Government as represented by the
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
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIGFILE_OBJECT config_file;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
} gmconfig_t;


int gmconfig_Initialize(gmconfig_t *this, char *argv[])
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
  if (!example_check("CreateConfigFile", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  /* Load the configuration file and parse it */
  gmsec_LoadConfigFile(this->config_file, this->status);
  if (!example_check("LoadConfigFile", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  /* create the configuration  */
  gmsec_CreateConfig(&this->config, this->status);
  if (!example_check("CreateConfig", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  /* Get the configuration from the file */
  gmsec_LookupConfigFileConfig(this->config_file, "config1",
                               this->config, this->status);
  if (!example_check("LookupConfigFileConfig", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  /* Initilaize the configuration */
  example_initialize(this->config);

  return GMSEC_TRUE;
}

int gmconfig_Run(gmconfig_t *this)
{
  int k = 0;
  GMSEC_STR tmp = NULL;
  GMSEC_STR xml = NULL;
  GMSEC_STR pattern = NULL;

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

  /* Lookup subject pattern by name */
  gmsec_LookupConfigFileSubscription(this->config_file, "events", &pattern,
                                     this->status);
  if (!example_check("LookupConfigFileSubscription", this->status)) 
     return GMSEC_FALSE;

  LOG_INFO("Subscribe to: %s", pattern);

  /* Subscribe on the connection */
  gmsec_Subscribe(this->connection, pattern, this->status);
  if (!example_check("Subscribe(connection)", this->status)) return GMSEC_FALSE;

  /* Create a Message */
  gmsec_CreateMessageDflt(this->connection, &this->message, this->status);
  if (!example_check("CreateMessageDflt", this->status)) return GMSEC_FALSE;

  /* Get Message from the configuration file */
  gmsec_LookupConfigFileMessage(this->config_file, "msg1", this->message,
                                this->status);
  if (!example_check("LookupConfigFileMessage", this->status)) return GMSEC_FALSE;


  for (k=0; k < 10; k++)
  {
    /* Dump Message */
    gmsec_MsgToXML(this->message, &xml, this->status);
    if (!example_check("MsgToXML(message)", this->status)) return GMSEC_FALSE;
    if (xml)
    {
       LOG_INFO("prepared\n%s", xml);
    }

    /* Publish Message */
    gmsec_Publish(this->connection, this->message, this->status);
    if (!example_check("Publish(connection)", this->status)) return GMSEC_FALSE;
    LOG_INFO("published");
  }

  return GMSEC_TRUE;
}

void gmconfig_Cleanup(gmconfig_t *this)
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

  gmconfig_t gmconfig;

  if (argc <= 1)
  {
     printf("usage: gmconfig_c <filename>\n");
     return -1;
  }

  if (!gmconfig_Initialize(&gmconfig, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  gmconfig_Run(&gmconfig);

  gmconfig_Cleanup(&gmconfig);

  return 0;
}

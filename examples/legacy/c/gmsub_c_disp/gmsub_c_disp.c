
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmsub_c_disp.c
 *
 * A C example demonstation of GMSEC subscriber using dispatcher.
 *
 */

#include "../example.h"


typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
} gmsub_c_disp_t;


void gmsub_c_disp_printUsage()
{
  printf("\nusage: gmsub_c_disp connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}



void CALL_TYPE gmsub_c_disp_PublishOnMessage(GMSEC_CONNECTION_OBJECT conn, 
                                             GMSEC_MESSAGE_OBJECT msg)
{
  GMSEC_STATUS_OBJECT status = NULL;
  GMSEC_STR           xml    = NULL;

  gmsec_CreateStatus(&status);

  /* Dump GMSEC_MESSAGE_HANDLE */
  gmsec_MsgToXML(msg, &xml, status);

  example_check("MsgToXML(OnMessage)", status);
  if (xml)
  {
     LOG_INFO("received from OnMessage =>\n%s", xml);
     fflush(stdout);
  }
  
  LOG_INFO("Press the enter key to exit");
}

int gmsub_c_disp_Initialize(gmsub_c_disp_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;

  /* create status */
  gmsec_CreateStatus(&(this->status));

  /* create config */
  gmsec_CreateConfigParams(&(this->config), argc, argv, this->status);

  if (!example_check("CreateConfigParam", this->status)) return GMSEC_FALSE;

  gmsec_ConfigAddValue(this->config, "isThreaded", "GMSEC_TRUE", this->status);
  if (!example_check("CreateAddValue(isThreaded)", this->status)) return GMSEC_FALSE;

  /* Initialize the configuration */
  example_initialize(this->config);

  add_to_config_from_file(&(this->config));

  return GMSEC_TRUE;
}

int gmsub_c_disp_Run(gmsub_c_disp_t *this)
{
  GMSEC_STR temp_buffer = NULL;
  GMSEC_STR subject = NULL;
  example_getStringDefault(this->config, "SUBJECT", "GMSEC.TEST.PUBLISH",
                           &subject);

  /* output GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Establish the Connection */
  this->connection = example_openConnection(this->config, this->status);
  if (this->connection == NULL) return GMSEC_FALSE;

  /* Output the middleware version */
  temp_buffer = NULL;
  gmsec_GetLibraryVersion(this->connection, &temp_buffer, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;

  if (temp_buffer)
  {
     LOG_INFO("%s", temp_buffer);
  }

  /* Subscribe */
  LOG_INFO("Subscribing to Publisher");
  gmsec_SubscribeWCallback(this->connection, subject,
                           gmsub_c_disp_PublishOnMessage, this->status);
                           
  if (!example_check("SubscribeWCallback", this->status)) return GMSEC_FALSE;


  LOG_INFO("Starting AutoDispatch");
  gmsec_StartAutoDispatch(this->connection, this->status);
  if (!example_check("StartAutoDispatch", this->status)) return GMSEC_FALSE;

  LOG_INFO("Press the enter key to exit");

  /* Wait for any input */
  example_await_input();

  LOG_INFO("Stopping AutoDispatch");
  gmsec_StopAutoDispatch(this->connection, this->status);
  example_check("StopAutoDispatch", this->status);

  return GMSEC_TRUE;
}

void gmsub_c_disp_Cleanup(gmsub_c_disp_t *this)
{
  /* Destroy the connection */
  if (this->connection != NULL)
  {
     example_closeConnection(this->connection, this->status);
     if (!example_check("Disconnection", this->status)) return;
  }

  /* Destroy the Config */
  gmsec_DestroyConfig(&(this->config), this->status);

  /* Destroy the Status */
  gmsec_DestroyStatus(&(this->status));
}

int main(int argc, char* argv[])
{
  gmsub_c_disp_t gmsub_disp;

  if (!gmsub_c_disp_Initialize(&gmsub_disp, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }


  if (example_option_invalid(gmsub_disp.config, argc))
  {
     gmsub_c_disp_printUsage();
     return -1;
  }

  gmsub_c_disp_Run(&gmsub_disp);

  gmsub_c_disp_Cleanup(&gmsub_disp);

  return 0;
}

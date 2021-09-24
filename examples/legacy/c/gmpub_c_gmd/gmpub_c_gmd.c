
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmpub_c_gmd.c
 *
 * A C example demonstration of SmartSockets GMD publishing.
 * 
 * NOTE:  For more information about SmartSockets GMD, 
 *        please refer to the SmartSockets User's Guide,
 */

#include "../example.h"

#define GMPUB_C_GMD_SUBJECT "GMSEC.TEST.PUBLISH" 

typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONFIG_OBJECT msg_config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_FIELD_OBJECT field;
} gmpub_c_gmd_t;


int gmpub_c_gmd_printUsage(GMSEC_CONFIG_OBJECT config, int nbr_of_args)
{
  const char *str_level = NULL;

  if (nbr_of_args <= 1 ||
      ((!example_getString(config, "connectiontype", &str_level) &&
       !example_getString(config, "mw-id", &str_level)) ||
       !example_getString(config, "server", &str_level))) {

     printf("\nusage: gmpub_c_gmd connectiontype=<middleware> "
            "server=<server> [ <parameter>=<value> ]\n"
            "\n\tNote that the parameter 'connectiontype' and "
            "'server' are required. "
            "\n\tThe rest of other parameters are optional.\n"
            "\n\tsubject=<subject name>"
            "\n\titerations=<iterations>"
            "\n\tloglevel=<log level>"
            "\n\tcfgfile=<config_filepath.xml>"
            "\n\nFor more information, see API SmartSockets User's Guide\n");
     return GMSEC_TRUE;
  }

  return GMSEC_FALSE;
}


void gmpub_c_gmd_Callback(GMSEC_CONNECTION_OBJECT conn,
                          GMSEC_MESSAGE_OBJECT msg,
                          GMSEC_STATUS_OBJECT status, GMSEC_STR event)
{
  GMSEC_STATUS_OBJECT result;
  GMSEC_STR tmp = NULL;

  LOG_INFO("-- GMD Message Failure Notification Recieved --");

  gmsec_CreateStatus(&result);

  gmsec_GetMsgSubject(msg, &tmp, result);
  LOG_INFO("GMD failure: %s %s", tmp, gmsec_GetStatus(status));

  /* Display received GMD Failure Message. */
  gmsec_MsgToXML(msg, &tmp, result);
  if (tmp)
  {
    LOG_INFO("%s", tmp);
  }

  fflush(stdout);

  /*--------------------------------------------------
   *                       NOTE:
   *--------------------------------------------------
   *
   *  Put Code HERE to process a GMD Timeout Failure
   *
   *--------------------------------------------------
   */
}

void gmpub_c_gmd_DispatchCallback(GMSEC_CONNECTION_OBJECT conn,
                                  GMSEC_MESSAGE_OBJECT msg,
                                  GMSEC_STATUS_OBJECT status, GMSEC_STR event)
{
  LOG_INFO("Dispatch failure: %s", gmsec_GetStatus(status));
}


int gmpub_c_gmd_Initialize(gmpub_c_gmd_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->config = NULL;
  this->msg_config = NULL;
  this->connection = NULL;
  this->message = NULL;
  this->field = NULL;

  /* Create status */
  gmsec_CreateStatus(&(this->status));


  /* Create configuration for connection */
  gmsec_CreateConfigParams(&this->config, argc, argv, this->status);
  if (!example_check("CreateConfigParam", this->status))
     return GMSEC_FALSE;

  /* Initialize the configuration */
  example_initialize(this->config);

  add_to_config_from_file(&(this->config));

  /* Create configuration message */
  gmsec_CreateConfig(&this->msg_config, this->status);
  if (!example_check("CreateConfig(msg_config)", this->status))
     return GMSEC_FALSE;

  /* Create field */
  gmsec_CreateField(&this->field, this->status);
  if (!example_check("CreateField", this->status))
     return GMSEC_FALSE;

  return GMSEC_TRUE;
}


int gmpub_c_gmd_Publish(gmpub_c_gmd_t *this, GMSEC_I32 iteration)
{
  GMSEC_I32 value = iteration + 1;

  gmsec_SetFieldName(this->field, "sequence number", this->status);
  gmsec_SetFieldValueI32(this->field, value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("MsgAddField(sequence number)", this->status))
     return GMSEC_FALSE;

  /* Publish Message */
  gmsec_Publish(this->connection, this->message, this->status);
  LOG_INFO("Published sequence->%ld", value);

  return GMSEC_TRUE;
}


int gmpub_c_gmd_Run(gmpub_c_gmd_t *this)
{
  GMSEC_STR tmp = NULL;
  GMSEC_STR subject = NULL;
  GMSEC_I32 j = 1;
  GMSEC_I32 iterations;

  /* Not needed in production, but it allows you to see what
   * options actually get set in SS (as a sanity check)
   *   NOTE: See GMSEC API Users Guide for other settings.
   *
   *   ConfigAddValue(cfg, "COMMAND_FEEDBACK","interactive", result);
   *   ConfigAddValue(cfg, "COMMAND_FEEDBACK","never", result);
   */
  gmsec_ConfigAddValue(this->config, "COMMAND_FEEDBACK", "always",
                       this->status);

  /* Add the gmdSubject to the connection configuration.
   *
   * This subject needs to be static yet unique amongst GMD clients so
   * that the RTServer can associate a connection with each client.
   * In other words, the GMD subject used here can not be used by any
   * other client in the system.
   */
  gmsec_ConfigAddValue(this->config, "gmdSubject", 
                       "GMSEC.GMD_Subject_publisher", this->status);

  /* Turn on ether Memory OR File based GMD
   * 
   *   ConfigAddValue(cfg, "IPC_GMD_TYPE","default", result);
   */
  gmsec_ConfigAddValue(this->config, "IPC_GMD_TYPE", "memory",
                       this->status);

  /* Set the Server mode, so it 'remembers' if you leave and come back.
   * 
   * NOTE: 
   *    See GMSEC API Users Guide for other settings.
   *
   *    ConfigAddValue(cfg, "SERVER_DISCONNECT_MODE","gmd_success", result);
   *    ConfigAddValue(cfg, "SERVER_DISCONNECT_MODE","gmd_failure", result);
   */
  gmsec_ConfigAddValue(this->config, "SERVER_DISCONNECT_MODE", "warm",
                       this->status);

  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Establish the connection */
  this->connection = example_openConnection(this->config, this->status);
  if (this->connection == NULL)
     return GMSEC_FALSE;

  /* Output the middleware version */
  gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
  if (!example_check("Getting Library Version", this->status))
     return GMSEC_FALSE;

  if (tmp)
  {
     LOG_INFO("%s", tmp);
  }

  example_getStringDefault(this->config, "SUBJECT", GMPUB_C_GMD_SUBJECT,
                           &subject);
  /* Get Iterations */
  iterations = (GMSEC_I32) example_getInteger(this->config, "ITERATIONS", 1);
  if (iterations > 0)
  {
     LOG_INFO("Waiting for up to %d", iterations);
  }

  LOG_INFO("Registering error dispatcher callback");
  gmsec_RegisterErrorCallback(this->connection, "CONNECTION_DISPATCHER_ERROR",
                              &gmpub_c_gmd_DispatchCallback, this->status);
  if (!example_check("RegisterErrorCallback(dispatcher)", this->status))
     return GMSEC_FALSE;

  LOG_INFO("Registering error callback");
  gmsec_RegisterErrorCallback(this->connection, "SS_GMD_ERROR",
                              gmpub_c_gmd_Callback, this->status);
  if (!example_check("RegisterErrorCallback(regular)", this->status))
     return GMSEC_FALSE;

  LOG_INFO("Starting Auto Dispatch");
  gmsec_StartAutoDispatch(this->connection, this->status);
  if (!example_check("StartAutoDispatch", this->status))
     return GMSEC_FALSE;

  /* Required to be set on the Message for GMD.  The T_IPC_DELIVERY_ALL 
   * setting says to assure delivery to ALL subscribers before sending back 
   * an ACK.
   * 
   * NOTE:  See GMSEC API Users Guide for other settings.
   *
   *   ConfigAddValue(msgcfg, "SETDELIVERYMODE","T_IPC_DELIVERY_BEST_EFFORT", 
   *                  result);
   *   ConfigAddValue(msgcfg, "SETDELIVERYMODE","T_IPC_DELIVERY_ORDERED", 
   *                  result);
   *   ConfigAddValue(msgcfg, "SETDELIVERYMODE","T_IPC_DELIVERY_SOME", result);
   */
  gmsec_ConfigAddValue(this->msg_config, "SETDELIVERYMODE",
                       "T_IPC_DELIVERY_ALL", this->status);

  /* Set Delivery Timeout to 15 seconds.  The Delivery Timeout determines how 
   * long the publisher is willing to wait for all subscribers to acknowledge
   * the messaage.  It also determines the size (in units of seconds) of the
   * resend queue.  In this scenario the resend queue would contain 15 seconds 
   * worth messages.
   */
  gmsec_ConfigAddValue(this->msg_config, "SETDELIVERYTIMEOUT", "15.0",
                       this->status);

  /* Create a Message */
  gmsec_CreateMessageWCFG(this->connection, subject,
                          GMSEC_MSG_PUBLISH, &this->message, 
                          this->msg_config, this->status);
  example_check("CreateMessageWCFG", this->status);

  if (!gmsec_isStatusError(this->status))
  {
    if (gmsec_GetStatusClass(this->status) == GMSEC_STATUS_FIELD_ERROR 
        && gmsec_GetStatusClass(this->status) == GMSEC_INVALID_CONNECTION)
       return GMSEC_FALSE;
  }

  for (j=0; j < iterations; ++j)
  {
     if (!gmpub_c_gmd_Publish(this, j))
        return GMSEC_FALSE;

     if (j < iterations - 1)
        example_sleep(1);
  }

  return GMSEC_TRUE;
}


void gmpub_c_gmd_Cleanup(gmpub_c_gmd_t *this)
{
  /* Stop auto dispatcher */
  gmsec_StopAutoDispatch(this->connection, this->status);
  if (!example_check("StopAutoDispatch", this->status)) return;

  /* Destroy Fields */
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final DestroyField", this->status);
  }

  /* Destory message */
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

  /* Destroy the msg_config objects/variables */
  if (this->msg_config != NULL)
  {
     gmsec_DestroyConfig(&this->msg_config, this->status);
     if (!example_check("Final DestroyConfig", this->status)) return;
  }

  /* Destroy the config objects/variables */
  if (this->config != NULL)
  {
     gmsec_DestroyConfig(&this->config, this->status);
     if (!example_check("Final DestroyConfig", this->status)) return;
  }

  /* Destroy Status */
  gmsec_DestroyStatus(&this->status);
}


int main(int argc, char* argv[])
{
  gmpub_c_gmd_t gmpub_c_gmd;

  if (!gmpub_c_gmd_Initialize(&gmpub_c_gmd, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmpub_c_gmd");
     return -1;
  }

  if (gmpub_c_gmd_printUsage(gmpub_c_gmd.config, argc))
     return -1;

  gmpub_c_gmd_Run(&gmpub_c_gmd);
  gmpub_c_gmd_Cleanup(&gmpub_c_gmd);

  return 0;
}

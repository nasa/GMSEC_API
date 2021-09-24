
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmsub_c_gmd.c
 *
 * A C example demonstation of SmartSockets GMD subscribing.
 *
 * NOTE: For more information about SmartSockets GMD please refer to the
 *       SmartSockets User's Guide,
 */


#include "../example.h"

#define GMSUB_C_GMD_SUBJECT "GMSEC.>"

typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_FIELD_OBJECT field;
} gmsub_c_gmd_t;

int gmsub_c_gmd_printUsage(GMSEC_CONFIG_OBJECT config, int nbr_of_args)
{
  const char *str_level = NULL;

  if (nbr_of_args <= 1 ||
      ((!example_getString(config, "connectiontype", &str_level) &&
       !example_getString(config, "mw-id", &str_level)) || 
       !example_getString(config, "server", &str_level))) {

     printf("\nusage: gmsub_c_gmd connectiontype=<middleware> "
            "server=<server> [ <parameter>=<value> ]\n"
            "\n\tNote that the parameter 'connectiontype' and "
            "'server' are required. "
            "\n\tThe rest of other parameters are optional.\n"
            "\n\tsubject=<subject name>"
            "\n\titerations=<iterations>"
            "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
            "\n\tloglevel=<log level>"
            "\n\tcfgfile=<config_filepath.xml>"
            "\n\nFor more information, see API SmartSockets User's Guide\n");
     return GMSEC_TRUE;
  }

  return GMSEC_FALSE;
}


int gmsub_c_gmd_Initialize(gmsub_c_gmd_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->config = NULL;
  this->connection = NULL;
  this->message = NULL;
  this->field = NULL;

  /* Create status */
  gmsec_CreateStatus(&(this->status));

  /* Create configuration */
  gmsec_CreateConfigParams(&(this->config), argc, argv, this->status);
  if (!example_check("CreateConfigParam", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  /* Initilaize the configuration */
  example_initialize(this->config);

  add_to_config_from_file(&(this->config));

  /* Create Fields */
  gmsec_CreateField(&this->field, this->status);
  if (!example_check("CreateField", this->status))
     return GMSEC_FALSE;

  return GMSEC_TRUE;
}


int gmsub_c_gmd_Run(gmsub_c_gmd_t *this)
{
  GMSEC_STR tmp = NULL;
  GMSEC_STR subject = NULL;
  GMSEC_I32 seq;
  GMSEC_STR temp_subject;
  GMSEC_MESSAGE_OBJECT temp_msg;
  int iterations;
  int msg_timeout_ms;
  int done;
  int count;

  /* Not needed in production, but it allows you to see what
   * options actually get set in SS (as a sanity check)
   */
  gmsec_ConfigAddValue(this->config, "COMMAND_FEEDBACK", "always", 
                       this->status);

  /* Add the gmdSubject to the connection configuration. */
  gmsec_ConfigAddValue(this->config, "gmdSubject",
                       "GMSEC.GMD_Subject_subscriber", this->status);

  /* Turn on Memory based GMD */
  gmsec_ConfigAddValue(this->config, "IPC_GMD_TYPE", "memory", this->status);

  /* Set the Server mode, so it 'remembers' if you leave and come back. */
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

  example_getStringDefault(this->config, "SUBJECT", GMSUB_C_GMD_SUBJECT,
                           &subject);
  /* Get Iterations */
  iterations = example_getInteger(this->config, "ITERATIONS", 0);
  if (iterations > 0)
  {
     LOG_INFO("Waiting for up to %d", iterations);
  }

  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS",
                                      EXAMPLE_MSG_TIMEOUT);

  /* Subscribe */
  LOG_INFO("Subscribing-> %s", subject);

  gmsec_Subscribe(this->connection, subject, this->status);
  if (!example_check("Subscribing...", this->status))
     return GMSEC_FALSE;

  /* Continue looping through the next messages printing
   * out the sequence number field each time 
   */
  done = GMSEC_FALSE;
  count = 0;

  while (!done)
  {
    gmsec_GetNextMsg(this->connection, &this->message, msg_timeout_ms,
                     this->status);
    example_check("GetNextMsg", this->status);
    if (gmsec_isStatusError(this->status))
    {
       if (gmsec_GetStatusCode(this->status) == GMSEC_TIMEOUT_OCCURRED)
          LOG_INFO("timeout occurred");
       else
          return GMSEC_FALSE;
    }

    if (gmsec_isMsgValid(this->message))
    {
       ++count;
       if (iterations > 0 && count >= iterations)
          done = GMSEC_TRUE;

       gmsec_MsgGetField(this->message, "sequence number", this->field,
                         this->status);

       if (!gmsec_isStatusError(this->status))
       {
          gmsec_GetFieldValueI32(this->field, &seq, this->status);
          if (!example_check("GetField(I32)", this->status))
             return GMSEC_FALSE;
          LOG_INFO("sequence number->%ld", seq);
       }

       fflush(stdout);

       temp_subject = NULL;
       gmsec_GetMsgSubject(this->message, &temp_subject,
                           this->status);
       if (strcmp(temp_subject, "GMSEC.TERMINATE") == 0)
          done = GMSEC_TRUE;

       /* You must Destroy the incoming message to Acknowledge
        * the GMD message was recieved.
        */
       temp_msg = this->message;
       this->message = NULL;

       gmsec_DestroyMessage(this->connection, temp_msg, this->status);

       if (!example_check("DestroyMessage", this->status))
          return GMSEC_FALSE;
    }
  }
  return GMSEC_TRUE;

}

void gmsub_c_gmd_Cleanup(gmsub_c_gmd_t *this)
{
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

  /* Destroy the Config */
  gmsec_DestroyConfig(&(this->config), this->status);

  /* Destroy Status */
  gmsec_DestroyStatus(&this->status);
}


int main(int argc, char* argv[])
{
  gmsub_c_gmd_t gmsub_c_gmd;

  if (!gmsub_c_gmd_Initialize(&gmsub_c_gmd, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmsub_c_gmd");
     return -1;
  }

  if (gmsub_c_gmd_printUsage(gmsub_c_gmd.config, argc))
     return -1;

  gmsub_c_gmd_Run(&gmsub_c_gmd);
  gmsub_c_gmd_Cleanup(&gmsub_c_gmd);

  return 0;
}

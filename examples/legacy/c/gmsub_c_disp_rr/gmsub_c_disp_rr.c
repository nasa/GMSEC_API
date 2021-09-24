
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 * @file gmsub_c_disp_rr.c
 *
 * A C example demonstration of GMSEC subscriber using the dispatcher.
 *
 * The callback is registered with the Connection.  A seperate thread, transmits a request
 * and waits for a reply.  The Dispatcher is then started, which launches in its own
 * thread.  We're looking for contention issues with the message queue, you should be
 * able to recieve messages while still waiting for a reply.
 *
 * Notes:
 *  - Uses the GNU Common C++ library for cross platform threading support
 */

#include "../example.h"


typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT request;
} gmsub_c_disp_rr_t;


void gmsub_c_disp_rr_printUsage()
{
  printf("\nusage: gmsub_c_disp_rr connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
		 "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
         "\n\tprog_timeout_s=<timeout period (seconds)>"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}


void CALL_TYPE gmsub_c_disp_rr_PublishOnMessage(GMSEC_CONNECTION_OBJECT conn,
                                                GMSEC_MESSAGE_OBJECT message)
{
  GMSEC_STATUS_OBJECT status = NULL;
  GMSEC_STR           xml    = NULL;

  gmsec_CreateStatus(&status);

  /* Dump Message */
  gmsec_MsgToXML(message, &xml, status);

  example_check("MsgToXML(PublishOnMessage)", status);
  if (xml)
  {
     LOG_INFO("received from PublishOnMessage =>\n%s", xml);
     fflush(stdout);
  }

  //LOG_INFO("Press the enter key to exit");
}


void CALL_TYPE gmsub_c_disp_rr_RequestOnMessage(GMSEC_CONNECTION_OBJECT conn,
                                                GMSEC_MESSAGE_OBJECT message)
{
  /* Dump Message */
  GMSEC_STR xml = NULL;
  gmsec_MsgToXML(message, &xml, NULL);
  example_check("MsgToXML(RequestOnMessage)", NULL);
  if (xml)
  {
     LOG_INFO("******* Reply Message **********\n%s", xml);
  }
}

void CALL_TYPE gmsub_c_disp_rr_RequestOnError(GMSEC_CONNECTION_OBJECT conn,
                              GMSEC_MESSAGE_OBJECT message, 
                              GMSEC_STATUS_OBJECT status, GMSEC_STR event)
{
  GMSEC_STR xml = NULL;
  gmsec_MsgToXML(message, &xml, NULL);
  example_check("MsgToXML(RequestOnError)", NULL);
  if (xml)
  {
     LOG_INFO("received from ErrorOnMessage =>\n%s", xml);
  }
}

int gmsub_c_disp_rr_Initialize(gmsub_c_disp_rr_t *this, 
                                   int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->request = NULL; 

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


int gmsub_c_disp_rr_Run(gmsub_c_disp_rr_t *this)
{
  int prog_timeout_s;
  int msg_timeout_ms;
  int deltaTime = 0;
  GMSEC_STR temp_buffer = NULL;
  GMSEC_STR subject = NULL;

  example_getStringDefault(this->config, "SUBJECT", 
                           "GMSEC.TEST.PUBLISH.NOREPLY", &subject);

  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS",
							EXAMPLE_MSG_TIMEOUT);
  
  prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S", 
							EXAMPLE_PROG_TIMEOUT);


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

  /* Register the callback for Publish and Subscribe */
  LOG_INFO("Subscribing to Publisher");

  gmsec_SubscribeWCallback(this->connection, subject, 
                           gmsub_c_disp_rr_PublishOnMessage, this->status);
  if (!example_check("SubscribeWCallback", this->status)) return GMSEC_FALSE; 

  /* Launch the auto dispatcher in a seperate thread */
  LOG_INFO("Starting AutoDispatch");
  gmsec_StartAutoDispatch(this->connection, this->status);
  if (!example_check("StartAutoDispatch", this->status)) return GMSEC_FALSE;

  /* Create a request and reply callback, and make a request */
  LOG_INFO("Creating a request message");
  gmsec_CreateMessage(this->connection, subject, GMSEC_MSG_REQUEST,
                      &this->request, this->status);
  if (!example_check("CreateMessage(request)", this->status)) return GMSEC_FALSE;

  LOG_INFO("Registering error callback");
  gmsec_RegisterErrorCallback(this->connection, "CONNECTION_REQUEST_TIMEOUT", 
                              gmsub_c_disp_rr_RequestOnError, this->status);
  if (!example_check("RegisterErrorCallback", this->status)) return GMSEC_FALSE;

  LOG_INFO("Sending the request");
  gmsec_RequestWCallback(this->connection, this->request, msg_timeout_ms, 
                         gmsub_c_disp_rr_RequestOnMessage, this->status);
  if (!example_check("RequestWCallback", this->status)) return GMSEC_FALSE;

  
  //Wait 
  
  if(prog_timeout_s == EXAMPLE_PROG_TIMEOUT)
  {
	  LOG_INFO("Press the enter key to exit");
	  example_await_input();
  }
  else
  {
	  while (deltaTime < prog_timeout_s)
	  {
		 example_millisleep(1000);
	     deltaTime += 1;
	  }
	  LOG_INFO("Program timeout reached!");
  }

  LOG_INFO("Stopping AutoDispatch");
  gmsec_StopAutoDispatch(this->connection, this->status);
  example_check("StopAutoDispatch", this->status);

  return GMSEC_TRUE;
}


void gmsub_c_disp_rr_Cleanup(gmsub_c_disp_rr_t *this)
{
  /* Destory the request message */
  if (this->request != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->request, this->status);
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

  /* Destroy the status */
  gmsec_DestroyStatus(&this->status);
}


int main(int argc, char* argv[])
{
  gmsub_c_disp_rr_t gmsub_disp_rr;

  if (!gmsub_c_disp_rr_Initialize(&gmsub_disp_rr, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  if (example_option_invalid(gmsub_disp_rr.config, argc))
  {
     gmsub_c_disp_rr_printUsage();
     return -1;
  }

  gmsub_c_disp_rr_Run(&gmsub_disp_rr);

  gmsub_c_disp_rr_Cleanup(&gmsub_disp_rr);

  return 0;
}

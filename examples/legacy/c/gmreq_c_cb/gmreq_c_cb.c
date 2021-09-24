
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmreq_c_cb.c
 *
 * A C example demonstration of GMSEC callback requestor functionality.
 *
 */

#include "../example.h"

#define GMREQ_CB_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"

int gReplyCallbackFlag = 0;


typedef struct 
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_FIELD_OBJECT field;
} gmreq_cb_t;


void gmreq_cb_printUsage()
{
  printf("\nusage: gmrpl_cb_c connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
         "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}


int gmreq_cb_Initialize(gmreq_cb_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->message = NULL;
  this->field = NULL;

  /* create status */
  gmsec_CreateStatus(&(this->status));

  /* create config */
  gmsec_CreateConfigParams(&(this->config), argc, argv, this->status);
  if (!example_check("CreateConfigParam", this->status))
  {
     fprintf(stderr, "%s\n", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }
  
  /* Initilaize the configuration */
  example_initialize(this->config);

  add_to_config_from_file(&(this->config));

  return GMSEC_TRUE;
}


void reply_callback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT reply)
{
  GMSEC_STR xml = NULL;
  GMSEC_STATUS_OBJECT result = NULL;

  /* create result */
  gmsec_CreateStatus(&result);

  /* Dump Reply */
  gmsec_MsgToXML(reply, &xml, result);
  example_check("MsgToXML(Dump Reply via callback)", result); 
  if (xml != NULL)
  {
     LOG_INFO("built request via callback =>\n%s", xml);
  }

  /* NOTE: reply message is 'owned' by the framework and should not be freed */

  /* Destroy Status */
  gmsec_DestroyStatus(&result);

  /* flag that callback has been called */
  gReplyCallbackFlag = 1;
}


int gmreq_cb_Run(gmreq_cb_t *this)
{
	GMSEC_STR subject;
  GMSEC_STR tmp = NULL;
  int msg_timeout_ms;
  int deltaTime = 0;
  GMSEC_STR xml;

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

  if (tmp != NULL)
  {
     LOG_INFO("%s", tmp);
  }

  subject = NULL;
  example_getStringDefault(this->config, "SUBJECT", GMREQ_CB_DEFAULT_SUBJECT,
                           &subject);

  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS",
                                       EXAMPLE_MSG_TIMEOUT);

  /* Create request */
  gmsec_CreateMessage(this->connection, subject, GMSEC_MSG_REQUEST,
                      &this->message, this->status);

  if (!example_check("CreateMessage", this->status))
     return GMSEC_FALSE;

  /* Create field */
  gmsec_CreateField(&this->field, this->status);
  if (!example_check("CreateField", this->status))
     return GMSEC_FALSE;

  /* Add Fields */
  gmsec_SetFieldName(this->field, "QUESTION", this->status);
  gmsec_SetFieldValueSTR(this->field,
                         "Does the request/reply functionality still work?",
                         this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(QUESTION)", this->status))
     return GMSEC_FALSE;

  /* Add the name of the component making the Request. */
  gmsec_SetFieldName(this->field, "COMPONENT", this->status);
  gmsec_SetFieldValueSTR(this->field, "GMREQ", this->status);
  gmsec_MsgAddField(this->message ,this->field , this->status);
  if (!example_check("AddField(COMPONENT)", this->status))
     return GMSEC_FALSE;

  /* Add the Message ID of this Request.  In theory the MSG-ID should be
   * unique, but here it is not for the sake of simplicity.  See API
   * Interface Specification for suggetions on creating a MSG-ID.
   */
  gmsec_SetFieldName(this->field, "MSG-ID", this->status);
  gmsec_SetFieldValueSTR(this->field, "GMREQ-MSG", this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(MSG-ID)", this->status))
     return GMSEC_FALSE;

  /* Dump Request */
  xml = NULL;
  gmsec_MsgToXML(this->message, &xml, this->status);
  if (!example_check("MsgToXML(message)", this->status))
     return GMSEC_FALSE; 
  LOG_INFO("built request =>\n%s", xml);

  /* Send Request */   
  gmsec_RequestWCallback(this->connection, this->message, msg_timeout_ms,
                         reply_callback, this->status);

  if (!example_check("RequestWCallback(connection)", this->status))
  {
     return GMSEC_FALSE;
  }

	
  LOG_INFO("Waiting for response");
  while(gReplyCallbackFlag == 0)
  {
	  example_millisleep(10);
	  if(msg_timeout_ms != EXAMPLE_MSG_TIMEOUT)
	  {
		  deltaTime += 10;
		  if(deltaTime > msg_timeout_ms)
			  break;
	  }//else wait forever
  }


  if (gReplyCallbackFlag)
  {
     LOG_INFO("Response Received");
  }
  else
  {
     LOG_INFO("No response received");
  }

  return GMSEC_TRUE;
}

void gmreq_cb_Cleanup(gmreq_cb_t *this)
{
  /* Destroy Field */
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final DestroyField", this->status);
  }

  /* Destroy the Request */
  if (this->message != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->message, this->status);
     example_check("Final DestroyMessage", this->status);
  }

  /* Destroy connection */
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
  gmreq_cb_t gmreq_cb;

  if (!gmreq_cb_Initialize(&gmreq_cb, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmreq");
     return -1;
  }

  if (example_option_invalid(gmreq_cb.config, argc))
  {
     gmreq_cb_printUsage();
     return -1;
  }

  gmreq_cb_Run(&gmreq_cb);

  gmreq_cb_Cleanup(&gmreq_cb);

  return 0;
}

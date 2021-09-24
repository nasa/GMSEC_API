
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmreq.c
 *
 *
 *  A C example demonstation of the GMSEC request functionality.
 */

#include "../example.h"

#define GMREQ_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL"
#define GMREQ_QUESTION "Does the request/reply functionality still work?"

typedef struct 
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT request;
  GMSEC_MESSAGE_OBJECT reply;
  GMSEC_FIELD_OBJECT field;
} gmreq_t;


void gmreq_printUsage()
{
  printf("\nusage: gmreq_c connectiontype=<middleware> "
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


int gmreq_Initialize(gmreq_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->request = NULL;
  this->reply = NULL;
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


int gmreq_Run(gmreq_t *this)
{
  GMSEC_STR tmp = NULL;
  GMSEC_STR xml = NULL;
  GMSEC_STR subject = NULL;
  int msg_timeout_ms;

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
 
  example_getStringDefault(this->config, "SUBJECT", GMREQ_DEFAULT_SUBJECT,
                           &subject);

  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS", 
                                       EXAMPLE_MSG_TIMEOUT);

  /* Create a Request */
  gmsec_CreateMessage(this->connection, subject,
                      GMSEC_MSG_REQUEST, &(this->request), this->status);
  if (!example_check("CreateMessage(request)", this->status))
     return GMSEC_FALSE;

  /* Add Fields */
  gmsec_CreateField(&this->field, this->status);
  if (!example_check("CreateField", this->status))
     return GMSEC_FALSE;
  
  /* Add Fields */
  gmsec_SetFieldName(this->field, "question", this->status);
  gmsec_SetFieldValueSTR(this->field, GMREQ_QUESTION, this->status);
  gmsec_MsgAddField(this->request, this->field, this->status);
  if (!example_check("MsgAddField(question)", this->status)) return GMSEC_FALSE;

  /* Add the name of the component making the Request. */
  gmsec_SetFieldName(this->field, "COMPONENT", this->status);
  gmsec_SetFieldValueSTR(this->field, "GMREQ", this->status);
  gmsec_MsgAddField(this->request, this->field, this->status);
  if (!example_check("MsgAddField(COMPONENT)", this->status)) return GMSEC_FALSE;

  /* Add the Message ID of this Request.  In theory the MSG-ID should be 
   * unique, but here it is not for the sake of simplicity.  
   * See API Interface Specification for suggetions on creating a MSG-ID.
   */
  gmsec_SetFieldName(this->field, "MSG-ID", this->status);
  gmsec_SetFieldValueSTR(this->field, "REQ-MSG", this->status);
  gmsec_MsgAddField(this->request, this->field, this->status);
  if (!example_check("MsgAddField(MSG-ID)", this->status)) return GMSEC_FALSE;

  /* Dump Request */
  gmsec_MsgToXML(this->request, &xml, this->status);
  if (!example_check("MsgToXML(request)", this->status)) return GMSEC_FALSE;
  LOG_INFO("built request =>\n%s", xml);

  /* Send Request */
  gmsec_Request(this->connection, this->request, msg_timeout_ms,
                &(this->reply), this->status);
  if (!example_check("Request(connection)", this->status))
     return GMSEC_FALSE;

  /* Dump Reply */
  gmsec_MsgToXML(this->reply, &xml, this->status);
  if (!example_check("MsgToXML(reply)", this->status)) return GMSEC_FALSE;
  LOG_INFO("received reply =>\n%s", xml);

  fflush(stdout);
 
  return GMSEC_FALSE;
}

void gmreq_Cleanup(gmreq_t *this)
{
  /* Destroy Fields */
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final DestroyField", this->status);
  }

  /* Destroy Reply */
  if (this->reply != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->reply, this->status);
     example_check("Final DestroyMessage(reply)", this->status);
  }

  /* Destroy Request */
  if (this->request != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->request, this->status);
     example_check("Final DestroyMessage(request)", this->status);
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
  gmsec_DestroyStatus(&(this->status));
}

int main(int argc, char* argv[])
{
  gmreq_t gmreq;

  if (!gmreq_Initialize(&gmreq, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmreq");
     return -1;
  }

  if (example_option_invalid(gmreq.config, argc))
  {
     gmreq_printUsage();
     return -1;
  }

  gmreq_Run(&gmreq);

  gmreq_Cleanup(&gmreq);


  return 0;
}


/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmrpl_c_cb.c
 *
 * A C example demonstration of GMSEC callback this->reply functionality. 
 */

#include "../example.h"

#ifdef _WIN32
#include <conio.h>
#else

#endif

#define GMRPL_CB_DEFAULT_SUBJECT "GMSEC.>"

typedef struct 
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
} gmrpl_cb_t;

typedef struct 
{
  GMSEC_STATUS_OBJECT result;
  GMSEC_MESSAGE_OBJECT reply;
  GMSEC_FIELD_OBJECT tempField;
  GMSEC_FIELD_OBJECT field;
  char *stringHolder; 
} gmrpl_cb_OnMessage_t;

const size_t MAX_STR_LEN = 255;


void gmrpl_cb_printUsage()
{
  printf("\nusage: gmrpl_c_cb connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
         "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
		 "\n\tprog_timeout_s=<timeout period (seconds)>"
		 "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}


int gmrpl_cb_Initialize(gmrpl_cb_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->message = NULL;

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

int gmrpl_cb_OnMessage_Run(GMSEC_CONNECTION_OBJECT connection,
                               GMSEC_MESSAGE_OBJECT message,
                               gmrpl_cb_OnMessage_t *this)
{ GMSEC_STR xml = NULL;
  GMSEC_STR compString;
  GMSEC_STR msgIdString;
  char statusString[] = "1";
  char reply_subject[256];

  /* Initialization for clear */
  this->reply = NULL;
  this->result = NULL;
  this->tempField = NULL;
  this->field = NULL;
  this->stringHolder = (char*)malloc(MAX_STR_LEN);
  strncpy(this->stringHolder, "", MAX_STR_LEN - 1);

  /* create status */
  gmsec_CreateStatus(&this->result);

  gmsec_MsgToXML(message, &xml, this->result);
  if (!example_check("MsgToXML(callback)", this->result)) return GMSEC_FALSE;
  LOG_INFO("received via callback=>\n%s", xml);

  /* Create Reply Subject */
  gmsec_CreateField(&this->tempField, this->result);
  if (!example_check("CreateField via callback", this->result)) return GMSEC_FALSE;

  gmsec_MsgGetField(message, "COMPONENT", this->tempField, this->result);
  gmsec_GetFieldValueSTR(this->tempField, &compString, this->result);
  if (!example_check("GetFieldValueSTR(COMPONENT) via callback", this->result))
     return GMSEC_FALSE;
  strncpy(this->stringHolder, compString, MAX_STR_LEN - 1);

  gmsec_MsgGetField(message, "MSG-ID", this->tempField, this->result);
  gmsec_GetFieldValueSTR(this->tempField, &msgIdString, this->result);
  if (!example_check("GetFieldValueSTR(MSG-ID) via callback", this->result))
     return GMSEC_FALSE;

  snprintf(reply_subject, sizeof(reply_subject) - 1, "GMSEC.MISSION.SAT_ID.RESP.%s.%s.%s", 
          this->stringHolder, msgIdString, statusString);

  /* Create Reply */
  gmsec_CreateMessage(connection, reply_subject, GMSEC_MSG_REPLY, 
                      &this->reply, this->result);
  if (!example_check("CreateMessage(this->reply) via callback", this->result))
     return GMSEC_FALSE;

  /* Add Fields */
  gmsec_CreateField(&this->field, this->result);
  gmsec_SetFieldName(this->field, "ANSWER", this->result);
  gmsec_SetFieldValueSTRING(this->field, "Sure looks like it!", this->result);
  gmsec_MsgAddField(this->reply, this->field, this->result);
  if (!example_check("AddField(ANSWER) via callback", this->result))
     return GMSEC_FALSE;

  /* Add name of component that's Replying */
  gmsec_SetFieldName(this->field, "COMPONENT", this->result);
  gmsec_SetFieldValueSTR(this->field, "GMRPL", this->result);
  gmsec_MsgAddField(this->reply, this->field, this->result);
  if (!example_check("AddField(COMPONENT) via callback", this->result))
     return GMSEC_FALSE;

  /* Dump Reply */
  gmsec_MsgToXML(this->reply, &xml, this->result);
  if (!example_check("MsgToXML(Dump Reply) via callback", this->result))
     return GMSEC_FALSE;
  LOG_INFO("prepared reply via callback =>\n%s", xml);

  fflush(stdout);

  /* Send Reply */
  gmsec_Reply(connection, message, this->reply, this->result);
  if (!example_check("Reply(connection) via callback", this->result))
     return GMSEC_FALSE;

  return GMSEC_TRUE;
}

void gmrpl_cb_OnMessage_Cleanup(GMSEC_CONNECTION_OBJECT connection,
                                gmrpl_cb_OnMessage_t *this)
{
  /* Destroy two Fields */
  if (this->tempField != NULL)
  {
     gmsec_DestroyField(&this->tempField, this->result);
     example_check("DestroyField(tempField) via callback", this->result);
  }
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->result);
     example_check("DestroyField(field) via callback", this->result); 
  }

  /* Destroy Reply */
  if (this->reply != NULL)
  {
     gmsec_DestroyMessage(connection, this->reply, this->result);
     example_check("DestroyMessage(reply) via callback", this->result); 
  }

  /* Destroy status */
  gmsec_DestroyStatus(&this->result);

  /* Deallocate memory */
  if (this->stringHolder)
     free(this->stringHolder);
}


void CALL_TYPE gmrpl_cb_PublishOnMessage(GMSEC_CONNECTION_OBJECT connection,
                                         GMSEC_MESSAGE_OBJECT message)
{
  gmrpl_cb_OnMessage_t gmrpl_callback;

  if (!gmrpl_cb_OnMessage_Run(connection, message, &gmrpl_callback))
  {
     LOG_WARNING("gmrpl_cb_PublishOnMessage failed");
  }

  gmrpl_cb_OnMessage_Cleanup(connection, &gmrpl_callback);
}
 

int gmrpl_cb_Run(gmrpl_cb_t *this)
{
  GMSEC_STR tmp;
  GMSEC_STR subject;
  GMSEC_STR temp_subject;
  GMSEC_MESSAGE_OBJECT temp_msg;
  int msg_timeout_ms;
  int prog_timeout_s;
  int done;

  time_t prevTime;
  time_t nextTime;
  double elapsedTime = 0; 

  msg_timeout_ms = example_getInteger(this->config, "MSG_TIMEOUT_MS",
                                      EXAMPLE_MSG_TIMEOUT);   
  prog_timeout_s = example_getInteger(this->config, "PROG_TIMEOUT_S",
									EXAMPLE_PROG_TIMEOUT);

  if(msg_timeout_ms == EXAMPLE_MSG_TIMEOUT && prog_timeout_s != EXAMPLE_PROG_TIMEOUT)
  {
	  LOG_INFO("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
	 //gmrpl_cb_printUsage();
	 return GMSEC_FALSE;
  }

  /* Add the configuration for threading */
  gmsec_ConfigAddValue(this->config, "isThreaded", "GMSEC_TRUE", this->status);
  if (!example_check("ConfigAddValue(isThreaded)", this->status))
     return GMSEC_FALSE;

  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Establish the connection */
  this->connection = example_openConnection(this->config, this->status);

  if (this->connection == NULL)
     return GMSEC_FALSE;

  /* Output the middleware version */
  tmp = NULL;
  gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
  if (!example_check("Getting Library Version", this->status)) 
     return GMSEC_FALSE;

  if (tmp != NULL)
  {
     LOG_INFO("%s", tmp);
  }

  subject = NULL;
  example_getStringDefault(this->config, "SUBJECT", GMRPL_CB_DEFAULT_SUBJECT,
                           &subject);

  /* Subscribe */
  LOG_INFO("Subscribing to %s (using callback)", subject);
  gmsec_SubscribeWCallback(this->connection, subject,
                           gmrpl_cb_PublishOnMessage, this->status);

  if (!example_check("Subscribing using callback...", this->status))
     return GMSEC_FALSE;

  LOG_INFO("Subscribing to GMSEC.TERMINATE (using callback)");
  gmsec_SubscribeWCallback(this->connection, "GMSEC.TERMINATE",
                           gmrpl_cb_PublishOnMessage, this->status);
                      
  if (!example_check("Subscribing using callback...", this->status))
     return GMSEC_FALSE;


  done = GMSEC_FALSE;
  time(&prevTime);
 
  /* Listen */
  while (!done)
  {

	if(prog_timeout_s != EXAMPLE_PROG_TIMEOUT && elapsedTime >= prog_timeout_s)
	{
		LOG_INFO("Program Timeout Reached!");
		return GMSEC_FALSE;
	}

    gmsec_GetNextMsg(this->connection, &(this->message), msg_timeout_ms,
                     this->status);

	if(prog_timeout_s != EXAMPLE_PROG_TIMEOUT)
	{
		time(&nextTime);
		elapsedTime += difftime(nextTime, prevTime);
		prevTime = nextTime;
	}

    if (!example_check("GetNextMsg", this->status))
    {
       if (gmsec_GetStatusCode(this->status) == GMSEC_TIMEOUT_OCCURRED)
          LOG_INFO("timeout occurred");
       else
          return GMSEC_FALSE;
    }
    else
    {
       /* Dispatch callbacks */
       gmsec_DispatchMsg(this->connection, this->message, this->status);
       if (!example_check("DispatchMsg", this->status)) return GMSEC_FALSE;

       temp_subject = NULL;
       gmsec_GetMsgSubject(this->message, &temp_subject, this->status);
       if (!example_check("GetMsgSubject", this->status)) return GMSEC_FALSE;

       if (strcmp(temp_subject, "GMSEC.TERMINATE") == 0)
          done = GMSEC_TRUE;
       
       /* Destroy inbound message */
       temp_msg = this->message;
       this->message = NULL;
       gmsec_DestroyMessage(this->connection, temp_msg, this->status);
       if (!example_check("DestoryMessage", this->status))
          return GMSEC_FALSE; 
    }
  }

  return GMSEC_TRUE;
}

void gmrpl_cb_Cleanup(gmrpl_cb_t *this)
{
  /* Destroy Message */
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
  gmsec_DestroyStatus(&(this->status));
}


int main(int argc, char* argv[])
{
 
  gmrpl_cb_t gmrpl_cb;

  if (!gmrpl_cb_Initialize(&gmrpl_cb, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmrpl_cb");
     return -1;
  }

  if (example_option_invalid(gmrpl_cb.config, argc))
  {
     gmrpl_cb_printUsage();
     return -1;
  }
  
  gmrpl_cb_Run(&gmrpl_cb);

  gmrpl_cb_Cleanup(&gmrpl_cb);

  return 0;
}

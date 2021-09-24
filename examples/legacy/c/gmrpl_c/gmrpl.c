
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmrpl.c
 *
 *   A C example demonstration of GMSEC reply functionality.
 */

#include "../example.h"

/* @TODO: need to determine what the default subject should be.
 * #define GMRPL_DEFAULT_SUBJECT "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL" 
 */
#define GMRPL_DEFAULT_SUBJECT "GMSEC.>"

typedef struct 
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_MESSAGE_OBJECT reply;
  GMSEC_FIELD_OBJECT tempField;
  GMSEC_FIELD_OBJECT field;
  char *stringHolder;
} gmrpl_t;

const size_t MAX_STR_LEN = 255;

void gmrpl_printUsage()
{
  printf("\nusage: gmrpl_c connectiontype=<middleware> "
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


int gmrpl_Initialize(gmrpl_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->message = NULL;
  this->reply = NULL;
  this->tempField = NULL;
  this->field = NULL;

  this->stringHolder = (char*) malloc(MAX_STR_LEN);
  strncpy(this->stringHolder, "", MAX_STR_LEN - 1);

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

int gmrpl_Run(gmrpl_t *this)
{
  GMSEC_STR tmp = NULL;
  GMSEC_STR xml = NULL;
  GMSEC_STR compString = NULL;
  GMSEC_STR msgIdString = NULL;
  GMSEC_STR subject;
  GMSEC_MSG_KIND kind;
  GMSEC_FIELD_OBJECT temp_fd;
  GMSEC_MESSAGE_OBJECT temp_msg;
  char statusString[] = "1";
  char reply_subject[256];
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
     //gmrpl_printUsage();
	 return GMSEC_FALSE;
  }


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

  /* Subscribe */
  example_getStringDefault(this->config, "SUBJECT", GMRPL_DEFAULT_SUBJECT,
                           &subject);
  LOG_INFO("Subscribing to %s", subject);
  gmsec_Subscribe(this->connection, subject, this->status);
  if (!example_check("Subscribing...", this->status))
     return GMSEC_FALSE;

  subject = "GMSEC.TERMINATE";
  LOG_INFO("Subscribing to %s", subject);
  gmsec_Subscribe(this->connection, subject, this->status);
  if (!example_check("Subscribing...", this->status))
     return GMSEC_FALSE;

  done = GMSEC_FALSE;
  time(&prevTime);

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
       GMSEC_STR temp_subject = NULL;
       gmsec_GetMsgSubject(this->message, &temp_subject, this->status);
       if (!example_check("GetMsgSubject", this->status)) return GMSEC_FALSE;

       if (strcmp(temp_subject, "GMSEC.TERMINATE") == 0)
          done = GMSEC_TRUE;

       /* Dump the message */
       gmsec_MsgToXML(this->message, &xml, this->status);
       if (!example_check("MsgToXML", this->status))
          return GMSEC_FALSE;

       LOG_INFO("received =>\n%s", xml);
       fflush(stdout);

       /* check for request */
       gmsec_GetMsgKind(this->message, &kind, this->status);
       example_check("GetMsgKind", this->status);
       if (kind == GMSEC_MSG_REQUEST)
       {
          /* Create Reply Subject */
          gmsec_CreateField(&this->tempField, this->status);
          if (!example_check("CreateField(tempField)", this->status)) 
             return GMSEC_FALSE;

          gmsec_MsgGetField(this->message, "COMPONENT", this->tempField,
                            this->status);
          gmsec_GetFieldValueSTR(this->tempField, &compString, this->status);
          if (!example_check("GetFieldValueSTR(COMPONENT)", this->status))
             return GMSEC_FALSE;
          strncpy(this->stringHolder, compString, MAX_STR_LEN - 1);

          gmsec_MsgGetField(this->message, "MSG-ID", this->tempField, 
                            this->status);
          gmsec_GetFieldValueSTR(this->tempField, &msgIdString, this->status);
          if (!example_check("GetFieldValueSTR(MSD-ID)", this->status))
             return GMSEC_FALSE;

          snprintf(reply_subject, sizeof(reply_subject) - 1, "GMSEC.MISSION.SAT_ID.RESP.%s.%s.%s", 
                  this->stringHolder, msgIdString, statusString);

          /* Create Reply */
          gmsec_CreateMessage(this->connection, reply_subject, GMSEC_MSG_REPLY,
                              &this->reply, this->status);
          if (!example_check("CreateMessage(reply)", this->status))
             return GMSEC_FALSE;

          /* Add Fields */
          gmsec_CreateField(&this->field, this->status);
          if (!example_check("CreateField", this->status))
             return GMSEC_FALSE;

          gmsec_SetFieldName(this->field, "ANSWER", this->status);
          gmsec_SetFieldValueSTR(this->field, "Sure looks like it!", 
                                 this->status);
          gmsec_MsgAddField(this->reply, this->field, this->status);
          if (!example_check("AddField(ANSWER)", this->status))
             return GMSEC_FALSE;

          /* Add name of component that's Replying */
          gmsec_SetFieldName(this->field, "COMPONENT", this->status);
          gmsec_SetFieldValueSTR(this->field, "GMRPL", this->status);
          gmsec_MsgAddField(this->reply, this->field, this->status);
          if (!example_check("AddField(COMPONENT)", this->status))
             return GMSEC_FALSE;

          /* Dump Reply */
          gmsec_MsgToXML(this->reply, &xml, this->status);
          if (!example_check("MsgToXML->reply", this->status)) return GMSEC_FALSE;
          LOG_INFO("prepared reply =>\n%s", xml);

          /* Send Reply */
          gmsec_Reply(this->connection, this->message, this->reply, 
                      this->status);
          if (!example_check("Reply(connection...)", this->status))
             return GMSEC_FALSE;
 
          LOG_INFO("reply sent!");

          /* Destroy Field */
          temp_fd = this->field;
          this->field = NULL;
          gmsec_DestroyField(&temp_fd, this->status);
          if (!example_check("DestroyField(field)", this->status))
             return GMSEC_FALSE;
       }

       /* Destroy Message */
       temp_msg = this->message;
       this->message = NULL;
       gmsec_DestroyMessage(this->connection, temp_msg, this->status);
       if (!example_check("DestroyMessage", this->status))
          return GMSEC_FALSE;
    }
  }
  return GMSEC_TRUE;
}


void gmrpl_Cleanup(gmrpl_t *this)
{
  /* Destroy two Fields */
  if (this->field != NULL) 
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final DestroyField(field)", this->status);
  } 
  if (this->tempField != NULL) 
  {
     gmsec_DestroyField(&this->tempField, this->status);
     example_check("Final DestroyField(tempField)", this->status);
  } 

  /* Destroy Reply */
  if (this->reply != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->reply, this->status);
     example_check("Final DestroyMessage(reply)", this->status);
  }

  /* Destroy Message */
  if (this->message != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->message, this->status);
     example_check("Final DestroyMessage(message)", this->status);
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

  /* Deallocate memory */
  free(this->stringHolder);
}


int main(int argc, char* argv[])
{
  gmrpl_t gmrpl;

  if (!gmrpl_Initialize(&gmrpl, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the gmrpl");
     return -1;
  }

  if (example_option_invalid(gmrpl.config, argc))
  {
     gmrpl_printUsage();
     return -1;
  }

  gmrpl_Run(&gmrpl);

  gmrpl_Cleanup(&gmrpl);

  return 0;
}

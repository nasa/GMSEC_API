
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file logging.c
 *
 * A C example demonstration of GMSEC Logging Framework.
 *
 */

#include "../example.h"


typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_FIELD_OBJECT field;
} logging_t;


void logging_c_printUsage()
{
  printf("\nusage: logging_c connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
         "\n\titerations=<iterations>"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}


void logging_ErrorLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On ERROR Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

void logging_WarningLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On WARNING Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

void logging_InfoLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On INFO Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

void logging_VerboseLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On VERBOSE Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

void logging_DebugLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On DEBUG Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

void logging_AnyLogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  printf("On ANY Message:\n");
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n", tempBuffer, 
         gmsec_LogLevelToString(entry->level), entry->message);
}

int logging_Initialize(logging_t *this, int argc, char *argv[])
{
  this->status = NULL;
  this->connection = NULL;
  this->config = NULL;
  this->message = NULL;
  this->field = NULL;

  /* Create and register log handlers */
  gmsec_RegisterLogHandlerByLevel(logERROR, logging_ErrorLogHandler);
  gmsec_RegisterLogHandlerByLevel(logWARNING, logging_WarningLogHandler);
  gmsec_RegisterLogHandlerByLevel(logINFO, logging_InfoLogHandler);
  gmsec_RegisterLogHandlerByLevel(logVERBOSE, logging_VerboseLogHandler);
  gmsec_RegisterLogHandlerByLevel(logDEBUG, logging_DebugLogHandler);

  gmsec_SetReportingLevel(logVERBOSE);

  /* create status */
  gmsec_CreateStatus(&(this->status));

  /* create configuration */
  gmsec_CreateConfigParams(&(this->config), argc, argv, this->status);
  add_to_config_from_file(&(this->config));

  if (!example_check("CreateConfigParam", this->status))
  {
     LOG_ERROR("%s", gmsec_GetStatus(this->status));
     return GMSEC_FALSE;
  }

  return GMSEC_TRUE;
}


int logging_Run(logging_t *this)
{
  int i = 0;
  int value = 123;
  int iterations;
  GMSEC_STR tmp = NULL;
  GMSEC_STR xml = NULL;

  GMSEC_STR subject = NULL;
  example_getStringDefault(this->config, "SUBJECT",
                           "GMSEC.TEST.PUBLISH", &subject);

  iterations = example_getInteger(this->config, "ITERATIONS", 10);

  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  /* Establish the connection */
  this->connection = example_openConnection(this->config, this->status);

  if (this->connection == NULL)
     return GMSEC_FALSE;

  /* Output the middleware version */
  gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;

  if (tmp != NULL)
  {
    LOG_INFO("%s", tmp);
  }

  /* Create a Message */
  gmsec_CreateMessage(this->connection, subject, GMSEC_MSG_PUBLISH,
                      &this->message, this->status);
  if (!example_check("CreateMessage", this->status)) return GMSEC_FALSE;

  /* Add Fields */
  gmsec_CreateField(&this->field, this->status);
  if (!example_check("CreateField", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "C", this->status);
  gmsec_SetFieldValueCHAR(this->field, (GMSEC_CHAR)'c', this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_CHAR>)", this->status)) return GMSEC_FALSE;


  gmsec_SetFieldName(this->field, "T", this->status);
  gmsec_SetFieldValueBOOL(this->field, (GMSEC_BOOL)GMSEC_TRUE, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_TRUE>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "F", this->status);
  gmsec_SetFieldValueBOOL(this->field, GMSEC_FALSE, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_FALSE>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "J", this->status);
  gmsec_SetFieldValueI16(this->field, (GMSEC_I16) value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_I16>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "K", this->status);
  gmsec_SetFieldValueU16(this->field, (GMSEC_U16) value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_U16>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "I", this->status);
  gmsec_SetFieldValueI32(this->field, (GMSEC_I32) value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_I32>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "COUNT", this->status);
  gmsec_SetFieldValueI32(this->field, (GMSEC_I32) iterations, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(COUNT)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "U", this->status);
  gmsec_SetFieldValueU32(this->field, (GMSEC_U32) value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_U32>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "S", this->status);
  gmsec_SetFieldValueSTR(this->field, "This is a test",this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_STR>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "E", this->status);
  gmsec_SetFieldValueFLOAT(this->field, (GMSEC_F32) (1 + 1./ value), 
                           this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_F32>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "D", this->status);
  gmsec_SetFieldValueF64(this->field, (GMSEC_F64) (1 + 1. / value),
                         this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_F64>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "X", this->status);
  gmsec_SetFieldValueBIN(this->field, (GMSEC_BIN)"JLMNOPQ", 7, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_BIN>)", this->status)) return GMSEC_FALSE;

  for (i=0; i < iterations; i++)
  {
    printf("Setting log level to %u\n", gmsec_LogLevelFromString("VERBOSE"));

    gmsec_SetReportingLevel(logVERBOSE);

    LOG_INFO("The reporting level is set to %u", gmsec_GetReportingLevel());

    if (gmsec_IsLoggingEnabled(logINFO))
    {
        LOG_INFO("Logging is enabled for the logINFO level, as expected.");
    }
    else
    {
        LOG_ERROR("Logging is *not* enabled for the logINFO level; "
                  "unexpected!");
    }

    /* Dump Message */
    gmsec_MsgToXML(this->message, &xml, this->status);
    if (!example_check("MsgToXML(Dump Message)", this->status)) return GMSEC_FALSE;
    LOG_INFO("prepared\n%s", xml);

    /* Publish Message */
    gmsec_Publish(this->connection, this->message, this->status);
    if (!example_check("Publish", this->status)) return GMSEC_FALSE;

    LOG_INFO("published #%d", (i+1));

    LOG_ERROR("This is an example error message for iteration %d\n.", i);
    LOG_WARNING("This is an example warning message for iteration %d\n.", i);
    LOG_VERBOSE("This is an example \"verbose\" message "
                "for iteration %d\n.", i);
    LOG_DEBUG("This is an example debug message for iteration %d, "
              "which should not show.\n", i);
    // This last message cannot be shown right now because
    // gmsec_SetReportingLevel(logVERBOSE), used above, does not
    // allow DEBUG messages to come out.
    LOG_VERBOSE("This is another example \"verbose\" message "
                "for iteration %d.\n", i);


    /* Set logging reporting level to now allow DEBUG messages to be shown */
    gmsec_SetReportingLevel(logDEBUG);
    if (gmsec_GetReportingLevel() == logDEBUG)
    {
        LOG_INFO("Changed reporting level to logDEBUG");
    }
    else
    {
        LOG_ERROR("Failed to change reporting level to logDEBUG");
    }
    LOG_DEBUG("This is an example debug message for iteration %d, "
              "which should show.\n", i);

  }

  LOG_DEBUG("NONE reporting level, numerically, is %u",
            gmsec_LogLevelFromString("NONE"));
  LOG_DEBUG("ERROR reporting level, numerically, is %u",
            gmsec_LogLevelFromString("ERROR"));
  LOG_DEBUG("SECURE reporting level, numerically, is %u",
            gmsec_LogLevelFromString("SECURE"));
  LOG_DEBUG("WARNING reporting level, numerically, is %u",
            gmsec_LogLevelFromString("WARNING"));
  LOG_DEBUG("INFO reporting level, numerically, is %u",
            gmsec_LogLevelFromString("INFO"));
  LOG_DEBUG("VERBOSE reporting level, numerically, is %u",
            gmsec_LogLevelFromString("VERBOSE"));
  LOG_DEBUG("DEBUG reporting level, numerically, is %u",
            gmsec_LogLevelFromString("DEBUG"));

  gmsec_CAPILog(logINFO, "This is a test (%d, %d, %d)\n",
                1, 2, 3);

  /* Register general-purpose handler and test */

  gmsec_RegisterLogHandler(logging_AnyLogHandler);

  LOG_ERROR("NONE reporting level, numerically, is %u",
            gmsec_LogLevelFromString("NONE"));
  LOG_ERROR("ERROR reporting level, numerically, is %u",
            gmsec_LogLevelFromString("ERROR"));
  LOG_WARNING("WARNING reporting level, numerically, is %u",
              gmsec_LogLevelFromString("WARNING"));
  LOG_INFO("INFO reporting level, numerically, is %u",
           gmsec_LogLevelFromString("INFO"));
  LOG_VERBOSE("VERBOSE reporting level, numerically, is %u",
              gmsec_LogLevelFromString("VERBOSE"));
  LOG_DEBUG("DEBUG reporting level, numerically, is %u",
            gmsec_LogLevelFromString("DEBUG"));
 
  return GMSEC_TRUE;

}

void logging_Cleanup(logging_t *this)
{
  gmsec_RegisterLogHandler((GMSEC_LOGGER_HANDLER* ) NULL);

  /* Destroy Field */
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final DestroyField", this->status);
  } 

  /* Destroy the Message */
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

  /* Destroy the Status */
  gmsec_DestroyStatus(&this->status);

}


int main(int argc, char* argv[])
{
  logging_t example_log;

  if (!logging_Initialize(&example_log, argc, argv)) 
     return -1;

  if (example_option_invalid(example_log.config, argc))
  {
     logging_c_printUsage();
     return -1;
  }

  logging_Run(&example_log);

  logging_Cleanup(&example_log);

  return 0;
}



/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** 
 * @file gmpub.c
 *
 * A C example demonstration of GMSEC publisher functionality.
 */

#include "../example.h"
#include <gmsec/c/config_c.h>


typedef struct
{
  GMSEC_STATUS_OBJECT status;
  GMSEC_CONFIG_OBJECT config;
  GMSEC_CONNECTION_OBJECT connection;
  GMSEC_MESSAGE_OBJECT message;
  GMSEC_FIELD_OBJECT field;
} gmpub_t;

void gmpub_printUsage()
{
  printf("\nusage: gmpub_c connectiontype=<middleware> "
         "[ <parameter>=<value> ]\n"
         "\n\tNote that the parameter 'connectiontype' is required. "
         "\n\tThe rest of other parameters are optional.\n"
         "\n\tserver=<server name> "
         "(required if middleware is not bolt/MB locally)"
         "\n\tsubject=<subject name>"
         "\n\titerations=<iterations>"
         "\n\tinterval_ms=<interval (milliseconds)>"
         "\n\tloglevel=<log level>"
         "\n\tcfgfile=<config_filepath.xml>"
         "\n\nFor more information, see API User's Guide\n");
}

int gmpub_Initialize(gmpub_t *this, int argc, char *argv[])
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

  if (!example_check("CreateConfigParam", this->status)) return GMSEC_FALSE;

  /* Initialize the configuration */
  example_initialize(this->config);

  add_to_config_from_file(&(this->config));

  return GMSEC_TRUE;
}

int gmpub_Publish(gmpub_t *this, int iteration, const char *subject)
{
  GMSEC_STR xml;
  int       value = iteration + 1;
  char*     value_ptr = NULL;

  /* Create a Message */
  gmsec_CreateMessage(this->connection, subject, GMSEC_MSG_PUBLISH,
                      &(this->message), this->status);
  if (!example_check("CreateMessage", this->status)) return GMSEC_FALSE;

  /* Add Fields */
  this->field = NULL;
  gmsec_CreateField(&(this->field), this->status);
  if (!example_check("CreateField", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "C", this->status);
  gmsec_SetFieldValueCHAR(this->field, (GMSEC_CHAR) 'c', this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_CHAR>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field,"T", this->status);
  gmsec_SetFieldValueBOOL(this->field, GMSEC_TRUE, this->status);
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
  gmsec_SetFieldValueI32(this->field, (GMSEC_I32) iteration, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(COUNT)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "U", this->status);
  gmsec_SetFieldValueU32(this->field, (GMSEC_U32) value, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_U32>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "S", this->status);
  gmsec_SetFieldValueSTR(this->field, "This is a test", this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_STR>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "E", this->status);
  gmsec_SetFieldValueF32(this->field, (GMSEC_F32) (1 + 1. / value), 
                         this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_F32>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "D", this->status);
  gmsec_SetFieldValueF64(this->field, (GMSEC_F64) (1 + 1. / value), 
                         this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_F64>)", this->status)) return GMSEC_FALSE;

  gmsec_SetFieldName(this->field, "X", this->status);
  gmsec_SetFieldValueBIN(this->field, (GMSEC_BIN) "JLMNOPQ", 7, this->status);
  gmsec_MsgAddField(this->message, this->field, this->status);
  if (!example_check("AddField(<GMSEC_BIN>)", this->status)) return GMSEC_FALSE;

  ConfigGetValue(this->config, "ENCRYPT", (const char** ) &value_ptr,
                 this->status);

  if (value_ptr != NULL)
  {
#ifndef WIN32
      if (strcasecmp(value_ptr, "true") == 0)
#else
      if (_stricmp(value_ptr, "true") == 0)
#endif
      {

          gmsec_SetFieldName(this->field, "SEC-ENCRYPT", this->status);
          gmsec_SetFieldValueBOOL(this->field, GMSEC_TRUE, this->status);
          gmsec_MsgAddField(this->message, this->field, this->status);
          if (!example_check("AddField(SEC-ENCRYPT)", this->status)) return GMSEC_FALSE;
      }

  }

  /* Dump Message */
  xml = NULL;
  gmsec_MsgToXML(this->message, &xml, this->status);
  if (!example_check("MsgToXML(Dump Message)", this->status)) return GMSEC_FALSE;
  LOG_INFO("prepared\n%s\n", xml);

  /* Publish Message */
  gmsec_Publish(this->connection, this->message, this->status);
  if (!example_check("Publish", this->status)) return GMSEC_FALSE;

  LOG_INFO("Published!");

  /* Destroy Field */
  gmsec_DestroyField(&this->field, this->status);
  if (!example_check("Destroy Field", this->status)) return GMSEC_FALSE;
  
  return GMSEC_TRUE;
}

int gmpub_Run(gmpub_t *this)
{
  GMSEC_STR subject;
  GMSEC_STR tmp;
  int iterations;
  int interval_ms;
  int i;


  /* Output the GMSEC API version */
  LOG_INFO("%s", gmsec_GetAPIVersion());

  subject = NULL;
  example_getStringDefault(this->config, "SUBJECT",
                           "GMSEC.TEST.PUBLISH", &subject);

  iterations = example_getInteger(this->config, "ITERATIONS", 1);
  interval_ms = example_getInteger(this->config, "INTERVAL_MS", 1000);

  /* Establish the Connection */
  this->connection = example_openConnection(this->config, this->status);
  if (this->connection == NULL) return GMSEC_FALSE;

  /* Output the middleware version */
  tmp = NULL;
  gmsec_GetLibraryVersion(this->connection, &tmp, this->status);
  if (!example_check("GetLibraryVersion", this->status)) return GMSEC_FALSE;

  if (tmp != NULL)
  {
     LOG_INFO("%s",tmp);
  }

  for (i=0; i < iterations; i++)
  {
     if (!gmpub_Publish(this, i, subject)) return GMSEC_FALSE;

     if (i < iterations - 1)
        example_millisleep(interval_ms);
  }

  return GMSEC_TRUE;
}

void gmpub_Cleanup(gmpub_t *this)
{
  /* Destroy Field */
  if (this->field != NULL)
  {
     gmsec_DestroyField(&this->field, this->status);
     example_check("Final Destroy Field", this->status);
  }

  /* Destroy the Message */
  if (this->message != NULL)
  {
     gmsec_DestroyMessage(this->connection, this->message, this->status);
     example_check("Final DestroyMessage", this->status);
  }

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
  gmpub_t gmpub;

  if (!gmpub_Initialize(&gmpub, argc, argv))
  {
     fprintf(stderr, "\nFailure of initializing the example");
     return -1;
  }

  if (example_option_invalid(gmpub.config, argc)) 
  {
     gmpub_printUsage();
     return -1;
  }

  gmpub_Run(&gmpub);

  gmpub_Cleanup(&gmpub);
 
  return 0;
}

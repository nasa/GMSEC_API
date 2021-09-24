
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 * @file example.h
 *
 *  A C example containg functions support C example programs.
 */

#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <gmsec/c/api.h>
#include <gmsec/util/log_c.h>
#include <gmsec/util/timeutil_c.h>
#include <gmsec/c/status_c.h>
#include <gmsec/c/config_c.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>

	#define snprintf sprintf_s
	#define strncpy(d,s,n) strcpy_s((d),(n),(s))
#else
	#include <sys/time.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
#endif

#define EXAMPLE_MSG_TIMEOUT GMSEC_WAIT_FOREVER
#define EXAMPLE_PROG_TIMEOUT GMSEC_WAIT_FOREVER

#define EXAMPLE_INT_PROMPT "Please enter number or 'x' to exit: "
#define EXAMPLE_EMPTY_INPUT "Empty Input."
#define EXAMPLE_INVALID_INPUT "Invalid Input."
#define EXAMPLE_OUT_OF_RANGE "Invalid Input due to out of range."

#define EXAMPLE_ERR_PROMPT(s, x,y) \
        printf("\n%s\nPlease enter integer only (%d to %d) or 'x' to exit: ",\
               s, x, y)

#define EXAMPLE_TECH_MSG "Technical issue.  Please contact the developer..."

#define MAX_CHARS_PER_LINE 1024

int example_getString(GMSEC_CONFIG_OBJECT config, const char *key, 
                          const char **value)
{
  GMSEC_STATUS_OBJECT status = NULL;
  char *configValue = NULL;
  int ret_val = GMSEC_FALSE;

  gmsec_CreateStatus(&status);
  gmsec_ConfigGetValue(config, key, ((const char **)(&configValue)), status);
  if (!gmsec_isStatusError(status) && configValue != NULL)
  {
     *value = configValue;
     ret_val = GMSEC_TRUE;
  }
  gmsec_DestroyStatus(&status);

  return ret_val;

}


int example_option_invalid(GMSEC_CONFIG_OBJECT config, int nbr_of_args)
{
  const char *str_level = NULL;

  if (nbr_of_args <= 1 || 
      (!example_getString(config, "connectiontype", &str_level) && 
       !example_getString(config, "mw-id", &str_level))) {
     return GMSEC_TRUE;
  }

  return GMSEC_FALSE;
}


void add_to_config_from_file(GMSEC_CONFIG_OBJECT* config)
{
	const char* cfgfilename = NULL;

	//
	// This assumes, now, that there is only _one_ CFGFILE=filename.xml arg
	// specified.
	//
	if (example_getString(*config, "CFGFILE", &cfgfilename))
	{
#ifndef WIN32
		FILE* fp = fopen(cfgfilename, "r");

		if (fp)
#else
		FILE*   fp;
		errno_t err = fopen_s(&fp, cfgfilename, "r");

		if (err == 0)
#endif
		{

			//
			// Do this as a 2-pass read, once to determine
			// approximately how big the file is; once
			// to allocate a string commensurate to that size
			// and actually populate it with the XML file
			// contents.
			//
			char                text_line[MAX_CHARS_PER_LINE];
			int                 num_lines = 0;
			const char*         file_text_contents = NULL;
			GMSEC_CONFIG_OBJECT config_from_file;
			GMSEC_STATUS_OBJECT status;

			while (!feof(fp))
			{
				fgets(text_line, MAX_CHARS_PER_LINE, fp);

				if (!feof(fp))
				{
					num_lines++;
				}
			}

			fclose(fp);

			file_text_contents =
			  (const char* ) malloc(MAX_CHARS_PER_LINE*num_lines);

#ifndef WIN32
			fp = fopen(cfgfilename, "r");
#else
			fopen_s(&fp, cfgfilename, "r");
#endif

			while (!feof(fp))
			{
				fgets(text_line, MAX_CHARS_PER_LINE, fp);

				if (!feof(fp) &&
				    strcmp(text_line, "") != 0)
				{
#ifndef WIN32
					strcat((char* ) file_text_contents, text_line);
					strcat((char* )file_text_contents, "\n");
#else
					strcat_s((char* ) file_text_contents,
					         MAX_CHARS_PER_LINE*num_lines,
					         text_line);
					strcat_s((char* )file_text_contents,
					         MAX_CHARS_PER_LINE*num_lines,
					         "\n");
#endif
					LOG_DEBUG(text_line);
				}
			}

			fclose(fp);

			gmsec_CreateStatus(&status);
			gmsec_CreateConfig(&config_from_file, status);

			gmsec_ConfigFromXML(config_from_file, file_text_contents, status);

			if (!gmsec_isStatusError(status))
			{
	                        //
        	                // name and value C strings are allocated,
        	                // and are supposed to be memory-handled in
        	                // general, in  GetFirst(), GetNext(),
        	                // and the destructor of the BaseConfig class
				// which are ultimately called down the line
				// in C when gmsec_ConfigGetFirst(),
				// gmsec_ConfigGetNext(), and DestroyConfig()
				// are called.
        	                //

				const char* name = NULL;
				const char* value = NULL;

				//
				// Read all the config file name, value pairs
				// and add them to the current base
				// configuration (which can have its
				// (name, value) pairs from both command-line
				// pairs and from the input current config 
				// file that was read).
				//
				gmsec_ConfigGetFirst(config_from_file, &name, &value, status);
				while (!gmsec_isStatusError(status))
				{
					LOG_DEBUG("Adding (%s, %s) from config file %s",
					          name, value, cfgfilename);

					gmsec_ConfigAddValue(*config, name, value, status);

					gmsec_ConfigGetNext(config_from_file, &name, &value, status);
				}
			}
			else
			{
				LOG_WARNING("Error in gmsec_ConfigFromXML in "
				            "add_to_config_from_file");
			}

			free((char* ) file_text_contents);

		}
		else
		{
			LOG_WARNING("Non-valid-filepath config argument %s seen; ignoring it",
			            cfgfilename);
		}

	}// if there is a config file argument at all

}


int example_check(const char *tag, GMSEC_STATUS_OBJECT status)
{
  LOG_VERBOSE("calling %s", tag);

  if (!gmsec_isStatusError(status))
     return GMSEC_TRUE;

  LOG_WARNING("%s: %s", tag, gmsec_GetStatus(status));

  return GMSEC_FALSE;
}


GMSEC_CONNECTION_OBJECT example_openConnection(GMSEC_CONFIG_OBJECT config, 
                                               GMSEC_STATUS_OBJECT status)
{
  GMSEC_CONNECTION_OBJECT connection = NULL;

  /* Create the connection  */
  gmsec_CreateConnectionForConfig(config, &connection, status);
  if (example_check("Creating Connection", status))
  {
     /* Connect */
     gmsec_Connect(connection, status);
     if (!example_check("Connecting", status))
        return NULL;
  }

  return connection;
}


int example_closeConnection(GMSEC_CONNECTION_OBJECT connection,
                                GMSEC_STATUS_OBJECT status)
{
  /* Disconnect */
  if (gmsec_IsConnected(connection, status))
  {
     gmsec_Disconnect(connection, status);
     if (!example_check("Disconnecting", status)) return GMSEC_FALSE;
  }

  /* Destroy the connection */
  gmsec_DestroyConnection(connection, status);
  if (!example_check("DestoryConnection", status)) return GMSEC_FALSE;

  return GMSEC_TRUE;
}


void example_getStringDefault(GMSEC_CONFIG_OBJECT config, const char *key, 
                              const char *xdefault, const char **value)
{
  const char *configValue = NULL;
  if (!example_getString(config, key, &configValue))
     *value = (char *) xdefault;
  else
     *value = configValue;
}

int example_getInteger(GMSEC_CONFIG_OBJECT config, const char *key, int xdefault)
{
  int int_value = xdefault;
  size_t i, len;
  const char *str_value;
  char *pend = 0;

  if (example_getString(config, key, &str_value))
  {
     len = strlen(str_value);
     for (i=0; i < len; i++)
     {
        if (!isdigit(str_value[i]))
        {
           LOG_WARNING("value(%s) is not digit!", str_value);
           return xdefault;
        }
     }

     int_value = (int) strtol(str_value, &pend, 10);
     if (pend && *pend)
     {
        LOG_WARNING("%s", EXAMPLE_TECH_MSG);
        return xdefault; 
     }
     int_value = atoi(str_value);
  }

  return int_value;

}

void example_LogHandler(const GMSEC_LOG_ENTRY *entry)
{
  char tempBuffer[50];
  gmsec_FormatTime_s(entry->time, tempBuffer);
  printf("%s:%s:%s\n\n", tempBuffer, gmsec_LogLevelToString(entry->level), 
         entry->message);
}

int example_initialize(GMSEC_CONFIG_OBJECT config)
{
  const char *str_level = NULL;
  int log_level;

  example_getStringDefault(config, "LOGLEVEL", "INFO", &str_level);

  log_level = gmsec_LogLevelFromString(str_level);
  gmsec_SetReportingLevel(log_level);
  gmsec_RegisterLogHandler(example_LogHandler);

  return GMSEC_TRUE;
}

void example_sleep(int seconds)
{
  if (seconds < 1)
     seconds = 1;

#ifdef WIN32
  Sleep(seconds*1000);
#else
  sleep(seconds);
#endif
}

void example_millisleep(int milliseconds)
{
  if (milliseconds < 1)
     milliseconds = 1;

#ifdef WIN32
  Sleep(milliseconds);
#else
  struct timeval tv = { milliseconds / 1000, (milliseconds % 1000) * 1000 };
  select(0, NULL, NULL, NULL, &tv);
#endif 

}

#ifndef WIN32
int example_kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
#endif

void example_await_input(void)
{
#ifdef _WIN32
  while (!_kbhit())
  {
  }
#else
  while (!example_kbhit())
  {
  }
#endif
}

void example_trim_whitespace(char *str)
{
  char *start = str;
  char *end = start + strlen(str);

  /* Trim trailing whitespace */
  while (--end >= start)
  {
    if (!isspace(*end))
       break;
  }
  *(++end) = '\0';

  /* Trim leading whitespace */
  while (isspace(*start))
     start++;

  if (start != str)
     memmove(str, start, end - start + 1);
}

int example_strToI16(GMSEC_STR str_value, GMSEC_I16 *value)
{
  char *pend = 0;
  *value = (GMSEC_I16) strtol(str_value, &pend, 10);
  if (pend && *pend)
  {
     LOG_WARNING("Not able to convert the string '%s' to integer",
                 str_value);
     return GMSEC_FALSE;
   }
   return GMSEC_TRUE;
}

int example_input_integer(int min, int max)
{
  int i;
  int len;
  int int_number = -1;
  char str[256];
  int done = GMSEC_FALSE;

  if (min > max)
  {
     LOG_WARNING("%s", EXAMPLE_TECH_MSG);
     return -1;
  }

  printf("\n%s", EXAMPLE_INT_PROMPT);

  while (!done)
  {
    done = GMSEC_TRUE;
    fgets (str, sizeof(str)-1, stdin);
    example_trim_whitespace(str);
    len = (int) strlen(str);

    if (len == 0)
    {
       EXAMPLE_ERR_PROMPT(EXAMPLE_EMPTY_INPUT, min, max);
       done = GMSEC_FALSE;
    }
    if (len == 1 && toupper(str[0]) == 'X')
       return -1;

    for (i=0; i < len; i++)
    {
       if (!isdigit(str[i]))
       {
          EXAMPLE_ERR_PROMPT(EXAMPLE_INVALID_INPUT, min, max);
          done = GMSEC_FALSE;
          break;
       }
    }
    if (done)
    {
       char *pend = 0;
       int_number = (int) strtol(str, &pend, 10);
       if (pend && *pend)
       {
          LOG_WARNING("%s", EXAMPLE_TECH_MSG);
          return -1; 
       }
       if (int_number < 1 || int_number > max)
       {
           EXAMPLE_ERR_PROMPT(EXAMPLE_OUT_OF_RANGE, min, max);
           done = GMSEC_FALSE;
       }
    }
  }

  return int_number;
}

int example_stopMenu(const char* question)
{
  char str[256];

  /* Forever until it meets 'N' or 'Y' */
  while (1) 
  {
    printf("\n%s (y/n)? ", question);
    fgets (str, sizeof(str)-1, stdin);
    example_trim_whitespace(str);

    if (strlen(str) == 1)
    {
       if (toupper(str[0]) == 'Y')
           return GMSEC_FALSE;
       else if (toupper(str[0]) == 'N')
           return GMSEC_TRUE;
    }
  }
}


/* @TODO: is it useful for gmreq_c_cb? */
int example_timeout_done(time_t start, int timeout_ms)
{
  int timeout;
  double diff_time;

  if (timeout_ms < 0)
     return GMSEC_TRUE;

  timeout = timeout_ms / 1000;

  diff_time = difftime(time(NULL), start);

  return (diff_time < timeout);
}

#endif  /* _EXAMPLE_H_ */

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file heartbeat_generator.c
 *
 * This file contains an example describing how to instantiate and
 * start/stop a Heartbeat (Message) Generator.
 */

#include <gmsec4_c.h>

#include <stdio.h>
#include <stdlib.h>


const char*     HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB";
const GMSEC_U16 HB_PUBLISH_RATE    = 5;


//o Helper functions
void initializeLogging(GMSEC_Config config, GMSEC_Status status);
void checkStatus(GMSEC_Status status);


int main (int argc, char* argv[])
{
	GMSEC_Status             status       = statusCreate();
	GMSEC_Config             config       = NULL;
	GMSEC_HeartbeatGenerator hbgen        = NULL;
	size_t                   numStdFields = 0;
	GMSEC_Field*             stdFields    = NULL;
	unsigned int             version;
	size_t                   n;

	if (argc <= 1)
	{
		fprintf(stderr, "usage: %s mw-id=<middleware ID>\n", argv[0]);
		return -1;
	}


	//o Setup configuration with the supplied command line arguments
	config = configCreateWithArgs(argc, argv);

	//o Unless otherwise configured, setup configuration that allows us to
	//  log messages to stderr.
	initializeLogging(config, status);

	//o Display the version number of the GMSEC API
	GMSEC_INFO(connectionManagerGetAPIVersion());

	//o Get our message specification version, if available.
	//  If not availabe, use current specification.
	version = configGetIntegerValueWithDefault(config, "gmsec-specification-version", C_GMSEC_ISD_CURRENT, NULL);

	//o Define the standard fields that will be included with the heartbeat messages
	if (version == C_GMSEC_ISD_2014_00)
	{
		numStdFields = 5;
	}
	else if (version == C_GMSEC_ISD_2016_00)
	{
		numStdFields = 4;
	}
	else
	{
		numStdFields = 6;
	}
	stdFields = malloc(numStdFields * sizeof(GMSEC_Field));

	stdFields[0] = stringFieldCreate("COMPONENT", "HEARTBEAT-GENERATOR", NULL);
	stdFields[1] = stringFieldCreate("MISSION-ID", "MY-MISSION", NULL);
	stdFields[2] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", NULL);
	stdFields[3] = stringFieldCreate("FACILITY", "MY-FACILITY", NULL);

	if (version == C_GMSEC_ISD_2014_00)
	{
		stdFields[4] = stringFieldCreate("MSG-ID", "MY-MSG-ID", NULL);
	}
	else if (version >= C_GMSEC_ISD_2018_00)
	{
		stdFields[4] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", NULL);
		stdFields[5] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", NULL);
	}

	//o Instantiate the heartbeat generator
	hbgen = heartbeatGeneratorCreateWithFields(config, HB_MESSAGE_SUBJECT, HB_PUBLISH_RATE, stdFields, numStdFields, status);
	checkStatus(status);

	//o Start heartbeat generator (use gmsub utility to view messages being published)
	heartbeatGeneratorStart(hbgen, status);
	checkStatus(status);
	GMSEC_INFO("Heartbeat Generator is running; use gmsub or other utility to monitor messages.");

	//o Wait for user input to stop the heartbeat generator
	GMSEC_INFO("Press <enter> to stop the heartbeat generator");
	getchar();

	//o Stop heartbeat generator
	heartbeatGeneratorStop(hbgen, status);
	checkStatus(status);
	GMSEC_INFO("Heartbeat Generator has been stopped.");

	//o Destroy all objects created
	heartbeatGeneratorDestroy(&hbgen);
	configDestroy(&config);
	statusDestroy(&status);

	for (n = 0; n < numStdFields; ++n)
	{
		fieldDestroy(&stdFields[n]);
	}
	free(stdFields);

	return 0;
}


void initializeLogging(GMSEC_Config config, GMSEC_Status status)
{
	const char* logLevel = configGetValue(config, "LOGLEVEL", status);
	const char* logFile  = configGetValue(config, "LOGFILE", status);

	if(!logLevel)
	{
		configAddValue(config, "LOGLEVEL", "INFO", status);
	}
	if(!logFile)
	{
		configAddValue(config, "LOGFILE", "STDOUT", status);
	}
}


void checkStatus(GMSEC_Status status)
{
	if(!statusIsError(status))
	{
		return;
	}

	GMSEC_ERROR(statusGet(status));
	exit(-1);
}

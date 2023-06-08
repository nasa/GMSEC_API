/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file heartbeat_generator.c
 *
 * @brief This example program provides a basic demonstration of using
 * the HeartbeatGenerator to publish heartbeat messages onto the GMSEC
 * Bus.
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdio.h>
#include <stdlib.h>


void checkStatus(const char* operation, GMSEC_Status status);

void getStandardFields(GMSEC_Field** standardFields, size_t* numFields);
void freeStandardFields(GMSEC_Field** standardFields, size_t numFields);


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to the bus
	configAddValue(config, "gmsec-msg-content-validate-send", "true", NULL);

	// Define the heartbeat publish rate
	GMSEC_U32 publishRate = 2;  // in seconds

	// Acquire standard fields to be included with the heartbeat messages
	GMSEC_Field* standardFields = {0};
	size_t       numFields = 0;

	getStandardFields(&standardFields, &numFields);

	// Create HeartbeatGenerator instance.
	GMSEC_HeartbeatGenerator hbgen = heartbeatGeneratorCreateWithFields(config, publishRate, standardFields, numFields, status);
	checkStatus("heartbeatGeneratorCreateWithFields", status);

	// Release resources associated with standard fields
	freeStandardFields(&standardFields, numFields);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("HeartbeatGenerator publish rate is: %u seconds", publishRate);

	// Start the heartbeat generator
	heartbeatGeneratorStart(hbgen, status);
	checkStatus("heartbeatGeneratorStart", status);

	GMSEC_INFO("HeartbeatGenerator is running; use gmsub or other utility to monitor messages");

	GMSEC_INFO("Press <enter> to stop the HeartbeatGenerator");
	getchar();

	// Stop the HeartbeatGenerator
	heartbeatGeneratorStop(hbgen, status);
	checkStatus("heartbeatGeneratorStop", status);

	GMSEC_INFO("HeartbeatGenerator has been stopped");

	// Cleanup resources
	heartbeatGeneratorDestroy(&hbgen);
	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}


void checkStatus(const char* operation, GMSEC_Status status)
{
	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("%s failed; reason: %s", operation, statusGet(status));
		exit(-1);
	}
}


void getStandardFields(GMSEC_Field** standardFields, size_t* numFields)
{
	*numFields      = 8;
	*standardFields = malloc(*numFields * sizeof(GMSEC_Field));

	(*standardFields)[0] = stringFieldCreate("MISSION-ID", "MY-MISSION", GMSEC_TRUE, NULL);
	(*standardFields)[1] = stringFieldCreate("SAT-ID-PHYSICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	(*standardFields)[2] = stringFieldCreate("SAT-ID-LOGICAL", "MY-SAT-ID", GMSEC_TRUE, NULL);
	(*standardFields)[3] = stringFieldCreate("CONSTELLATION-ID", "MY-CNST", GMSEC_TRUE, NULL);
	(*standardFields)[4] = stringFieldCreate("FACILITY", "MY-FACILITY", GMSEC_TRUE, NULL);
	(*standardFields)[5] = stringFieldCreate("DOMAIN1", "MY-DOMAIN-1", GMSEC_TRUE, NULL);
	(*standardFields)[6] = stringFieldCreate("DOMAIN2", "MY-DOMAIN-2", GMSEC_TRUE, NULL);
	(*standardFields)[7] = stringFieldCreate("COMPONENT", "HB-GEN", GMSEC_TRUE, NULL);
}


void freeStandardFields(GMSEC_Field** standardFields, size_t numFields)
{
	size_t i;

	for (i = 0; i < numFields; ++i)
	{
		fieldDestroy(&(*standardFields)[i]);
	}

	free(*standardFields);

	*standardFields = NULL;
}

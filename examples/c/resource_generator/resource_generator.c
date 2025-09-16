/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file resource_generator.c
 *
 * @brief This example program provides a basic demonstration of using
 * the ResourceGenerator to publish resource messages onto the GMSEC
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

	// Define the resource publish rate and how often resource are collected
	GMSEC_U16 publishRate     = 5;  // in seconds
	GMSEC_U16 sampleInterval  = 1;  // in seconds
	GMSEC_U16 averageInterval = 10; // in seconds

	// Acquire standard fields to be included with the resource messages
	GMSEC_Field* standardFields = {0};
	size_t       numFields = 0;

	getStandardFields(&standardFields, &numFields);


	// Create ResourceGenerator instance.
	GMSEC_ResourceGenerator rsrcgen = resourceGeneratorCreateWithFields(config,
	                                                                    publishRate,
	                                                                    sampleInterval,
	                                                                    averageInterval,
	                                                                    standardFields,
	                                                                    numFields,
	                                                                    status);
	checkStatus("resourceGeneratorCreateWithFields", status);

	// Release resources associated with standard fields
	freeStandardFields(&standardFields, numFields);

	// Output information regarding the API version and connection
    GMSEC_INFO(connectionGetAPIVersion());
    GMSEC_INFO("ResourceGenerator publish rate is: %u seconds", publishRate);

	// Start the resource generator
	resourceGeneratorStart(rsrcgen, status);
	checkStatus("resourceGeneratorStart", status);

	GMSEC_INFO("ResourceGenerator is running; use gmsub or other utility to monitor messages");

	GMSEC_INFO("Press <enter> to stop the ResourceGenerator");
	getchar();

	// Stop the ResourceGenerator
	resourceGeneratorStop(rsrcgen, NULL);

	GMSEC_INFO("ResourceGenerator has been stopped");

	// Cleanup resources
	resourceGeneratorDestroy(&rsrcgen);
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
	(*standardFields)[7] = stringFieldCreate("COMPONENT", "RSRC-GEN", GMSEC_TRUE, NULL);
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

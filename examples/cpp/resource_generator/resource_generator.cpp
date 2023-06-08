/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file resource_generator.cpp
 *
 * @brief This example program provides a basic demonstration of using
 * the ResourceGenerator to publish resource messages onto the GMSEC
 * Bus.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <iostream>
#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


List<Field*> getStandardFields();
void freeStandardFields(List<Field*>& standardFields);


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to the bus
	config.addValue("gmsec-msg-content-validate-send", "true");

	// Define the resource publish rate and how often resource are collected
	GMSEC_U16 publishRate     = 5;  // in seconds
	GMSEC_U16 sampleInterval  = 1;  // in seconds
	GMSEC_U16 averageInterval = 10; // in seconds

	try
	{
		// Acquire standard fields to be included with the resource messages
		List<Field*> standardFields = getStandardFields();

		// Create ResourceGenerator instance.
		ResourceGenerator rsrcgen(config, publishRate, sampleInterval, averageInterval, standardFields);

		// Release resources associated with standard fields
		freeStandardFields(standardFields);

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "ResourceGenerator publish rate is: " << publishRate << " seconds";

		// Start the resource generator
		rsrcgen.start();

		GMSEC_INFO << "ResourceGenerator is running; use gmsub or other utility to monitor messages";

		GMSEC_INFO << "Press <enter> to stop the ResourceGenerator";
		std::string enter;
		std::getline(std::cin, enter);

		// Stop the ResourceGenerator
		rsrcgen.stop();

		GMSEC_INFO << "ResourceGenerator has been stopped";
	}
	catch (const GmsecException& e)
	{
		// If here, the most likely case is the middleware could not be
		// deduced, or a failure occurred when attempting to connect to
		// the GMSEC Bus.
		GMSEC_ERROR << "GmsecException: " << e.what();
	}
}


List<Field*> getStandardFields()
{
	List<Field*> standardFields;

	standardFields.push_back(new StringField("MISSION-ID", "MY-MISSION", true));
	standardFields.push_back(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
	standardFields.push_back(new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
	standardFields.push_back(new StringField("CONSTELLATION-ID", "MY-CNST", true));
	standardFields.push_back(new StringField("FACILITY", "MY-FACILITY", true));
	standardFields.push_back(new StringField("DOMAIN1", "MY-DOMAIN-1", true));
	standardFields.push_back(new StringField("DOMAIN2", "MY-DOMAIN-2", true));
	standardFields.push_back(new StringField("COMPONENT", "RSRC-GEN", true));

	return standardFields;
}


void freeStandardFields(List<Field*>& standardFields)
{
	for (List<Field*>::iterator it = standardFields.begin(); it != standardFields.end(); ++it)
	{
		delete *it;
	}
	standardFields.clear();
}

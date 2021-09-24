/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file heartbeat_generator.cpp
 * 
 * This file contains an example describing how to instantiate and
 * start/stop a Heartbeat (Message) Generator.
 */

#include "gmsec4_cpp.h"

#include <string>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


const char*     HB_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.HEARTBEAT-GENERATOR.HB";
const GMSEC_U16 HB_PUBLISH_RATE    = 5;


//o Helper functions
void initializeLogging(Config& config);

DataList<Field*> defineStandardFields(const Config& config);


int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	//o Setup configuration with the supplied command line arguments
	Config config(argc, argv);

	//o Unless otherwise configured, setup configuration that allows us to
	//  log messages to stderr.
	initializeLogging(config);

	//o Display the version number of the GMSEC API
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Define the standard fields that will be included with the heartbeat messages
		DataList<Field*> standardFields = defineStandardFields(config);

		//o Instantiate the heartbeat generator
		HeartbeatGenerator hbgen(config, HB_MESSAGE_SUBJECT, HB_PUBLISH_RATE, standardFields);

		//o Clean up
		for (DataList<Field*>::iterator it = standardFields.begin(); it != standardFields.end(); ++it)
		{
			delete *it;
		}

		//o Start heartbeat generator (use gmsub utility to view messages being published)
		hbgen.start();
		GMSEC_INFO << "Heartbeat Generator is running; use gmsub or other utility to monitor messages.";

		//o Wait for user input to stop the heartbeat generator
		std::string enter;
		GMSEC_INFO << "Press <enter> to stop the heartbeat generator";
		std::getline(std::cin, enter);

		//o Stop heartbeat generator
		hbgen.stop();
		GMSEC_INFO << "Heartbeat Generator has been stopped.";
	}
	catch (const Exception& e)
	{
		//o Handle unforeseen error
		GMSEC_ERROR << e.what();
		return -1;
	}
}


void initializeLogging(Config& config)
{
	const char* logLevel = config.getValue("LOGLEVEL");
	const char* logFile  = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}


DataList<Field*> defineStandardFields(const Config& config)
{
	DataList<Field*> standardFields;

	standardFields.push_back(new StringField("COMPONENT", "HEARTBEAT-GENERATOR"));
	standardFields.push_back(new StringField("MISSION-ID", "MY-MISSION"));
	standardFields.push_back(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID"));
	standardFields.push_back(new StringField("FACILITY", "MY-FACILITY"));

	if (config.getIntegerValue("gmsec-specification-version", (int) mist::GMSEC_ISD_CURRENT) == (int) mist::GMSEC_ISD_2014_00)
	{
		standardFields.push_back(new StringField("MSG-ID", "MY-MSG-ID"));
	}
	else if (config.getIntegerValue("gmsec-specification-version", (int) mist::GMSEC_ISD_CURRENT) > (int) mist::GMSEC_ISD_2016_00)
	{
		standardFields.push_back(new StringField("DOMAIN1", "MY-DOMAIN-1"));
		standardFields.push_back(new StringField("DOMAIN2", "MY-DOMAIN-2"));
	}

	return standardFields;
}

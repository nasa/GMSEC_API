/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file validation_addendum.cpp
 * 
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>
#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;

//o Note: The name 'EXAMPLE' in this subject represents the name of the message
// specification which is being used.  If using message definitions outlined in
// the GMSEC Interface Specification Document, this would instead be, "GMSEC."
const char* EXAMPLE_MESSAGE_SUBJECT = "EXAMPLE.MISSION.SPACECRAFT.MSG.LOG";

//o Helper functions
void initializeLogging(Config& config);
std::string addTimeToString(const char* preface);


int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	initializeLogging(config);

	//o Enable Message validation.  This parameter is "false" by default.
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	//o Tell the API that there is an additional layer of message schema to
	// validate (The 'EXAMPLE' message definitions).  This value is set to
	// 0 (Only 'GMSEC' specification validation) by default.
	//
	// Note: These levels for validation are determined by the "LEVEL-X"
	// attributes defined in the .DIRECTORY.xml file contained in the XML
	// templates directory.  In thise case, Level-0 means GMSEC and Level-1
	// means EXAMPLE.
	//
	// Note: The GMSEC team envisions using message specifications in a
	// layered hierarchical fashion.  For example, the "GMSEC" message
	// specification would be 'layer 0', followed by an organization-level
	// message specification at 'layer 1' which builds upon the message
	// specification outlined in the GMSEC ISD.  This would be followed by
	// a mission-level message specification at 'layer 2' and so on.
	config.addValue("GMSEC-SCHEMA-LEVEL", "1");

	//o Tell the API where to find all of the schema files.
	//
	// Note: This example only demonstrates a simple usage case of this
	// functionality.  When using this functionality, if the intent is
	// to use any of the GMSEC message definitions, then ALL of the XML
	// template files must be contained in the same directory.
	// e.g. GMSEC_API/templates/2016.00 (Or the location defined in
	// GMSEC-SCHEMA-PATH)
	config.addValue("GMSEC-SCHEMA-PATH", "templates");

	//o Since this example relies on the 2016.00 version of the templates,
	//  we indicate such within the configuration object.
	config.addValue("GMSEC-SPECIFICATION-VERSION", "201600");

	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connMgr(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		GMSEC_INFO << connMgr.getLibraryVersion();

		DataList<Field*> definedFields;

		StringField missionField("MISSION-ID", "MISSION");
		StringField satIdField("SAT-ID-PHYSICAL", "SPACECRAFT");
		StringField facilityField("FACILITY", "GMSEC Lab");
		StringField componentField("COMPONENT", "log_message");

		definedFields.push_back(&missionField);
		definedFields.push_back(&satIdField);
		definedFields.push_back(&facilityField);
		definedFields.push_back(&componentField);

		connMgr.setStandardFields(definedFields);

		//o Create a Message using a subject defined in the XML template
		// outlining our example addendum message definitions
		MistMessage message(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification());

		//o Add all of the necessary Fields to our message
		message.addField("NUM-OF-EVENTS", (GMSEC_U16) 2);
		message.setValue("EVENT.1", addTimeToString("AOS occurred at: ").c_str());
		message.setValue("EVENT.2", addTimeToString("Telemetry downlink began at: ").c_str());

		connMgr.addStandardFields(message);

		//o Publish the message to the middleware bus
		connMgr.publish(message);

		//o Display the XML string representation of the Message for
		// the sake of review
		GMSEC_INFO << "Published message:\n" << message.toXML();

		//o Setup a new message without some of the Required Fields and
		// attempt to publish it (i.e. Trigger a validation failure)
		MistMessage badMessage(EXAMPLE_MESSAGE_SUBJECT, "MSG.LOG", connMgr.getSpecification());

		try
		{
			connMgr.publish(badMessage);
		}
		catch (Exception e)
		{
			GMSEC_ERROR << "This error is expected:\n" << e.what();
		}

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();
	}
	catch (Exception e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}


void initializeLogging(Config& config)
{
	const char* logLevel  = config.getValue("LOGLEVEL");
	const char* logFile   = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}

std::string addTimeToString(const char* preface)
{
	std::stringstream result;
	char theTime[22];
	TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
	result << preface << theTime;
	return result.str();
}


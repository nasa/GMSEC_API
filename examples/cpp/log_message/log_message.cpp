/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file log_message.cpp
 * 
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant LOG message to the middleware bus using the Messaging
 * Interface Standardization Toolkit (MIST) namespace ConnectionManager class
 * function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>
#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;

const char* GMSEC_SPEC_VERSION  = "201600";
const char* LOG_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.LOG";

//o Helper functions
void initializeLogging(Config& config);


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	initializeLogging(config);

	//o Set the GMSEC message specification version to be used to determine
	// what the structure of messages is for verification and the
	// construction of MistMessages
	config.addValue("GMSEC-SPECIFICATION-VERSION", GMSEC_SPEC_VERSION);

	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connManager(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connManager.initialize();

		GMSEC_INFO << connManager.getLibraryVersion();

		//o Begin the steps necessary to create a GMSEC-compliant LOG
		// message using the ConnectionManager

		//o Create all of the GMSEC Message header Fields which will
		// be used by all GMSEC Messages
		//
		// Note: Since these Fields contain variable values which are
		// based on the context in which they are used, they cannot be
		// automatically populated using MistMessage.
		DataList<Field*> definedFields;

		StringField missionField("MISSION-ID", "MISSION");
		// Note: SAT-ID-PHYSICAL is an optional header Field, according
		// to the GMSEC ISD.
		StringField satIdField("SAT-ID-PHYSICAL", "SPACECRAFT");
		StringField facilityField("FACILITY", "GMSEC Lab");
		StringField componentField("COMPONENT", "log_message");

		definedFields.push_back(&missionField);
		definedFields.push_back(&satIdField);
		definedFields.push_back(&facilityField);
		definedFields.push_back(&componentField);

		//o Use setStandardFields to define a set of header fields for
		// all messages which are created or published on the
		// ConnectionManager using the following functions:
		// createLogMessage, publishLog, createHeartbeatMessage,
		// startHeartbeatService, createResourceMessage, 
		// publishResourceMessage, or startResourceMessageService
		connManager.setStandardFields(definedFields);

		//o Use MistMessage to construct a GMSEC LOG message based off
		// of the latest XML templates provided with the GMSEC API.
		// This will automatically populate the Message with all of the
		// Fields which have specific values defined for them in the XML
		// template files.  For more information on which Fields have
		// values defined for them, please review the XML template files
		// located in GMSEC_API/templates.
		//
		// Note: The second parameter is an identifier for the type of
		// message to construct.
		MistMessage logMsg(LOG_MESSAGE_SUBJECT, "MSG.LOG", connManager.getSpecification());

		//o Add the LOG-specific fields to the LOG message
		//
		// Note: Since these Fields contain variable values which are
		// based on the context in which they are used, they cannot be
		// automatically populated using MistMessage.
		char eventTimeBuf[GMSEC_TIME_BUFSIZE];
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), eventTimeBuf);

		logMsg.addField("SEVERITY", (GMSEC_I16) 1);
		logMsg.setValue("MSG-TEXT", "Creating an example GMSEC LOG Message");
		logMsg.setValue("OCCURRENCE-TYPE", "SYS");
		logMsg.setValue("SUBCLASS", "AST");
		logMsg.setValue("EVENT-TIME", eventTimeBuf);

		//o Add the standard fields to the LOG message
		connManager.addStandardFields(logMsg);

		connManager.publish(logMsg);

		GMSEC_INFO << "Published LOG message:\n" << logMsg.toXML();

		connManager.cleanup();
	}
	catch (gmsec::api::Exception& e)
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

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file resource_service.cpp
 *
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's ResourceMessageService
 * to publish GMSEC-compliant Resource (RSRC) messages to the middleware bus.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the HB Messages which are published to the
 * bus.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* RSRC_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.RESOURCE-SERVICE.RSRC";
const int   RSRC_PUBLISH_RATE    = 5;  // in seconds

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

	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connManager(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connManager.initialize();

		GMSEC_INFO << connManager.getLibraryVersion();

		//o Create all of the GMSEC Message header Fields which will
		// be used by all GMSEC Messages
		DataList<Field*> headerFields;

		int version = connManager.getSpecification().getVersion();

		StringField missionField("MISSION-ID", "MY-MISSION");
		StringField satIDField("SAT-ID-PHYSICAL", "MY-SAT-ID");
		StringField facilityField("FACILITY", "MY-FACILITY");
		StringField componentField("COMPONENT", "RESOURCE-SERVICE");
		StringField domain1Field("DOMAIN1", "MY-DOMAIN-1");
		StringField domain2Field("DOMAIN2", "MY-DOMAIN-2");
		StringField msgID("MSG-ID", "MY-MSG-ID");

		headerFields.push_back(&missionField);
		headerFields.push_back(&satIDField);
		headerFields.push_back(&facilityField);
		headerFields.push_back(&componentField);

		if (version == 201400)
		{
			headerFields.push_back(&msgID);
		}
		else if (version >= 201800)
		{
			headerFields.push_back(&domain1Field);
			headerFields.push_back(&domain2Field);
		}

		//o Use setStandardFields to define a set of header fields for
		// all messages which are created or published on the
		// ConnectionManager using the following functions:
		// createLogMessage, publishLog, createHeartbeatMessage,
		// startHeartbeatService, createResourceMessage,
		// publishResourceMessage, or startResourceMessageService
		connManager.setStandardFields(headerFields);

		//o Create and publish a Resource message using
		// createResourceMessage() and publish()
		//
		// Note: This is useful for applications which may need to add
		// additional Fields to the Resource Messages which are not
		// currently added by the GMSEC API
		Message rsrcMsg = connManager.createResourceMessage(RSRC_MESSAGE_SUBJECT);
		GMSEC_INFO << "Publishing the GMSEC C2CX RSRC message which was created using createResourceMessage():\n" << rsrcMsg.toXML();
		connManager.publish(rsrcMsg);

		//o Kick off the Resource Service -- This will publish resource
		// messages automatically every X seconds, where X is the second
		// parameter provided to the startResourceMessageService() function.
		// If an interval is not provided, the service will default to
		// publishing a message every 60 seconds.
		GMSEC_INFO << "Starting the Resource Message service, a message will be published every " << RSRC_PUBLISH_RATE << " seconds";
		connManager.startResourceMessageService(RSRC_MESSAGE_SUBJECT, RSRC_PUBLISH_RATE);

		//o Wait for user input to end the program
		std::string enter;
		GMSEC_INFO << "Publishing C2CX Resource Messages indefinitely, press <enter> to exit the program";
		std::getline(std::cin, enter);

		//o Stop the Heartbeat Service
		connManager.stopResourceMessageService();

		//o Cleanup
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

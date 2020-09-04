/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file heartbeat_service.cpp
 * 
 * This file contains an example outlining how to use the Messaging Interface
 * Standardization Toolkit (MIST) namespace ConnectionManager's HeartbeatService
 * to publish GMSEC-compliant Heartbeat (HB) messages to the middleware bus.
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

const char* HB_MESSAGE_SUBJECT = "GMSEC.MISSION.SPACECRAFT.MSG.C2CX.HB";

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

		StringField missionField("MISSION-ID", "GMSEC");
		StringField facilityField("FACILITY", "GMSEC Lab");
		StringField componentField("COMPONENT", "heartbeat_service");

		headerFields.push_back(&missionField);
		headerFields.push_back(&facilityField);
		headerFields.push_back(&componentField);

		//o Use setStandardFields to define a set of header fields for
		// all messages which are created or published on the
		// ConnectionManager using the following functions:
		// createLogMessage, publishLog, createHeartbeatMessage,
		// startHeartbeatService, createResourceMessage, 
		// publishResourceMessage, or startResourceMessageService
		connManager.setStandardFields(headerFields);

		// Note: Fields are immutable, so plan appropriately if you wish
		// to re-use variable names!
		{
			//o Create all of the GMSEC Message header Fields which
			// will be used by all GMSEC HB Messages
			DataList<Field*> hbStandardFields;

			//o Determine which version of the GMSEC message specification
			// the ConnectionManager was initialized with and add
			// the correctly typed Fields to the Message
			int version = connManager.getSpecification().getVersion();
			if (version == 201600)
			{
				hbStandardFields.push_back(new U16Field("PUB-RATE", (GMSEC_U16) 30));
				hbStandardFields.push_back(new U16Field("COUNTER",  (GMSEC_U16) 1));
			}
			else if (version == 201400)
			{
				hbStandardFields.push_back(new I16Field("PUB-RATE", (GMSEC_I16) 30));
				hbStandardFields.push_back(new I16Field("COUNTER",  (GMSEC_I16) 1));
			}
			//o Note: COMPONENT-STATUS is an optional field used to
			// denote the operating status of the component, the
			// values are as follows:
			// 0 - Debug
			// 1 - Normal / Green
			// 2 - Warning / Yellow
			// 3 - Orange
			// 4 - Error / Red
			I16Field componentStatusField("COMPONENT-STATUS", (GMSEC_I16) 0);

			hbStandardFields.push_back(&componentStatusField);

			//o Create and publish a Heartbeat message using
			// createLogMessage() and publish()
			//
			// Note: This is useful for applications which may need
			// to create proxy heartbeats on behalf of a subsystem,
			// as creating multiple ConnectionManagers can consume
			// more memory than necessary.  In this case, extra
			// logic would need to be added to handle the timing of
			// the publications.
			Message hbMsg = connManager.createHeartbeatMessage(HB_MESSAGE_SUBJECT, hbStandardFields);
			GMSEC_INFO << "Publishing the GMSEC C2CX HB message which was just created using createHeartbeatMessage():\n" << hbMsg.toXML();
			connManager.publish(hbMsg);

			//o Kick off the Heartbeat Service -- This will publish
			// heartbeat messages automatically every X seconds,
			// where Xis the value which was provided for PUB-RATE
			// Note: If PUB-RATE was not provided, it will default
			// to 30 seconds per automatic Heartbeat publication
			GMSEC_INFO << "Starting the Heartbeat service, a message will be published every " << hbStandardFields.get(0)->getStringValue() << " seconds";
			connManager.startHeartbeatService(HB_MESSAGE_SUBJECT, hbStandardFields);
		}

		{
			//o Use setHeartbeatServiceField to change the state of the
			// COMPONENT-STATUS Field to indicate that the component has
			// transitioned from a startup/debug state to a running/green
			// state.
			I16Field componentStatusField("COMPONENT-STATUS", (GMSEC_I16) 1);
			connManager.setHeartbeatServiceField(componentStatusField);
		}

		//o Wait for user input to end the program
		std::string enter;
		GMSEC_INFO << "Publishing C2CX Heartbeat Messages indefinitely, press <enter> to exit the program";
		std::getline(std::cin, enter);

		//o Stop the Heartbeat Service
		connManager.stopHeartbeatService();

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

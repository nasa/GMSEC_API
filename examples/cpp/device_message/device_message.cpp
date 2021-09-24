/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file device_message.cpp
 * 
 * This file contains an example outlining how to create and publish a
 * GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 * Messaging Interface Standardization Toolkit (MIST) namespace
 * ConnectionManager class function calls available for such operations.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 * bus.
 */

#include <gmsec4_cpp.h>

#include <sstream>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;

const char* DEV_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.DEV.DEVICE_MESSAGE.DEV";

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

	//o Enable Message validation.  This parameter is "false" by default.
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connManager(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connManager.initialize();

		GMSEC_INFO << connManager.getLibraryVersion();

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
		StringField componentField("COMPONENT", "device_message");

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

		I32Field paramVal("DEVICE.1.PARAM.1.VALUE", (GMSEC_I32) 79);
		DeviceParam param("DEV parameter 1", "parameter 1 timestamp", paramVal);

		Device device1("device 1", Device::RED);
		device1.setGroup("group");
		device1.setRole("role");
		device1.setModel("model");
		device1.setSerial("1138");
		device1.setVersion("1.4.5.2.3.4.5");
		I16Field devInfo("info", 5);
		device1.setInfo(devInfo);
		I16Field devNum("num", 5);
		device1.setNumber(devNum);
		device1.addParam(param);

		//o Construct an DEV Message and add the Device values to it
		mist::message::DeviceMessage devMessage(DEV_MESSAGE_SUBJECT, connManager.getSpecification());
		devMessage.addDevice(device1);

		connManager.addStandardFields(devMessage);

		connManager.publish(devMessage);

		GMSEC_INFO << "Published DEV message:\n" << devMessage.toXML();

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

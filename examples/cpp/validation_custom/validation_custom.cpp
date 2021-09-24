/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file validation_custom.cpp
 * 
 * This file contains an example demonstrating how to implement additional
 * Message validation logic in addition to that which the GMSEC API provides.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


const char* HB_MSG_SUBJECT     = "GMSEC.MISSION.SATELLITE.MSG.HB.VALIDATION-CUSTOM";
const char* ALT_HB_MSG_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.C2CX.VALIDATION-CUSTOM.HB";


//o Helper functions
void initializeLogging(Config& config);
void setupStandardFields(unsigned int specVersion);
void clearStandardFields();


//o Define custom message validator that can be used in combination with
//  the GMSEC API validation.
//
//  Note: This is an arbitrary exercise to demonstrate the functionality
//  of a custom message validator.
class CustomMessageValidator : public MessageValidator
{
public:
	Status validateMessage(const Message& msg)
	{
		Status status;

		try
		{
			//o Get message type and subtype
			std::string type;
			std::string subtype;

			if (msg.hasField("MESSAGE-TYPE"))
			{
				type = msg.getStringValue("MESSAGE-TYPE");
			}
			if (msg.hasField("C2CX-SUBTYPE"))
			{
				subtype = msg.getStringValue("C2CX-SUBTYPE");
			}
			else if (msg.hasField("MESSAGE-SUBTYPE"))
			{
				subtype = msg.getStringValue("MESSAGE-SUBTYPE");
			}

			//o Ensure we have a Heartbeat message and it contains the PUB-RATE field
			if (type == "MSG" && subtype == "HB" && msg.hasField("PUB-RATE"))
			{
				GMSEC_I64 pubRate = msg.getIntegerValue("PUB-RATE");

				//o Ensure PUB-RATE field value is between 10 and 60 (inclusive)
				if (pubRate < 10 || pubRate > 60)
				{
					status.set(MSG_ERROR, VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value");
				}
			}
			else
			{
				status.set(MSG_ERROR, INVALID_MSG, "Non-Heartbeat message received");
			}
		}
		catch (const Exception& e)
		{
			status = Status(e);
		}

		return status;
	}
};


int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	initializeLogging(config);

	//o Enable Message validation via connection configuration
	config.addValue("gmsec-msg-content-validate-send", "true");
	config.addValue("gmsec-validation-level", "3");

	GMSEC_INFO << "API version: " << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connMgr(config);

		connMgr.initialize();

		GMSEC_INFO << "Middleware version: " << connMgr.getLibraryVersion();

		//o Register custom message validator
		CustomMessageValidator cmv;
		connMgr.registerMessageValidator(&cmv);

		//o Set up standard/common fields used with all messages
		unsigned int specVersion = connMgr.getSpecification().getVersion();
		setupStandardFields(specVersion);

		//o Create Heartbeat Message
		//o Note: Message subject and schema ID vary depending on the specification in use
		const char* subject  = (specVersion > GMSEC_ISD_2018_00 ? HB_MSG_SUBJECT : ALT_HB_MSG_SUBJECT);
		const char* schemaID = (specVersion > GMSEC_ISD_2018_00 ? "MSG.HB" : "MSG.C2CX.HB");

		MistMessage msg(subject, schemaID, connMgr.getSpecification());

		//o Add PUB-RATE field with illegal value
		msg.setValue("PUB-RATE", "5");

		//o For very old specifications, we need to add a MSG-ID field
		if (specVersion <= GMSEC_ISD_2014_00)
		{
			msg.addField("MSG-ID", "12345");
		}

		//o Attempt to publish malformed message
		try
		{
			GMSEC_INFO << "Attempting to publish malformed message...";
			connMgr.publish(msg);

			GMSEC_WARNING << "Was expecting an error";
		}
		catch (const Exception& e)
		{
			//o We expect to encounter error with the PUB-RATE field
			GMSEC_INFO << "This is an expected error:\n" << e.what();
		}

		//o Fix PUB-RATE field with legal value
		msg.setValue("PUB-RATE", "15");

		//o Publish a good message
		try
		{
			GMSEC_INFO << "Attempting to publish good message...";
			connMgr.publish(msg);

			GMSEC_INFO << "Message published!";
		}
		catch (const Exception& e)
		{
			GMSEC_WARNING << "Unexpected error:\n" << e.what();
		}

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();

		//o Clear standard/common fields used with all messages
		clearStandardFields();
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


void setupStandardFields(unsigned int specVersion)
{
	DataList<Field*> standardFields;

	StringField mission("MISSION-ID", "MISSION");
	StringField constellation("CONSTELLATION-ID", "CONSTELLATION");
	StringField satIdPhys("SAT-ID-PHYSICAL", "SATELLITE");
	StringField satIdLog("SAT-ID-LOGICAL", "SATELLITE");
	StringField facility("FACILITY", "GMSEC-LAB");
	StringField component("COMPONENT", "VALIDATION-CUSTOM");
	StringField domain1("DOMAIN1", "DOMAIN1");
	StringField domain2("DOMAIN2", "DOMAIN2");

	standardFields.push_back(&mission);
	standardFields.push_back(&constellation);
	standardFields.push_back(&satIdPhys);
	standardFields.push_back(&satIdLog);
	standardFields.push_back(&facility);
	standardFields.push_back(&component);

	if (specVersion >= GMSEC_ISD_2018_00)
	{
		standardFields.push_back(&domain1);
		standardFields.push_back(&domain2);
	}

	MistMessage::setStandardFields(standardFields);
}


void clearStandardFields()
{
	MistMessage::clearStandardFields();
}

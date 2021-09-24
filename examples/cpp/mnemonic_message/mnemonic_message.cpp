/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file mnemonic_message.cpp
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

const char* GMSEC_SPEC_VERSION   = "201600";
const char* MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE";

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
		StringField componentField("COMPONENT", "mnemonic_message");

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

		//o Populate the Mnemonic Sample(s)
		MnemonicSample mSample("MS1", I32Field("MS1", 15));
		mSample.setEUValue(F32Field("My EU", 15.0));
		mSample.setFlags(1);
		mSample.setLimit(MnemonicSample::RED_HIGH);
		// Implicitly set limit enable/disable with setting of limit
		mSample.setQuality(true);
		mSample.setStalenessStatus(false);
		mSample.setTextValue("15");

		gmsec::api::util::DataList<MnemonicSample*> mnemonic_samples;
		mnemonic_samples.add(&mSample);

		//o Add the Mnemonic values to a Mnemonic object
		Mnemonic mnemonic("M1", mnemonic_samples);
		I16Field statusField("status", 5);
		mnemonic.setStatus(statusField);
		mnemonic.setUnits("units");

		//o Determine which version of the GMSEC message specification
		// the ConnectionManager was initialized with
		int version = connManager.getSpecification().getVersion();
		std::string gmsecSpecVersion = "";
		if (version == 201600)
		{
			gmsecSpecVersion = "2016.00";
		}
		else if (version == 201400)
		{
			gmsecSpecVersion = "2014.00";
		}

		//o Build up the Schema ID -- This is used to identify the
		// specific type of MVAL message to construct
		std::stringstream schemaId;
		schemaId << gmsecSpecVersion << ".GMSEC.MSG.MVAL";

		//o Construct an MVAL Message and add the Mnemonic values to it
		mist::message::MnemonicMessage mvalMessage(MVAL_MESSAGE_SUBJECT, schemaId.str().c_str(), connManager.getSpecification());
		mvalMessage.addMnemonic(mnemonic);

		//o If validating with the 2014 spec, the MSG-ID field is
		// required
		if (version == 201400)
		{
			mvalMessage.setValue("MSG-ID", "MVAL Request MSG-ID would go here");
		}

		//o Add the header fields to the MVAL message
		connManager.addStandardFields(mvalMessage);

		GMSEC_INFO << "Publishing MVAL message:\n" << mvalMessage.toXML();
		connManager.publish(mvalMessage);
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

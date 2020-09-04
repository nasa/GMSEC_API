/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmmist_validation.cpp
 * 
 * A C++ example demonstration of GMSEC the ConnectionManager class services
 * for general message validation. 
 * 
 */

#include "../example.h"

#include <gmsec4/util/DataList.h>

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class gmmist_validation
{
public:
	gmmist_validation(Config& config);
	~gmmist_validation();
	bool run();

private:
	Config& config;
};


gmmist_validation::gmmist_validation(Config& c)
	: config(c)
{
	/* Initialize config */
	example::initialize(c);
}


gmmist_validation::~gmmist_validation()
{
	Connection::shutdownAllMiddlewares();
}


bool gmmist_validation::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	Field** definedFields    = NULL;
	Field** logDefinedFields = NULL;

	try
	{
		mist::ConnectionManager connManager(config, true, mist::GMSEC_ISD_CURRENT);

		connManager.initialize();

		//o setStandardFields()
		GMSEC_INFO << "Setting fields which will be applied globally to message from this ConnectionManager";
		GMSEC_INFO << "These fields are usually certain values from the GMSEC Header which apply broadly";

		DataList<Field*> definedFields;

		F32Field    fieldVersion("HEADER-VERSION", (GMSEC_F32) 2010.0);
		StringField fieldMission("MISSION-ID", "GMSEC");
		StringField fieldFacility("FACILITY", "GMSEC Lab");
		StringField fieldComponent("COMPONENT", "gmmist-demo");

		definedFields.push_back(&fieldVersion);
		definedFields.push_back(&fieldMission);
		definedFields.push_back(&fieldFacility);
		definedFields.push_back(&fieldComponent);

		connManager.setStandardFields(definedFields);


		//o createLogMessage()
		DataList<Field*> logDefinedFields;

		GMSEC_INFO << "No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!";
		GMSEC_INFO << "Adding general log fields";

		StringField fieldSubclass("SUBCLASS", "TAC");
		StringField fieldOccurType("OCCURRENCE-TYPE", "NORM");

		logDefinedFields.push_back(&fieldSubclass);
		logDefinedFields.push_back(&fieldOccurType);

		Message msg = connManager.createLogMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);

		msg.addField("SEVERITY", (GMSEC_I16) 1);
		msg.addField("MSG-TEXT", "Self-published log message");


		//o Publish valid log message
		GMSEC_INFO << "Attempting to manually publish a valid log message...";
		connManager.publish(msg);

		GMSEC_INFO << "Demonstrating the fast log functionality...";
		GMSEC_INFO << "First we set the defaults for all log messages called with 'publishLog'";
		connManager.setLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);
		I16Field severityField("SEVERITY", 1);
		GMSEC_INFO << "...Then all we have to do is call 'publishLog' with a log string and a severity";
		connManager.publishLog("Self-published log message", severityField);
		connManager.publishLog("Another log, with one function call!", severityField);


		//o Attempt to publish invalid log message
		msg.addField("SEVERITY", "SEVERITY Field in a LOG should be an I16");

		try {
			GMSEC_INFO << "Attempting to manually publish an invalid log message...";
			connManager.publish(msg);
			GMSEC_WARNING << "Ignored improper severity and published anyway";
		}
		catch (Exception& e) {
			GMSEC_INFO << "Invalid log message was NOT published; reason: " << e.what();
		}


		//o Create Product message
		GMSEC_INFO << "Creating PROD message...";
		Message prodMsg("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", Message::PUBLISH);

		GMSEC_INFO << "Setting fields related to PROD message header...";
		connManager.addStandardFields(prodMsg);
		prodMsg.addField("MESSAGE-TYPE", "MSG");
		prodMsg.addField("MESSAGE-SUBTYPE", "PROD");
		if(mist::GMSEC_ISD_CURRENT == mist::GMSEC_ISD_2014_00)
		{
			prodMsg.addField("CONTENT-VERSION", (GMSEC_F32) 2013.0);
		}
		else if(mist::GMSEC_ISD_CURRENT == mist::GMSEC_ISD_2016_00)
		{
			prodMsg.addField("CONTENT-VERSION", (GMSEC_F32) 2016.0);
		}
		prodMsg.addField("RESPONSE-STATUS", (GMSEC_I16) 6);
		prodMsg.addField("PROD-TYPE", "AUTO");
		prodMsg.addField("PROD-SUBTYPE", "DM");


		//o Publish valid PROD message
		GMSEC_INFO << "Publishing PROD message...";
		connManager.publish(prodMsg);


		//o Alter PROD message to make it invalid
		prodMsg.addField("PROD-SUBTYPE", (GMSEC_F32) 45);


		//o Attempt to publish invalid PROD message
		try {
			GMSEC_INFO << "Attempting to publish invalid PROD message...";
			connManager.publish(prodMsg);
			GMSEC_WARNING << "Ignored improper PROD-SUBTYPE and published anyway";
		}
		catch (Exception& e) {
			GMSEC_INFO << "Invalid PROD message was NOT published; reason: " << e.what();
		}

		GMSEC_INFO << "But wait, publishing a product is even easier!";

		mist::ProductFileMessage productFileMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", 
			mist::ResponseStatus::FINAL_MESSAGE, "AUTO", "DM", mist::GMSEC_ISD_CURRENT);

		mist::ProductFile myFile("My Product", "My Product Description", "Version", 
			"TXT", "A path to somewhere");

		productFileMessage.addProductFile(myFile);
		connManager.addStandardFields(productFileMessage);

		try {
			GMSEC_INFO << "Attempting to publish PROD message with ProductFileMessage";
			connManager.publish(productFileMessage);
		}
		catch (Exception& e) {
			GMSEC_INFO << "PROD message was NOT published; reason: " << e.what();
		}

		GMSEC_INFO << "Mnemonic Messages are also easier!";

		mist::MnemonicSample ms1("Timestamp 1", I32Field("Timestamp 1", 15));  // Copy of I32Field made
		ms1.setEUValue(F32Field("My EU", 15.0));
		ms1.setFlags(1);
		ms1.setLimit(mist::MnemonicSample::RED_HIGH);
		//implicitly set limit enable/disable with setting of limit
		ms1.setQuality(true);
		ms1.setStalenessStatus(false);
		ms1.setTextValue("15");
		
		mist::MnemonicSample ms2("Timestamp 2", I32Field("Timestamp 2", 16));  
		mist::MnemonicSample ms3("Timestamp 3", I32Field("Timestamp 3", 17));  
		mist::MnemonicSample ms4("Timestamp 4", I32Field("Timestamp 4", 18));

		DataList<mist::MnemonicSample*> mnemonic_samples;
		mnemonic_samples.add(&ms1);
		mnemonic_samples.add(&ms2);
		mnemonic_samples.add(&ms3);
		mnemonic_samples.add(&ms4);

		mist::Mnemonic m1("MyMnemonic", mnemonic_samples);
		I16Field statusField("My status", 5);
		m1.setStatus(statusField);
		m1.setUnits("MW");

		mist::MnemonicMessage mMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.MVAL.EXAMPLE-PROG", gmsec::api::mist::GMSEC_ISD_CURRENT);
		mMessage.addMnemonic(m1);
		connManager.addStandardFields(mMessage);

		if(mist::GMSEC_ISD_CURRENT == mist::GMSEC_ISD_2014_00)
		{
			//MSG-ID was required in GMSEC messages before the 2016 specification.
			mMessage.addField("MSG-ID", "MYMSG");
		}

		GMSEC_INFO << "MnemonicMessage formats my GMSEC data automatically!";
		GMSEC_INFO << mMessage.toXML();
		connManager.publish(mMessage);

		GMSEC_INFO << "Device Messages have their own subclass, as well";
		

		mist::Device device1("Device 1", mist::Device::RED);
		I32Field paramVal("Param1Val", (GMSEC_I32) 79);
		mist::DeviceParam param("DEV Param 1", "Param 1 Timestamp", paramVal);
		device1.addParam(param);

		mist::Device device2("Device 2", mist::Device::NOMINAL_GREEN);
		I32Field param1Val("Param1Val", (GMSEC_I32) 79);
		mist::DeviceParam param1("DEV Param 1", "Param 1 Timestamp", param1Val);
		StringField param2Val("Param2Val", "79");
		mist::DeviceParam param2("DEV Param 2", "Param 2 Timestamp", param2Val);
		device2.addParam(param1);
		device2.addParam(param2);

		mist::DeviceMessage deviceMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE-PROG.DEV.EXAMPLE-PROG-2", mist::GMSEC_ISD_CURRENT);
		deviceMessage.addDevice(device1);
		deviceMessage.addDevice(device2);

		GMSEC_INFO << "DeviceMessage formats my GMSEC data automatically!";
		GMSEC_INFO << deviceMessage.toXML();

		connManager.addStandardFields(deviceMessage);
		connManager.publish(deviceMessage);

	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	delete [] definedFields;
	delete [] logDefinedFields;

	return success;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmmist_validation");
		return -1;
	}

	gmmist_validation validation(config);

	if (validation.run())
	{
		GMSEC_INFO << "Successful completion!";
	}
	else
	{
		GMSEC_ERROR << "Failure!";
		return -1;
	}
}


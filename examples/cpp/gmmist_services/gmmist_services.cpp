/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmmist_services.cpp
 * 
 * A C++ example demonstration of GMSEC the ConnectionManager class services
 * for logging and heartbeats, as well as general message validation. 
 * 
 */

#include "../example.h"

#include <gmsec4/util/DataList.h>

#include <string>


using namespace gmsec::api;
using namespace gmsec::api::util;


class gmmist_services
{
public:
	gmmist_services(Config& config);
	~gmmist_services();
	bool run();

private:
	Config& config;
};


gmmist_services::gmmist_services(Config& c)
	: config(c)
{
	/* Initialize config */
	example::initialize(c);
}


gmmist_services::~gmmist_services()
{
	Connection::shutdownAllMiddlewares();
}


bool gmmist_services::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		mist::ConnectionManager connManager(config);

		connManager.initialize();

		//o SetStandardFields()
		GMSEC_INFO << "Setting fields which will be applied globally to message from this ConnectionManager";
		GMSEC_INFO << "These fields are usually certain values from the GMSEC Header which apply broadly";

		DataList<Field*> definedFields;

		F32Field    fieldVersion("HEADER-VERSION", (GMSEC_F32) 2010.0);
		StringField fieldMission("MISSION-ID", "GMSEC");
		StringField fieldFacility("FACILITY", "GMSEC Lab");
		StringField fieldComponent("COMPONENT", "gmmist");

		definedFields.push_back(&fieldVersion);
		definedFields.push_back(&fieldMission);
		definedFields.push_back(&fieldFacility);
		definedFields.push_back(&fieldComponent);

		connManager.setStandardFields(definedFields);


		DataList<Field*> logDefinedFields;

		GMSEC_INFO << "No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!";
		GMSEC_INFO << "Adding general log fields";

		StringField fieldSubclass("SUBCLASS", "TAC");
		StringField fieldOccurType("OCCURRENCE-TYPE", "NORM");

		logDefinedFields.push_back(&fieldSubclass);
		logDefinedFields.push_back(&fieldOccurType);

		connManager.setLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);


		I16Field fieldLogSeverity("SEVERITY", (GMSEC_I16) 1);
		GMSEC_INFO << "Sending log message regarding imminent start of heartbeat service";
		connManager.publishLog("About to start heartbeat service", fieldLogSeverity);


		DataList<Field*> hbDefinedFields;

		GMSEC_INFO << "No need to add MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE for a heartbeat, we do that for you!";
		GMSEC_INFO << "Adding general heartbeat fields";

		I16Field    i16FieldPubRate("PUB-RATE", (GMSEC_I16) 1);
		U16Field    u16FieldPubRate("PUB-RATE", (GMSEC_U16) 1);
		I16Field    i16FieldCounter("COUNTER",  (GMSEC_I16) 0);
		U16Field    u16FieldCounter("COUNTER",  (GMSEC_U16) 0);
		StringField fieldMsgId("MSG-ID", "My heartbeat identifier");

		hbDefinedFields.push_back(&fieldMsgId);

		std::string specVersion = example::get(config, "gmsec-specification-version", "201600");

		if (specVersion == "201400")
		{
			GMSEC_INFO << "Using I16Field for PUB-RATE and optional COUNTER as dictated by 201400 ISD";
			hbDefinedFields.push_back(&i16FieldPubRate);
			hbDefinedFields.push_back(&i16FieldCounter);
		}
		else
		{
			GMSEC_INFO << "Using U16Field for PUB-RATE and optional COUNTER as dictated by 201600 and later versions of the ISD";
			hbDefinedFields.push_back(&u16FieldPubRate);
			hbDefinedFields.push_back(&u16FieldCounter);
		}


		GMSEC_INFO << "Starting heartbeat service";
		connManager.startHeartbeatService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.HB", hbDefinedFields);
		GMSEC_INFO << "Heartbeat service has been started. Publish rate is defined by PUB-RATE, defaulting to 30 seconds if not defined. "
		           << "Component status will be reported as nominal unless otherwise instructed. The counter will automatically increment.";
	

		connManager.publishLog("Heartbest service has been started!", fieldLogSeverity);


		GMSEC_INFO << "Time to start up the resource service. This will automatically publish resource information";
		connManager.startResourceMessageService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE.RSRC.EXAMPLE2");

		GMSEC_INFO << "Waiting for 60 seconds to allow heartbeats and resources to publish.";
		GMSEC_INFO << "The user may wish to use gmsub to observe message traffic.";
		TimeUtil::millisleep(60000);

		GMSEC_INFO << "Changing the example component status to RED";
		I16Field compStatus("COMPONENT-STATUS", 4);
		connManager.changeComponentStatus(compStatus);

		GMSEC_INFO << "Wait another 60 seconds to observe the change";
		TimeUtil::millisleep(60000);


		connManager.publishLog("End of demonstration, cleaning up ConnectionManager", fieldLogSeverity);
		GMSEC_INFO << "End of demonstration, cleaning up ConnectionManager";


		connManager.stopHeartbeatService();
		GMSEC_INFO << "Stopping heartbeat service. This will happen automatically when the ConnectionManager cleans up resources, "
		           << "should the user not perform so manually.";
	}
	catch (gmsec::api::Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmmist_services");
		return -1;
	}

	gmmist_services services(config);

	if (services.run())
	{
		GMSEC_INFO << "Successful completion!";
	}
	else
	{
		GMSEC_ERROR << "Failure!";
		return -1;
	}
}


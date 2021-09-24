/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec_cpp.h>
#include "../example.h"
#include <gmsec/util/sysutil.h>

#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gmmist_services {

public:

	gmmist_services(Config &config);
	~gmmist_services();
	Status Run();

private:

	Config &config;

};

gmmist_services::gmmist_services(Config &c) :
		config(c) {

	/* Initialize config */
	initialize(c);

}

gmmist_services::~gmmist_services() {

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmmist_services::Run() {

	Status result;

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	mist::ConnectionManager connManager(config);
	result = connManager.Initialize();
	check("Initialize ConnectionManager", result);

	//o SetStandardFields()
	LOG_INFO << "Setting fields which will be applied globally to message from this ConnectionManager";
	LOG_INFO << "These fields are usually certain values from the GMSEC Header which apply broadly";

	size_t countDefinedFields = 4;
	Field ** definedFields = new Field*[countDefinedFields];

	Field fieldVersion("HEADER-VERSION", (GMSEC_F32) 2010.0);
	definedFields[0] = &fieldVersion;

	Field fieldMission("MISSION-ID", "GMSEC");
	definedFields[1] = &fieldMission;

	Field fieldFacility("FACILITY", "GMSEC Lab");
	definedFields[2] = &fieldFacility;

	Field fieldComponent("COMPONENT", "gmmist");
	definedFields[3] = &fieldComponent;

	result = connManager.SetStandardFields(definedFields, countDefinedFields);
	check("SetStandardFields", result);

	size_t logDefinedFieldCount = 3;
	Field ** logDefinedFields = new Field*[logDefinedFieldCount];

	LOG_INFO << "No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!";

	LOG_INFO << "Adding general log fields";

	Field fieldContentVersion("CONTENT-VERSION", (GMSEC_F32) 1.0);
	logDefinedFields[0] = &fieldContentVersion;

	Field fieldSubclass("SUBCLASS", "TAC");
	logDefinedFields[1] = &fieldSubclass;

	Field fieldOccurType("OCCURRENCE-TYPE", "NORM");
	logDefinedFields[2] = &fieldOccurType;

	result = connManager.SetLoggingDefaults(logDefinedFields, logDefinedFieldCount, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG");
	check("SetLoggingDefaults", result);

	Field fieldLogSeverity("SEVERITY", (GMSEC_I16) 1);
	LOG_INFO << "Sending log message regarding imminent start of heartbeat service";
	result = connManager.QuickLog("About to start heartbeat service", fieldLogSeverity);

	size_t hbDefinedFieldCount = 3;
	Field ** hbDefinedFields = new Field*[hbDefinedFieldCount];

	LOG_INFO << "No need to add MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE for a heartbeat, we do that for you!";

	LOG_INFO << "Adding general heartbeat fields";

	Field hbContentVersion("CONTENT-VERSION", (GMSEC_F32) 2013.0);
	hbDefinedFields[0] = &hbContentVersion;

	Field fieldPubRate("PUB-RATE", (GMSEC_I16) 1);
	hbDefinedFields[1] = &fieldPubRate;

	Field fieldMsgId("MSG-ID", "My heartbeat identifier");
	hbDefinedFields[2] = &fieldMsgId;

	LOG_INFO << "Starting heartbeat service";
	result = connManager.StartHeartbeatService(hbDefinedFields, hbDefinedFieldCount, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.HB");
	check("StartHeartbeatService", result);
	LOG_INFO << "Heartbeat service has been started. Publish rate is defined by PUB-RATE, defaulting to 30 seconds if not defined. Component status will be reported as nominal unless otherwise instructed. The counter will automatically increment.";
	
	result = connManager.QuickLog("Heartbest service has been started!", fieldLogSeverity);
	check("QuickLog", result);
	
	LOG_INFO << "About to deliberately corrupt the heartbeat service with an invalid type of parameter. If validation is enabled, this will fail";
	Field fieldInvalidCpuUtil("CONTENT-VERSION", "This value should be a F32 instead of a string!");
	result = connManager.ChangeCPUUtil(fieldInvalidCpuUtil);
	if(result.IsError()){
		fieldLogSeverity.SetValue((GMSEC_I16) 4);
		result = connManager.QuickLog("Error detected when trying to set an invalid CPU utilization", fieldLogSeverity);
		LOG_ERROR << "Error detected when trying to set an invalid CPU utilization";
	}else{
		LOG_INFO << "No error detected in changing CPU Utilization in heartbeat";
		result = connManager.QuickLog("No error detected in changing CPU Utilization in heartbeat", fieldLogSeverity);
	}
	check("QuickLog", result);

	LOG_INFO << "Waiting for 5 seconds to allow heartbeats to publish";
	millisleep(5000);

	fieldLogSeverity.SetValue((GMSEC_I16) 1);
	result = connManager.QuickLog("End of demonstration, cleaning up ConnectionManager", fieldLogSeverity);
	LOG_INFO << "End of demonstration, cleaning up ConnectionManager";
	check("QuickLog", result);

	result = connManager.StopHeartbeatService();
	check("StopHeartbeatService", result);
	LOG_INFO << "Stopping heartbeat service. This will happen automatically when the ConnectionManager cleans up resources, if the user does not instruct it to do so manually.";

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmmist_services");
		return -1;
	}


	try {

		gmmist_services state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}


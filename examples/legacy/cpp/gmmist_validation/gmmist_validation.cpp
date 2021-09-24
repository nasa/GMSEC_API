/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec_cpp.h>
#include "../example.h"
#include <gmsec/util/sysutil.h>

#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gmmist_validation {

public:

	gmmist_validation(Config &config);
	~gmmist_validation();
	Status Run();

private:

	Config &config;

};

gmmist_validation::gmmist_validation(Config &c) :
		config(c) {

	/* Initialize config */
	initialize(c);

}

gmmist_validation::~gmmist_validation() {

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmmist_validation::Run() {

	Status result;

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	mist::ConnectionManager connManager(config, 201400, true);
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

	Field fieldComponent("COMPONENT", "gmmist-demo");
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

	Message *msg = NULL;
	result = connManager.GetLogMessage(logDefinedFields, logDefinedFieldCount, msg, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG");
	check("GetLogMessage", result);

	Field fieldLogSeverity("SEVERITY", (GMSEC_I16) 1);
	msg->AddField(fieldLogSeverity);

	Field fieldText("MSG-TEXT", "Self-published log message");
	msg->AddField(fieldText);

	result = connManager.Publish(msg);
	if(result.IsError()){
		LOG_ERROR << "Error attempting to publish with valid log smessage";
		return result;
	}else{
		LOG_INFO << "Manually published log message";
	}

	fieldLogSeverity.SetValue("SEVERITY Field in a LOG should be an I16");
	msg->AddField(fieldLogSeverity);
	result = connManager.Publish(msg);
	if(result.IsError()){
		LOG_ERROR << "If validation is active, this should fail due to the improper SEVERITY";
	}else{
		LOG_WARNING << "Ignored improper severity and published anyway";
	}

	Message * prodMsg = NULL;
	result = connManager.CreateMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", GMSEC_MSG_PUBLISH, prodMsg);
	check("CreateMessage", result);

	LOG_INFO << "Fields related to PROD message header";

	Field fieldType("MESSAGE-TYPE", "MSG");
	result = prodMsg->AddField(fieldType);
	check("AddField", result);

	Field fieldSubtype("MESSAGE-SUBTYPE", "PROD");
	result = prodMsg->AddField(fieldSubtype);
	check("AddField", result);

	fieldContentVersion.SetName("CONTENT-VERSION");
	fieldContentVersion.SetValue((GMSEC_F32) 2013.0);
	result = prodMsg->AddField(fieldContentVersion);
	check("AddField", result);

	Field fieldResponseStatus("RESPONSE-STATUS", (GMSEC_I16) 6);
	result = prodMsg->AddField(fieldResponseStatus);
	check("AddField", result);

	Field fieldProdName("PROD-TYPE", "AUTO");
	result = prodMsg->AddField(fieldProdName);
	check("AddField", result);

	Field fieldProdSubType("PROD-SUBTYPE", "DM");
	result = prodMsg->AddField(fieldProdSubType);
	check("AddField", result);

	result = connManager.Publish(prodMsg);
	if(result.IsError()){
		LOG_ERROR << "Error attempting to publish with valid prod message";
		return result;
	}else{
		LOG_INFO << "Manually published product message";
	}

	LOG_INFO << "Publishing a manually corrupted product message";
	fieldProdSubType.SetValue((GMSEC_F32) 45);
	result = prodMsg->AddField(fieldProdSubType);
	check("AddField", result);
	LOG_INFO << "Publishing a manually corrupted product message";

	result = connManager.Publish(prodMsg);
	if(result.IsError()){
		LOG_ERROR << "If validation is active, this should fail due to the improper PROD-SUBTYPE";
	}else{
		LOG_WARNING << "Ignored improper PROD-SUBTYPE and published anyway";
	}

	result = connManager.DestroyMessage(msg);
	check("DestroyMessage", result);
	result = connManager.DestroyMessage(prodMsg);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmmist_validation");
		return -1;
	}


	try {

		gmmist_validation state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}


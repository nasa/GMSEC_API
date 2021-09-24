/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** @file Requestor.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request
 * reply functionality using callbacks. This works in sync
 * with Replier.cpp.
 *  
 */

#include "gmsec_cpp.h" 
#include "../example.h"
#include <iostream>	
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <gmsec/util/sysutil.h>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;

ConfigFile *cfile = NULL;

class RequestReplyCallback: public Callback {

public:

	virtual void CALL_TYPE OnMessage(Connection * connection, Message * message) {

		Status result;
		Field tempField;
		GMSEC_MSG_KIND msgKind;
		GMSEC_I16 tempStatus = 0;
		Message* logMessage = NULL;
		GMSEC_STR tempString = NULL;
		std::string stringHolder;

		/* We don't care about anything but reply messages */
		message->GetKind(msgKind);
		if (msgKind != GMSEC_MSG_REPLY)
			return;

		/* Get the status of the response (successful, failed, ect) and act accordingly */
		result = message->GetField("RESPONSE-STATUS", tempField);
		if (!checkNoThrow("GetField(RESPONSE-STATUS)", result))
			return;
		result = tempField.GetValue(tempStatus);
		if (!checkNoThrow("GetValue(RESPONSE-STATUS)", result))
			return;

		switch (tempStatus) {

			case 1 :
			LOG_INFO << "\nAcknowledgement";
			break;
			case 2 :
			LOG_INFO << "\nWorking/keep alive";
			break;
			case 3 :
			/* Obtain the DATA field which was filled with what request this reply was generated from */
			result = message->GetField("DATA", tempField);
			if (!checkNoThrow("GetField(DATA)", result))
				return;

			result = tempField.GetValue(tempString);
			if (!checkNoThrow("GetValue(DATA)", result))
				return;
			if (tempString) {
				stringHolder = tempString;
				/* Output that we've received a reply */
				LOG_INFO << "\nReceived Reply (DATA=" << tempString << "): " << gmsec::Message::GetTime();
			}
			break;
			case 4 :
			LOG_INFO << "\nFailed completion";
			break;
			case 5 :
			LOG_INFO << "\nInvalid Request";
			break;
			default :
			break;
		}

		/* Create a generic message container for the log message */
		result = connection->CreateMessage(logMessage);
		if (!checkNoThrow("CreateMessage", result))
			return;

		/* Find and load the config file log message definition */
		result = cfile->LookupMessage("LOG-REQ", logMessage);
		if (!checkNoThrow("LookupMessage", result))
			return;

		/* Fill the log message time */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("EVENT-TIME");
		tempField.SetValue(gmsec::Message::GetTime());
		result = logMessage->AddField(tempField);
		if (!check("AddField(EVENT-TIME)", result))
			return;

		/* Fill the log message message text */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("MSG-TEXT");
		tempField.SetValue(stringHolder.c_str());
		result = logMessage->AddField(tempField);
		if (!checkNoThrow("AddField(MSG-TEXT)", result))
			return;

		/* Publish the log message */
		result = connection->Publish(logMessage);
		if (!checkNoThrow("Publish", result))
			return;

		/* Clean up */
		connection->DestroyMessage(logMessage);
	}

};

class Requestor {

public:

	Requestor(ConfigFile * c);
	~Requestor();

	Status Run();

private:

	Connection * connection;

};

Requestor::Requestor(ConfigFile * c) :
		connection(0) {

	cfile = c;
	Config tmp;
	initialize(tmp);

}

Requestor::~Requestor() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status Requestor::Run() {

	/* Variable declarations */
	Status result;
	Config connectionConfig;
	Config programConfig;
	char *configValue = NULL;
	Field tempField;
	Message *heartbeatMessage = NULL;
	Message *directiveRequestMessage = NULL;
	GMSEC_I16 requestCount = 0;
	GMSEC_I16 heartbeatCount = 0;

	/* Config variables */
	GMSEC_I16 heartbeatHolder = 0;
	GMSEC_I16 heartbeatCountdown = 0;
	GMSEC_I16 directiveHolder = 0;
	GMSEC_I16 directiveCountdown = 0;
	GMSEC_I16 loopCountdown = 0;
	GMSEC_I16 updateRate = 0;

	/* Subscription callback */
	RequestReplyCallback cb;

	/* Load the config file */
	result = cfile->Load();
	check("ConfigFileLoad", result);

	/* Look up the config file value that contains the configuration for the middleware */
	result = cfile->LookupConfig("connection-config", &connectionConfig);
	check("LookupConfig", result);

	/* Create a connection object off of the configuration */
	result = ConnectionFactory::Create(&connectionConfig, connection);
	check("Creating Connection", result);

	/* Connect to the network */
	result = connection->Connect();
	check("Connect", result);

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Output middleware version */
	LOG_INFO << "Middleware version = " << connection->GetLibraryVersion();

	/* Look up additional program configuration information */
	result = cfile->LookupConfig("program-config", &programConfig);
	check("LookupConfig", result);

	/* Set program config values */
	result = programConfig.GetValue((const char *) ("update-rate"),
			(const char *&) (configValue));
	check("GetValue", result);
	updateRate = atoi(configValue);

	result = programConfig.GetValue((const char*) ("loop-time"),
			(const char*&) (configValue));
	check("GetValue", result);
	loopCountdown = atoi(configValue);

	result = programConfig.GetValue((const char*) ("send-directive-rate"),
			(const char *&) (configValue));
	check("GetValue", result);
	directiveHolder = atoi(configValue);

	/* Create a generic message container for the heartbeat message */
	result = connection->CreateMessage(heartbeatMessage);
	check("CreateMessage", result);

	/* Find and load the config file heartbeat message definition */
	result = cfile->LookupMessage("C2CX-HEARTBEAT-REQ", heartbeatMessage);
	check("LookupMessage", result);

	/* Obtain the publish rate field, by name, from the heartbeat message */
	result = heartbeatMessage->GetField("PUB-RATE", tempField);
	check("GetField", result);

	result = tempField.GetValue(heartbeatHolder);
	check("GetValue", result);

	/* Create a generic message container for the Directive Request Message */
	result = connection->CreateMessage(directiveRequestMessage);
	check("CreateMessage", result);

	/* Find and load the config file Directive Request Message definition */
	result = cfile->LookupMessage("DIRECTIVE-REQUEST", directiveRequestMessage);
	check("LookupMessage", result);

	/* Output some general program information */
	LOG_INFO << "Publishing for " << loopCountdown << " seconds.";
	LOG_INFO << "Publishing Directive Request Messages every "
			<< directiveHolder << " seconds.";
	LOG_INFO << "Publishing Heartbeat Messages every " << heartbeatHolder
			<< " seconds.";

	/* Start time */
	LOG_INFO << "Start Time: " << gmsec::Message::GetTime();

	/* Publishing loop */
	for (; loopCountdown > 0; loopCountdown--) {
		/* When the countdown reaches 0, we publish a directive message and
		 reset the counter */

		std::ostringstream oss;

		if (directiveCountdown < 1) {

			requestCount++;

			/* Alter the directive request to a unique string */
			oss << requestCount;
			tempField.SetType(GMSEC_TYPE_STRING);
			tempField.SetName("DIRECTIVE-KEYWORD");
			tempField.SetValue(oss.str().c_str());
			result = directiveRequestMessage->AddField(tempField);
			check("AddField", result);

			/* Alter the directive request to a unique string */
			oss.str("");
			oss << "DIRECTIVE REQUEST MESSAGE " << requestCount;
			tempField.SetType(GMSEC_TYPE_STRING);
			tempField.SetName("DIRECTIVE-STRING");
			tempField.SetValue(oss.str().c_str());
			result = directiveRequestMessage->AddField(tempField);
			check("AddField", result);

			/* Publish time */
			LOG_INFO << "\n\nPublishing Directive Request Message ("
			         << requestCount << "): " << gmsec::Message::GetTime();

			/* Send the request with the reply being send to the callback we created */
			result = connection->Request(directiveRequestMessage, GMSEC_WAIT_FOREVER, &cb);
			check("Requesting", result);

			/* Reset the countdown */
			directiveCountdown = directiveHolder;
		}

		/* When the countdown reaches 0, we publish a heartbeat message and
		 reset the counter */
		if (heartbeatCountdown < 1) {

			heartbeatCount++;

			/* Update the message counter field */
			tempField.SetType(GMSEC_TYPE_STRING);
			tempField.SetName("COUNTER");
			tempField.SetValue(heartbeatCount);
			result = heartbeatMessage->AddField(tempField);
			check("AddField", result);

			/* Publish the heartbeat message*/
			result = connection->Publish(heartbeatMessage);
			check("Publish", result);

			/* Ouput a heartbeat marker and reset the counter */
			LOG_INFO << "Published Heartbeat";
			heartbeatCountdown = heartbeatHolder;

		}

		/* Decrement the counters */
		heartbeatCountdown -= updateRate;
		directiveCountdown -= updateRate;

		/* Sleep for updateRate seconds */
		millisleep(updateRate * 1000);

	}

	/* End time */
	LOG_INFO << "\n\nEnd Time: " << gmsec::Message::GetTime();

	result = connection->DestroyMessage(heartbeatMessage);
	check("DestroyMessage", result);

	result = connection->DestroyMessage(directiveRequestMessage);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char *argv[]) {

	if (argc <= 1) {

		cout << "usage: Requestor <filename>" << endl;
		return -1;

	}

	ConfigFile * cf = new ConfigFile(argv[1]);

	try {

		Requestor state(cf);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	/* Delete the config file object */
	if (cfile != NULL)
		delete cfile;

	return 0;

}

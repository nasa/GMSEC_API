/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** @file Recorder.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and publishing.
 *  
 */

#include "gmsec_cpp.h"
#include "../example.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;

GMSEC_I16 globalHeartbeatCount = 0;

class LogCallback: public Callback {

public:

	virtual void CALL_TYPE OnMessage(Connection *connection, Message *message) {

		Status result;
		const char * xml;
		result = message->ToXML(xml);
		if (checkNoThrow("ToXML", result)) {
			LOG_INFO << xml;
		}
	}

};

class Recorder {

public:

	Recorder(ConfigFile &c);
	~Recorder();

	Status Run();

private:

	ConfigFile &cfile;
	Connection * connection;

};

Recorder::Recorder(ConfigFile &c) :
		cfile(c), connection(0) {

	Config tmp;
	initialize(tmp);

}

Recorder::~Recorder() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status Recorder::Run() {

	Status result;
	Config connectionConfig;
	Config programConfig;
	Message * heartbeatMessage = 0;
	Field tempField;
	const char * configValue = NULL;
	const char * tempSubject = NULL;

	/* Config variables */
	GMSEC_I16 heartbeatHolder = 0;
	GMSEC_I16 heartbeatCountdown = 0;
	GMSEC_I16 loopCountdown = 0;
	GMSEC_I16 updateRate = 0;

	/* Subscription callback */
	LogCallback cb;

	/* Load the config file */
	result = cfile.Load();
	check("ConfigFileLoad", result);

	/* Look up the config file value that contains the configuration for the middleware */
	result = cfile.LookupConfig("connection-config", &connectionConfig);
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
	result = cfile.LookupConfig("program-config", &programConfig);
	check("LookupConfig", result);

	/* Set program config values */
	result = programConfig.GetValue("update-rate", configValue);
	check("GetValue", result);
	updateRate = atoi(configValue);

	result = programConfig.GetValue("loop-time", configValue);
	check("GetValue", result);
	loopCountdown = atoi(configValue);

	/* Create subscriptions from subscription templates in the config file using callbacks */
	result = cfile.LookupSubscription("RECEIVE-LOG", tempSubject);
	check("LookupSubscription", result);

	result = connection->Subscribe(tempSubject, &cb);
	check("Subscribe", result);

	result = cfile.LookupSubscription("SEND-LOG", tempSubject);
	check("LookupSubscription", result);

	result = connection->Subscribe(tempSubject, &cb);
	check("Subscribe", result);

	/* Create a generic message container for the heartbeat message */
	result = connection->CreateMessage(heartbeatMessage);
	check("CreateMessage", result);

	/* Find and load the config file heartbeat message definition */
	result = cfile.LookupMessage("C2CX-HEARTBEAT-REC", heartbeatMessage);
	check("LookupMessage", result);

	/* Obtain the publish rate field, by name, from the heartbeat message */
	result = heartbeatMessage->GetField("PUB-RATE", tempField);
	if (!(result.isError())) {

		result = tempField.GetValue(heartbeatHolder);
		check("GetValue", result);

	} else {

		// Default value
		heartbeatHolder = 30;

	}

	/* Output some general program information */
	LOG_INFO << "Publishing for " << loopCountdown << " seconds.";
	LOG_INFO << "Publishing Heartbeat Messages every " << heartbeatHolder
			<< " seconds.";

	/* Call to create the thread that will automatically retrieve messages off the message queue */
	result = connection->StartAutoDispatch();
	check("Starting AutoDispatch", result);

	/* Start time */
	LOG_INFO << "Start Time: " << gmsec::Message::GetTime();

	/* Publishing loop */
	for (; loopCountdown > 0; loopCountdown--) {

		/* When the countdown reaches 0, we publish a heartbeat message and
		 reset the counter */
		if (heartbeatCountdown < 1) {

			globalHeartbeatCount++;

			/* Update the message counter */
			tempField.SetType(GMSEC_TYPE_I16);
			tempField.SetName("COUNTER");
			tempField.SetValue(globalHeartbeatCount);
			result = heartbeatMessage->AddField(tempField);
			check("AddField", result);

			/* Publish the heartbeat message */
			result = connection->Publish(heartbeatMessage);
			check("Publish", result);

			/* Ouput a heartbeat marker and reset the counter */
			LOG_INFO << "Published Heartbeat";
			heartbeatCountdown = heartbeatHolder;
		}

		/* Decrement the counters */
		heartbeatCountdown -= updateRate;

		/* Sleep for 1 second */
		millisleep(updateRate * 1000);

	}

	/* End time */
	LOG_INFO << "\n\nEnd Time: " << gmsec::Message::GetTime();

	result = connection->StopAutoDispatch();
	check("StopAutoDispatch", result);

	result = connection->DestroyMessage(heartbeatMessage);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char *argv[]) {

	if (argc <= 1) {

		cout << "usage: Recorder <filename>" << endl;
		return -1;

	}

	ConfigFile cf(argv[1]);

	try {

		Recorder state(cf);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

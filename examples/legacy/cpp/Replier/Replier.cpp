/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** @file Replier.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request
 * reply functionality using callbacks.  This works in sync
 * with Requestor.cpp.
 *  
 */

#include "gmsec_cpp.h"  
#include "../example.h"
#include <iostream>	
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;

ConfigFile * cfile;

class RequestReplyCallback: public Callback {

public:

	virtual void CALL_TYPE OnMessage(Connection * connection, Message * message) {
		try {
			OnMessageAux(connection, message);
		}
		catch (exception &e) {
			LOG_WARNING << "[RequestReplyCallback::OnMessage] " << e.what();
		}
	}

	void OnMessageAux(Connection * connection, Message * message) {
		Status result;
		Field tempField;
		Message *replyMessage = 0;
		Message *logMessage = 0;
		GMSEC_STR subject = 0;
		GMSEC_MSG_KIND msgKind;
		GMSEC_STR tempString = 0;
		std::string stringHolder;

		/* We don't care about anything but request messages */
		message->GetKind(msgKind);
		if (msgKind != GMSEC_MSG_REQUEST)
			return;

		/* Obtain the DIRECTIVE-KEYWORD field from the message.  It holds the request count. */
		message->GetField("DIRECTIVE-KEYWORD", tempField);
		tempField.GetValue(tempString);

		/* Copy and output the request number which has been placed in the DIRECTIVE-KEYWORD field */
		stringHolder = tempString;
		LOG_INFO << "\n\nReceived Request (" << stringHolder << "): " << gmsec::Message::GetTime();

		/* Create a generic message container for the directive response message */
		result = connection->CreateMessage(replyMessage);
		check("CreateMessage", result);

		/* Find and load the config file directive response message definition */
		result = cfile->LookupMessage("DIRECTIVE-RESPONSE", replyMessage);
		check("LookupMessage", result);

		/* Replace the DATA field with the request number we are responding to */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("DATA");
		tempField.SetValue(stringHolder.c_str());
		result = replyMessage->AddField(tempField);
		check("AddField", result);

		/* Replace the TIME-COMPLETE field with the correct time */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("TIME-COMPLETED");
		tempField.SetValue(Message::GetTime());
		result = replyMessage->AddField(tempField);
		check("AddField", result);

		/* Set the reply subject to the request message's subject */
		message->GetSubject(subject);
		replyMessage->SetSubject(subject);

		/* Send the reply */
		result = connection->Reply(message,replyMessage);
		check("Replying", result);

		LOG_INFO << "\nReply Sent (" << stringHolder << "): " << Message::GetTime();

		/* Create a generic message container for the log message */
		result = connection->CreateMessage(logMessage);
		check("CreateMessage", result);

		/* Find and load the config file log message definition */
		result = cfile->LookupMessage("LOG-REP", logMessage);
		check("LookupMessage", result);

		/* Fill the log message time */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("EVENT-TIME");
		tempField.SetValue(Message::GetTime());
		result = logMessage->AddField(tempField);
		check("AddField", result);

		/* Fill the log message message text */
		tempField.SetType(GMSEC_TYPE_STRING);
		tempField.SetName("MSG-TEXT");
		tempField.SetValue(stringHolder.c_str());
		result = logMessage->AddField(tempField);
		check("AddField", result);

		// Publish the log message
		result = connection->Publish(logMessage);
		check("Publish", result);

		/* Clean up */
		connection->DestroyMessage(logMessage);
		connection->DestroyMessage(replyMessage);
	}

};

class Replier {

public:

	Replier(ConfigFile * c);
	~Replier();

	Status Run();

private:

	Connection * connection;

};

Replier::Replier(ConfigFile * c) :
		connection(0) {

	cfile = c;
	Config tmp;
	initialize(tmp);

}

Replier::~Replier() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status Replier::Run() {

	Status result;
	Config connectionConfig;
	Config programConfig;
	Message * heartbeatMessage = 0;
	Field tempField;
	GMSEC_STR tempString = NULL;

	GMSEC_I16 heartbeatCount = 0;

	/* Config variables */
	GMSEC_I16 heartbeatHolder = 0;
	GMSEC_I16 heartbeatCountdown = 0;
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
	updateRate = get(programConfig, "update-rate", 1);
	loopCountdown = get(programConfig, "loop-time", 30);

	/* Create subscriptions from subscription templates in the config file using callbacks */
	result = cfile->LookupSubscription("DIRECTIVE-REQUEST", tempString);
	check("LookupSubscription", result);

	result = connection->Subscribe(tempString, &cb);
	check("Subscribe", result);

	/* Create a generic message container for the heartbeat message */
	result = connection->CreateMessage(heartbeatMessage);
	check("CreateMessage", result);

	/* Find and load the config file heartbeat message definition */
	result = cfile->LookupMessage("C2CX-HEARTBEAT-REP", heartbeatMessage);
	check("LookupMessage", result);

	/* Obtain the publish rate field, by name, from the heartbeat message */
	result = heartbeatMessage->GetField("PUB-RATE", tempField);
	check("GetField", result);

	result = tempField.GetValue(heartbeatHolder);
	check("GetValue", result);

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

			heartbeatCount++;

			/* Update the message counter */
			tempField.SetType(GMSEC_TYPE_I16);
			tempField.SetName("COUNTER");
			tempField.SetValue(heartbeatCount);
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
	LOG_INFO << "\n\nEnd Time: " << Message::GetTime();

	result = connection->StopAutoDispatch();
	check("StopAutoDispatch", result);

	result = connection->DestroyMessage(heartbeatMessage);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char *argv[]) {

	if (argc <= 1) {

		cout << "usage: Replier <filename>" << endl;
		return -1;

	}

	ConfigFile * cf = new ConfigFile(argv[1]);

	try {

		Replier state(cf);
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

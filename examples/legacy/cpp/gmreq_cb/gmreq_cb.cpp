/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmreq_cb.cpp
 * 
 * Another C++ example demonstrating the reply/request abilities of 
 * GMSEC.  This example involves the use of Callback functionality,
 * which allows for code to be performed anytime a Message is 
 * received.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"

#include <string>

using namespace std;
using namespace gmsec;
using namespace example;

int gReplyCallbackFlag = 0;


class MyReplyCallback: public ReplyCallback {

public:

	virtual void CALL_TYPE OnReply(Connection * connection, Message * request, Message * reply) {

		Status result;

		/* Dump Reply */
		const char * xml = 0;
		result = reply->ToXML(xml);
		if (checkNoThrow("reply.ToXML", result) && xml)
		{
			LOG_INFO << "[MyReplyCallback::OnReply]\n" << xml;
		}

		gReplyCallbackFlag = 1;
	};

	virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *result, const char * text) {

		if (!text) {
			text = "(void)";
		}

		if (result->isError()) {
			LOG_WARNING << "[MyReplyCallback::OnError] " << result->Get() << " text=" << text;
		}
	};

};

class gmreq_cb {

public:

	gmreq_cb(Config &config);
	~gmreq_cb();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * request;

	void Request(int iteration, string subject);

};

gmreq_cb::gmreq_cb(Config &c) :
		config(c), connection(0) {

	/* Initialize Config */
	initialize(c);

}

gmreq_cb::~gmreq_cb() {

	if (request)
		checkNoThrow("Cleaning up Request", connection->DestroyMessage(request));

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmreq_cb::Run() {

	Status result;

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	/* Get information from the command line */
	string subject = get(config, "SUBJECT",
			"GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);

	/* output connection middleware version */
	LOG_INFO << connection->GetLibraryVersion();

	/* Output information */
	LOG_INFO << "Using subject '" << subject << "'";

	Request(msg_timeout_ms, subject);

	return result;

}

void gmreq_cb::Request(int msg_timeout_ms, string subject) {

	Status result;
	Field fd;

	result = connection->CreateMessage(subject.c_str(), GMSEC_MSG_REQUEST,
			request);
	check("CreateMessage", result);

	/* Add Fields */
	fd.SetName("QUESTION");
	fd.SetValue("Does the request/reply functionality still work?");
	result = request->AddField(fd);
	check("AddField(QUESTION)", result);

	/* Add the name of the component making the Request. */
	fd.SetName("COMPONENT");
	fd.SetValue("GMREQ");
	result = request->AddField(fd);
	check("AddField(COMPONENT)", result);

	/* Add the Message ID of this Request.  In theory the MSG-ID should be unique,
	 but here it is not for the sake of simplicity.  See API Interface Specification
	 for suggetions on creating a MSG-ID. */
	fd.SetName("MSG-ID");
	fd.SetValue("GMREQ-MSG");
	result = request->AddField(fd);
	check("AddField(MSG-ID)", result);

	if (config.CheckBoolValue("ENCRYPT", false)) {
		fd.SetName("SEC-ENCRYPT");
		fd.SetValue(GMSEC_TRUE);
		result = request->AddField(fd);
		check("AddField(SEC-ENCRYPT)", result);
	}

	/* Dump Request */
	const char * reqxml = 0;
	result = request->ToXML(reqxml);
	check("ToXML", result);
	LOG_INFO << "Requesting:\n" << reqxml;

	MyReplyCallback cb;
	result = connection->Request(request, msg_timeout_ms, &cb);
	check("Request", result);

	int deltaTime = 0;
	LOG_INFO << "Waiting for response";
	while(gReplyCallbackFlag == 0)
	{
		example_millisleep(100);
		if(msg_timeout_ms != GMSEC_WAIT_FOREVER)
		{
			deltaTime +=100;
			if(deltaTime > msg_timeout_ms)
			{
				break;
			}
		}//else wait forever
	}

	if (gReplyCallbackFlag)
	{
		LOG_INFO << "Response Received!";
	}
	else
	{
		LOG_INFO << "No response received";
	}
}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmreq_cb");
		return -1;
	}

	try {

		gmreq_cb state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmsub_disp_rr.cpp
 *
 * This file contains an example subscriber using the GMSEC standard C++ API dispatcher.
 * The callback is registered with the Connection.  A separate thread, transmits a request
 * and waits for a reply.  The Dispatcher is then started, which launches in its own
 * thread.  We're looking for contention issues with the message queue, you should be
 * able to receive messages while still waiting for a reply.
 */

#include <gmsec_cpp.h>
#include <gmsec/util/sysutil.h>
#include "../example.h"

#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class PublishCallback: public Callback {

public:

	virtual void CALL_TYPE OnMessage(Connection * connection, Message * message) {

		Status result;

		const char * xml = 0;
		result = message->ToXML(xml);
		if (checkNoThrow("ToXML", result)) {
			if (xml) {
				LOG_INFO << "received\n" << xml;
			}
		}

	//	LOG_INFO << "Press the enter key to exit";

	}

};

class RequestCallback: public Callback, public ErrorCallback {

public:
	virtual void CALL_TYPE OnMessage(Connection * connection, Message * message) {

		Status result;

		/* Dump Reply */
		const char * xml = 0;
		result = message->ToXML(xml);
		if (checkNoThrow("[RequestCallback::OnMessage] ToXML", result)) {
			if (xml) {
				LOG_INFO << "Replying:\n" << xml;
			}
		}

	};

	virtual void CALL_TYPE OnError(Connection * connection, Message * message, Status * result, const char * event) {

		if (!event)
			event = "(null)";

		LOG_WARNING << "[RequestCallback::OnError] Status=" << result->Get()
				<< " event=" << event;

	};

};

class gmsub_disp_rr {

public:

	gmsub_disp_rr(Config &config);
	~gmsub_disp_rr();

	Status Run();

private:

	Config &config;
	Connection * connection;
	vector<string> subjects;

};

gmsub_disp_rr::gmsub_disp_rr(Config &c) :
		config(c), connection(0), subjects(0) {

	/* Initialize config */
	initialize(c);

}

gmsub_disp_rr::~gmsub_disp_rr() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmsub_disp_rr::Run() {

	Status result;

	result = config.AddValue("isThreaded", "true");
	check("AddValue", result);

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	/* Determine the subjects to listen to */
	determineSubjects(config, subjects);

	/* Output the middleware information */
	LOG_INFO << "Middleware version = " << connection->GetLibraryVersion();

	/* Subscribe */
	PublishCallback cb;

	/* Subscribe */
	for (int i = 0; i < (int) subjects.size(); i++) {

		LOG_INFO << "Subscribing to " << subjects[i];
		result = connection->Subscribe(subjects[i].c_str(), &cb);
		check("Subscribe", result);

	}

	LOG_INFO << "Starting AutoDispatch";
	result = connection->StartAutoDispatch();
	check("StartAutoDispatch", result);

	RequestCallback rcb;
	Message * reqMsg;

	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
	int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

	string subject = "GMSEC.TEST.PUBLISH.NOREPLY";
	if(subjects[0] != "GMSEC.>")
	{
		subject = subjects[0];
	}

	LOG_INFO << "Creating a request message";
	result = connection->CreateMessage(subject.c_str(), GMSEC_MSG_REQUEST,
			reqMsg);
	check("Creating Request Message", result);

	LOG_INFO << "Registering error callback";
	result = connection->RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT",
			&rcb);
	check("RegisterErrorCallback", result);

	LOG_INFO << "Sending the request";
	result = connection->Request(reqMsg, msg_timeout_ms, &rcb);
	check("Request", result);
	
	if(prog_timeout_s == GMSEC_WAIT_FOREVER)
	{
		LOG_INFO << "Press the enter key to exit";
		cin.get();
	}
	else{
		example_millisleep(prog_timeout_s * 1000);
		LOG_INFO << "Program Timeout Reached!";
	}

	LOG_INFO << "Stopping AutoDispatch";
	result = connection->StopAutoDispatch();
	check("StopAutoDispatch", result);

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

        addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmsub_disp_rr");
		return -1;
	}

	try {

		gmsub_disp_rr state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

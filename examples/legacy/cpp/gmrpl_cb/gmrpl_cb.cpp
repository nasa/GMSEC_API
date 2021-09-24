/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmrpl_cb.cpp
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
#include <stdio.h>

using namespace std;
using namespace gmsec;
using namespace example;


class RequestCallback: public Callback {

public:

	virtual void CALL_TYPE OnMessage(Connection * connection, Message *message) {
		try {
			OnMessageAux(connection, message);
		}
		catch (exception &e) {
			LOG_WARNING << "[RequestCallback::OnMessage] " << e.what();
		}
	}


	void OnMessageAux (Connection *connection, Message *message) {

		Status result;
		Message * reply = 0;

		/* Dump the message */
		const char *tmp;
		result = message->ToXML(tmp);
		check("ToXML", result);

		LOG_INFO << "Received\n" << tmp;

		// Construct Reply subject.
		Field msg_fld;
		message->GetField("COMPONENT", msg_fld);
		GMSEC_STR comp_name;
		msg_fld.GetValue(comp_name);

		Field id_fld;
		message->GetField("MSG-ID", id_fld);
		GMSEC_STR msg_id;
		id_fld.GetValue(msg_id);

		/* Set Status Code.  See API Interface Specification for available status codes. */
		GMSEC_STR status_code = "1"; // 1 = Acknowledgement

		/* Create Reply */
		std::ostringstream reply_subject;

		reply_subject << "GMSEC.MISSION.SAT_ID.RESP." << comp_name << "." << msg_id << "." << status_code;

		result = connection->CreateMessage(reply_subject.str().c_str(), GMSEC_MSG_REPLY, reply);
		check("CreateMessage", result);

		/* Add Fields */
		Field fd;

		// Add the name of the component sending the Reply.
		fd.SetName("COMPONENT");
		fd.SetValue("GMRPL");
		result = reply->AddField(fd);
		check("AddField(COMPONENT)", result);

		fd.SetName("ANSWER");
		fd.SetValue("Sure looks like it!");
		result = reply->AddField(fd);
		check("AddField(ANSWER)", result);

		/* Dump Reply */
		result = reply->ToXML(tmp);
		check("ToXML", result);
		LOG_INFO << "Prepared Reply:\n" << tmp;

		/* Send Reply */
		result = connection->Reply(message,reply);
		check("Reply Sent", result);

	};

};

class gmrpl_cb {

public:

	gmrpl_cb(Config &config);
	~gmrpl_cb();

	bool Run();

private:

	Config &config;
	Connection * connection;
	vector<string> subjects;

};

gmrpl_cb::gmrpl_cb(Config &c) :
		config(c), connection(0) {

	/* Initialize config */
	initialize(c);

}

gmrpl_cb::~gmrpl_cb() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

bool gmrpl_cb::Run() {

	Status result;
	Message * message = 0;

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connecting", result);

	/* output connection middleware version */
	LOG_INFO << connection->GetLibraryVersion();

	/* Get info from command line */
	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
	int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

	/* Determine the subjects to listen to */
	determineSubjects(config, subjects);

	/* Subscribe */
	RequestCallback cb;
	for (int i = 0; i < (int) subjects.size(); i++) {

		LOG_INFO << "Subscribing to " << subjects[i];
		result = connection->Subscribe(subjects[i].c_str(), &cb);
		check("Subscribe", result);

	}

	bool done = true;
	time_t prevTime;
	time_t nextTime;
	double elapsedTime = 0;
	time(&prevTime);

	while (done) {

		if(prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			LOG_INFO << "Program Timeout Reached!";
			return true;
		}

		/* read next */
		result = connection->GetNextMsg(message, msg_timeout_ms);

		if(prog_timeout_s != GMSEC_WAIT_FOREVER)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (result.isError()) {

			// do not abort on timeouts
			if (result.GetCode() == GMSEC_TIMEOUT_OCCURRED) {
				LOG_INFO << "timeout occurred";
				continue;
			}

			check("GetNextMsg", result);
		}

		/* Dump the message */
		const char *tmp;
		result = message->ToXML(tmp);
		check("ToXML", result);

		LOG_INFO << "Received\n" << tmp;

		/* dispatch callbacks */
		connection->DispatchMsg(message);

		const char * tmpSubject = 0;
		result = message->GetSubject(tmpSubject);
		check("GetSubject", result);

		if (tmpSubject) {

			string tmp(tmpSubject);

			if (tmp == "GMSEC.TERMINATE")
				done = false;

		}

		/* Destroy inbound message */
		result = connection->DestroyMessage(message);
		check("DestroyMessage", result);

	}

	return true;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc, "gmrpl_cb")) {
		printUsage("gmrpl_cb");
		return -1;
	}

	try {

		gmrpl_cb state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

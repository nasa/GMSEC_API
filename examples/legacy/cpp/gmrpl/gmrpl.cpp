/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmrpl.cpp
 * 
 * A C++ example demonstration of GMSEC request / reply functionality.
 * The associated example gmreq will provide the request.  
 * gmrpl should be run before gmreq.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"
#include <stdio.h>
#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


class gmrpl {

public:

	gmrpl(Config &config);
	~gmrpl();

	bool Run();

private:

	Config &config;
	Connection * connection;
	vector<string> subjects;

};

gmrpl::gmrpl(Config &c) :
		config(c), connection(0), subjects(0) {

	/* Initialize config */
	initialize(c);

}

gmrpl::~gmrpl() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

bool gmrpl::Run() {

	Status result;
	Message * message = 0;
	Message * reply = 0;

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
	string subject = get(config, "SUBJECT",
			"GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
	int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);


	/* Subscribe */
	determineSubjects(config, subjects);

	/* Subscribe */
	for (int i = 0; i < (int) subjects.size(); i++) {
		LOG_INFO << "Subscribing to " << subjects[i];
		result = connection->Subscribe(subjects[i].c_str());
		check("Subscribe", result);
	}

	bool done = false;
	time_t prevTime;
	time_t nextTime;
	double elapsedTime = 0;
	time(&prevTime);

	while (!done) {

		if(prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			LOG_INFO << "Program Timeout Reached!";
			return true;
		}

		result = connection->GetNextMsg(message, msg_timeout_ms);

		if(prog_timeout_s != GMSEC_WAIT_FOREVER)
		{
			time(&nextTime);
			elapsedTime += difftime(nextTime, prevTime);
			prevTime = nextTime;
		}

		if (result.isError()) {

			// do not abort on timeouts
			if (result.GetCode() == GMSEC_TIMEOUT_OCCURRED)
			{
				LOG_INFO << "timeout occurred";
				continue;
			}

			check("GetNextMsg", result);
		}

		const char * tmpSubject = 0;
		result = message->GetSubject(tmpSubject);
		check("GetSubject", result);

		if (tmpSubject) {

			string tmp(tmpSubject);

			if (tmp == "GMSEC.TERMINATE")
				done = true;

		}

		/* Dump the message */
		const char *tmp;
		result = message->ToXML(tmp);
		check("ToXML", result);

		LOG_INFO << "Received\n" << tmp;

		/* check for request */
		GMSEC_MSG_KIND kind;
		message->GetKind(kind);
		if (GMSEC_MSG_REQUEST == kind) {

			/* Construct Reply subject. */
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

			std::ostringstream reply_subject;
			reply_subject << "GMSEC.MISSION.SAT_ID.RESP." << comp_name << "." << msg_id << "." << status_code;

			/* Create Reply */
			if (!done) {

				result = connection->CreateMessage(reply_subject.str().c_str(), GMSEC_MSG_REPLY, reply);
				check("CreateMessage", result);

			} else {

				result = connection->CreateMessage(tmpSubject, GMSEC_MSG_REPLY, reply);
				check("CreateMessage", result);

			}

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

			if (config.CheckBoolValue("ENCRYPT", false)) {

				fd.SetName("SEC-ENCRYPT");
				fd.SetValue(GMSEC_TRUE);
				result = reply->AddField(fd);
				check("AddField(SEC-ENCRYPT)", result);
			}

			/* Dump Reply */
			result = reply->ToXML(tmp);
			check("ToXML", result);
			LOG_INFO << "Prepared Reply:\n" << tmp;

			/* Send Reply */
			result = connection->Reply(message, reply);
			check("Reply Sent", result);
		}

		/* Destroy messages */
		Message * tmp1 = message;
		message = 0;
		result = connection->DestroyMessage(tmp1);
		check("DestroyMessage", result);

		Message * tmp2 = reply;
		reply = 0;
		result = connection->DestroyMessage(tmp2);
		check("DestroyMessage", result);

	}

	return true;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc, "gmrpl")) {
		printUsage("gmrpl");
		return -1;
	}


	try {

		gmrpl state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

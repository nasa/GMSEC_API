/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmsub.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will publish a message and this program
 * will subscribe and listen for it.
 * gmsub should be run before gmpub.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"

#include <string>
#include <vector>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gmsub {

public:

	gmsub(Config &config);
	~gmsub();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * message;
	vector<string> subjects;
	const char* value;

};

gmsub::gmsub(Config &c) :
		config(c), connection(0), message(0), subjects(0) {

	/* Initialize config */
	initialize(c);

}

gmsub::~gmsub() {

	Status result;

	if (message)
		checkNoThrow("Cleaning up Message", connection->DestroyMessage(message));

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmsub::Run() {

	Status result;

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

	/* Determine how many messages to receive */
	int iterations = get(config, "ITERATIONS", 0);
	if (iterations > 0) {
		LOG_INFO << "Waiting for up to " << iterations << " messages";
	}

	/* Determine the timeout period */
	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);
	int prog_timeout_s = get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S);

	/* Output the middleware information */
	LOG_INFO << "Middleware version = " << connection->GetLibraryVersion();

	/* Subscribe */
	for (int i = 0; i < (int) subjects.size(); i++) {

		LOG_INFO << "Subscribing to " << subjects[i];
		result = connection->Subscribe(subjects[i].c_str());
		check("Subscribe", result);

	}

	/* Wait for, and print out messages */
	int count = 0;
	int done = 0;
	time_t prevTime;
	time_t nextTime;
	double elapsedTime = 0;
	time(&prevTime);

	while (!done) {

		if(prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
		{
			LOG_INFO << "Program Timeout Reached!";
			done = 1;
			continue;
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
				LOG_INFO << "timeout occurred";
			else
				check("GetNextMsg", result);
		}

		if (message) {
			++count;
			if (iterations > 0 && count >= iterations)
				done = 1;

			const char * xml = 0;
			result = message->ToXML(xml);
			check("ToXML", result);

 			if (xml) {
				LOG_INFO << "received\n" << xml;
			}

			const char * subject = 0;
			result = message->GetSubject(subject);
			check("GetSubject", result);

			if (subject) {
				string tmp(subject);

				if (tmp == "GMSEC.TERMINATE")
					done = 1;
			}

			result = connection->DestroyMessage(message);
			check("DestroyMessage", result);
			message = 0;

		}

	}

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc, "gmsub")) {
		printUsage("gmsub");
		return -1;
	}

	try {

		gmsub state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}


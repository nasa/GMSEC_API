/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmsub_cb.cpp
 * 
 * Another C++ example demonstrating the subscription abilities of 
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

	};

};

class gmsub_cb {

public:

	gmsub_cb(Config &config);
	~gmsub_cb();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * message;
	vector<string> subjects;

};

gmsub_cb::gmsub_cb(Config &c) :
		config(c), connection(0), message(0), subjects(0) {

	/* Initialize config */
	initialize(c);

}

gmsub_cb::~gmsub_cb() {

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

Status gmsub_cb::Run() {

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
	PublishCallback cb;
	

	//result = connection->StartAutoDispatch();

	//PublishCallback scb;
	//PublishCallback scb2;

	//LOG_INFO << "Sub cb 1a";
	//result = connection->Subscribe("SUB.USUB.SUB",&scb);
	//check("Sub w/ CB 1.a", result);

	//LOG_INFO << "Sub cb 1b";
	//result = connection->Subscribe("SUB.USUB.SUB",&scb2);
	//check("Sub w/ CB 1.b",result);

	//LOG_INFO << "Pub msgA";
	//Message *msgA;
	//connection->CreateMessage("SUB.USUB.SUB",GMSEC_MSG_PUBLISH, msgA);
	//connection->Publish(msgA);
	//
	//example_millisleep(2000);
	//
	//LOG_INFO << "UNsub str";
	//result = connection->UnSubscribe("SUB.USUB.SUB");
	//check("UNSub",result);

	//example_millisleep(2000);

	////LOG_INFO <<"Sub no cb";
	////result = connection->Subscribe("SUB.USUB.SUB");
	////check("Sub no str", result);

	////LOG_INFO << "Sub cb 1a";
	////result = connection->Subscribe("SUB.USUB.SUB",&scb);
	////check("Sub w/ CB 2", result);
	////
	////example_millisleep(2000);

	//LOG_INFO << "Pub msgB";
	//Message *msgB;
	//connection->CreateMessage("SUB.USUB.SUB",GMSEC_MSG_PUBLISH,msgB);
	//connection->Publish(msgB);

	//result = connection->StopAutoDispatch();
	////connection->DestroyMessage(msgA);
	////connection->DestroyMessage(msgB);

	/* Subscribe */
	for (int i = 0; i < (int) subjects.size(); i++) {

		LOG_INFO << "Subscribing to " << subjects[i];
		result = connection->Subscribe(subjects[i].c_str(), &cb);
		check("Subscribe", result);

	}

	/* Listen */
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

			/* dispatch callbacks */
			connection->DispatchMsg(message);

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

	if (isOptionInvalid(config, argc, "gmsub_cb")) {
		printUsage("gmsub_cb");
		return -1;
	}


	try {

		gmsub_cb state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

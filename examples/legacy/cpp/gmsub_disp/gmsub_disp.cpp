/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmsub_disp.cpp
 * 
 * Another C++ example demonstrating the subscribe capabilities
 * of the GMSEC API using the AutoDispatch routine.
 * 
 */

#include <gmsec_cpp.h>
#include <gmsec/util/sysutil.h>
#include "../example.h"

#include <iostream>
#include <string>
#include <vector>

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

		LOG_INFO << "Press the enter key to exit";

	};

};

class gmsub_disp {

public:

	gmsub_disp(Config &config);
	~gmsub_disp();

	Status Run();

private:

	Config &config;
	Connection * connection;
	vector<string> subjects;

};

gmsub_disp::gmsub_disp(Config &c) :
		config(c), connection(0), subjects(0) {

	/* Initialize config */
	initialize(c);

}

gmsub_disp::~gmsub_disp() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmsub_disp::Run() {

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
	check("Start AutoDispatch", result);

	LOG_INFO << "Press the enter key to exit";

	cin.get();

	LOG_INFO << "Stopping AutoDispatch";
	result = connection->StopAutoDispatch();
	check("Stop AutoDispatch", result);

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmsub_disp");
		return -1;
	}

	try {

		gmsub_disp state(config);
		state.Run();
	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

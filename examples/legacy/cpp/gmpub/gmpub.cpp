/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmpub.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will listen for this publish program.
 * gmsub should be run before gmpub.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"
#include <gmsec/util/sysutil.h>

#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gmpub {

public:

	gmpub(Config &config);
	~gmpub();
	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * message;

	void Publish(int iteration, string subject);

};

gmpub::gmpub(Config &c) :
		config(c), connection(0), message(0) {

	/* Initialize config */
	initialize(c);

}

gmpub::~gmpub() {

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

Status gmpub::Run() {

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
	string subject = get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
	int iterations = get(config, "ITERATIONS", 1);
	int interval_ms = get(config, "INTERVAL_MS", 1000);

	/* Output middleware version */
	LOG_INFO << "Middleware version = " << connection->GetLibraryVersion();

	/* Output information */
	LOG_INFO << "Using subject '" << subject << "'";
	LOG_INFO << "Publishing " << iterations << " message(s)";

	/* Publish messages */
	if (iterations > 1) {
		LOG_INFO << "Publish interval " << interval_ms << " [ms]";
	}

	for (int i = 0; i < iterations; i++) {

		Publish(i, subject);

		if (i < iterations - 1)
			millisleep(interval_ms);

	}

	return result;

}

void gmpub::Publish(int iteration, string subject) {

	Status result;
	Field field;
	int value = iteration + 1;

	/* Create Message */
	message = 0;
	result = connection->CreateMessage(subject.c_str(), GMSEC_MSG_PUBLISH,
			message);
	LOG_INFO << "working";
	check("CreateMessage", result);

	/* Add Fields */
	field.SetName("C");
	field.SetValue((GMSEC_CHAR) 'c');
	result = message->AddField(field);
	check("AddField(<GMSEC_CHAR>)", result);

	field.SetName("T");
	field.SetValue(GMSEC_TRUE);
	result = message->AddField(field);
	check("AddField(GMSEC_TRUE)", result);

	field.SetName("F");
	field.SetValue(GMSEC_FALSE);
	result = message->AddField(field);
	check("AddField(GMSEC_FALSE)", result);

	field.SetName("J");
	field.SetValue((GMSEC_I16) value);
	result = message->AddField(field);
	check("AddField(<GMSEC_I16>)", result);

	field.SetName("K");
	field.SetValue((GMSEC_U16) value);
	result = message->AddField(field);
	check("AddField(<GMSEC_U16>)", result);

	field.SetName("I");
	field.SetValue((GMSEC_I32) value);
	result = message->AddField(field);
	check("AddField(<GMSEC_I32>)", result);

	field.SetName("COUNT");
	field.SetValue((GMSEC_I32) iteration);
	result = message->AddField(field);
	check("AddField(COUNT)", result);

	field.SetName("U");
	field.SetValue((GMSEC_U32) value);
	result = message->AddField(field);
	check("AddField(<GMSEC_U32>)", result);

	field.SetName("S");
	field.SetValue("This is a test");
	result = message->AddField(field);
	check("AddField(<GMSEC_STR>)", result);

	field.SetName("E");
	field.SetValue(GMSEC_F32(1 + 1. / value));
	result = message->AddField(field);
	check("AddField(<GMSEC_F32>)", result);

	field.SetName("D");
	field.SetValue(GMSEC_F64(1 + 1. / value));
	result = message->AddField(field);
	check("AddField(<GMSEC_F64>)", result);

	field.SetName("X");
	field.SetValue((GMSEC_BIN) "JLMNOPQ", 7);
	result = message->AddField(field);
	check("AddField(<GMSEC_BIN>)", result);

	if (config.CheckBoolValue("ENCRYPT", false)) {

		field.SetName("SEC-ENCRYPT");
		field.SetValue(GMSEC_TRUE);
		result = message->AddField(field);
		check("AddField(SEC-ENCRYPT)", result);

	}

	/* Dump Message */
	const char * xml = 0;
	result = message->ToXML(xml);
	check("ToXML", result);
	LOG_INFO << "Sending:\n" << xml;

	/* Publish Message */
	result = connection->Publish(message);
	check("Publish", result);
	LOG_INFO << "Published!";

	/* Destroy the Message */
	Message * tmp = message;
	message = 0;
	result = connection->DestroyMessage(tmp);
	check("DestroyMessage", result);

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmpub");
		return -1;
	}


	try {

		gmpub state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}


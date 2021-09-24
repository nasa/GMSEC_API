/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmreq.cpp
 * 
 * A C++ example demonstration of GMSEC request / reply functionality.
 * The associated example gmrpl will provide the reply.  
 * gmrpl should be run before gmreq.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"

#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


class gmreq {

public:

	gmreq(Config &config);
	~gmreq();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * request;
	Message * reply;

	void Request(int iteration, string subject);

};

gmreq::gmreq(Config &c) :
		config(c), connection(0) {

	/* Initialize Config */
	initialize(c);

}

gmreq::~gmreq() {

	if (request)
		check("Cleaning up Request", connection->DestroyMessage(request));

	if (reply)
		check("Cleaning up Reply", connection->DestroyMessage(reply));

	if (connection) {

		if (connection->IsConnected())
			check("Disconnect", connection->Disconnect());

		check("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	check("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmreq::Run() {

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

void gmreq::Request(int msg_timeout_ms, string subject) {

	Status result;
	Field fd;

	result = connection->CreateMessage(subject.c_str(), GMSEC_MSG_REQUEST,
			request);
	check("CreateMessage", result);

	/* Add Fields */
	fd.SetName("QUESTION");
	fd.SetValue("Does the request/reply functionality still work?");
	result = request->AddField(fd);
	check("AddField(<GMSEC_STR>)", result);

	/* Add the name of the component making the Request. */
	fd.SetName("COMPONENT");
	fd.SetValue("GMREQ");
	result = request->AddField(fd);
	check("AddField(<GMREQ>)", result);

	/* Add the Message ID of this Request.  In theory the MSG-ID should be unique,
	 but here it is not for the sake of simplicity.  See API Interface Specification
	 for suggetions on creating a MSG-ID. */
	fd.SetName("MSG-ID");
	fd.SetValue("GMREQ-MSG");
	result = request->AddField(fd);
	check("AddField(<GMREQ-MSG>)", result);

	if (config.CheckBoolValue("ENCRYPT", false)) {
		fd.SetName("SEC-ENCRYPT");
		fd.SetValue(GMSEC_TRUE);
		result = request->AddField(fd);
		check("AddField(SEC-ENCRYPT)", result);
	}

	/* Dump Request */
	const char * reqxml = 0;
	result = request->ToXML(reqxml);
	check("request ToXML", result);
	LOG_INFO << "Requesting:\n" << reqxml;

	/* Send Request Message */
	result = connection->Request(request, msg_timeout_ms, reply);
	check("Request", result);

	/* Dump Reply */
	const char * rplxml = 0;
	result = reply->ToXML(rplxml);
	check("reply ToXML", result);
	LOG_INFO << "Received reply:\n" << rplxml;

	/* Destroy the Reply */
	Message * rpltmp = reply;
	reply = 0;
	result = connection->DestroyMessage(rpltmp);
	check("DestroyMessage", result);

	/* Destroy the Request */
	Message * reqtmp = request;
	request = 0;
	result = connection->DestroyMessage(reqtmp);
	check("DestroyMessage", result);

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("gmreq");
		return -1;
	}

	try {

		gmreq state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}

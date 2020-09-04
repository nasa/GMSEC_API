/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <string>

using namespace gmsec::api;


class gmreq
{
public:
	gmreq(Config& config);
	~gmreq();
	bool run();

private:
	Config&     config;
	Connection* connection;

	void request(int iteration, const std::string& subject);
};


gmreq::gmreq(Config& c)
	: config(c),
	  connection(0)
{
	// Initialize Config
	example::initialize(c);
}


gmreq::~gmreq()
{
	if (connection)
	{
		connection->disconnect();

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmreq::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Get information from the command line */
		std::string subject        = example::get(config, "SUBJECT", "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
		int         msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);

		//o output connection middleware version
		GMSEC_INFO << connection->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Using subject '" << subject.c_str() << "'";

		request(msg_timeout_ms, subject);
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void gmreq::request(int msg_timeout_ms, const std::string& subject)
{
	//o Create message
	Message request(subject.c_str(), Message::REQUEST);

	//o Add fields to message
	request.addField("QUESTION", "Does the request/reply functionality still work?");
	request.addField("COMPONENT", "GMREQ");

	if (config.getBooleanValue("ENCRYPT", false))
	{
		request.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of request message
	GMSEC_INFO << "Requesting:\n" << request.toXML();

	//o Send Request Message
	Message* reply = connection->request(request, msg_timeout_ms);

	if (reply)
	{
		//o Got reply; display XML representation of reply message
		GMSEC_INFO << "Received reply:\n" << reply->toXML();

		//o Destroy the reply message
		connection->release(reply);
	}
	else
	{
		GMSEC_WARNING << "Reply message not receive after a timeout of " << msg_timeout_ms << " msec.";
	}
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmreq");
		return -1;
	}

	gmreq(config).run();
}

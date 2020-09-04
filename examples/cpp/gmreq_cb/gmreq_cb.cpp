/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <string>

using namespace gmsec::api;


bool gReplyCallbackFlag = false;


class MyReplyCallback: public ReplyCallback
{
public:
	virtual void CALL_TYPE onReply(Connection& connection, const Message& request, const Message& reply)
	{
		//o Display XML representation of reply message
		GMSEC_INFO << "[MyReplyCallback::onReply]\n" << reply.toXML();

		gReplyCallbackFlag = true;
	}

	virtual void CALL_TYPE onEvent(Connection& connection, const Status& status, Connection::ConnectionEvent event)
	{
		if (status.isError())
		{
			GMSEC_ERROR << "[MyReplyCallback::onError] " << status.get() << ", event=" << event;
		}
	}
};


class gmreq_cb
{
public:
	gmreq_cb(Config& config);
	~gmreq_cb();
	bool run();

private:
	Config&     config;
	Connection* connection;

	void request(int iteration, const std::string& subject);
};


gmreq_cb::gmreq_cb(Config &c)
	: config(c),
	  connection(0)
{
	// Initialize Config
	example::initialize(c);
}


gmreq_cb::~gmreq_cb()
{
	if (connection)
	{
		connection->disconnect();
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmreq_cb::run()
{
	bool success = true;

	//o Output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Get information from the command line
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


void gmreq_cb::request(int msg_timeout_ms, const std::string& subject)
{
	//o Create request message
	Message request(subject.c_str(), Message::REQUEST);

	//o Add fields to request message
	request.addField("QUESTION", "Does the request/reply functionality still work?");
	request.addField("COMPONENT", "GMREQ");

	if (config.getBooleanValue("ENCRYPT", false))
	{
		request.addField("SEC-ENCRYPT", true);
	}

	//o Display XML representation of request message
	GMSEC_INFO << "Requesting:\n" << request.toXML();

	MyReplyCallback cb;
	connection->request(request, msg_timeout_ms, &cb);

	int deltaTime = 0;
	GMSEC_INFO << "Waiting for response...";
	while (gReplyCallbackFlag == 0)
	{
		example::millisleep(100);

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
		GMSEC_INFO << "Response Received!";
	}
	else
	{
		GMSEC_WARNING << "No response received";
	}
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmreq_cb");
		return -1;
	}

	gmreq_cb(config).run();
}

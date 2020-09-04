/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <iostream>
#include <string>
#include <vector>

using namespace gmsec::api;


class PublishCallback : public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		GMSEC_INFO << "[PublishCallback::onMessage] Received:\n" << message.toXML();
	}
};


class RequestCallback : public ReplyCallback
{
public:
	virtual void CALL_TYPE onReply(Connection& connection, const Message& request, const Message& reply)
	{
		GMSEC_INFO << "[RequestCallback::onReply] Received Reply:\n" << reply.toXML();
	}

	virtual void CALL_TYPE onEvent(Connection& connection, const Status& status, Connection::ConnectionEvent event)
	{
		GMSEC_WARNING << "[RequestCallback::onEvent] Status=" << status.get()
		              << " event=" << event;
	}
};


class gmsub_disp_rr
{
public:
	gmsub_disp_rr(Config& config);
	~gmsub_disp_rr();
	bool run();

private:
	typedef std::vector<std::string> Subjects;

	Config&     config;
	Connection* connection;
	Subjects    subjects;
	SubscriptionInfo** info;
};


gmsub_disp_rr::gmsub_disp_rr(Config& c)
	: config(c),
	  connection(0),
	  subjects()
{
	/* Initialize config */
	example::initialize(c);
}


gmsub_disp_rr::~gmsub_disp_rr()
{
	if (connection)
	{
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Unsubscribing from " << subjects[i].c_str();
			connection->unsubscribe(info[i]);
		}
		delete[] info;
		connection->disconnect();
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmsub_disp_rr::run()
{
	bool success = true;

	//o output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		config.addValue("isThreaded", "true");

		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Determine the subjects to listen to
		example::determineSubjects(config, subjects);

		//o Output the middleware information
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Subscribe using Callback sub-class
		PublishCallback cb;

		info = new SubscriptionInfo*[subjects.size()];

		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();
			info[i] = connection->subscribe(subjects[i].c_str(), &cb);
		}

		GMSEC_INFO << "Starting AutoDispatch";
		connection->startAutoDispatch();

		RequestCallback rcb;

		int msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
		int prog_timeout_s = example::get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

		std::string subject = "GMSEC.TEST.PUBLISH.NOREPLY";
		if (subjects[0] != "GMSEC.>")
		{
			subject = subjects[0];
		}

		GMSEC_INFO << "Creating a request message";
		Message reqMsg(subject.c_str(), Message::REQUEST);

		GMSEC_INFO << "Registering event callback";
		connection->registerEventCallback(Connection::REQUEST_TIMEOUT_EVENT, &rcb);

		GMSEC_INFO << "Sending the request";
		connection->request(reqMsg, msg_timeout_ms, &rcb);
	
		if (prog_timeout_s == GMSEC_WAIT_FOREVER)
		{
			GMSEC_INFO << "Press the enter key to exit";
			std::cin.get();
		}
		else
		{
			example::millisleep(prog_timeout_s * 1000);
			GMSEC_INFO << "Program Timeout Reached!";
		}

		GMSEC_INFO << "Stopping AutoDispatch";
		connection->stopAutoDispatch();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmsub_disp_rr");
		return -1;
	}

	return (gmsub_disp_rr(config).run() ? 0 : -1);
}

/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <gmsec4/util/TimeUtil.h>

#include <string>
#include <vector>

using namespace gmsec::api;
using namespace gmsec::api::util;


class PublishCallback: public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		GMSEC_INFO << "[PublishCallback::onMessage] Received:\n" << message.toXML();
	}
};


class gmsub_cb
{
public:
	gmsub_cb(Config &config);
	~gmsub_cb();
	bool run();

private:
	typedef std::vector<std::string> Subjects;

	Config&     config;
	Connection* connection;
	Subjects    subjects;
	SubscriptionInfo** info;
};


gmsub_cb::gmsub_cb(Config& c)
	: config(c),
	  connection(0),
	  subjects()
{
	/* Initialize config */
	example::initialize(c);
}


gmsub_cb::~gmsub_cb()
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


bool gmsub_cb::run()
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

		//o Determine the subjects to listen to
		example::determineSubjects(config, subjects);

		//o Get control parameters for this app
		int iterations     = example::get(config, "ITERATIONS", 0);
		int msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
		int prog_timeout_s = example::get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

		if (iterations > 0)
		{
			GMSEC_INFO << "Waiting for up to " << iterations << " messages";
		}

		//o Output the middleware information */
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Subscribe using a Callback sub-class
		PublishCallback cb;
		
		info = new SubscriptionInfo*[subjects.size()];
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();
			info[i] = connection->subscribe(subjects[i].c_str(), &cb);
		}

		/* Listen for messages */
		int    count = 0;
		bool   done = false;
		double prevTime;
		double nextTime;
		double elapsedTime = 0;

		prevTime = TimeUtil::getCurrentTime_s();

		while (!done)
		{
			if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
			{
				GMSEC_INFO << "Program Timeout Reached!";
				done = true;
				continue;
			}

			Message* message = connection->receive(msg_timeout_ms);

			if (prog_timeout_s != GMSEC_WAIT_FOREVER)
			{
				nextTime = TimeUtil::getCurrentTime_s();
				elapsedTime += (nextTime - prevTime);
				prevTime = nextTime;
			}

			if (!message)
			{
				GMSEC_INFO << "timeout occurred";
				continue;
			}

			++count;
			if (iterations > 0 && count >= iterations)
			{
				done = true;
			}

			//o dispatch callbacks
			connection->dispatch(*message);

			std::string subject = message->getSubject();
			if (subject == "GMSEC.TERMINATE")
			{
				done = true;
			}

			connection->release(message);
		}
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

	if (example::isOptionInvalid(config, argc, "gmsub_cb"))
	{
		example::printUsage("gmsub_cb");
		return -1;
	}

	return (gmsub_cb(config).run() ? 0 : -1);
}

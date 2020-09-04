/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <gmsec4/util/TimeUtil.h>

#include <string>
#include <vector>

using namespace gmsec::api;
using namespace gmsec::api::util;


class gmsub
{
public:
	gmsub(Config& config);
	~gmsub();
	bool run();

private:
	typedef std::vector<std::string> Subjects;

	Config&     config;
	Connection* connection;
	Subjects    subjects;
	SubscriptionInfo** info;
};


gmsub::gmsub(Config &c)
	: config(c),
	  connection(0),
	  subjects(0)
{
	// Initialize config
	example::initialize(c);
}


gmsub::~gmsub()
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


bool gmsub::run()
{
	bool success = true;

	//o output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Determine the subjects to listen to
		example::determineSubjects(config, subjects);

		//o Determine runtime settings
		int iterations     = example::get(config, "ITERATIONS", 0);
		int msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
		int prog_timeout_s = example::get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

		if (iterations > 0)
		{
			GMSEC_INFO << "Waiting for up to " << iterations << " messages";
		}

		//o Output the middleware information
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Subscribe
		info = new SubscriptionInfo*[subjects.size()];
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();
			info[i] = connection->subscribe(subjects[i].c_str());
		}

		//o Wait for, and print out messages
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
			}
			else
			{
				GMSEC_INFO << "Received:\n" << message->toXML();

				++count;

				done = (iterations > 0 && count >= iterations);
				done = done || (std::string(message->getSubject()) == "GMSEC.TERMINATE");

				connection->release(message);
			}
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

	if (example::isOptionInvalid(config, argc, "gmsub"))
	{
		example::printUsage("gmsub");
		return -1;
	}

	gmsub(config).run();
}

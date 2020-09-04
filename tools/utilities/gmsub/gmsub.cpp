/*
 * Copyright 2007-2018 United States Government as represented by the
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

#include "../Utility.h"

#include <gmsec4/util/TimeUtil.h>

#include <string>
#include <vector>

using namespace gmsec::api;
using namespace gmsec::api::util;


class GMSEC_Subscriber : public Utility
{
public:
	GMSEC_Subscriber(const Config& config);

	~GMSEC_Subscriber();

	void usage(const std::string& programName);

	bool run();

private:
	typedef std::vector<std::string>       Subjects;
	typedef std::vector<SubscriptionInfo*> Subscriptions;


	Connection*   connection;
	Subjects      subjects;
	Subscriptions subscriptions;
};


GMSEC_Subscriber::GMSEC_Subscriber(const Config& c)
	: Utility(c),
	  connection(0),
	  subjects(),
	  subscriptions()
{
	// Initialize utility
	initialize();
}


GMSEC_Subscriber::~GMSEC_Subscriber()
{
	try
	{
		if (connection)
		{
			for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
			{
				SubscriptionInfo* info = *it;

				GMSEC_INFO << "Unsubscribing from " << info->getSubject();
				connection->unsubscribe(info);
			}

			connection->disconnect();

			Connection::destroy(connection);
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
	}

	Connection::shutdownAllMiddlewares();
}


void GMSEC_Subscriber::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject[.N]=<subscription pattern>"
	          << "\n"
	          << "\n\t\t\tNote: N, if provided, must be 1 or greater."
	          << "\n\t\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B"
	          << "\n"
	          << "\n\t\t* iterations=<number of messages to receive>"
	          << "\n"
	          << "\n\t\t* msg-timeout-ms=<timeout period (milliseconds)>"
	          << "\n"
	          << "\n\t\t* prog-timeout-s=<timeout period (seconds)>"
	          << "\n"
	          << "\n\t\t\tNote: msg-timeout-ms must be defined to use prog-timeout-s"
	          << "\n"
	          << std::endl;
}


bool GMSEC_Subscriber::run()
{
	bool success = true;

	//o output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(getConfig());

		//o Connect
		connection->connect();

		//o Determine the subjects to listen to
		determineSubjects(subjects);

		//o Determine runtime settings
		int iterations     = get("ITERATIONS", 0);
		int msg_timeout_ms = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		int prog_timeout_s = get("PROG-TIMEOUT-S", GMSEC_WAIT_FOREVER);

		if (iterations > 0)
		{
			GMSEC_INFO << "Waiting for up to " << iterations << " messages";
		}

		//o Output the middleware information
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Subscribe
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			SubscriptionInfo* info = connection->subscribe(subjects[i].c_str());

			subscriptions.push_back(info);
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

	GMSEC_Subscriber gmsub(config);

	gmsub.addToConfigFromFile();

	if (gmsub.isOptionInvalid(argc, "gmsub"))
	{
		gmsub.usage("gmsub");
		return 1;
	}

	gmsub.run();
}

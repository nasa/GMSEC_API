/*
 * Copyright 2007-2021 United States Government as represented by the
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


class GMSEC_Subscriber : public gmsec::api::util::Utility
{
public:
	GMSEC_Subscriber(const gmsec::api::Config& config);

	~GMSEC_Subscriber();

	void usage(const std::string& programName);

	bool run();

private:
	typedef std::vector<std::string> Subjects;
	typedef std::vector<gmsec::api::mist::SubscriptionInfo*> Subscriptions;


	gmsec::api::mist::ConnectionManager* connMgr;
	Subjects                             subjects;
	Subscriptions                        subscriptions;
};


GMSEC_Subscriber::GMSEC_Subscriber(const gmsec::api::Config& c)
	: Utility(c),
	  connMgr(0),
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
		if (connMgr)
		{
			//o Normally, applications should not have to unsubscribe(). When not performed,
			//  the ConnectionManager will take care of this automatically when cleanup() is called.
			for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
			{
				gmsec::api::mist::SubscriptionInfo* info = *it;

				GMSEC_INFO << "Unsubscribing from " << info->getSubject();

				connMgr->unsubscribe(info);
			}

			connMgr->cleanup();

			delete connMgr;
		}
	}
	catch (const gmsec::api::Exception& e)
	{
		GMSEC_ERROR << e.what();
	}

	gmsec::api::mist::ConnectionManager::shutdownAllMiddlewares();
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
	GMSEC_INFO << gmsec::api::mist::ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		connMgr = new gmsec::api::mist::ConnectionManager(getConfig());

		//o Connect
		connMgr->initialize();

		//o Determine runtime settings
		GMSEC_I32 iterations     = get("ITERATIONS", 0);
		GMSEC_I32 msg_timeout_ms = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		GMSEC_I32 prog_timeout_s = get("PROG-TIMEOUT-S", GMSEC_WAIT_FOREVER);

		if (iterations > 0)
		{
			GMSEC_INFO << "Waiting for up to " << iterations << " messages";
		}

		unsigned int specVersion = connMgr->getSpecification().getVersion();
		gmsec::api::mist::Specification::SchemaLevel schemaLevel = connMgr->getSpecification().getSchemaLevel();

		//o Determine the subjects to listen to
		determineSubjects(specVersion, schemaLevel, subjects);

		//o Output the middleware information
		GMSEC_INFO << "Middleware version = " << connMgr->getLibraryVersion();

		//o Subscribe
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			gmsec::api::mist::SubscriptionInfo* info = connMgr->subscribe(subjects[i].c_str());

			subscriptions.push_back(info);
		}

		//o Wait for, and print out messages
		int    count = 0;
		bool   done = false;
		double prevTime;
		double nextTime;
		double elapsedTime = 0;

		prevTime = gmsec::api::util::TimeUtil::getCurrentTime_s();

		while (!done)
		{
			if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
			{
				GMSEC_INFO << "Program Timeout Reached!";
				done = true;
				continue;
			}

			gmsec::api::Message* message = connMgr->receive(msg_timeout_ms);

			if (prog_timeout_s != GMSEC_WAIT_FOREVER)
			{
				nextTime = gmsec::api::util::TimeUtil::getCurrentTime_s();
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

				connMgr->release(message);
			}
		}
	}
	catch (const gmsec::api::Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	gmsec::api::Config config(argc, argv);

	GMSEC_Subscriber gmsub(config);

	if (!gmsub.areOptionsValid(argc, "gmsub"))
	{
		gmsub.usage("gmsub");
		return 1;
	}

	gmsub.addToConfigFromFile();

	gmsub.run();
}

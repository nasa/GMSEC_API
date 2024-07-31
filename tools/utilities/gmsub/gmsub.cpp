/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmsub.cpp
 */

#include "../Utility.h"

#include <gmsec5/util/TimeUtil.h>

#include <string>
#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


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


	Connection*   conn;
	Subscriptions subscriptions;
};


GMSEC_Subscriber::GMSEC_Subscriber(const Config& c)
	: Utility(c),
	  conn(0),
	  subscriptions()
{
	// Initialize utility
	initialize();
}


GMSEC_Subscriber::~GMSEC_Subscriber()
{
	try
	{
		if (conn)
		{
			//o Normally, applications should not have to unsubscribe(). When not performed,
			//  the Connection will take care of this automatically when disconnect() is called.
			for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
			{
				SubscriptionInfo* info = *it;

				GMSEC_INFO << "Unsubscribing from " << info->getSubjectPattern();

				conn->unsubscribe(info);
			}

			conn->disconnect();

			delete conn;
		}
	}
	catch (const GmsecException& e)
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
	          << "\n\t\t* exclude-subject[.N]=<subscription pattern>"
	          << "\n"
	          << "\n\t\t\tNote: N, if provided, must be 1 or greater."
	          << "\n\t\t\tFor example, exclude-subject.1=GMSEC.A exclude-subject.2=GMSEC.B"
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
		conn = new Connection(getConfig());

		//o Output the middleware information
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		//o Connect
		conn->connect();

		//o Determine runtime settings
		GMSEC_I32 iterations     = get("ITERATIONS", 0);
		GMSEC_I32 msg_timeout_ms = get("MSG-TIMEOUT-MS", GMSEC_WAIT_FOREVER);
		GMSEC_I32 prog_timeout_s = get("PROG-TIMEOUT-S", GMSEC_WAIT_FOREVER);

		unsigned int specVersion = conn->getMessageFactory().getSpecification().getVersion();
		Specification::SchemaLevel schemaLevel = conn->getMessageFactory().getSpecification().getSchemaLevel();

		//o Determine the subjects to listen to, and those (if any) to exclude
		Subjects subjects;
		determineSubjects(specVersion, schemaLevel, subjects);

		Subjects excludedSubjects;
		determineExcludedSubjects(excludedSubjects);

		//o Subscribe to desired topics
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			SubscriptionInfo* info = conn->subscribe(subjects[i].c_str(), getConfig());

			subscriptions.push_back(info);
		}

		//o Exclude desired topics
		for (size_t i = 0; i < excludedSubjects.size(); ++i)
		{
			GMSEC_INFO << "Excluding subscription topic " << excludedSubjects[i].c_str();

			conn->excludeSubject(excludedSubjects[i].c_str());
		}

		//o Wait for, and print out messages
		int    count = 0;
		bool   done = false;
		double prevTime;
		double nextTime;
		double elapsedTime = 0;

		prevTime = TimeUtil::getCurrentTime_s();

		if (iterations > 0)
		{
			GMSEC_INFO << "Waiting for up to " << iterations << " messages";
		}

		while (!done)
		{
			if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
			{
				GMSEC_INFO << "Program Timeout Reached!";
				done = true;
				continue;
			}

			Message* message = conn->receive(msg_timeout_ms);

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
				//o Display XML/JSON representation of message
				Utility::displayMessage("Received:\n", *message);

				++count;

				done = (iterations > 0 && count >= iterations);

				message->acknowledge();

				Message::destroy(message);
			}
		}
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		GMSEC_Subscriber gmsub(config);

		if (!gmsub.areOptionsValid(argc, "gmsub"))
		{
			gmsub.usage("gmsub");
			return 1;
		}

		gmsub.addToConfigFromFile();

		gmsub.run();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
	}
}

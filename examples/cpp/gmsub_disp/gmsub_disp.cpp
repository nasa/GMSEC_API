/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmsub_disp.cpp
 * 
 * Another C++ example demonstrating the subscribe capabilities
 * of the GMSEC API using the AutoDispatch routine.
 * 
 */

#include "../example.h"

#include <iostream>
#include <string>
#include <vector>

using namespace gmsec::api;


class PublishCallback: public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		GMSEC_INFO << "[PublishCallback::onMessage] Received:\n" << message.toXML();
	}
};


class gmsub_disp
{
public:
	gmsub_disp(Config& config);
	~gmsub_disp();
	bool run();

private:
	typedef std::vector<std::string>       Subjects;
	typedef std::vector<SubscriptionInfo*> Subscriptions;

	Config&       config;
	Connection*   connection;
	Subjects      subjects;
	Subscriptions subscriptions;
};


gmsub_disp::gmsub_disp(Config& c)
	: config(c),
	  connection(0),
	  subjects(),
	  subscriptions()
{
	/* Initialize config */
	example::initialize(c);
}


gmsub_disp::~gmsub_disp()
{
	if (connection)
	{
		for (Subscriptions::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
		{
			SubscriptionInfo* info = *it;

			GMSEC_INFO << "Unsubscribing from " << info->getSubject();

			connection->unsubscribe(info);
		}
		try
		{
			connection->disconnect();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
		}
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmsub_disp::run()
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

		//o Output the middleware information */
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Subscribe using Callback
		PublishCallback cb;

		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();

			SubscriptionInfo* info = connection->subscribe(subjects[i].c_str(), &cb);

			subscriptions.push_back(info);
		}

		GMSEC_INFO << "Starting AutoDispatch";
		connection->startAutoDispatch();

		GMSEC_INFO << "Press the <Enter> key to exit";
		std::cin.get();

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
		example::printUsage("gmsub_disp");
		return -1;
	}

	return (gmsub_disp(config).run() ? 0 : -1);
}

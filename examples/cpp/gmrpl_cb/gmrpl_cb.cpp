/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmrpl_cb.cpp
 * 
 * Another C++ example demonstrating the reply/request abilities of 
 * GMSEC.  This example involves the use of Callback functionality,
 * which allows for code to be performed anytime a Message is 
 * received.
 * 
 */

#include "../example.h"

#include <string>
#include <vector>

using namespace gmsec::api;


class RequestCallback : public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		try
		{
			//o Display the XML representation of the received message
			GMSEC_INFO << "[RequestCallback:onMessage] Received:\n" << message.toXML();

			//o Construct Reply subject.
			const char* component = 0;

			try {
				component = message.getStringField("COMPONENT").getValue();
			}
			catch (Exception& e) {
				GMSEC_WARNING << "COMPONENT field is not available: " << e.what();
			}

			//o Set Status Code.  See API Interface Specification for available status codes.
			const char* status_code = "1"; // 1 = Acknowledgement

			//o Create reply message
			std::ostringstream reply_subject;

			reply_subject << "GMSEC.MISSION.SAT_ID.RESP." << (component ? component : "UNKNOWN-COMPONENT") << "." << status_code;

			Message reply(reply_subject.str().c_str(), Message::REPLY);

			//o Add fields to the reply message
			reply.addField("COMPONENT", "GMRPL");
			reply.addField("ANSWER", "Sure looks like it!");

			//o Display XML representation of reply message
			GMSEC_INFO << "Prepared Reply:\n" << reply.toXML();

			//o Send Reply
			connection.reply(message,reply);
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << "[RequestCallback::onMessage] " << e.what();
		}
	}
};


class gmrpl_cb
{
public:
	gmrpl_cb(Config& config);
	~gmrpl_cb();
	bool run();

private:
	typedef std::vector<std::string> Subjects;

	Config&     config;
	Connection* connection;
	Subjects    subjects;
	SubscriptionInfo** info;
};


gmrpl_cb::gmrpl_cb(Config& c)
	: config(c),
	  connection(0)
{
	/* Initialize config */
	example::initialize(c);
}


gmrpl_cb::~gmrpl_cb()
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


bool gmrpl_cb::run()
{
	bool success = true;

	//o output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o output connection middleware version
		GMSEC_INFO << connection->getLibraryVersion();

		//o Get info from command line
		int msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
		int prog_timeout_s = example::get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

		//o Determine the subjects to listen to
		example::determineSubjects(config, subjects);

		//o Subscribe
		RequestCallback cb;
		
		info = new SubscriptionInfo*[subjects.size()];
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();
			info[i] = connection->subscribe(subjects[i].c_str(), &cb);
		}

		bool   done = false;
		time_t prevTime;
		time_t nextTime;
		double elapsedTime = 0;

		time(&prevTime);

		while (!done)
		{
			if (prog_timeout_s != GMSEC_WAIT_FOREVER && elapsedTime >= prog_timeout_s)
			{
				GMSEC_INFO << "Program Timeout Reached!";
				return true;
			}

			//o Receive next message
			Message* message = connection->receive(msg_timeout_ms);

			if (prog_timeout_s != GMSEC_WAIT_FOREVER)
			{
				time(&nextTime);
				elapsedTime += difftime(nextTime, prevTime);
				prevTime = nextTime;
			}

			if (!message)
			{
				GMSEC_INFO << "timeout occurred";
				continue;
			}
			else
			{
				//o Display the XML representation of the received message
				GMSEC_INFO << "Received:\n" << message->toXML();

				//o dispatch callbacks
				connection->dispatch(*message);

				std::string subject = message->getSubject();

				if (subject == "GMSEC.TERMINATE")
				{
					done = true;
				}

				//o Destroy received message
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

	if (example::isOptionInvalid(config, argc, "gmrpl_cb"))
	{
		example::printUsage("gmrpl_cb");
		return -1;
	}

	return (gmrpl_cb(config).run() ? 0 : -1);
}

/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmrpl.cpp
 * 
 * A C++ example demonstration of GMSEC request / reply functionality.
 * The associated example gmreq will provide the request.  
 * gmrpl should be run before gmreq.
 * 
 */

#include "../example.h"

#include <string>
#include <vector>

using namespace gmsec::api;


class gmrpl
{
public:
	gmrpl(Config& config);
	~gmrpl();
	bool run();

private:
	typedef std::vector<std::string> Subjects;

	Config&     config;
	Connection* connection;
	Subjects    subjects;
	SubscriptionInfo** info;

};


gmrpl::gmrpl(Config &c)
	: config(c),
	  connection(0),
	  subjects()
{
	/* Initialize config */
	example::initialize(c);
}


gmrpl::~gmrpl()
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


bool gmrpl::run()
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

		//o output connection middleware version
		GMSEC_INFO << connection->getLibraryVersion();

		//o Get info from command line
		std::string subject        = example::get(config, "SUBJECT", "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
		int         msg_timeout_ms = example::get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER);
		int         prog_timeout_s = example::get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER);

		//o Subscribe
		example::determineSubjects(config, subjects);

		//o Subscribe

		info = new SubscriptionInfo*[subjects.size()];
		for (size_t i = 0; i < subjects.size(); ++i)
		{
			GMSEC_INFO << "Subscribing to " << subjects[i].c_str();
			info[i] = connection->subscribe(subjects[i].c_str());
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

			//o receive the next message
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
				done = (std::string(message->getSubject()) == "GMSEC.TERMINATE");

				//o Display the XML representation of the received message
				GMSEC_INFO << "Received\n" << message->toXML();

				//o check for request
				if (message->getKind() == Message::REQUEST)
				{
					const char* component = 0;

					//o Construct Reply message
					try {
						const StringField& compField = message->getStringField("COMPONENT");
						component = compField.getValue();
					}
					catch (Exception& e) {
						GMSEC_WARNING << e.what();
					}

					//o Set Status Code.  See API Interface Specification for available status codes.
					const char* status_code = "1"; // 1 = Acknowledgement

					std::ostringstream reply_subject;
					reply_subject << "GMSEC.MISSION.SAT_ID.RESP."
					              << (component ? component : "UNKNOWN-COMPONENT") << "."
					              << status_code;

					//o Create reply message
					Message reply(reply_subject.str().c_str(), Message::REPLY);

					//o Add fields to the reply message
					reply.addField("COMPONENT", "GMRPL");
					reply.addField("ANSWER", "Sure looks like it!");

					if (config.getBooleanValue("ENCRYPT", false))
					{
						reply.addField("SEC-ENCRYPT", true);
					}

					//o Display XML representation of the reply message
					GMSEC_INFO << "Prepared Reply:\n" << reply.toXML();

					//o Send Reply
					connection->reply(*message, reply);
				}

				//o Destroy request message */
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

	if (example::isOptionInvalid(config, argc, "gmrpl"))
	{
		example::printUsage("gmrpl");
		return -1;
	}

	return (gmrpl(config).run() ? 0 : -1);
}

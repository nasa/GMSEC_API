/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmpub.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will listen for this publish program.
 * gmsub should be run before gmpub.
 * 
 */

#include "../Utility.h"

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::util;


const char* const DEFAULT_MESSAGE_SUBJECT = "GMSEC.MY-MISSION.MY-SAT-ID.MSG.C2CX.GMPUB.HB";


class GMSEC_Publisher : public Utility
{
public:
	GMSEC_Publisher(const Config& config);

	~GMSEC_Publisher();

	void usage(const std::string& programName);

	bool run();

private:
	ConnectionManager* connMgr;

	void publish(Message& msg);
	void publish(GMSEC_U16 counter, GMSEC_U32 interval_ms, const std::string& subject);
};


GMSEC_Publisher::GMSEC_Publisher(const Config& c)
	: Utility(c),
	  connMgr(0)
{
	/* Initialize utility */
	initialize();
}


GMSEC_Publisher::~GMSEC_Publisher()
{
	if (connMgr)
	{
		try
		{
			connMgr->cleanup();
		}
		catch (const Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete connMgr;
	}

	Connection::shutdownAllMiddlewares();
}


void GMSEC_Publisher::usage(const std::string& programName)
{
	Utility::usage(programName);

	std::cerr << "\n\t\t* subject=<message subject>"
	          << "\n"
	          << "\n\t\t* msg-file=<file with message definition>"
	          << "\n"
	          << "\n\t\t* cfg-file=<configuration file>"
	          << "\n"
	          << "\n\t\t\tNote: msg-name must be defined when using cfg-file"
	          << "\n"
	          << "\n\t\t* msg-name=<name of message within configuration file>"
	          << "\n"
	          << "\n\t\t* iterations=<number of messages to publish, or -1 to publish indefinitely>"
	          << "\n"
	          << "\n\t\t* interval-ms=<publish interval (milliseconds)>"
	          << "\n"
	          << std::endl;
}


bool GMSEC_Publisher::run()
{
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		connMgr = new ConnectionManager(getConfig());

		//o Connect
		connMgr->initialize();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr->getLibraryVersion();

		//o Get information from the command line
		std::string msgFile     = get("MSG-FILE", "");
		std::string cfgFile     = get("CFG-FILE", "");
		GMSEC_I32   iterations  = get("ITERATIONS", 1);
		GMSEC_U32   interval_ms = get("INTERVAL-MS", 1000);
		std::string subject;

		StdUniquePtr<Message> msg;

		if (!msgFile.empty())
		{
			msg.reset(readMessageFile(msgFile));

			subject = msg->getSubject();
		}
		else if (!cfgFile.empty())
		{
			std::string msgName = get("MSG-NAME", "");

			if (msgName.empty())
			{
				GMSEC_ERROR << "When using cfg-file option, user must also provide msg-name option";
				return false;
			}

			msg.reset(readConfigFile(cfgFile, msgName));

			subject = msg->getSubject();
		}
		else
		{
			subject = get("SUBJECT", DEFAULT_MESSAGE_SUBJECT);
		}

		//o Output information
		if (iterations < -1 || iterations == 0)
		{
			GMSEC_INFO << "No messages will be published";
		}
		else if (iterations == -1)
		{
			GMSEC_INFO << "Publishing indefinitely";
		}
		else if (iterations > 0)
		{
			GMSEC_INFO << "Publishing " << iterations << " message(s)";
		}

		if (iterations == -1 || iterations >= 1)
		{
			GMSEC_INFO << "Publish interval " << interval_ms << " [ms]";
			GMSEC_INFO << "Message subject '" << subject.c_str() << "'";
		}

		//o Publish message(s)
		for (GMSEC_I32 i = 0; i < iterations || iterations == -1; ++i)
		{
			if (msg.get())
			{
				// Publish custom message
				publish(*(msg.get()));
			}
			else
			{
				GMSEC_U16 counter;

				if (connMgr->getSpecification().getVersion() == mist::GMSEC_ISD_2014_00)
				{
					counter = (GMSEC_U16)((i+1) % 32768);
				}
				else
				{
					counter = (GMSEC_U16)((i+1) % 65536);
				}

				if (counter == 0)
				{
					++counter;  // COUNTER is always 1+
				}

				// Publish canned message
				publish(counter, interval_ms, subject);
			}

			if (i < (iterations - 1) || iterations == -1)
			{
				TimeUtil::millisleep(interval_ms);
			}
		}
	}
	catch (const Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Publisher::publish(Message& message)
{
	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		message.addField("SEC-ENCRYPT", true);
	}

	//o Publish Message
	connMgr->publish(message);

	//o Display XML representation of message
	GMSEC_INFO << "Published:\n" << message.toXML();
}


void GMSEC_Publisher::publish(GMSEC_U16 counter, GMSEC_U32 interval_ms, const std::string& subject)
{
	MistMessage message(subject.c_str(), "MSG.C2CX.HB", connMgr->getSpecification());

	message.setValue("MISSION-ID", "MY-MISSION");
	message.setValue("FACILITY", "MY-FACILITY");
	message.setValue("COMPONENT", "GMPUB");

	if (connMgr->getSpecification().getVersion() == mist::GMSEC_ISD_2014_00)
	{
		std::stringstream oss;
		oss << "MY-MESSAGE-ID-" << counter;

		message.addField("MSG-ID", oss.str().c_str());
	}
	else if (connMgr->getSpecification().getVersion() >= mist::GMSEC_ISD_2018_00)
	{
		message.addField("DOMAIN1", "MY-DOMAIN-1");
		message.addField("DOMAIN2", "MY-DOMAIN-2");
	}

	message.setValue("COUNTER", (GMSEC_I64) counter);
	message.setValue("PUB-RATE", (GMSEC_I64) (interval_ms/1000));

	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		message.addField("SEC-ENCRYPT", true);
	}

	//o Publish Message
	connMgr->publish(message);

	//o Display XML representation of message
	GMSEC_INFO << "Published:\n" << message.toXML();
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	GMSEC_Publisher gmpub(config);

	gmpub.addToConfigFromFile();

	if (gmpub.isOptionInvalid(argc))
	{
		gmpub.usage("gmpub");
		return 1;
	}

	gmpub.run();
}

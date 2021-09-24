/*
 * Copyright 2007-2021 United States Government as represented by the
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


const char* const GMSEC_MESSAGE_SUBJECT        = "SPECIFICATION.MISSION-ID.SAT-ID-PHYSICAL.MESSAGE-TYPE.MESSAGE-SUBTYPE.COMPONENT";
const char* const GMSEC_LEGACY_MESSAGE_SUBJECT = "SPECIFICATION.MISSION-ID.SAT-ID-PHYSICAL.MESSAGE-TYPE.MESSAGE-SUBTYPE.COMPONENT.C2CX-SUBTYPE";

const char* const C2MS_MESSAGE_SUBJECT         = "SPECIFICATION.DOMAIN1.DOMAIN2.MISSION-ID.CONSTELLATION-ID.SAT-ID-PHYSICAL.MESSAGE-TYPE.MESSAGE-SUBTYPE.COMPONENT";
const char* const C2MS_LEGACY_MESSAGE_SUBJECT  = "SPECIFICATION.DOMAIN1.DOMAIN2.MISSION-ID.CONSTELLATION-ID.SAT-ID-PHYSICAL.MESSAGE-TYPE.MESSAGE-SUBTYPE.COMPONENT.C2CX-SUBTYPE";


class GMSEC_Publisher : public Utility
{
public:
	GMSEC_Publisher(const Config& config);

	~GMSEC_Publisher();

	void usage(const std::string& programName);

	bool run();

private:
	void publish(Message& msg);
	void publish(GMSEC_U16 counter, GMSEC_U32 interval_ms, const std::string& subject);

	ConnectionManager* connMgr;
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
			subject = get("SUBJECT", "");
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
	const char* schemaID = "HB";
	const char* subj     = (subject.length() > 0 ? subject.c_str() : "WILL.SET.SUBJECT.LATER");

	MistMessage message(subj, schemaID, connMgr->getSpecification());

	message.setValue("MISSION-ID", "MSSN");
	message.setValue("CONSTELLATION-ID", "CNS1");
	message.setValue("SAT-ID-PHYSICAL", "SAT1");
	message.setValue("SAT-ID-LOGICAL", "SAT1");
	message.setValue("FACILITY", "MY-FACILITY");
	message.setValue("COMPONENT", "GMPUB");

	if (connMgr->getSpecification().getVersion() == mist::GMSEC_ISD_2014_00)
	{
		std::stringstream oss;
		oss << "MY-MESSAGE-ID-" << counter;

		message.setValue("MSG-ID", oss.str().c_str());
	}
	else if (connMgr->getSpecification().getVersion() >= mist::GMSEC_ISD_2018_00)
	{
		message.setValue("DOMAIN1", "DOM1");
		message.setValue("DOMAIN2", "DOM2");
	}

	message.setValue("COUNTER", (GMSEC_I64) counter);
	message.setValue("PUB-RATE", (GMSEC_I64) (interval_ms/1000));

	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		message.addField("SEC-ENCRYPT", true);
	}

	if (subject.empty())
	{
		// Need to set the message's subject based on field contents

		std::string msgSubjPattern;

		Specification::SchemaLevel schemaLevel = connMgr->getSpecification().getSchemaLevel();

		if (connMgr->getSpecification().getVersion() >= GMSEC_ISD_2019_00)
		{
			msgSubjPattern = (schemaLevel == Specification::LEVEL_2 ? GMSEC_MESSAGE_SUBJECT : C2MS_MESSAGE_SUBJECT);
		}
		else
		{
			msgSubjPattern = (schemaLevel == Specification::LEVEL_1 ? GMSEC_LEGACY_MESSAGE_SUBJECT : C2MS_LEGACY_MESSAGE_SUBJECT);
		}

		std::vector<std::string> fieldNames = Utility::split(msgSubjPattern, ".");

		std::string messageSubject;

		for (std::vector<std::string>::iterator it = fieldNames.begin(); it != fieldNames.end(); ++it)
		{
			if (message.hasField(it->c_str()))
			{
				if (connMgr->getSpecification().getVersion() < GMSEC_ISD_2018_00 && *it == "CONSTELLATION-ID")
				{
					continue;
				}
				messageSubject.append(message.getStringValue(it->c_str())).append(".");
			}
			else
			{
				if ((*it == "SPECIFICATION") && (connMgr->getSpecification().getVersion() < GMSEC_ISD_2018_00))
				{
					messageSubject.append("GMSEC.");
				}
			}
		}
		messageSubject = messageSubject.substr(0, messageSubject.length() - 1);

		message.setSubject(messageSubject.c_str());
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

	if (!gmpub.areOptionsValid(argc))
	{
		gmpub.usage("gmpub");
		return 1;
	}

	gmpub.addToConfigFromFile();

	gmpub.run();
}

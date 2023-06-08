/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmpub.cpp
 */

#include "../Utility.h"

#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


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

	Connection* conn;
};


GMSEC_Publisher::GMSEC_Publisher(const Config& c)
	: Utility(c),
	  conn(0)
{
	/* Initialize utility */
	initialize();
}


GMSEC_Publisher::~GMSEC_Publisher()
{
	if (conn)
	{
		try
		{
			conn->disconnect();
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << e.what();
		}

		delete conn;
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
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		conn = new Connection(getConfig());

		//o Connect
		conn->connect();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

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
				subject = get("SUBJECT", "");
			}
			else
			{
				msg.reset(readConfigFile(cfgFile, msgName));

				subject = msg->getSubject();
			}
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
				publish(*msg.get());
			}
			else
			{
				GMSEC_U16 counter;

				if (conn->getMessageFactory().getSpecification().getVersion() == GMSEC_MSG_SPEC_2014_00)
				{
					counter = static_cast<GMSEC_U16>((i+1) % 32768);
				}
				else
				{
					counter = static_cast<GMSEC_U16>((i+1) % 65536);
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
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void GMSEC_Publisher::publish(Message& message)
{
	//o Publish Message
	conn->publish(message, getConfig());

	//o Display XML/JSON representation of message
	Utility::displayMessage("Published:\n", message);
}


void GMSEC_Publisher::publish(GMSEC_U16 counter, GMSEC_U32 interval_ms, const std::string& subject)
{
	// TODO: If a custom subject is provided, then ideally these fields
	// should be derived from it, perhaps going as far as to use
	// FILL for values that cannot be extracted from the subject.
	Utility::setStandardFields( conn->getMessageFactory(), "GMPUB" );

	const char* schemaID = "HB";
	Message message( conn->getMessageFactory().createMessage(schemaID) );

	if (!subject.empty())
	{
		message.setSubject(subject.c_str());
	}

	message.setFieldValue("COUNTER", static_cast<GMSEC_I64>(counter));
	message.setFieldValue("PUB-RATE", static_cast<GMSEC_I64>(interval_ms / 1000));
	message.setFieldValue("SW-VERSION", GMSEC_VERSION_NUMBER);

	//o Publish Message
	conn->publish(message, getConfig());

	//o Display XML/JSON representation of message
	Utility::displayMessage("Published:\n", message);
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	try
	{
		GMSEC_Publisher gmpub(config);

		if (!gmpub.areOptionsValid(argc))
		{
			gmpub.usage("gmpub");
			return 1;
		}

		gmpub.addToConfigFromFile();

		gmpub.run();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
		return 1;
	}
}

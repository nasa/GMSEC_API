/*
 * Copyright 2007-2018 United States Government as represented by the
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

#include <memory>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class GMSEC_Publisher : public Utility
{
public:
	GMSEC_Publisher(const Config& config);

	~GMSEC_Publisher();

	void usage(const std::string& programName);

	bool run();

private:
	Connection* connection;

	void publish(Message& msg);
	void publish(int iteration, const std::string& subject);
};


GMSEC_Publisher::GMSEC_Publisher(const Config& c)
	: Utility(c),
	  connection(0)
{
	/* Initialize utility */
	initialize();
}


GMSEC_Publisher::~GMSEC_Publisher()
{
	if (connection)
	{
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
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(getConfig());

		//o Connect
		connection->connect();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Get information from the command line
		std::string msgFile     = get("MSG-FILE", "");
		std::string cfgFile     = get("CFG-FILE", "");
		int         iterations  = get("ITERATIONS", 1);
		int         interval_ms = get("INTERVAL-MS", 1000);
		std::string subject;

		std::auto_ptr<Message> msg;

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
			subject = get("SUBJECT", "GMSEC.TEST.PUBLISH");
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
		for (int i = 0; i < iterations || iterations == -1; ++i)
		{
			if (msg.get())
			{
				// Publish custom message
				publish(*(msg.get()));
			}
			else
			{
				// Publish canned message
				publish(i, subject);
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
	connection->publish(message);

	//o Display XML representation of message
	GMSEC_INFO << "Published:\n" << message.toXML();
}


void GMSEC_Publisher::publish(int iteration, const std::string& subject)
{
	int value = iteration + 1;

	//o Create message
	Message message(subject.c_str(), Message::PUBLISH);

	//o Add fields to message
	message.addField("CHAR-FIELD", (GMSEC_CHAR) 'c');
	message.addField("BOOL-FIELD-TRUE", true);
	message.addField("BOOL-FIELD-FALSE", false);
	message.addField("I8-FIELD", (GMSEC_I8) value);
	message.addField("I16-FIELD", (GMSEC_I16) value);
	message.addField("I32-FIELD", (GMSEC_I32) value);
	message.addField("I64-FIELD", (GMSEC_I64) value);
	message.addField("U8-FIELD", (GMSEC_U8) value);
	message.addField("U16-FIELD", (GMSEC_U16) value);
	message.addField("U32-FIELD", (GMSEC_U32) value);
	message.addField("U64-FIELD", (GMSEC_U64) value);
	message.addField("COUNT", (GMSEC_I32) iteration);
	message.addField("STRING-FIELD", "This is a test");
	message.addField("F32-FIELD", GMSEC_F32(1 + 1. / (value == 0 ? 1 : value)));
	message.addField("F64-FIELD", GMSEC_F64(1 + 1. / (value == 0 ? 1 : value)));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);

	if (getConfig().getBooleanValue("ENCRYPT", false))
	{
		message.addField("SEC-ENCRYPT", true);
	}

	//o Publish Message
	connection->publish(message);

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

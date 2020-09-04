/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class gmpub
{
public:
	gmpub(Config& config);
	~gmpub();
	bool run();

private:
	Config&     config;
	Connection* connection;

	void publish(int iteration, const std::string& subject);
};


gmpub::gmpub(Config &c)
	: config(c),
	  connection(0)
{
	/* Initialize config */
	example::initialize(c);
}


gmpub::~gmpub()
{
	if (connection)
	{
		connection->disconnect();

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmpub::run()
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

		//o Get information from the command line
		std::string subject     = example::get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
		int         iterations  = example::get(config, "ITERATIONS", 1);
		int         interval_ms = example::get(config, "INTERVAL_MS", 1000);

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Output information
		GMSEC_INFO << "Using subject '" << subject.c_str() << "'";
		GMSEC_INFO << "Publishing " << iterations << " message(s)";

		if (iterations > 1)
		{
			GMSEC_INFO << "Publish interval " << interval_ms << " [ms]";
		}

		//o Publish message(s)
		for (int i = 0; i < iterations; ++i)
		{
			publish(i, subject);

			if (i < iterations - 1)
			{
				TimeUtil::millisleep(interval_ms);
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


void gmpub::publish(int iteration, const std::string& subject)
{
	int value = iteration + 1;

	GMSEC_INFO << "working";

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
	message.addField("F32-FIELD", GMSEC_F32(1 + 1. / value));
	message.addField("F64-FIELD", GMSEC_F64(1 + 1. / value));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);

	if (config.getBooleanValue("ENCRYPT", false))
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

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmpub");
		return -1;
	}

	gmpub(config).run();
}

/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmpub_wsmq_async.cpp
 * 
 * A C++ example demonstration of GMSEC publish/subscribe functionality.
 * The associated example gmsub.cpp will listen for this publish program.
 * gmsub should be run before gmpub_wsmq_async.
 * 
 */

#include "../example.h"

#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


// This callback is user-defined and can be used to handle the Status object returned by this event
class AsyncStatusCheckCallback : public EventCallback
{
public:
	void CALL_TYPE onEvent(Connection& conn, const Status& status, Connection::ConnectionEvent event)
	{
		GMSEC_INFO << status.getReason();

		if (status.isError())
		{
			GMSEC_ERROR << "The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " << status.getCustomCode();
		}
	}
};


class gmpub_wsmq_async
{
public:
	gmpub_wsmq_async(Config& config);
	~gmpub_wsmq_async();
	bool run();

private:
	Config&     config;
	Connection* connection;

	void publish(int iteration, const std::string& subject);
};


gmpub_wsmq_async::gmpub_wsmq_async(Config &c)
	: config(c),
	  connection(0)
{
	/* Initialize config */
	example::initialize(c);
}


gmpub_wsmq_async::~gmpub_wsmq_async()
{
	if (connection)
	{
		connection->disconnect();

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gmpub_wsmq_async::run()
{
	AsyncStatusCheckCallback asccb;	
	bool success = true;

	/* output GMSEC API version */
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		connection = Connection::create(config);

		//o Connect
		connection->connect();

		//o Register the event callback with the connection to catch the WebSphere asynchronous publish status events
		connection->registerEventCallback(Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT, &asccb);

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
				gmsec::api::example::millisleep(interval_ms);
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


void gmpub_wsmq_async::publish(int iteration, const std::string& subject)
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
		example::printUsage("gmpub_wsmq_async");
		return -1;
	}

	gmpub_wsmq_async(config).run();
}

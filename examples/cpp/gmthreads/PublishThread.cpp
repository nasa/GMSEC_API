/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file PublishThread.cpp
 * 
 * This file contains a simple example publisher using the GMSEC standard C++ API.
 * 
 */

#include "PublishThread.hpp"

#include <string>

using namespace gmsec::api;


PublishThread::PublishThread(Config &c)
	: config(c),
	  connection(0)
{
	example::initialize(config);
}


PublishThread::~PublishThread()
{
	teardown();
}


void CALL_TYPE PublishThread::run()
{
	try
	{
		setup();

		//o Output GMSEC API and middleware version information
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << connection->getLibraryVersion();

		//o Delay to allow subscriber thread to set up
		example::millisleep(100);

		//o Publish messages using various subjects/topics
		publish("GMSEC.TEST.PUBLISH");
		publish("GMSEC.TEST1.PUBLISH");
		publish("GMSEC.TEST2.PUBLISH");
		publish("GMSEC.TEST2.PUBLISH.ANOTHER");
		publish("GMSEC.TEST.QUIT");

		teardown();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << "Failure: " << e.what();
	}
}


void PublishThread::publish(const char* subject)
{
	//o Create message
	Message message(subject, Message::PUBLISH);

	//o Add fields
	int i = 123;

	message.addField("CHAR-FIELD", (GMSEC_CHAR) 'c');
	message.addField("BOOL-FIELD", false);
	message.addField("I32-FIELD", (GMSEC_I32) i);
	message.addField("U16-FIELD", (GMSEC_U16) i);
	message.addField("STRING-FIELD", "This is a test");
	message.addField("F64-FIELD", GMSEC_F64(123 / .1));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);

	//o Publish Message
	connection->publish(message);

	//o Display XML representation of message
	GMSEC_INFO << "Published:\n" << message.toXML();
}


void PublishThread::setup()
{
	//o Create the Connection
	connection = Connection::create(config);

	//o Connect to the GMSEC bus
	connection->connect();
}


void PublishThread::teardown()
{
	if (connection)
	{
		connection->disconnect();
		Connection::destroy(connection);
	}
}

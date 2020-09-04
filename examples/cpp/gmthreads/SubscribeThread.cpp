/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file SubscribeThread.cpp
 *  
 * This file contains a simple example subscriber using the GMSEC standard C++ API.
 * 
 */

#include "SubscribeThread.hpp"

#include <string>

using namespace gmsec::api;


SubscribeThread::SubscribeThread(Config &c)
	: config(c),
	  connection(0)
{
	example::initialize(config);
}


SubscribeThread::~SubscribeThread()
{
	teardown();
}


void CALL_TYPE SubscribeThread::run()
{
	try
	{
		setup();

		//o Output API and middelware version information
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << connection->getLibraryVersion();

		//o Listen
		bool done = false;

		while (!done)
		{
			Message* message = connection->receive(GMSEC_WAIT_FOREVER);

			if (!message)
			{
				GMSEC_WARNING << "Timeout occurred";
				continue;
			}

			// Display XML representation of the message
			GMSEC_INFO << "Received:\n" << message->toXML();

			// Examine the message's subject to check if we should quit
			std::string subject = message->getSubject();

			done = (subject == "GMSEC.TEST.QUIT");

			connection->release(message);
		}

		GMSEC_INFO << "SubscribeThread GMSEC.TEST.QUIT message found.";

		teardown();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << "Failure: " << e.what();
	}
}


void SubscribeThread::setup()
{
	//o Create the Connection
	connection = Connection::create(config);

	//o Connect
	connection->connect();

	//o Subscribe
	info = connection->subscribe("GMSEC.>");
}


void SubscribeThread::teardown()
{
	if (connection)
	{
		GMSEC_INFO << "Unsubscribing from " << info->getSubject();
		connection->unsubscribe(info);
		connection->disconnect();
		Connection::destroy(connection);
	}
}

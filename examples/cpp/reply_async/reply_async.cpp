/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_async.cpp
 * 
 * This file contains an example demonstrating how to asynchronously reply to a
 * request message.
 *
 * Asynchronous receipt operations essentially mean that a Callback subclass is
 * implemented, then the corresponding Callback object is registered to a
 * subscription, and that messages are received asynchronously after starting
 * the AutoDispatcher. In this case, replies are created and sent asynchronously
 * when receiving request messages.
 *
 * This example program is intended to be run before starting up either the
 * 'request' or 'request_async' example program.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper function
void initializeLogging(Config& config);

//o Globals
bool replySent = false;


class ExampleCallback : public ConnectionManagerCallback
{
public:
	virtual void CALL_TYPE onMessage(ConnectionManager& connMgr, const Message& message)
	{
		try
		{
			//o Display the XML representation of the received message
			GMSEC_INFO << "[ExampleCallback:onMessage] Received:\n" << message.toXML();

			//o Construct Reply subject.
			const char* component = 0;

			try
			{
				component = message.getStringField("COMPONENT").getValue();
			}
			catch (Exception& e)
			{
				GMSEC_WARNING << "COMPONENT field is not available: " << e.what();
			}

			//o Set Status Code to indicate Successful Completion.
			// The GMSEC Interface Specification Document defines 6
			// unique STATUS-CODE values:
			// 1 - Acknowledgement
			// 2 - Working/keep alive
			// 3 - Successful completion
			// 4 - Failed completion
			// 5 - Invalid request
			// 6 - Final message
			// If an asynchronous requestor is awaiting a reply, the
			// ReplyCallback in use will remain active for multiple
			// messages, so long as the messages it receives contain
			// a STATUS-CODE of either 1 or 2.
			const char* status_code = "3";

			//o Create the reply subject.
			// See API Interface Specificaiton Document for
			// more information on Reply Message subjects.
			// Generally speaking, they are composed
			// accordingly:
			// [Spec].[Mission].[Satellite ID].
			// [Message Type].[Component Name].[Status Code]
			std::ostringstream reply_subject;
			reply_subject << "GMSEC.MISSION.SAT_ID.RESP.REPLY_ASYNC." << status_code;

			Message reply(reply_subject.str().c_str(), Message::REPLY);

			//o Add fields to the reply message
			reply.addField("COMPONENT", component);
			reply.addField("ANSWER", "Sure looks like it!");

			//o Display XML representation of reply message
			GMSEC_INFO << "Prepared Reply:\n" << reply.toXML();

			//o Send Reply
			connMgr.reply(message,reply);

			replySent = true;
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << "[ExampleCallback::onMessage] " << e.what();
		}
	}
};


int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which is used to
	// pass configuration options into objects such as Connections,
	// ConnectionManagers, Subscribe and Publish function calls, Messages,
	// etc.
	Config config(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	config.addValue("gmsec-msg-content-validate-all", "false");

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);

	//o Output GMSEC API version
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Connect
		connMgr.initialize();

		//o output connection middleware version
		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Subscribe
		ExampleCallback cb;
		connMgr.subscribe(DEFAULT_REQUEST_SUBJECT, &cb);

		//o Start the AutoDispatcher to begin async message receipt
		connMgr.startAutoDispatch();

		GMSEC_INFO << "Awaiting request message from 'request' or 'request_async' example programs...";

		//o Loop while waiting for the asynchronous response until done
		while (replySent == 0)
		{
			TimeUtil::millisleep(100);
		}

		//o Wait for a few moments to ensure that the asynchronous reply
		// transaction went through with the middleware before closing
		// the connection and exiting the process
		for (int i = 0; i < 5; i++)
		{
			TimeUtil::millisleep(100);
		}

		//o Clean up
		connMgr.stopAutoDispatch();
		connMgr.cleanup();

	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}


void initializeLogging(Config& config)
{
	const char* logLevel  = config.getValue("LOGLEVEL");
	const char* logFile   = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}


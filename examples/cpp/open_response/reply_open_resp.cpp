/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file reply_open_resp.cpp
 * 
 * This file contains an example demonstrating how to utilize the open
 * response functionality to expose reply messages to normal subscribers.
 *
 * This example program is intended to be run before executing the
 * 'request_open_resp' example program.
 *
 * It is also recommended that you run a subscriber application
 * (i.e. GMSEC_API/bin/gmsub) with the configuration option 'mw-expose-resp'
 * set to true in order to see how any subscriber can receive reply messages
 * while using the open-response functionality.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* OPEN_RESP_REQUEST_SUBJECT = "GMSEC.MISSION.SAT_ID.RESP.REQUEST_OPENRESP";
const char* OPEN_RESP_REPLY_SUBJECT   = "GMSEC.MISSION.SAT_ID.RESP.REPLY_OPENRESP";

void initializeLogging(Config& config);


int main(int argc, char* argv[])
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

	//o Ensure that the open-response is enabled
	config.addValue("GMSEC-REQ-RESP", "OPEN-RESP");

	//o Output GMSEC API version
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Open the connection to the middleware
		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		//o Output middleware client/wrapper version
		GMSEC_INFO << connMgr.getLibraryVersion();

		connMgr.subscribe(OPEN_RESP_REQUEST_SUBJECT);

		//o Call receive() to synchronously retrieve a message that has
		// been received by the middleware client libraries
		// Timeout periods:
		// -1 - Wait forever
		//  0 - Return immediately
		// >0 - Time in milliseconds before timing out
		Message* requestMsg = connMgr.receive(-1);

		// Example error handling for calling receive() with a timeout
		if (requestMsg)
		{
			//o Display the XML representation of the received message
			GMSEC_INFO << "Received a message\n" << requestMsg->toXML();

			//o Double-check the Message type to ensure that it is a request
			if (requestMsg->getKind() == Message::REQUEST)
			{
				//o Get the name of the component who issued the request
				const char* component = 0;

				//o Construct a Reply message
				try
				{
					const StringField& compField = requestMsg->getStringField("COMPONENT");
					component = compField.getValue();
				}
				catch (Exception& e)
				{
					GMSEC_WARNING << e.what();
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

				//o Set the reply subject.
				// See API Interface Specificaiton Document for
				// more information on Reply Message subjects.
				// Generally speaking, they are composed
				// accordingly:
				// [Spec].[Mission].[Satellite ID].
				// [Message Type].[Component Name].[Status Code]
				std::ostringstream reply_subject;
				reply_subject << OPEN_RESP_REPLY_SUBJECT << "." << status_code;

				//o Create reply message
				Message replyMsg(reply_subject.str().c_str(), Message::REPLY);

				//o Add fields to the reply message
				replyMsg.addField("ANSWER", "Yup, I'm here!");
				replyMsg.addField("COMPONENT", component);

				//o Display XML representation of the reply message
				GMSEC_INFO << "Prepared Reply:\n" << replyMsg.toXML();

				//o Send Reply
				connMgr.reply(*requestMsg, replyMsg);

				//o Wait for a moment to ensure that the reply is sent out
				for (int i = 0; i < 10; i++)
				{
					TimeUtil::millisleep(100);
				}
			}

			//o Destroy request message to release its memory
			connMgr.release(requestMsg);
		}
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


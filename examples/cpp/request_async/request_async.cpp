/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file request_async.cpp
 * 
 * This file contains an example demonstrating how to asynchronously handle the
 * receipt of reply message(s) after issuing a request.
 * Asynchronous request operations essentially mean that a Callback subclass is
 * implemented, then a corresponding Callback object is supplied when invoking
 * request. Reply messages that are then received asynchronously and handled by
 * the Callback object.
 *
 * This example program is intended to be run after either starting up the
 * 'reply' or 'reply_async' example program.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* DEFAULT_REQUEST_SUBJECT = "GMSEC.TEST.REQUEST";

//o Helper functions
void initializeLogging(Config& config);

//o Globals
bool receivedReply = false;


class ExampleReplyCallback: public ConnectionManagerReplyCallback
{
public:
	virtual void CALL_TYPE onReply(ConnectionManager& connection, const Message& request, const Message& reply)
	{
		//o Display XML representation of reply message
		GMSEC_INFO << "[ExampleReplyCallback::onReply]\n" << reply.toXML();

		receivedReply = true;
	}

	virtual void CALL_TYPE onEvent(ConnectionManager& connection, const Status& status, Connection::ConnectionEvent event)
	{
		if (status.isError())
		{
			GMSEC_ERROR << "[ExampleReplyCallback::onError] " << status.get() << ", event=" << event;
		}
	}
};


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	config.addValue("gmsec-msg-content-validate-all", "false");

	// If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);

	//o Output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Connect
		connMgr.initialize();

		//o output connection middleware version
		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Create request message
		Message requestMsg(DEFAULT_REQUEST_SUBJECT, Message::REQUEST);

		//o Add fields to request message
		requestMsg.addField("QUESTION", "Does the request/reply functionality still work?");
		requestMsg.addField("COMPONENT", "request_async");

		//o Display XML representation of request message
		GMSEC_INFO << "Requesting:\n" << requestMsg.toXML();

		ExampleReplyCallback cb;
		connMgr.request(requestMsg, -1, &cb);

		GMSEC_INFO << "Waiting for response...";

		// Loop while waiting for the asynchronous response until done
		while (receivedReply == 0)
		{
			TimeUtil::millisleep(100);
		}

		if (receivedReply)
		{
			GMSEC_INFO << "Response Received!";
		}
		else
		{
			GMSEC_WARNING << "No response received";
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


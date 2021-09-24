/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file websphere_publish_async.cpp
 * 
 * This file contains an example which demonstrates how to utilize the
 * WebSphere MQ asynchronous publish functionality, as well as how to check on
 * the status of publish operations (Since the client does not wait to receive
 * the status of each operation in order to achieve asynchronicity) by
 * registering an EventCallback with the ConnectionManager.
 *
 * It is recommended that users execute the program GMSEC_API/bin/gmsub to
 * receive the messages that this example publishes.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

const char* EXAMPLE_MESSAGE_SUBJECT = "GMSEC.TEST.PUBLISH";

//o Helper functions
void initializeLogging(Config& config);
void populateMessage(Message& message, int i);

//o Globals
bool eventFired = false;

//o Define an EventCallback to report the status of WebSphere async publications
// Note: The WebSphere MQ client libraries only report the first warning or
// failure code encountered during the series of message publications made
// since the last time that the client libraries retrieved the status of
// asynchronous publish operations.
class AsyncStatusCheckCallback : public ConnectionManagerEventCallback
{
public:
	void CALL_TYPE onEvent(ConnectionManager& connMgr, const Status& status, Connection::ConnectionEvent event)
	{
		// Print the status of publish operations.  This includes counts
		// for successes, warnings, and errors.
		GMSEC_INFO << status.getReason();

		if (status.isError())
		{
			GMSEC_ERROR << "The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " << status.getCustomCode();
		}

		eventFired = true;
	}
};


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	AsyncStatusCheckCallback cb;	

	//o Load the command-line input into a GMSEC Config object
	Config config(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	config.addValue("gmsec-msg-content-validate-all", "false");

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);

	const char* mwID = config.getValue("mw-id");

	if (mwID == NULL || std::string(mwID).find("websphere") == std::string::npos)
	{
		GMSEC_ERROR << "This example program will only run an mw-id set to a websphere wrapper (e.g. mw-id=ibmmq90)";
		return -1;
	}

	//o Toggle the WebSphere MQ client libraries-level asynchronous publish
	// functionality on
	config.addValue("MW-ASYNC-PUBLISH", "true");

	//o Toggle the periodic WebSphere MQ asynchronous publish status report
	// functionality on.  The GMSEC API Connection will periodically fire
	// off an Event which will trigger the onEvent function of an
	// EventCallback registered to the event labeled,
	// Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT
	config.addValue("MW-ASYNC-STATUS-CHECK", "true");

	//o Additionally, the "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL"
	// configuration option can be used to instruct the GMSEC Connection
	// on how frequently (in number of publish operations) it should
	// periodically fire the WSMQ_ASYNC_STATUS_CHECK_EVENT. By default, it
	// will fire once every 100 messages.
	// For the sake of this example, we will use 500 messages. 
	config.addValue("MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL", "500");

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the Connection
		ConnectionManager connMgr(config);

		//o Connect
		connMgr.initialize();

		//o Register the event callback with the connection to catch
		// the WebSphere asynchronous publish status events which are
		// eminated from the API
		connMgr.registerEventCallback(Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT, &cb);

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connMgr.getLibraryVersion();

		GMSEC_INFO << "Publishing messages using the subject: " << EXAMPLE_MESSAGE_SUBJECT;

		//o Create a GMSEC Message object
		Message message(EXAMPLE_MESSAGE_SUBJECT, Message::PUBLISH);

		//o Publish message as quickly as possible
		// (i.e. No sleep operation between each publish operation)
		int count = 0;
		while (!eventFired)
		{
			//o Populate the Message with fields, increment a
			// counter so that a publisher can track how many
			// messages were published (if they are interested)
			populateMessage(message, count++);

			//o Publish the message to the middleware bus
			connMgr.publish(message);

			// Note: We do not recommend printing the output of a
			// message when publishing it while using the WebSphere
			// async publish functionality, as it is
			// counter-intuitive to take take up CPU resources
			// performing I/O operations when attempting to achieve
			// high message throughput rates. As such, if you want
			// to analyze the messages published by this program,
			// we recommend you execute GMSEC_API/bin/gmsub to
			// receive the messages.
		}

		GMSEC_INFO << "Event detected, ending example demonstration";

		//o Clean up the ConnectionManager before exiting the program
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


void populateMessage(Message& message, int value)
{
	// Note: Since C++ is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
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
	message.addField("STRING-FIELD", "This is a test");
	message.addField("F32-FIELD", GMSEC_F32(1 + 1. / value));
	message.addField("F64-FIELD", GMSEC_F64(1 + 1. / value));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);
}


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request_async.cpp
 *
 * @brief This example program provides a basic demonstration of issuing
 * an asynchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message(s) via a
 * ReplyCallback.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void setStandardFields(MessageFactory& factory, const char* component);


class MyReplyCallback : public ReplyCallback
{
public:
	MyReplyCallback()
		: responseComplete(false)
	{
	}

	void onReply(Connection& conn, const Message& reqMsg, const Message& repMsg)
	{
		GMSEC_INFO << "MyReplyCallback onReply:\n" << repMsg.toXML();

		if (repMsg.hasField("RESPONSE-STATUS"))
		{
			GMSEC_I32 status = repMsg.getI32Value("RESPONSE-STATUS");

			switch ( static_cast<Message::ResponseStatus>(status) )
			{
			case Message::ResponseStatus::ACKNOWLEDGEMENT:
			case Message::ResponseStatus::WORKING_KEEP_ALIVE:
				responseComplete = false;
				break;

			case Message::ResponseStatus::SUCCESSFUL_COMPLETION:
			case Message::ResponseStatus::FAILED_COMPLETION:
			case Message::ResponseStatus::INVALID_REQUEST:
			case Message::ResponseStatus::FINAL_MESSAGE:
				responseComplete = true;
				break;
			}
		}
		else
		{
			responseComplete = true;
		}

		// Acknowledge processing the response message
		repMsg.acknowledge();
	}

	void onEvent(Connection& conn, const Status& status, Connection::Event event)
	{
		if (status.hasError())
		{
			GMSEC_ERROR << "MyReplyCallback onEvent: " << status.get() << ", event: " << static_cast<int>(event);
		}
		else
		{
			GMSEC_INFO << "MyReplyCallback onEvent: " << status.get() << ", event: " << static_cast<int>(event);
		}
	}

	bool isResponseComplete() const
	{
		return responseComplete;
	}

private:
	bool responseComplete;
};


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to and as they are received from the bus
	config.addValue("gmsec-msg-content-validate-all", "true");

	// IMPORTANT! This config option must be set to receive multiple response messages
	config.addValue("mw-multi-resp", "true");

	// Define instance of MyReplyCallback
	MyReplyCallback rcb;

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "REQUEST-ASYNC");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a Request Directive message using the MessageFactory.
		// Set both required and optional fields within the message.
		Message reqMsg = conn->getMessageFactory().createMessage("REQ.DIR");
		reqMsg.addField(StringField("DESTINATION-COMPONENT", "REPLY-MULTI", true));   // to whom is the request intended for
		reqMsg.addField("DIRECTIVE-STRING", "Turn on the lights");                    // operation to perform
		reqMsg.addField("RESPONSE", true);                                            // a response message is requested
		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(0));

		// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
		// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
		// prevent re-sending request message should a timeout occur.
		GMSEC_I32 timeout   = 5000;
		GMSEC_I32 republish = 5000;

		// Issue Request Directive.
		// Note, this is a non-blocking call (i.e. an asynchronous operation).
		conn->request(reqMsg, timeout, &rcb, republish);

		// Output in XML what we have sent
		GMSEC_INFO << "Sending Request Directive Message:\n" << reqMsg.toXML();
		GMSEC_INFO << "Use reply_multi example program to issue response messages";

		// Normally an application would continue with other operation, however for
		// demonstration purposes, we will loop until it has been verified that a
		// response message has been received.
		while (!rcb.isResponseComplete())
		{
			GMSEC_INFO << "Waiting to receive response message...";

			TimeUtil::millisleep(1000);
		}

		// Disconnect from the GMSEC Bus, and terminate subscriptions.
		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		// If here, the most likely case is the middleware could not be
		// deduced, or a failure occurred when attempting to connect to
		// the GMSEC Bus.
		GMSEC_ERROR << "GmsecException: " << e.what();
	}

	conn.reset();

	Connection::shutdownAllMiddlewares();
}


// Convenience function for associating standard (i.e. common) fields
// with the MessageFactory which will be included automatically within
// all messages created using the factory.
void setStandardFields(MessageFactory& factory, const char* component)
{
	using namespace gmsec::api5::util;

	StringField field1("MISSION-ID", "MY-MISSION", true);
	StringField field2("SAT-ID-PHYSICAL", "MY-SAT-ID", true);
	StringField field3("SAT-ID-LOGICAL", "MY-SAT-ID", true);
	StringField field4("CONSTELLATION-ID", "MY-CNST", true);
	StringField field5("FACILITY", "MY-FACILITY", true);
	StringField field6("DOMAIN1", "MY-DOMAIN-1", true);
	StringField field7("DOMAIN2", "MY-DOMAIN-2", true);
	StringField field8("COMPONENT", component, true);

	List<Field*> standardFields;
	standardFields.push_back(&field1);
	standardFields.push_back(&field2);
	standardFields.push_back(&field3);
	standardFields.push_back(&field4);
	standardFields.push_back(&field5);
	standardFields.push_back(&field6);
	standardFields.push_back(&field7);
	standardFields.push_back(&field8);

	factory.setStandardFields(standardFields);
}

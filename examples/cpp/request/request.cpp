/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request.cpp
 *
 * @brief This example program provides a basic demonstration of issuing
 * a synchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>

using namespace gmsec::api5;


void setStandardFields(MessageFactory& factory, const char* component);


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before being sent to and as they are received from the bus
	config.addValue("gmsec-msg-content-validate-all", "true");

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "REQUEST");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a Request Directive message using the MessageFactory.
		// Set both required and optional fields within the message.
		Message reqMsg = conn->getMessageFactory().createMessage("REQ.DIR");
		reqMsg.addField(StringField("DESTINATION-COMPONENT", "REPLY", true));  // to whom is the request intended for
		reqMsg.addField("DIRECTIVE-STRING", "Turn on the lights");             // operation to perform
		reqMsg.addField("RESPONSE", true);                                     // a response message is requested
		reqMsg.addField("REQUEST-ID", static_cast<GMSEC_U16>(0));

		// Output in XML what we intend to send
		GMSEC_INFO << "Sending Request Message:\n" << reqMsg.toXML();

		// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
		// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
		// prevent re-sending request message should a timeout occur.
		GMSEC_I32 timeout   = 5000;
		GMSEC_I32 republish = 5000;

		// Issue Request Directive.
		// Note, this is a blocking call (i.e. a synchronous operation).
		Message* rspMsg = conn->request(reqMsg, timeout, republish);

		// We have returned from the previous blocking call; check if we have a message.
		if (rspMsg)
		{
			GMSEC_INFO << "Received Response Directive message:\n" << rspMsg->toXML();

			// Acknowledge processing the response message
			rspMsg->acknowledge();

			// Release resources associated with the received message.
			Message::destroy(rspMsg);
		}
		else
		{
			// This else-block is reached if a response message is not received.
			// If the republish period is set to a time period other than
			// GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
			GMSEC_WARNING << "Time out; no Response Directive message received";
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

	conn.release();

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

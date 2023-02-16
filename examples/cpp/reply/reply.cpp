/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file reply.cpp
 *
 * @brief This example program provides a basic demonstration of synchronously
 * receiving a Request Directive message from the GMSEC Bus, and then issuing
 * a Response Directive message (should one be requested).
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

	// Define subscription topic for receiving request message(s).
	const char* subscription_topic;
	int level = config.getIntegerValue("gmsec-schema-level", static_cast<GMSEC_I32>(Specification::SchemaLevel::LEVEL_0));

	switch ( static_cast<Specification::SchemaLevel>(level) )
	{
	case Specification::SchemaLevel::LEVEL_0:
		subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
		break;

	case Specification::SchemaLevel::LEVEL_1:
		subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY";
		break;

	case Specification::SchemaLevel::LEVEL_2:
	default:
		subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY";
		break;
	}

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance
		conn.reset( new Connection(config) );

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "REPLY");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Set up our subscription for receiving a request message.
		conn->subscribe(subscription_topic);
		GMSEC_INFO << "Subscribed to topic: " << subscription_topic;

		// Timeout was arbitrarily chosen as GMSEC_WAIT_FOREVER so that
		// we block indefinitely waiting to receive a request message.
		// It can be set to other time values (e.g. 1000 is 1 second) so
		// the application does not block indefinitely waiting for a
		// request message.
		GMSEC_I32 timeout = GMSEC_WAIT_FOREVER;

		// Block for a Request Directive message to arrive.
		Message* reqMsg = conn->receive(timeout);

		// Check if we have received a message.
		if (reqMsg)
		{
			// Output in XML the received message.
			GMSEC_INFO << "Received Request Message:\n" << reqMsg->toXML();

			// Check if a response message should be returned.
			if (reqMsg->hasField("RESPONSE") && reqMsg->getBooleanValue("RESPONSE"))
			{
				// Create a Response Directive message using the MessageFactory.
				Message rspMsg = conn->getMessageFactory().createMessage("RESP.DIR");

				// Insert required and optional fields into the Response Directive
				// using values derived from the incoming Request Directive.
				if (reqMsg->hasField("COMPONENT"))
				{
					rspMsg.addField(StringField("DESTINATION-COMPONENT", reqMsg->getStringValue("COMPONENT"), true));
				}
				if (reqMsg->hasField("REQUEST-ID"))
				{
					rspMsg.addField(*reqMsg->getField("REQUEST-ID"));
				}

				// Set the response status for the request.
				rspMsg.setFieldValue("RESPONSE-STATUS", static_cast<GMSEC_I32>(Message::ResponseStatus::SUCCESSFUL_COMPLETION));

				// Send the Response Directive message.
				conn->reply(*reqMsg, rspMsg);

				// Output in XML the message that has been sent.
				GMSEC_INFO << "Sent Response Message:\n" << rspMsg.toXML();
			}

			// Acknowledge processing the request message
			reqMsg->acknowledge();

			// Release resources associated with the received message.
			Message::destroy(reqMsg);
		}
		else
		{
			// This else-block is reached if a reuqest message is not received.
			// When the timeout period is set to GMSEC_WAIT_FOREVER, then this
			// block would never be reached.
			GMSEC_WARNING << "Time out; no Request Directive message received";
		}

		// Disconnect from the GMSEC Bus, and terminate subscriptions.
		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		// If here, the most likely case is the middleware could not be
		// deduced, or a failure occurred when attempting to connect toi
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

/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file custom_validation.cpp
 *
 * @brief This example program demonstrates how to implement additional
 * message validation logic in addition to that which the GMSEC API
 * provides.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>
#include <string>

using namespace gmsec::api5;


void setStandardFields(MessageFactory& factory, const char* component);


// Define a custom message validator
class CustomMessageValidator : public MessageValidator
{
public:
	Status validateMessage(const Message& msg)
	{
		Status status;

		// Check if we have a Heartbeat message, and if so, then check if
		// the PUB-RATE has an acceptable value.
		if (msg.hasField("MESSAGE-SUBTYPE") &&
		    std::string("HB") == msg.getStringValue("MESSAGE-SUBTYPE"))
		{
			if (msg.hasField("PUB-RATE"))
			{
				GMSEC_I32 pubRate = msg.getI32Value("PUB-RATE");

				if (pubRate < 10 || pubRate > 60)
				{
					status.set(MSG_ERROR, VALUE_OUT_OF_RANGE,
					           "PUB-RATE field does not have a valid value");
				}
			}
			else
			{
				status.set(MSG_ERROR, MISSING_REQUIRED_FIELD, "PUB-RATE field not found");
			}
		}

		return status;
	}
};


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Validate messages before they are sent to the bus
	config.addValue("gmsec-msg-content-validate-send", "true");

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "CUSTOM-VALIDATION");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Register custom message validator
		CustomMessageValidator cmv;
		conn->getMessageFactory().registerMessageValidator(&cmv);

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a Heartbeat message using the MessageFactory.
		// Set both required and optional fields within the message.
		Message hbMsg = conn->getMessageFactory().createMessage("HB");

		// Publish the message without PUB-RATE (expecting custom validation error)
		try {
			GMSEC_INFO << "Publishing Message:\n" << hbMsg.toXML();
			conn->publish(hbMsg);

			GMSEC_WARNING << "An error was expected!";
		}
		catch (const GmsecException& e) {
			GMSEC_INFO << "This error is expected: " << e.what();
		}

		// Publish the message with invalid PUB-RATE value (expecting custom validation error)
		try {
			hbMsg.setFieldValue("PUB-RATE", static_cast<GMSEC_I32>(5));

			GMSEC_INFO << "Publishing Message:\n" << hbMsg.toXML();
			conn->publish(hbMsg);

			GMSEC_WARNING << "An error was expected!";
		}
		catch (const GmsecException& e) {
			GMSEC_INFO << "This error is expected: " << e.what();
		}

		// Publish the message with valid PUB-RATE value (expecting no validation error)
		try {
			hbMsg.setFieldValue("PUB-RATE", static_cast<GMSEC_I32>(10));

			GMSEC_INFO << "Publishing Message:\n" << hbMsg.toXML();
			conn->publish(hbMsg);

			GMSEC_INFO << "Message is valid!";
		}
		catch (const GmsecException& e) {
			GMSEC_ERROR << e.what();
		}

		// Disconnect from the GMSEC Bus.
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

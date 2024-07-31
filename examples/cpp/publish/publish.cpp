/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file publish.cpp
 *
 * @brief This example program provides a basic demonstration of publishing
 * a (heartbeat) message to the GMSEC Bus.
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

	// Validate messages before they are sent to the bus
	config.addValue("gmsec-msg-content-validate-send", "true");

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "PUBLISH");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a Heartbeat message using the MessageFactory.
		Message hbMsg = conn->getMessageFactory().createMessage("HB");

		// Publish the message
		conn->publish(hbMsg);

		// Output in XML what we have published
		GMSEC_INFO << "Publishing Message:\n" << hbMsg.toXML();

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

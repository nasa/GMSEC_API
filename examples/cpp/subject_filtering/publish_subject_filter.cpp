/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file publish_subject_filter.cpp
 * 
 * This file contains an example publishing a few messages with different
 * subjects in order to demonstrate the subject filtering functionality of the
 * API. 
 *
 * This example program is intended to be run after starting the
 * 'subscribe_subject_filter' example program.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>
#include <string>
#include <iostream>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


// Helper function
void setStandardFields(MessageFactory& factory, const char* component);


int main(int argc, char** argv)
{
	// Set up connection configuration options.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "PUBLISH-SUBJECT-FILTER");

		// Establish connection to the GMSEC Bus.
		conn->connect();
		
		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a valid Log message using the MessageFactory.
		Message logMsg = conn->getMessageFactory().createMessage("LOG");

		// Add required fields
		logMsg.addField("SUBCLASS", "INFO");
		logMsg.addField("OCCURRENCE-TYPE", "TEST");
		logMsg.addField("SEVERITY", static_cast<GMSEC_I16>(1));
		logMsg.addField("MSG-TEXT", "Hello subscriber!");

		// And the current (event) time
		GMSEC_Time theTime;
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		logMsg.addField("EVENT-TIME", theTime);

		// Create a valid Heartbeat message using the MessageFactory.
		Message hbMsg = conn->getMessageFactory().createMessage("HB");

		GMSEC_INFO << "Publishing two messages -- Log message will be received by the subscriber,"
		           << " the Heartbeat message will be filtered out";

		// Publish the Log message
		conn->publish(logMsg);
		GMSEC_INFO << "Published LOG message:\n" << logMsg.toXML();

		// Delay a bit
		TimeUtil::millisleep(2000);

		// Publish the Heartbeat message
		conn->publish(hbMsg);
		GMSEC_INFO << "Published HB message:\n" << hbMsg.toXML();

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

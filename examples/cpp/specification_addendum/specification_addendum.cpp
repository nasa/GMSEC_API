/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file specification_addendum.cpp
 * 
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>
#include <string>
#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


// Helper functions
void setStandardFields(MessageFactory& factory, const char* component);
std::string addTimeToString(const char* preface);


int main(int argc, char** argv)
{
	// Set up connection configuration options from the command line.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);


	// Enable Message validation. This setting is "false" by default.
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE-SEND", "true");


	// Tell the API that there is an additional layer of message schema to
	// validate (The 'EXAMPLE' message definitions). By default, this value
	// is set to 0 (only 'C2MS' specification validation).
	//
	// Note: These levels for validation are determined by the "LEVEL-X"
	// attributes defined in the DIRECTORY.xml file contained in the XML
	// templates directory. In the case of this example, Level-0 implies
	// C2MS, and Level-1 is our example extension.
	//
	// Note: The GMSEC team envisions using message specifications in a
	// layered hierarchical fashion. For example, the "C2MS" message
	// specification would be 'layer 0', followed by an organization-level
	// message specification at 'layer 1' which builds upon the message
	// specification outlined in C2MS.  This would be followed by a mission
	// level message specification at 'layer 2' (e.g. GMSEC) and so on.
	config.addValue("GMSEC-SCHEMA-LEVEL", "1");


	// Tell the API where to find all of the schema files.
	//
	// Note: This example only demonstrates a simple usage case of this
	// functionality. When using this functionality, if the intent is
	// to use any of the GMSEC message definitions, then ALL of the XML
	// template files must be contained in the same directory.
	// e.g. GMSEC_API/templates/2019.00 (or the location defined in
	// GMSEC-SCHEMA-PATH)
	config.addValue("GMSEC-SCHEMA-PATH", "templates");


	// Since this example relies on the 2019.00 version of the templates,
	// we indicate such within the configuration object.
	config.addValue("GMSEC-SPECIFICATION-VERSION", "201900");


	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance using custom configuration.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "SPECIFICATION-ADDENDUM");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Output information regarding the API version and connection.
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();


		// Create a LOG Message using a subject defined in the XML template
		// outlining our example addendum message definitions.
		Message logMsg = conn->getMessageFactory().createMessage("LOG");

		// Add all of the required fields to the LOG message.
		logMsg.addField("NUM-OF-EVENTS", static_cast<GMSEC_U16>(2));
		logMsg.addField("EVENT.1.DESC", addTimeToString("AOS occurred at: ").c_str());
		logMsg.addField("EVENT.2.DESC", addTimeToString("Telemetry downlink began at: ").c_str());

		// Publish the LOG message to the GMSEC Bus.
		conn->publish(logMsg);

		// Display the XML string representation of the LOG message just published.
		GMSEC_INFO << "Published custom LOG Message:\n" << logMsg.toXML();


		// Setup a new LOG message without the NUM-OF-EVENTS required field and
		// attempt to publish it (i.e. trigger a validation failure)
		Message badLogMsg = conn->getMessageFactory().createMessage("LOG");

		try
		{
			conn->publish(badLogMsg);
			GMSEC_WARNING << "Surprisingly was able to publish an invalid message:\n" << badLogMsg.toXML();
		}
		catch (const GmsecException& e)
		{
			GMSEC_INFO << "This validation error is expected for the incomplete LOG message:\n" << e.what();
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


std::string addTimeToString(const char* preface)
{
	GMSEC_Time theTime;
	TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);

	std::ostringstream oss;
	oss << preface << theTime;

	return oss.str();
}

/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_bins.cpp
 *
 * @brief This file contains an example describing how to aggregate multiple
 * messages into a singular message sent to the middleware server. This
 * functionality is useful for reducing the number of messages sent across
 * the * middleware bus, if there is an application which is publishing an
 * exorbitantly large number of (small) messages.
 *
 * It is recommended to execute the application GMSEC_API/bin/gmsub prior
 * to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void setStandardFields(MessageFactory& factory, const char* component);


int main(int argc, char** argv)
{
	// Set up connection configuration options, some from the command line,
	// others hard-coded.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subject and excluded subject for message(s) that will be binned and for those that will not
	const char* msg_bin_subject;
	const char* msg_bin_exclude_subject;
	GMSEC_I32 level = config.getIntegerValue("gmsec-schema-level", static_cast<GMSEC_I32>(Specification::SchemaLevel::LEVEL_0));

	switch ( static_cast<Specification::SchemaLevel>(level) )
	{
	case Specification::SchemaLevel::LEVEL_0:
		msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>";
		break;

	case Specification::SchemaLevel::LEVEL_1:
		msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>";
		break;

	case Specification::SchemaLevel::LEVEL_2:
	default:
		msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
		msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
		break;
	}

	// Validate messages before they are sent to the bus
	config.addValue("gmsec-msg-content-validate-send", "true");

	// Configure to enable message binning
	config.addValue("gmsec-use-msg-bins", "true");

	// Specify the number of messages to be aggregated prior to publishing
	// the aggregate message to the middleware server. This applies to all
	// of the messages which match the subject(s) provided in the
	// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
	// Note: The aggregate message will be sent to the middleware server
	// immediately upon this many messages being published, regardless of
	// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
	config.addValue("gmsec-msg-bin-size", "10");

	// Specify a timeout (in milliseconds) for the aggregate message to be
	// sent to the GMSEC Bus.
	// Note: The aggregate message will be sent to the GMSEC Bus after this
	// period of time if the message bin does not fill up (per the value
	// provided for gmsec-msg-bin-size) prior to this timeout
	config.addValue("gmsec-msg-bin-timeout", "5000");

	// Specify the message subject(s) of messages that should be aggregated.
	// Subscription wildcard syntax can be used here.
	// Note: If this option is not specified, then ALL messages will be
	// aggregated (except those that are excluded).
	config.addValue("gmsec-msg-bin-subject-1", msg_bin_subject);

	// Specify any message subject(s) that should be excluded from being
	// aggregated.
	// This is useful if a wildcard subscription is provided in one of the
	// gmsec-msg-bin-subject-N parameters.
	config.addValue("gmsec-msg-bin-exclude-subject-1", msg_bin_exclude_subject);

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Set up standard fields within the MessageFactory associated
		// with the connection object.
		setStandardFields(conn->getMessageFactory(), "MESSAGE-BINS");

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Create a Log messages using the MessageFactory.
		// Set both required and optional fields within the message.
		Message logMsg1 = conn->getMessageFactory().createMessage("LOG");
		Message logMsg2 = conn->getMessageFactory().createMessage("LOG");

		logMsg1.addField("SUBCLASS", "AAA");
		logMsg1.addField("OCCURRENCE-TYPE", "PLOT");
		logMsg1.addField("SEVERITY", static_cast<GMSEC_I16>(1));
		logMsg1.addField("MSG-TEXT", "Message will be binned");

		logMsg2.addField("SUBCLASS", "TLM");
		logMsg2.addField("OCCURRENCE-TYPE", "DATA");
		logMsg2.addField("SEVERITY", static_cast<GMSEC_I16>(1));
		logMsg2.addField("MSG-TEXT", "Message will NOT be binned");

		for (int i = 0; i < 5; ++i)
		{
			GMSEC_Time theTime;
			TimeUtil::formatTime( TimeUtil::getCurrentTime(), theTime );

			logMsg1.addField("EVENT-TIME", theTime);
			logMsg2.addField("EVENT-TIME", theTime);

			// Publish the first message; it will be aggregated and published at a later time
			conn->publish(logMsg1);
			GMSEC_INFO << "Publishing AAA Log Message " << (i+1) << " (will be aggregated):\n" << logMsg1.toXML();

			// Publish the second message; it will NOT be aggregated and will be published immediately
			conn->publish(logMsg2);
			GMSEC_INFO << "Publishing TLM Log Message " << (i+1) << " (will NOT be aggregated):\n" << logMsg2.toXML();

			// Add processing delay
			TimeUtil::millisleep(100);
		}

		// Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
		GMSEC_INFO << "Waiting for binned messages to be published...";
		TimeUtil::millisleep(6000);

		// Disconnect from the GMSEC Bus, and terminate subscriptions.
		// Note: Any remaining messages in message bins will be published
		// before disconnecting from the GMSEC Bus, thus ensuring no message
		// loss occurs.
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

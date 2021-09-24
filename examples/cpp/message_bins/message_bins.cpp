/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file message_bins.cpp
 * 
 * This file contains an example describing how to aggregate multiple messages
 * into a singular message sent to the middleware server.  This functionality
 * is useful for cutting down on the number of messages sent across the
 * middleware bus, if there is an application in the system which is publishing
 * an exorbitantly large number of messages.
 *
 * It is recommended that you execute the application GMSEC_API/bin/gmsub
 * prior to running this program in order to see that the aggregated messages
 * still appear on the receiving end as if they were individual messages.
 *
 * Please note that if an aggregated message is sent by the GMSEC API, any
 * receivers will need to be running at least version 4.2 of the API in order
 * to de-aggregate the messages. Older versions can receive the messages
 * without crashing, but they will be unusable.
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;

const char* EXAMPLE_MESSAGE_SUBJECT     = "GMSEC.AGGREGATE.PUBLISH";
const char* EXAMPLE_BIN_EXCLUDE_SUBJECT = "GMSEC.BIN-EXCLUDE.PUBLISH";

//o Helper functions
void initializeLogging(Config& config);
void populateMessage(Message& message, int count);


int main (int argc, char* argv[])
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

	//o Enable Message Binning
	config.addValue("GMSEC-USE-MSG-BINS", "true");

	//o Specify the number of messages to be aggregated prior to publishing
	// the aggregate message to the middleware server (This applies to all
	// of the messages which match the subject(s) provided in the
	// GMSEC-MSG-BIN-SUBJECT-N configuration parameters
	// Note: The aggregate message will be sent to the middleware server
	// immediately upon this many messages being published, regardless of
	// the value supplied for GMSEC-MSG-BIN-TIMEOUT.
	config.addValue("GMSEC-MSG-BIN-SIZE", "10");

	//o Specify a timeout (in milliseconds) for the aggregate message to be
	// sent to the middleware server
	// Note: The aggregate message will be sent to the middleware server
	// after this period of time if the message bin does not fill up (per
	// the value provided for GMSEC-MSG-BIN-SIZE) prior to this timeout
	config.addValue("GMSEC-MSG-BIN-TIMEOUT", "5000");

	//o Specify the subjects to aggreate messages for.
	// Note: Subscription wildcard syntax can be used here, and has been
	// supported since GMSEC API version 4.3.
	config.addValue("GMSEC-MSG-BIN-SUBJECT-1", "GMSEC.*.PUBLISH");

	//o Specify any subjects that should be excluded from being aggregated
	// This is useful if a wildcard subscription is provided in one of the
	// GMSEC-MSG-BIN-SUBJECT-N parameters.
	config.addValue("GMSEC-MSG-BIN-EXCLUDE-SUBJECT-1", EXAMPLE_BIN_EXCLUDE_SUBJECT);

	initializeLogging(config);

	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connMgr(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		GMSEC_INFO << connMgr.getLibraryVersion();

		{
			//o Create a message 
			Message message(EXAMPLE_MESSAGE_SUBJECT, Message::PUBLISH);

			for (int i = 0; i < 5; ++i)
			{
				populateMessage(message, i+1);

				//o Publish the message to the middleware bus
				connMgr.publish(message);

				//o Display the XML string representation of the Message for
				// the sake of review
				GMSEC_INFO << "Published message:\n" << message.toXML();
			}
		}

		{
			//o Create a message
			Message message(EXAMPLE_BIN_EXCLUDE_SUBJECT, Message::PUBLISH);

			populateMessage(message, 1);

			//o Publish the message to the middleware bus
			// Note: When calling publish(), if a message does NOT match
			// one of the subjects to be aggregated, it will be immediately
			// published
			connMgr.publish(message);

			//o Display the XML string representation of the Message for
			// the sake of review
			GMSEC_INFO << "Published message:\n" << message.toXML();
		}

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();
	}
	catch (Exception e)
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


void populateMessage(Message& message, int count)
{
	// Note: Since C++ is a strongly-typed language, it is possible to use
	// type casting to add Field objects to a Message without having to
	// first create a Field object, then add it to the message.
	// Note: If a field of the same name is added to an existing message,
	// the value passed when calling addField will overwrite the existing
	// value.
	message.addField("CHAR-FIELD", (GMSEC_CHAR) 'c');
	message.addField("BOOL-FIELD-TRUE", true);
	message.addField("BOOL-FIELD-FALSE", false);
	message.addField("I8-FIELD", (GMSEC_I8) count);
	message.addField("I16-FIELD", (GMSEC_I16) count);
	message.addField("I32-FIELD", (GMSEC_I32) count);
	message.addField("I64-FIELD", (GMSEC_I64) count);
	message.addField("U8-FIELD", (GMSEC_U8) count);
	message.addField("U16-FIELD", (GMSEC_U16) count);
	message.addField("U32-FIELD", (GMSEC_U32) count);
	message.addField("U64-FIELD", (GMSEC_U64) count);
	message.addField("STRING-FIELD", "This is a test");
	message.addField("F32-FIELD", GMSEC_F32(1 + 1. / count));
	message.addField("F64-FIELD", GMSEC_F64(1 + 1. / count));
	message.addField("BIN-FIELD", (GMSEC_BIN) "JLMNOPQ", 7);
}


/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file msg_field_container.cpp
 * 
 * 
 * This file contains an example demonstrating the creation of GMSEC Messages
 * using the two available container types for Field objects: Binary Tree Map
 * (STL map) or Hash Map (unordered map).
 */

#include <gmsec4_cpp.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace gmsec::api;

//o Helper functions
void initializeLogging();
void createMsgUsingConfigFile(const char* filename);
void createMsgUsingConfigObject();

Message buildMessage(const Config& config, size_t numFields);
void displayMessage(const Message& msg, const char* description = 0);


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " <config-file>" << std::endl;
		std::cerr << "\nPlease see the GMSEC_API/example/cpp/message_field_container directory for an example xml configuration file." << std::endl;
		return -1;
	}

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging();

	try
	{
		//o Demonstration of setting the configuration options via
		// configuration file
		createMsgUsingConfigFile(argv[1]);

		//o Demonstration of hard-coding the configuration options
		// using a Config object
		createMsgUsingConfigObject();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
}


void createMsgUsingConfigFile(const char* filename)
{
	//o Load and parse configuration file
	ConfigFile cfgFile;
	cfgFile.load(filename);

	//o Retrieve config from file
	Config cfg = cfgFile.lookupConfig("config");

	//o Loop through three different examples outlined in the configuration
	// file provided with this example
	for (int i = 0; i < 3; ++i)
	{
		//o Form message name (e.g. msg1) to reference.
		std::stringstream msgName;
		msgName << "msg" << (i+1);

		//o Parse the message from the configuration file and load it
		// into a GMSEC Message
		Message msg = cfgFile.lookupMessage(msgName.str().c_str());

		//o Display the message
		const char* description = 0;
		switch (i+1)
		{
			case 1: description = "Expecting TREE map (at size limit of 8 fields):"; break;
			case 2: description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
			case 3: description = "Expecting HASH map (size limit is ignored):"; break;
		}

		displayMessage(msg, description);
	}
}


void createMsgUsingConfigObject()
{
	Config cfg1, cfg2, cfg3, cfg4, empty;

	cfg1.addValue("gmsec-msgfld-store-type", "tree");
	//o If the Message object is using a binary tree map to store fields,
	// the 'gmsec-msgfield-store-size' configuration option indicates how
	// many fields should be allowed to be stored before the Message
	// object converts to using a hash map (Default is 50).
	cfg1.addValue("gmsec-msgfld-store-size", "10");

	cfg2.addValue("gmsec-msgfld-store-type", "hash");

	cfg3.addValue("gmsec-msgfld-store-type", "tree");    // container type
	cfg3.addValue("gmsec-msgfld-store-size", "0");       // never rollover

	cfg4.addValue("gmsec-sort-msg-fields", "true");      // effectively the same as cfg3 above

	Message msg1 = buildMessage(cfg1,   9);   // uses bin-tree container map (under size limit)
	Message msg2 = buildMessage(cfg1,  10);   // uses bin-tree container map (at size limit)
	Message msg3 = buildMessage(cfg1,  11);   // uses hash container map (exceeds size limit)
	Message msg4 = buildMessage(cfg2,   5);   // default to using hash container map (size limit ignored)
	Message msg5 = buildMessage(cfg3,  55);   // always uses bin-tree container map
	Message msg6 = buildMessage(empty, 50);   // uses bin-tree container map (default size limit not exceeded)
	Message msg7 = buildMessage(empty, 51);   // uses hash container map (exceeds default size limit)
	Message msg8 = buildMessage(cfg4,  55);   // message fields always sorted

	displayMessage(msg1, "Message using TREE container map (under limit):");
	displayMessage(msg2, "Message using TREE container map (at max limit):");
	displayMessage(msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):");
	displayMessage(msg4, "Message using HASH container map (limit ignored):");
	displayMessage(msg5, "Message using TREE container map (limit ignored):");
	displayMessage(msg6, "Message using TREE container map (at limit):");
	displayMessage(msg7, "Message using HASH container map (limit exceeded):");
	displayMessage(msg8, "Message fields always sorted:");
}


Message buildMessage(const Config& config, size_t numFields)
{
	Message msg("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", Message::PUBLISH, config);

	if (numFields > 0)
	{
		// Add Fields
		for (size_t i = 0; i < numFields; ++i)
		{
			std::ostringstream oss;
			oss << "FIELD-" << (i < 10 ? "00" : (i < 100 ? "0" : "")) << i;

			msg.addField(oss.str().c_str(), (GMSEC_U64) i);
		}
	}

	return msg;
}


void displayMessage(const Message& msg, const char* description)
{
	const char* xml = msg.toXML();
	GMSEC_INFO << description << "\n" << xml;
}


void initializeLogging()
{
	// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
	// object, it affects the logging level and output destination for all
	// logging macros in the entire API, regardless of whether that Config
	// object is kept around or not. In this case, we are creating a local
	// Config object just for the sake of initializing logging.
	Config config;

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


/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file configfile.cpp
 * 
 * A C++ example demonstration of loading pre-defined subscriptions, Config
 * objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 * configuration file used for this example program is "configfile.xml".
 */

#include <gmsec4_cpp.h>

#include <string>
#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::mist;

void initializeLogging();

int main (int argc, char* argv[])
{
	static std::string defaultConfigFile = "configfile.xml";

	try
	{
		initializeLogging();

		std::string cfgFilename = (argc > 1 ? argv[1] : defaultConfigFile);

		GMSEC_INFO << "Loading config file " << cfgFilename.c_str() << "...";

		// load configuration file
		ConfigFile cfgFile;
		cfgFile.load(cfgFilename.c_str());

		// retrieve config objects from the Config file
		Config c1 = cfgFile.lookupConfig("config1");
		Config c2 = cfgFile.lookupConfig("config2");

		// Display log of XML representation of Config objects
		GMSEC_INFO << "Config 1:\n" << c1.toXML();
		GMSEC_INFO << "Config 2:\n" << c2.toXML();

		// lookup subscription topic from configuration file, and any excluded topics associated with such
		try
		{
			const ConfigFile::SubscriptionEntry& subEntry = cfgFile.lookupSubscriptionEntry("events");
			GMSEC_INFO << "Subscription pattern: " << subEntry.getPattern();

			while (subEntry.hasNextExcludedPattern())
			{
				GMSEC_INFO << "Subscription excluded pattern: " << subEntry.nextExcludedPattern();
			}
		}
		catch (const Exception& e)
		{
			GMSEC_ERROR << "Failed to acquire 'events' subscription entry; reason: " << e.what();
		}

		// lookup a Message from the configuration file
		Message message = cfgFile.lookupMessage("msg1");

		// Display XML representation of the message
		GMSEC_INFO << "Message:\n" << message.toXML();

		// Obtain ConfigFile Iterator to examine all of the various
		// entries defined in the ConfigFile
		ConfigFileIterator& iter = cfgFile.getIterator();

		// Acquire and display all Config entries
		while (iter.hasNextConfig())
		{
			const ConfigFile::ConfigEntry& entry = iter.nextConfig();

			GMSEC_INFO << "\nConfig Name: " << entry.getName()
			           << "\nConfig     :\n" << entry.getConfig().toXML();
		}

		// Acquire and display all Message entries
		while (iter.hasNextMessage())
		{
			const ConfigFile::MessageEntry& entry = iter.nextMessage();

			GMSEC_INFO << "\nMessage Name: " << entry.getName()
			           << "\nMessage     :\n" << entry.getMessage().toXML();
		}

		// Acquire and display all subscription entries
		while (iter.hasNextSubscription())
		{
			const ConfigFile::SubscriptionEntry& entry = iter.nextSubscription();

			GMSEC_INFO << "\nSubscription Name : " << entry.getName()
			           << "\nSubscription Topic: " << entry.getPattern();

			while (entry.hasNextExcludedPattern())
			{
				GMSEC_INFO << "\nExcluded Pattern : " << entry.nextExcludedPattern();
			}
		}

		// Acquire and display all custom XML entries
		while (iter.hasNextCustomElement())
		{
			const char* element = iter.nextCustomElement();

			GMSEC_INFO << "\nCustom XML Element:\n" << element;
		}
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	return 0;
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


/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmconfig.cpp
 * 
 * A C++ example demonstration of loading a configuration from a standard
 * xml file.  The configuration file used for the example code is "gmconfig.xml".
 * 
 */

#include "../example.h"

#include <string>


using namespace gmsec::api;


class gmconfig
{
public:
	gmconfig(const char* filename);
	~gmconfig();
	bool run();

private:
	std::string cfgFilename;
	ConfigFile  cfgFile;
};


gmconfig::gmconfig(const char* filename)
	: cfgFilename(filename),
	  cfgFile()
{
	Config config;
	example::initialize(config);
}


gmconfig::~gmconfig()
{
}


bool gmconfig::run()
{
	bool success = true;

	try
	{
		// Attempt to load configuration file
		cfgFile.load(cfgFilename.c_str());

		// Attempt to retrieve configurations from the configuration file
		Config c1 = cfgFile.lookupConfig("config1");
		Config c2 = cfgFile.lookupConfig("config2");

		// Display log of XML representation of configuration objects
		GMSEC_INFO << "Config 1:\n" << c1.toXML();
		GMSEC_INFO << "Config 2:\n" << c2.toXML();

		// Attempt to create a Connection object using a configuration object
		Connection* connection = Connection::create(c2);

		// Destroy the connection object
		Connection::destroy(connection);

		// Attempt to lookup subscription topic from configuration file
		const char* pattern = cfgFile.lookupSubscription("events");

		if (!pattern)
		{
			throw Exception(CONFIGFILE_ERROR,
			                OTHER_ERROR_CODE,
			                "Failed to acquire 'events' subscription topic");
		}
		GMSEC_INFO << "Subscription topic: " << pattern;

		// Attempt to lookup message from configuration file
		Message message = cfgFile.lookupMessage("msg1");

		// Display XML representation of the message
		GMSEC_INFO << "Message:\n" << message.toXML();

		// Obtain Configuration File Iterator
		ConfigFileIterator& iter = cfgFile.getIterator();

		// Acquire and display all configuration entries
		while (iter.hasNextConfig())
		{
			const ConfigFile::ConfigEntry& entry = iter.nextConfig();

			GMSEC_INFO << "\nConfig Name: " << entry.getName()
			           << "\nConfig     :\n" << entry.getConfig().toXML();
		}

		// Acquire and display all message entries
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
			           << "\nSubscription Topic: " << entry.getSubject();
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
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: gmconfig <filename>" << std::endl;
		return -1;
	}

	return (gmconfig(argv[1]).run() ? 0 : -1);
}

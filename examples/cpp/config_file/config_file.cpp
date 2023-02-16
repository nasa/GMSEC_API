/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * config_file.cpp
 */

#include "config_file.h"

using namespace gmsec::api5;


config_file::config_file(const std::string &configFilename)
	: configFile(),
	  configFileName(configFilename)
{
	configFile.load(configFileName.c_str());

	std::cout << "Using config file --> " << configFilename << std::endl;
}


config_file::~config_file()
{
}


void config_file::showSubscription(const std::string &subscriptionName)
{
	ConfigFile::SubscriptionEntry sub = configFile.lookupSubscriptionEntry(subscriptionName.c_str());

	std::cout << "Subscription <" << subscriptionName.c_str() << ">:" << std::endl;
	std::cout << "  -> name: " << sub.getName() << " -> pattern: " << sub.getPattern() << std::endl;

	// if a subscription has an exclude pattern, it can be iterated through like this
	while (sub.hasNextExcludedPattern())
	{
		const char* exclude = sub.nextExcludedPattern();

		std::cout << "     -> exclude: " << exclude << std::endl;
	}

	std::cout << std::endl;
}


void config_file::showConfig(const std::string &configName)
{
	Config config = configFile.lookupConfig(configName.c_str());

	std::cout << "Config <" << configName.c_str() << ">:" << std::endl;

	// a way to get the guts of the config
	const char* key = 0;
	const char* value = 0;

	bool found = config.getFirst(key, value);

	while (found)
	{
		std::cout << "  -> key: " << key << " -> value: " << value << std::endl;

		found = config.getNext(key, value);
	}

	// but individual values may be found also
	std::cout << std::endl;
	const char* mwid = config.getValue("mw-id");

	if (mwid != NULL)
	{
		std::cout << "  -> found 'mw-id': " << config.getValue("mw-id") << std::endl;
	}
	else
	{
		std::cout << "  -> 'mw-id'is not defined in this config." << std::endl;
	}

	// or the entire thing can be printed as XML (or JSON)
	std::cout << std::endl;
	std::cout << config.toXML() << std::endl;
	std::cout << std::endl;
}


void config_file::showMessage(const std::string &messageName)
{
	Message msg = configFile.lookupMessage(messageName.c_str());

	std::cout << "Message <" << messageName.c_str() << ">:" << std::endl;

	// there are number getters such as this one
	std::cout << "  -> find 'STRING-FIELD': "
	          << msg.getStringField("STRING-FIELD").getStringValue()
	          << "\n"
	          << std::endl;

	// or the entire thing can be printed as XML (or JSON)
	std::cout << msg.toXML() << "\n" << std::endl;
}


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: Provide an example XML file of interest." << std::endl;
		return 1;
	}

	std::string filename(argv[1]);
	int exitcode = 0; // 1 being bad

	try
	{
		config_file demo(filename);

		demo.showSubscription("all-messages");
		demo.showSubscription("custom1");

		demo.showConfig("config1");
		demo.showConfig("config2");

		demo.showMessage("msg1");
		demo.showMessage("msg2");
	}
	catch (const GmsecException &e)
	{
		std::cout << "config_file failed [" << argv[1] << "] error -- " << e.what() << std::endl;
		exitcode = 1;
	}

	return exitcode;
}


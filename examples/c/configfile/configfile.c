/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file configfile.c
 *
 * A C++ example demonstration of loading pre-defined subscriptions, Config
 * objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 * configuration file used for this example program is "configfile.xml".
 */

#include <gmsec4_c.h>
#include <stdio.h>
#include <stdlib.h>

void checkStatus(GMSEC_Status status);
GMSEC_Config initializeLogging(GMSEC_Status status);

int main (int argc, char* argv[])
{
	GMSEC_Status status = statusCreate();
	GMSEC_ConfigFile cfgFile;
	GMSEC_Config config;
	GMSEC_Config c1;
	GMSEC_Config c2;
	GMSEC_SubscriptionEntry_Handle subEntry;
	GMSEC_Message message;
	GMSEC_ConfigFileIterator iter;

	if (argc <= 1)
	{
		printf("usage: %s <filename>\n", argv[0]);
		return -1;
	}

	config = initializeLogging(status);
	checkStatus(status);

	// load configuration file
	cfgFile = configFileCreate();
	configFileLoad(cfgFile, argv[1], status);
	checkStatus(status);

	// retrieve config objects from the Config file
	c1 = configFileLookupConfig(cfgFile, "config1", status);
	checkStatus(status);
	c2 = configFileLookupConfig(cfgFile, "config2", status);
	checkStatus(status);

	// Display log of XML representation of Config objects
	GMSEC_INFO("Config 1:\n%s", configToXML(c1, status));
	GMSEC_INFO("Config 2:\n%s", configToXML(c2, status));

	// lookup subscription topic from configuration file, and any excluded topics associated with such
	subEntry = configFileLookupSubscriptionEntry(cfgFile, "events", status);
	checkStatus(status);
	GMSEC_INFO("Subscription pattern: %s", subscriptionEntryGetPattern(subEntry, NULL));

	while (subscriptionEntryHasNextExcludedPattern(subEntry, NULL) == GMSEC_TRUE)
	{
		GMSEC_INFO("Subscription excluded pattern: %s", subscriptionEntryNextExcludedPattern(subEntry, NULL));
	}

	// lookup a Message from the configuration file
	message = configFileLookupMessage(cfgFile, "msg1", status);
	checkStatus(status);

	// Display XML representation of the message
	GMSEC_INFO("Message\n%s", messageToXML(message, status));

	// Obtain ConfigFile Iterator to examine all of the various
	// entries defined in the ConfigFile
	iter = configFileGetIterator(cfgFile, status);
	checkStatus(status);

	// Acquire and display all Config entries
	while (configFileIteratorHasNextConfig(iter, status))
	{
		GMSEC_ConfigEntry entry = configFileIteratorNextConfig(iter, status);
		checkStatus(status);

		GMSEC_INFO("\nConfig Name: %s\nConfig     :\n%s", entry.name, configToXML(entry.config, status));
		checkStatus(status);
	}

	// Acquire and display all Message entries
	while (configFileIteratorHasNextMessage(iter, status))
	{
		GMSEC_MessageEntry entry = configFileIteratorNextMessage(iter, status);
		checkStatus(status);

		GMSEC_INFO("\nMessage Name: %s\nMessage     :\n%s", entry.name, messageToXML(entry.message, status));
		checkStatus(status);
	}

	// Acquire and display all subscription entries
	while (configFileIteratorHasNextSubscription(iter, status))
	{
		GMSEC_SubscriptionEntry entry = configFileIteratorNextSubscription(iter, status);
		checkStatus(status);

		GMSEC_INFO("\nSubscription Name : %s\nSubscription Topic:\n%s", entry.name, entry.subject);
		checkStatus(status);
	}

	while (configFileIteratorHasNextCustomElement(iter, status))
	{
		const char* element = configFileIteratorNextCustomElement(iter, status);
		checkStatus(status);

		GMSEC_INFO("\nCustom XML Element:\n%s", element);
		checkStatus(status);
	}

	messageDestroy(&message);
	configFileDestroy(&cfgFile);
	configDestroy(&c1);
	configDestroy(&c2);
	configDestroy(&config);
	statusDestroy(&status);

	return 0;
}

GMSEC_Config initializeLogging(GMSEC_Status status)
{
	// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
	// object, it affects the logging level and output destination for all
	// logging macros in the entire API, regardless of whether that Config
	// object is kept around or not. In this case, we are creating a local
	// Config object just for the sake of initializing logging.
	GMSEC_Config config = configCreate();

	const char* logLevel = configGetValue(config, "LOGLEVEL", status);
	const char* logFile = configGetValue(config, "LOGFILE", status);

	if(!logLevel)
	{
		configAddValue(config, "LOGLEVEL", "INFO", status);
	}
	if(!logFile)
	{
		configAddValue(config, "LOGFILE", "STDOUT", status);
	}

	return config;
}

void checkStatus(GMSEC_Status status)
{
	if(!statusIsError(status))
	{
		return;
	}

	GMSEC_ERROR(statusGet(status));
	exit(-1);
}

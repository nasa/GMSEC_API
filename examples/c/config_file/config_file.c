/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_file.c
 *
 * @brief An extended example of reading a ConfigFile XML file and
 * retrieving element information.
 */

#include "config_file.h"	

#include <gmsec5_c.h>

#include <stdio.h>


// Globals
GMSEC_Status     status = NULL;
GMSEC_ConfigFile configFile = NULL;
const char*      configFilename;


void config_file(const char *filename)
{
	status         = statusCreate();
	configFile     = configFileCreate(status);
	configFilename = filename;

	printf("Using config file --> %s\n", configFilename);
}


int open()
{
	int result = 1;

	configFileLoad(configFile, configFilename, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("ConfigFile open failed [%s] error -- %s\n", configFilename, statusGet(status));
		result = 0;
	}

	return result;
}


void close()
{
	// destroy the configFile
	configFileDestroy(&configFile);
}


void showSubscription(const char *subscriptionName)
{
	GMSEC_SubscriptionEntry sub = configFileLookupSubscriptionEntry(configFile, subscriptionName, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Subscription [%s] error -- %s\n", subscriptionName, statusGet(status));
		return;
	}

	printf("Subscription <%s>:\n", subscriptionName);
	const char *name = subscriptionEntryGetName(sub, status);
	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Subscription.getName error -- %s\n",  statusGet(status));
		return;
	}

	const char *pattern = subscriptionEntryGetPattern(sub, status);
	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Subscription.getPattern error -- %s\n",  statusGet(status));
		return;
	}

	printf("  -> name: %s  -> pattern: %s\n", name, pattern);

	// if a subscription has an exclude pattern, it can be iterated through like this
	while (subscriptionEntryHasNextExcludedPattern(sub, status) == GMSEC_TRUE)
	{
		const char* exclude = subscriptionEntryNextExcludedPattern(sub, NULL);
		printf("     -> exclude: %s\n", exclude);
	}

	subscriptionEntryDestroy(&sub);
}


void showConfig(const char *configName)
{
	GMSEC_Config config = configFileLookupConfig(configFile, configName, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Config [%s] error -- %s\n", configName, statusGet(status));
		return;
	}

	printf("Config <%s>:", configName);

	// a way to get the guts of the config
	const char* name  = NULL;
	const char* value = NULL;

	configGetFirst(config, &name, &value, status);

	while (statusHasError(status) == GMSEC_FALSE)
	{
		printf("  -> key: %s -> value: %s\n", name, value);
		configGetNext(config, &name, &value, status);
	}

	// but individual values may be found also
	printf("\n");
	const char* mwid = configGetValue(config, "mw-id", status);

	if (mwid != NULL)
	{
		printf("  -> found 'mw-id': %s\n", mwid);
	}
	else
	{
		printf("  -> 'mw-id'is not defined in this config.\n");
	}

	// or the entire thing can be printed as XML (or JSON)
	printf("\n");
	printf("%s", configToXML(config, NULL));
	printf("\n");

	configDestroy(&config);
}


void showMessage(const char *messageName)
{
	GMSEC_Message msg = configFileLookupMessage(configFile, messageName, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Message [%s] error -- %s\n", messageName, statusGet(status));
		return;
	}

	printf("Message <%s>:\n", messageName);

	// there are a number getters such as this one
	GMSEC_Field sf = messageGetStringField(msg, "STRING-FIELD", status);
	printf("  -> find 'STRING-FIELD': %s\n", stringFieldGetValue(sf, status));

	// or the entire message can be printed as an XML (or JSON)
	printf("\n");
	printf("%s\n", messageToXML(msg, NULL));
	printf("\n");

	messageDestroy(&msg);
}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: Provide an example XML file of interest.\n");
		return 1;
	}

	config_file(argv[1]);

	if (open() < 1)
	{
		fprintf(stderr, "The file [%s] failed to open by the GMSEC API -- exiting.", argv[1]);
		return 1;
	}

	showSubscription("all-messages");
	showSubscription("custom1");

	showConfig("config1");
	showConfig("config2");

	showMessage("msg1");
	showMessage("msg2");

	close();

	return 0;
}

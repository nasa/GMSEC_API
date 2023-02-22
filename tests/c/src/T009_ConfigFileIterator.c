#include "TestCase.h"


static GMSEC_Status         status  = NULL;
static GMSEC_ConfigFile     cfgFile = NULL;
static GMSEC_MessageFactory factory = NULL;
static GMSEC_Config         config1 = NULL;
static GMSEC_Config         config2 = NULL;
static GMSEC_Message        msg1    = NULL;
static GMSEC_Message        msg2    = NULL;


void setup()
{
	status = statusCreate();

	cfgFile = configFileCreate(NULL);
	factory = messageFactoryCreate(NULL);

    GMSEC_SubscriptionEntry entry1 = subscriptionEntryCreate("AllMessages", "GMSEC.>", NULL);
    GMSEC_SubscriptionEntry entry2 = subscriptionEntryCreate("LogMessages", "GMSEC.*.*.MSG.LOG.+", NULL);

	config1 = configCreateUsingData("mw-id=activemq39 server=tcp://localhost:61616", KEY_VALUE_DATA, NULL);
	config2 = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, NULL);

	msg1 = messageFactoryCreateMessage(factory, "HB", NULL);
	msg2 = messageFactoryCreateMessage(factory, "LOG", NULL);

	// Add subscription entries
	configFileAddSubscriptionEntry(cfgFile, entry1, NULL);
	configFileAddSubscriptionEntry(cfgFile, entry2, NULL);

	// Add configuration entries
	configFileAddConfig(cfgFile, "ActiveMQ", config1, NULL);
	configFileAddConfig(cfgFile, "Bolt", config2, NULL);

	// Add message entries
	configFileAddMessage(cfgFile, "Heartbeat", msg1, NULL);
	configFileAddMessage(cfgFile, "Log", msg2, NULL);

	subscriptionEntryDestroy(&entry1);
	subscriptionEntryDestroy(&entry2);
}


void teardown()
{
	configFileDestroy(&cfgFile);
	messageFactoryDestroy(&factory);
	configDestroy(&config1);
	configDestroy(&config2);
	messageDestroy(&msg1);
	messageDestroy(&msg2);
	statusDestroy(&status);
}


void test_configFileIterator()
{
	GMSEC_INFO("Test configFileIteratorHasNextXXX() and configFileIteratorNextXXX()");

	GMSEC_ConfigFileIterator iter = configFileGetIterator(cfgFile, NULL);

	int numSubscriptions = 0;
	int numConfigs       = 0;
	int numMessages      = 0;

	while (configFileIteratorHasNextConfig(iter, NULL) == GMSEC_TRUE)
	{
		GMSEC_ConfigEntry entry = configFileIteratorNextConfig(iter, NULL);

		++numConfigs;

		if (numConfigs == 1)
		{
			testCheckBool("Unexpected config entry name", strcompare(entry.name, "ActiveMQ") == 0);
			testCheckBool("Unexpected config entry mw-id value", strcompare("activemq39", configGetValue(entry.config, "mw-id", NULL)) == 0);
			testCheckBool("Unexpected config entry server value", strcompare("tcp://localhost:61616", configGetValue(entry.config, "server", NULL)) == 0);
		}
		else	
		{
			testCheckBool("Unexpected config entry name", strcompare(entry.name, "Bolt") == 0);
			testCheckBool("Unexpected config entry mw-id value", strcompare("bolt", configGetValue(entry.config, "mw-id", NULL)) == 0);
			testCheckBool("Unexpected config entry server value", strcompare("localhost", configGetValue(entry.config, "server", NULL)) == 0);
		}
	}
	testCheckBool("Unexpected number of config entries", numConfigs == 2);

	while (configFileIteratorHasNextMessage(iter, NULL) == GMSEC_TRUE)
	{
		GMSEC_MessageEntry entry = configFileIteratorNextMessage(iter, NULL);

		++numMessages;

		if (numMessages == 1)
		{
			testCheckBool("Unexpected message entry name", strcompare(entry.name, "Heartbeat") == 0);
			testCheckBool("Unexpected message entry", strcompare(messageToXML(entry.message, NULL), messageToXML(msg1, NULL)) == 0);
		}
		else	
		{
			testCheckBool("Unexpected message entry name", strcompare(entry.name, "Log") == 0);
			testCheckBool("Unexpected message entry", strcompare(messageToXML(entry.message, NULL), messageToXML(msg2, NULL)) == 0);
		}
	}
	testCheckBool("Unexpected number of message entries", numMessages == 2);

	while (configFileIteratorHasNextSubscription(iter, NULL) == GMSEC_TRUE)
	{
		GMSEC_SubscriptionEntry entry = configFileIteratorNextSubscription(iter, NULL);

		testRequireBool("Expected a subscription entry", entry != NULL);

		++numSubscriptions;

		if (numSubscriptions == 1)
		{
			testCheckBool("Unexpected subscription entry name", strcompare(subscriptionEntryGetName(entry, NULL), "AllMessages") == 0);
			testCheckBool("Unexpected subscription entry pattern", strcompare(subscriptionEntryGetPattern(entry, NULL), "GMSEC.>") == 0);
		}
		else	
		{
			testCheckBool("Unexpected subscription entry name", strcompare(subscriptionEntryGetName(entry, NULL), "LogMessages") == 0);
			testCheckBool("Unexpected subscription entry pattern", strcompare(subscriptionEntryGetPattern(entry, NULL), "GMSEC.*.*.MSG.LOG.+") == 0);
		}
	}
	testCheckBool("Unexpected number of subscription entries", numSubscriptions == 2);

	// Off-nominal tests
	(void) configFileIteratorNextConfig(iter, status);
	testCheckBool("Expected error because no more configs", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorNextMessage(iter, status);
	testCheckBool("Expected error because no more messages", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorNextSubscription(iter, status);
	testCheckBool("Expected error because no more subscriptions", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorHasNextConfig(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorHasNextMessage(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorHasNextSubscription(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorNextConfig(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorNextMessage(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);

	(void) configFileIteratorNextSubscription(NULL, status);
	testCheckBool("Expected error using NULL config file", statusHasError(status) == GMSEC_TRUE);
}


void test_configFileIteratorReset()
{
	GMSEC_INFO("Test configFileIteratorReset()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_ConfigFileIterator iter = configFileGetIterator(cfgFile, NULL);

		testCheckBool("Expected to have subscriptions", configFileIteratorHasNextSubscription(iter, NULL) == GMSEC_TRUE);
		testCheckBool("Expected to have configs", configFileIteratorHasNextConfig(iter, NULL) == GMSEC_TRUE);
		testCheckBool("Expected to have messages", configFileIteratorHasNextMessage(iter, NULL) == GMSEC_TRUE);

		while (configFileIteratorHasNextSubscription(iter, NULL) == GMSEC_TRUE) (void) configFileIteratorNextSubscription(iter, NULL);
		while (configFileIteratorHasNextConfig(iter, NULL) == GMSEC_TRUE) (void) configFileIteratorNextConfig(iter, NULL);
		while (configFileIteratorHasNextMessage(iter, NULL) == GMSEC_TRUE) (void) configFileIteratorNextMessage(iter, NULL);

		testCheckBool("Expected NOT to have additional subscriptions", configFileIteratorHasNextSubscription(iter, NULL) == GMSEC_FALSE);
		testCheckBool("Expected NOT to have additional configs", configFileIteratorHasNextConfig(iter, NULL) == GMSEC_FALSE);
		testCheckBool("Expected NOT to have additional messages", configFileIteratorHasNextMessage(iter, NULL) == GMSEC_FALSE);

		configFileIteratorReset(iter, status);
		testCheckBool("Expected to have no error", statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Expected to have subscriptions", configFileIteratorHasNextSubscription(iter, NULL) == GMSEC_TRUE);
		testCheckBool("Expected to have configs", configFileIteratorHasNextConfig(iter, NULL) == GMSEC_TRUE);
		testCheckBool("Expected to have messages", configFileIteratorHasNextMessage(iter, NULL) == GMSEC_TRUE);
	}

	// Off-nominal test
	{
		configFileIteratorReset(NULL, status);
		testCheckBool("Expected to have error", statusHasError(status) == GMSEC_TRUE);
	}

	statusDestroy(&status);
}


int test_ConfigFileIterator()
{
	testSetDescription("Test ConfigFileIterator");

	setup();

	test_configFileIterator();
	test_configFileIteratorReset();

	teardown();

	return 0;
}
TEST_DRIVER(test_ConfigFileIterator)

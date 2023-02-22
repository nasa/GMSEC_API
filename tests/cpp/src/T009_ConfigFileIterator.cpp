#include "TestCase.h"

using namespace gmsec::api5;


static ConfigFile     cfgFile;
static MessageFactory msgFactory;



void setup()
{
	cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry("AllMessages", "GMSEC.>") );
	cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

	cfgFile.addConfig("ActiveMQ", Config("mw-id=activemq39 server=tcp://localhost:61616", DataType::KEY_VALUE_DATA));
	cfgFile.addConfig("Bolt", Config("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA));

	cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));
	cfgFile.addMessage("Log", msgFactory.createMessage("LOG"));
}


void test_iterator(Test& test)
{
	ConfigFileIterator& iter = cfgFile.getIterator();


	int numConfigs = 0;
	while (iter.hasNextConfig())
	{
		const ConfigFile::ConfigEntry& entry = iter.nextConfig();

		++numConfigs;

		if (numConfigs == 1)
		{
			test.check("Unexpected config entry name", std::string("ActiveMQ") == entry.getName());
			test.check("Unexpected config entry value", std::string("activemq39") == entry.getConfig().getValue("mw-id", "UNKNOWN-MW-ID"));
			test.check("Unexpected config entry value", std::string("tcp://localhost:61616") == entry.getConfig().getValue("server", "UNKNOWN-SERVER"));
		}
		else
		{
			test.check("Unexpected config entry name", std::string("Bolt") == entry.getName());
			test.check("Unexpected config entry value", std::string("bolt") == entry.getConfig().getValue("mw-id", "UNKNOWN-MW-ID"));
			test.check("Unexpected config entry value", std::string("localhost") == entry.getConfig().getValue("server", "UNKNOWN-SERVER"));
		}

		// Test ConfigEntry copy constructor
		ConfigFile::ConfigEntry clone = entry;
		test.check("Unexpected clone config name", strcompare(clone.getName(), entry.getName()) == 0);
		test.check("Unexpected clone config value", strcompare(clone.getConfig().toXML(), entry.getConfig().toXML()) == 0);
	}
	test.check("Unexpected number of config entries", 2 == numConfigs);


	int numMessages = 0;
	while (iter.hasNextMessage())
	{
		const ConfigFile::MessageEntry& entry = iter.nextMessage();

		++numMessages;

		if (numMessages == 1)
		{
			test.check("Unexpected message entry name", std::string("Heartbeat") == entry.getName());
			test.check("Unexpected message entry", std::string(msgFactory.createMessage("HB").toXML()) == entry.getMessage().toXML());
		}
		else
		{
			test.check("Unexpected message entry name", std::string("Log") == entry.getName());
			test.check("Unexpected message entry", std::string(msgFactory.createMessage("LOG").toXML()) == entry.getMessage().toXML());
		}

		// Test MessageEntry copy constructor
		ConfigFile::MessageEntry clone = entry;
		test.check("Unexpected clone message name", strcompare(clone.getName(), entry.getName()) == 0);
		test.check("Unexpected clone message value", strcompare(clone.getMessage().toXML(), entry.getMessage().toXML()) == 0);
	}
	test.check("Unexpected number of message entries", 2 == numMessages);


	int numSubscriptions = 0;
	while (iter.hasNextSubscription())
	{
		const ConfigFile::SubscriptionEntry& entry = iter.nextSubscription();

		++numSubscriptions;

		if (numSubscriptions == 1)
		{
			test.check("Unexpected subscription entry name", std::string("AllMessages") == entry.getName());
			test.check("Unexpected subscription entry pattern", std::string("GMSEC.>") == entry.getPattern());
		}
		else
		{
			test.check("Unexpected subscription entry name", std::string("LogMessages") == entry.getName());
			test.check("Unexpected subscription entry pattern", std::string("GMSEC.*.*.MSG.LOG.+") == entry.getPattern());
		}

		// Test SubscriptionEntry copy constructor
		ConfigFile::SubscriptionEntry clone = entry;
		test.check("Unexpected clone subscription name", strcompare(clone.getName(), entry.getName()) == 0);
		test.check("Unexpected clone subscription value", strcompare(clone.getPattern(), entry.getPattern()) == 0);
	}
	test.check("Unexpected number of subscription entries", 2 == numSubscriptions);


	// Off-nominal tests
	try {
		iter.nextConfig();
		test.check("Expected exception because there are no more config entries", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}

	try {
		iter.nextMessage();
		test.check("Expected exception because there are no more message entries", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}

	try {
		iter.nextSubscription();
		test.check("Expected exception because there are no more subscription entries", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}
}


void test_reset(Test& test)
{
	ConfigFileIterator& iter = cfgFile.getIterator();

	test.check("Expected to have subscriptions", true == iter.hasNextSubscription());
	test.check("Expected to have configs", true == iter.hasNextConfig());
	test.check("Expected to have messages", true == iter.hasNextMessage());

	while (iter.hasNextSubscription()) (void) iter.nextSubscription();
	while (iter.hasNextConfig()) (void) iter.nextConfig();
	while (iter.hasNextMessage()) (void) iter.nextMessage();

	test.check("Expected NOT to have additional subscriptions", false == iter.hasNextSubscription());
	test.check("Expected NOT to have additional configs", false == iter.hasNextConfig());
	test.check("Expected NOT to have additional messages", false == iter.hasNextMessage());

	iter.reset();

	test.check("Expected to have subscriptions", true == iter.hasNextSubscription());
	test.check("Expected to have configs", true == iter.hasNextConfig());
	test.check("Expected to have messages", true == iter.hasNextMessage());
}


int test_ConfigFileIterator(Test& test)
{
	test.setDescription("Test ConfigFileIterator");

	setup();

	test_iterator(test);
	test_reset(test);

	return 0;
}

TEST_DRIVER(test_ConfigFileIterator)

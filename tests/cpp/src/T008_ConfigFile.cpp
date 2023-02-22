#include "TestCase.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace gmsec::api5;


const char* const GOOD_CONFIG_FILE  = "good_config_file.xml";
const char* const BAD_CONFIG_FILE_1 = "bad_config_file1.xml";
const char* const BAD_CONFIG_FILE_2 = "bad_config_file2.xml";

const char* const XML = "<DEFINITIONS>\n" \
	                    "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&gt;\">\n" \
	                    "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n" \
	                    "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n" \
	                    "    </SUBSCRIPTION>\n" \
	                    "    <CONFIG NAME=\"Bolt\">\n" \
	                    "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n" \
	                    "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n" \
	                    "    </CONFIG>\n" \
	                    "    <MESSAGE NAME=\"Msg1\">\n" \
	                    "        <CONFIG NAME=\"msg_config\">\n" \
	                    "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n" \
	                    "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n" \
	                    "        </CONFIG>\n" \
	                    "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n" \
	                    "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n" \
	                    "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n" \
	                    "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n" \
	                    "    </MESSAGE>\n" \
	                    "</DEFINITIONS>";



void test_constructor_1(Test& test)
{
	GMSEC_INFO << "Test constructor (no args)";

	ConfigFile cfgFile;
	test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
}


void test_constructor_2(Test& test)
{
	GMSEC_INFO << "Test constructor (Config arg)";

	try
	{
		ConfigFile cfgFile( test.getConfig() );
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());

		cfgFile.load( test.getDataFile(GOOD_CONFIG_FILE).c_str() );

		Message msg = cfgFile.lookupMessage("Msg1");

		test.check("Expected C2MS specification", msg.getSchemaLevel() == Specification::SchemaLevel::LEVEL_0);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	try
	{
		Config config = test.getConfig();            // make a copy
		config.addValue("gmsec-schema-level", "2");  // let's use the NASA/GMSEC addendum

		ConfigFile cfgFile( config );
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());

		cfgFile.load( test.getDataFile(GOOD_CONFIG_FILE).c_str() );

		Message msg = cfgFile.lookupMessage("Msg1");

		test.check("Expected GMSEC specification", msg.getSchemaLevel() == Specification::SchemaLevel::LEVEL_2);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_load(Test& test)
{
	GMSEC_INFO << "Test load()";

	ConfigFile cfgFile;

	// Run nominal case
	try
	{
		cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());
		test.check("Expected to load good config file", cfgFile.isLoaded());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Run off-nominal cases
	try
	{
		cfgFile.load(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid or missing config file path") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.load("nonexistent.xml");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Error: File not found") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.load(test.getDataFile(BAD_CONFIG_FILE_1).c_str());
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid XML document - missing DEFINITIONS") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.load(test.getDataFile(BAD_CONFIG_FILE_2).c_str());
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Error: Mismatched element") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}
}


void test_save(Test& test)
{
	GMSEC_INFO << "Test save()";

	try
	{
		ConfigFile cfgFile;
		cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());

		cfgFile.save("new_config_file.xml");

		ConfigFile cfgFile2;
		cfgFile2.load("new_config_file.xml");

		test.check("Unexpected XML content", std::string(cfgFile.toXML()) == cfgFile2.toXML());

#ifdef WIN32
		_unlink("new_config_file.xml");
#else
		unlink("new_config_file.xml");
#endif
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_fromXML(Test& test)
{
	GMSEC_INFO << "Test fromXML()";

	ConfigFile cfgFile;

	// Nominal case
	try
	{
		cfgFile.fromXML(XML);
		test.check("Expected isLoaded to report true", cfgFile.isLoaded());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal cases
	try
	{
		cfgFile.fromXML(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid XML string") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.fromXML("");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid XML string") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.fromXML("<FOOBAR/>");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Invalid XML document - missing DEFINITIONS") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}

	try
	{
		cfgFile.fromXML("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Unable to parse XML string") != std::string::npos);
		test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());
	}
}


void test_toXML(Test& test)
{
	GMSEC_INFO << "Test toXML()";

	ConfigFile cfgFile;

	try
	{
		cfgFile.fromXML(XML);

		const char* c_xml = cfgFile.toXML();

		test.check("Unexpected XML content from config file", std::string(XML) == c_xml);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}
}


void test_isLoaded(Test& test)
{
	GMSEC_INFO << "Test isLoaded()";

	ConfigFile cfgFile;
	test.check("Expected isLoaded to report false", false == cfgFile.isLoaded());

	cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());
	test.check("Expected isLoaded to report true", true == cfgFile.isLoaded());
}


void test_add_config(Test& test)
{
	GMSEC_INFO << "Test addConfig()";

	Config config("name1=value1", DataType::KEY_VALUE_DATA);

	ConfigFile cfgFile;

	// Nominal test
	try
	{
		cfgFile.addConfig("Cfg1", config);

		Config tmp = cfgFile.lookupConfig("Cfg1");

		test.check("Unexpected config value", std::string(tmp.getValue("name1")) == config.getValue("name1"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		cfgFile.addConfig(NULL, config);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Configuration name cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		cfgFile.addConfig("", config);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Configuration name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_lookup_config(Test& test)
{
	GMSEC_INFO << "Test lookupConfig()";

	ConfigFile cfgFile;

	// Nominal case
	try
	{
		cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());

		Config cfg = cfgFile.lookupConfig("Bolt");

		test.check("Unexpected mw-id value", std::string("bolt") == cfg.getValue("mw-id"));
		test.check("Unexpected server value", std::string("localhost") == cfg.getValue("server"));

		cfg = cfgFile.lookupConfig("ActiveMQ");

		test.check("Unexpected mw-id value", std::string("activemq39") == cfg.getValue("mw-id"));
		test.check("Unexpected server value", std::string("tcp://localhost:61616") == cfg.getValue("server"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		(void) cfgFile.lookupConfig("bogus");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Configuration does not exist for the name bogus") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupConfig(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Configuration name is NULL, or is an empty string") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupConfig("");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Configuration name is NULL, or is an empty string") != std::string::npos);
	}
}


void test_remove_config(Test& test)
{
	GMSEC_INFO << "Test removeConfig()";

	ConfigFile cfgFile;

	cfgFile.addConfig("Bolt", Config("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA));

	test.check("Expected to remove existing config", true  == cfgFile.removeConfig("Bolt"));
	test.check("Did not expect to remove config", false == cfgFile.removeConfig("Bolt"));
	test.check("Did not expect to remove config", false == cfgFile.removeConfig("bogus"));
	test.check("Did not expect to remove config", false == cfgFile.removeConfig(NULL));
	test.check("Did not expect to remove config", false == cfgFile.removeConfig(""));
}


void test_add_message(Test& test)
{
	GMSEC_INFO << "Test addMessage()";

	ConfigFile     cfgFile;
	MessageFactory factory;

	Message msg = factory.createMessage("HB");
	msg.addField("FIELD1", "ONE");
	msg.addField("FIELD2", "TWO");

	// Nominal test
	try
	{
		cfgFile.addMessage("my-msg", msg);

		Message tmp = cfgFile.lookupMessage("my-msg");

		test.check("Unexpected message subject", std::string(msg.getSubject()) == tmp.getSubject());
		test.check("Unexpected message kind", msg.getKind() == tmp.getKind());
		test.check("Unexpected message field count", msg.getFieldCount() == tmp.getFieldCount());
		test.check("Unexpected field value", std::string("ONE") == tmp.getStringValue("FIELD1"));
		test.check("Unexpected field value", std::string("TWO") == tmp.getStringValue("FIELD2"));
		test.check("Unexpected XML content", std::string(msg.toXML()) == tmp.toXML());
		test.check("Unexpected JSON content", std::string(msg.toJSON()) == tmp.toJSON());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	try
	{
		cfgFile.addMessage(NULL, msg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message name cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		cfgFile.addMessage("", msg);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_lookup_message(Test& test)
{
	GMSEC_INFO << "Test lookupMessage()";

	ConfigFile cfgFile;

	// Nominal case
	try
	{
		cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());

		Message msg = cfgFile.lookupMessage("Msg1");
		test.check("Unexpected MESSAGE-TYPE", std::string("MSG") == msg.getStringValue("MESSAGE-TYPE"));
		test.check("Unexpected MESSAGE-SUBTYPE", std::string("HB") == msg.getStringValue("MESSAGE-SUBTYPE"));
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		(void) cfgFile.lookupMessage("bogus");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message does not exist for the name bogus") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupMessage(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message name cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupMessage("");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("Message name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_remove_message(Test& test)
{
	GMSEC_INFO << "Test removeMessage()";

	ConfigFile cfgFile;

	MessageFactory msgFactory;

	cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));

	test.check("Expected to remove existing message", true  == cfgFile.removeMessage("Heartbeat"));
	test.check("Did not expect to remove message", false == cfgFile.removeMessage("Heartbeat"));
	test.check("Did not expect to remove message", false == cfgFile.removeMessage("bogus"));
	test.check("Did not expect to remove message", false == cfgFile.removeMessage(NULL));
	test.check("Did not expect to remove message", false == cfgFile.removeMessage(""));
}


void test_add_subscription_entry(Test& test)
{
	GMSEC_INFO << "Test addSubscriptionEntry()";

	ConfigFile cfgFile;

	// Nominal test
	try
	{
		ConfigFile::SubscriptionEntry entry("Sub1", "GMSEC.>");
		entry.addExcludedPattern("GMSEC.*.*.MSG.HB.>");

		cfgFile.addSubscriptionEntry(entry);

		const ConfigFile::SubscriptionEntry& se = cfgFile.lookupSubscriptionEntry("Sub1");

		test.check("Unexpected subscription entry name", std::string("Sub1") == se.getName());
		test.check("Unexpected subscription entry pattern", std::string("GMSEC.>") == se.getPattern());
		test.check("Expected an excluded pattern", true == se.hasNextExcludedPattern());
		test.check("Unexpected excluded pattern", std::string("GMSEC.*.*.MSG.HB.>") == se.nextExcludedPattern());
		test.check("Another excluded pattern was not expected", false == se.hasNextExcludedPattern());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal tests
	struct TestArg
	{
		const char* name;
		const char* sub;
	};

	TestArg name_args[2] = { {NULL, NULL}, {"", NULL} };
	TestArg sub_args[2]  = { {"s1", NULL}, {"s2", ""} };

	for (size_t i = 0; i < sizeof(name_args)/sizeof(TestArg); ++i)
	{
		try
		{
			cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry(name_args[i].name, name_args[i].sub) );
			test.check("Was expecting an exception", false);
		}
		catch (const GmsecException& e)
		{
			test.check(e.what(), std::string(e.what()).find("SubscriptionEntry name cannot be NULL nor an empty string") != std::string::npos);
		}
	}

	for (size_t i = 0; i < sizeof(sub_args)/sizeof(TestArg); ++i)
	{
		try
		{
			cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry(sub_args[i].name, sub_args[i].sub) );
			test.check("Was expecting an exception", false);
		}
		catch (const GmsecException& e)
		{
			test.check(e.what(), std::string(e.what()).find("SubscriptionEntry pattern cannot be NULL nor an empty string") != std::string::npos);
		}
	}
}


void test_lookup_subscription_entry(Test& test)
{
	GMSEC_INFO << "Test lookupSubscriptionEntry()";

	ConfigFile cfgFile;

	// Nominal case
	try
	{
		cfgFile.load(test.getDataFile(GOOD_CONFIG_FILE).c_str());

		const ConfigFile::SubscriptionEntry& entry = cfgFile.lookupSubscriptionEntry("Sub1");

		test.check("Unexpected subscription entry name", std::string("Sub1") == entry.getName());
		test.check("Unexpected subscription entry pattern", std::string("GMSEC.>") == entry.getPattern());
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), false);
	}

	// Off-nominal case
	try
	{
		(void) cfgFile.lookupSubscriptionEntry("bogus");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SubscriptionEntry does not exist for the name bogus") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupSubscriptionEntry(NULL);
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SubscriptionEntry name cannot be NULL, nor an empty string") != std::string::npos);
	}

	try
	{
		(void) cfgFile.lookupSubscriptionEntry("");
		test.check("Was expecting an exception", false);
	}
	catch (const GmsecException& e)
	{
		test.check(e.what(), std::string(e.what()).find("SubscriptionEntry name cannot be NULL, nor an empty string") != std::string::npos);
	}
}


void test_remove_subscription(Test& test)
{
	GMSEC_INFO << "Test removeSubscription()";

	ConfigFile cfgFile;

	cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry("AllMessages", "GMSEC.>") );

	test.check("Expected to remove existing subscription", true  == cfgFile.removeSubscriptionEntry("AllMessages"));
	test.check("Did not expect to remove subscription", false == cfgFile.removeSubscriptionEntry("AllMessages"));
	test.check("Did not expect to remove subscription", false == cfgFile.removeSubscriptionEntry("bogus"));
	test.check("Did not expect to remove subscription", false == cfgFile.removeSubscriptionEntry(NULL));
	test.check("Did not expect to remove subscription", false == cfgFile.removeSubscriptionEntry(""));
}


void test_get_iterator(Test& test)
{
	GMSEC_INFO << "Test getIterator()";

	ConfigFile cfgFile;

	cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry("AllMessages", "GMSEC.>") );
	cfgFile.addSubscriptionEntry( ConfigFile::SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

	cfgFile.addConfig("Bolt", Config("mw-id=bolt server=localhost", DataType::KEY_VALUE_DATA));
	cfgFile.addConfig("ActiveMQ", Config("mw-id=activemq39 server=tcp://localhost:61616", DataType::KEY_VALUE_DATA));

	MessageFactory msgFactory;

	cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));
	cfgFile.addMessage("Log", msgFactory.createMessage("LOG"));

	ConfigFileIterator& iter = cfgFile.getIterator();

	int numSubscriptions = 0;
	int numConfigs = 0;
	int numMessages = 0;

	while (iter.hasNextSubscription())
	{
		(void) iter.nextSubscription();
		++numSubscriptions;
	}

	while (iter.hasNextConfig())
	{
		(void) iter.nextConfig();
		++numConfigs;
	}

	while (iter.hasNextMessage())
	{
		(void) iter.nextMessage();
		++numMessages;
	}

	test.check("Unexpected number of subscriptions", 2 == numSubscriptions);
	test.check("Unexpected number of configs", 2 == numConfigs);
	test.check("Unexpected number of messages", 2 == numMessages);
}


int test_ConfigFile(Test& test)
{
	test.setDescription("Test ConfigFile");

	test_constructor_1(test);
	test_constructor_2(test);
	test_load(test);
	test_save(test);
	test_fromXML(test);
	test_toXML(test);
	test_isLoaded(test);
	test_add_config(test);
	test_lookup_config(test);
	test_remove_config(test);
	test_add_message(test);
	test_lookup_message(test);
	test_remove_message(test);
	test_add_subscription_entry(test);
	test_lookup_subscription_entry(test);
	test_remove_subscription(test);
	test_get_iterator(test);

	return 0;
}

TEST_DRIVER(test_ConfigFile)

#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T008_ConfigFile extends TestCase
{
	constructor()
	{
		super();

		this.GOOD_CONFIG_FILE  = "good_config_file.xml";
		this.BAD_CONFIG_FILE_1 = "bad_config_file1.xml";
		this.BAD_CONFIG_FILE_2 = "bad_config_file2.xml";

		this.XML = "<DEFINITIONS>\n"
		           + "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&amp;gt\">\n"
		           + "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n"
		           + "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n"
		           + "    </SUBSCRIPTION>\n"
		           + "    <CONFIG NAME=\"Bolt\">\n"
		           + "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
		           + "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
		           + "    </CONFIG>\n"
		           + "    <MESSAGE NAME=\"Msg1\">\n"
		           + "        <CONFIG NAME=\"msg_config\">\n"
		           + "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n"
		           + "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n"
		           + "        </CONFIG>\n"
		           + "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
		           + "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
		           + "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n"
		           + "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n"
		           + "    </MESSAGE>\n"
		           + "</DEFINITIONS>";
	}

	testCase()
	{
		this.testConstructor_1();
		this.testConstructor_2();
		this.testLoad();
		this.testSave();
		this.testFromXML();
		this.testToXML();
		this.testIsLoaded();
		this.testAddConfig();
		this.testLookupConfig();
		this.testRemoveConfig();
		this.testAddMessage();
		this.testLookupMessage();
		this.testRemoveMessage();
		this.testAddSubscriptionEntry();
		this.testLookupSubscriptionEntry();
		this.testRemoveSubscription();
		this.testGetIterator();
	}

	testConstructor_1()
	{
		gmsec.Log.info("Test constructor w/ no args");

		try {
			var cfgFile = new gmsec.ConfigFile();
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
		catch (e) {
			this.check(e.message, false);
		}
	}

	testConstructor_2()
	{
		gmsec.Log.info("Test constructor w/ Config");

		try {
			var config  = new gmsec.Config();
			var cfgFile = new gmsec.ConfigFile(config);
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
		catch (e) {
			this.check(e.message, false);
		}

		try {
			var config  = new gmsec.Config("gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA);
			var cfgFile = new gmsec.ConfigFile(config);
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
		catch (e) {
			this.check(e.message, false);
		}
	}

	testLoad()
	{
		gmsec.Log.info("Test load()");

		var cfgFile = new gmsec.ConfigFile();

		// Nominal test
		try {
			cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));
			this.check("Expected isLoaded() to report true", cfgFile.isLoaded() == true);
		}
		catch (e) {
			this.check(e.message, false);
		}

		// Off-nominal tests
		try {
			// Non-existent configuration file
			cfgFile.load(this.getDataFile("nonexistent.xml"));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("File not found"));
		}

		try {
			// Non-conformant DEFINITIONS file
			cfgFile.load(this.getDataFile(this.BAD_CONFIG_FILE_1));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid XML document - missing DEFINITIONS"));
		}

		try {
			// Illegal XML content
			cfgFile.load(this.getDataFile(this.BAD_CONFIG_FILE_2));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Error: Mismatched element"));
		}
	}

	testSave()
	{
		gmsec.Log.info("Test save()");

		var cfgFile = new gmsec.ConfigFile();

		try {
			cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));

			cfgFile.save("new_config_file.xml");

			var cfgFile2 = new gmsec.ConfigFile();
			cfgFile2.load("new_config_file.xml");

			this.check("Unexpected XML content", cfgFile.toXML() === cfgFile2.toXML());
		}
		catch (e) {
			this.check(e.message, false);
		}

		{
			var fs = require('fs');
			fs.unlink("new_config_file.xml", function(err) { } );
		}
	}

	testFromXML()
	{
		gmsec.Log.info("Test fromXML()");

		var cfgFile = new gmsec.ConfigFile();

		// Nominal test
		try {
			cfgFile.fromXML(this.XML);
			this.check("Expected isLoaded() to report true", cfgFile.isLoaded());
		}
		catch (e) {
			this.check(e.message, false);
		}

		// Off-nominal tests
		try {
			// Empty string content
			cfgFile.fromXML("");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid XML string"));
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		try {
			// Invalid XML content
			cfgFile.fromXML("<FOOBAR/>");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid XML document - missing DEFINITIONS"));
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		try {
			// Incomplete content
			cfgFile.fromXML("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Unable to parse XML string"));
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
	}

	testToXML()
	{
		gmsec.Log.info("Test toXML()");

		var cfgFile = new gmsec.ConfigFile();
		cfgFile.fromXML(this.XML);
		this.check("Unexpected XML content from config file", cfgFile.toXML() === this.XML);
	}

	testIsLoaded()
	{
		gmsec.Log.info("Test isLoaded()");

		var cfgFile = new gmsec.ConfigFile();
		this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);

		cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));
		this.check("Expected isLoaded() to report true", cfgFile.isLoaded());

		try {
			cfgFile.load(this.getDataFile(this.BAD_CONFIG_FILE_1));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
	}

	testAddConfig()
	{
		gmsec.Log.info("Test addConfig()");

		var cfgFile = new gmsec.ConfigFile();
		var config  = new gmsec.Config("name1=value1", gmsec.DataType_KEY_VALUE_DATA);

		// Nominal test
		try {
			cfgFile.addConfig("Cfg1", config);
			var tmp = cfgFile.lookupConfig("Cfg1");
			this.check("Unexpected config value", config.getValue("name1") === tmp.getValue("name1"));
		}
		catch (e) {
			this.check(e.message, false);
		}

		// Off-nominal test
		try {
			// Empty string config name
			cfgFile.addConfig("", config);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Configuration name cannot be NULL, nor an empty string"));
		}
	}

	testLookupConfig()
	{
		gmsec.Log.info("Test lookupConfig()");

		var cfgFile = new gmsec.ConfigFile();
		cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));

		// Nominal tests
		var cfg = cfgFile.lookupConfig("Bolt");
		this.check("Unexpected mw-id value", cfg.getValue("mw-id") === "bolt");
		this.check("Unexpected server value", cfg.getValue("server") === "localhost");

		cfg = cfgFile.lookupConfig("ActiveMQ");
		this.check("Unexpected mw-id value", cfg.getValue("mw-id") === "activemq39");
		this.check("Unexpected server value", cfg.getValue("server") === "tcp://localhost:61616");

		// Off-nominal tests
		try {
			// Bogus config name
			cfgFile.lookupConfig("bogus");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Configuration does not exist for the name bogus"));
		}

		try {
			// Empty string config name
			cfgFile.lookupConfig("");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Configuration name is NULL, or is an empty string"));
		}
	}

	testRemoveConfig()
	{
		gmsec.Log.info("Test removeConfig()");

		var cfgFile = new gmsec.ConfigFile();

		cfgFile.addConfig("Bolt", new gmsec.Config("mw-id=bolt server=localhost", gmsec.DataType_KEY_VALUE_DATA));

		this.check("Expected to remove existing config", cfgFile.removeConfig("Bolt"));
		this.check("Did not expect to remove config", cfgFile.removeConfig("Bolt") == false);
		this.check("Did not expect to remove config", cfgFile.removeConfig("bogus") == false);
		this.check("Did not expect to remove config", cfgFile.removeConfig("") == false);
	}

	testAddMessage()
	{
		gmsec.Log.info("Test addMessage()");

		var cfgFile = new gmsec.ConfigFile();
		var factory = new gmsec.MessageFactory();

		this.setStandardFields(factory);

		var msg = factory.createMessage("HB");
		msg.addField(new gmsec.StringField("FIELD1", "ONE"));
		msg.addField(new gmsec.StringField("FIELD2", "TWO"));

		// Nominal test
		cfgFile.addMessage("my-msg", msg);

		var tmp = cfgFile.lookupMessage("my-msg");
		this.check("Unexpected message subject", tmp.getSubject() == msg.getSubject());
		this.check("Unexpected message kind", tmp.getKind() == msg.getKind());
		this.check("Unexpected message field count", tmp.getFieldCount() == msg.getFieldCount());
		this.check("Unexpected field value", tmp.getStringValue("FIELD1") == "ONE");
		this.check("Unexpected field value", tmp.getStringValue("FIELD2") == "TWO");
		this.check("Unexpected XML content", tmp.toXML() == msg.toXML());
		this.check("Unexpected JSON content", tmp.toJSON() == msg.toJSON());

		// Off-nominal test
		try {
			// Empty string message name
			cfgFile.addMessage("", msg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message name cannot be NULL, nor an empty string"));
		}
	}

	testLookupMessage()
	{
		gmsec.Log.info("Test lookupMessage()");

		var cfgFile = new gmsec.ConfigFile();

		// Nominal test
		cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));

		var msg = cfgFile.lookupMessage("Msg1");
		this.check("Unexpected MESSAGE-TYPE", msg.getStringValue("MESSAGE-TYPE") === "MSG");
		this.check("Unexpected MESSAGE-SUBTYPE", msg.getStringValue("MESSAGE-SUBTYPE") === "HB");

		// Off-nominal tests
		try {
			// Bogus message name
			cfgFile.lookupMessage("bogus");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message does not exist for the name bogus"));
		}

		try {
			// Empty string message name
			cfgFile.lookupMessage("");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message name cannot be NULL, nor an empty string"));
		}
	}

	testRemoveMessage()
	{
		gmsec.Log.info("Test removeMessage()");

		var cfgFile = new gmsec.ConfigFile();

		var factory = new gmsec.MessageFactory();
		this.setStandardFields(factory);

		cfgFile.addMessage("Heartbeat", factory.createMessage("HB"));

		this.check("Expected to remove existing message", cfgFile.removeMessage("Heartbeat"));
		this.check("Did not expect to remove message", cfgFile.removeMessage("Heartbeat") == false);
		this.check("Did not expect to remove message", cfgFile.removeMessage("bogus") == false);
		this.check("Did not expect to remove message", cfgFile.removeMessage("") == false);
	}

	testAddSubscriptionEntry()
	{
		gmsec.Log.info("Test addSubscriptionEntry()");

		var cfgFile = new gmsec.ConfigFile();

		// Nominal test
		var entry = new gmsec.SubscriptionEntry("Sub1", "GMSEC.>");
		entry.addExcludedPattern("GMSEC.*.*.MSG.HB.>");
		entry.addExcludedPattern("GMSEC.*.*.MSG.LOG.>");

		cfgFile.addSubscriptionEntry(entry);

		entry = cfgFile.lookupSubscriptionEntry("Sub1");

		this.check("Unexpected subscription entry name", entry.getName() == "Sub1");
		this.check("Unexpected subscription entry pattern", entry.getPattern() == "GMSEC.>");
		this.check("Expected an excluded pattern", entry.hasNextExcludedPattern() == true);
		this.check("Unexpected excluded pattern", entry.nextExcludedPattern() == "GMSEC.*.*.MSG.HB.>");
		this.check("Expected an excluded pattern", entry.hasNextExcludedPattern() == true);
		this.check("Unexpected excluded pattern", entry.nextExcludedPattern() == "GMSEC.*.*.MSG.LOG.>");
		this.check("Expected no additional excluded patterns", entry.hasNextExcludedPattern() == false);
	}

	testLookupSubscriptionEntry()
	{
		gmsec.Log.info("Test lookupSubscriptionEntry()");

		var cfgFile = new gmsec.ConfigFile();

		// Nominal case
		cfgFile.load(this.getDataFile(this.GOOD_CONFIG_FILE));

		var entry = cfgFile.lookupSubscriptionEntry("Sub1");
		this.check("Unexpected subscription entry name", entry.getName() === "Sub1");
		this.check("Unexpected subscription entry pattern", entry.getPattern() === "GMSEC.>");

		// Off-nominal cases
		try {
			// Bogus subscription name
			cfgFile.lookupSubscriptionEntry("bogus");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("SubscriptionEntry does not exist for the name bogus"));
		}

		try {
			// Empty string subscription name
			cfgFile.lookupSubscriptionEntry("");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("SubscriptionEntry name cannot be NULL, nor an empty string"));
		}
	}

	testRemoveSubscription()
	{
		gmsec.Log.info("Test removeSubscription()");

		var cfgFile = new gmsec.ConfigFile();

		cfgFile.addSubscriptionEntry(new gmsec.SubscriptionEntry("AllMessages", "GMSEC.>"));

		this.check("Expected to remove existing subscription", cfgFile.removeSubscriptionEntry("AllMessages"));
		this.check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("AllMessages") == false);
		this.check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("bogus") == false);
		this.check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("") == false);
	}

	testGetIterator()
	{
		gmsec.Log.info("Test getIterator()");

		var cfgFile = new gmsec.ConfigFile();

		cfgFile.addSubscriptionEntry(new gmsec.SubscriptionEntry("AllMessages", "GMSEC.>"));
		cfgFile.addSubscriptionEntry(new gmsec.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+"));

		cfgFile.addConfig("Bolt", new gmsec.Config("mw-id=bolt server=localhost", gmsec.DataType_KEY_VALUE_DATA));
		cfgFile.addConfig("ActiveMQ", new gmsec.Config("mw-id=activemq39 server=tcp://localhost:61616", gmsec.DataType_KEY_VALUE_DATA));

		var msgFactory = new gmsec.MessageFactory();

		this.setStandardFields(msgFactory);

		cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));
		cfgFile.addMessage("Log", msgFactory.createMessage("LOG"));

		var iter = cfgFile.getIterator();

		var numSubscriptions = 0;
		var numConfigs = 0;
		var numMessages = 0;

		while (iter.hasNextSubscription()) {
			iter.nextSubscription();
			numSubscriptions += 1;
		}

		while (iter.hasNextConfig()) {
			iter.nextConfig();
			numConfigs += 1;
		}

		while (iter.hasNextMessage()) {
			iter.nextMessage();
			numMessages += 1;
		}

		this.check("Unexpected number of subscriptions", numSubscriptions == 2);
		this.check("Unexpected number of configs", numConfigs == 2);
		this.check("Unexpected number of messages", numMessages == 2);
	}
}

test = new T008_ConfigFile();
test.setDescription("Test ConfigFile");
test.run(process.argv);

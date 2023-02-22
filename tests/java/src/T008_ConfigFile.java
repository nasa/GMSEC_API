import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.ConfigFile;
import gov.nasa.gsfc.gmsec.api5.ConfigFileIterator;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.io.File;


public class T008_ConfigFile extends TestCase
{
	public static void main(String[] args)
	{
		T008_ConfigFile test = new T008_ConfigFile();
		test.setDescription("Test ConfigFile");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_constructor_1();
			test_constructor_2();
			test_load();
			test_save();
			test_fromXML();
			test_toXML();
			test_isLoaded();
			test_add_config();
			test_lookup_config();
			test_remove_config();
			test_add_message();
			test_lookup_message();
			test_remove_message();
			test_add_subscription_entry();
			test_lookup_subscription_entry();
			test_remove_subscription();
			test_get_iterator();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private String GOOD_CONFIG_FILE  = "good_config_file.xml";
	private String BAD_CONFIG_FILE_1 = "bad_config_file1.xml";
	private String BAD_CONFIG_FILE_2 = "bad_config_file2.xml";

	private static String XML = "<DEFINITIONS>\n"
	                          + "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&gt;\">\n"
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


	private void test_constructor_1()
		throws Exception
	{
		Log.info("Test constructor (no args)");

		ConfigFile cfgFile = new ConfigFile();
		check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
	}


	private void test_constructor_2()
	{
		Log.info("Test constructor (Config arg)");

		try
		{
			ConfigFile cfgFile = new ConfigFile( getConfig() );
			check("Expected IsLoaded() to report false", cfgFile.isLoaded() == false);

			cfgFile.load( getDataFile(GOOD_CONFIG_FILE) );
			check("Expected IsLoaded to report false", cfgFile.isLoaded());

			Message msg = cfgFile.lookupMessage("Msg1");
			check("Expected C2MS specification", msg.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}

		try
		{
			ConfigFile cfgFile = new ConfigFile( new Config("gmsec-schema-level=2", Gmsec.DataType.KEY_VALUE_DATA) );
			check("Expected IsLoaded() to report false", cfgFile.isLoaded() == false);

			cfgFile.load( getDataFile(GOOD_CONFIG_FILE) );
			check("Expected IsLoaded to report false", cfgFile.isLoaded());

			Message msg = cfgFile.lookupMessage("Msg1");
			check("Expected GMSEC specification", msg.getSchemaLevel() == Specification.SchemaLevel.LEVEL_2);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
	}


	private void test_load()
		throws Exception
	{
		Log.info("Test load()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal test
		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));
		check("Expected isLoaded to report false", cfgFile.isLoaded());

		//o Off-nominal test
		try {
			cfgFile.load(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("File Path is null or contains an empty string"));
			//check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		try {
			cfgFile.load("nonexistent.xml");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Error: File not found"));
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		try {
			cfgFile.load(getDataFile(BAD_CONFIG_FILE_1));
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Invalid XML document - missing DEFINITIONS"));
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		try {
			cfgFile.load(getDataFile(BAD_CONFIG_FILE_2));
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Error: Mismatched element"));
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
	}


	private void test_save()
		throws Exception
	{
		Log.info("Test save()");

		ConfigFile cfgFile = new ConfigFile();
		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));

		cfgFile.save("new_config_file.xml");

		ConfigFile cfgFile2 = new ConfigFile();
		cfgFile2.load("new_config_file.xml");

		check("Unexpected XML content", cfgFile.toXML().equals(cfgFile2.toXML()));

		File file = new File("new_config_file.xml");
		file.delete();
	}


	private void test_fromXML()
		throws Exception
	{
		Log.info("Test fromXML()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal case
		cfgFile.fromXML(XML);
		check("Expected isLoaded() to report true", cfgFile.isLoaded());

		//o Off-nominal cases

		// null content
		try {
			cfgFile.fromXML(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("XML string is null or contains an empty string"));
			//check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		// empty-string content
		try {
			cfgFile.fromXML("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("XML string is null or contains an empty string"));
			//check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		// invalid content
		try {
			cfgFile.fromXML("<FOOBAR/>");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Invalid XML document - missing DEFINITIONS"));
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}

		// incomplete content
		try {
			cfgFile.fromXML("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Unable to parse XML string"));
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
	}


	private void test_toXML()
		throws Exception
	{
		Log.info("Test toXML()");

		ConfigFile cfgFile = new ConfigFile();

		cfgFile.fromXML(XML);

		check("Unexpected XML content from config file", cfgFile.toXML().equals(XML));
	}


	private void test_isLoaded()
		throws Exception
	{
		Log.info("Test isLoaded()");

		ConfigFile cfgFile = new ConfigFile();
		check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);

		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));
		check("Expected isLoaded() to report true", cfgFile.isLoaded());

		try {
			cfgFile.load(getDataFile(BAD_CONFIG_FILE_1));
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check("Expected isLoaded() to report false", cfgFile.isLoaded() == false);
		}
	}


	private void test_add_config()
		throws Exception
	{
		Log.info("Test addConfig()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal test
		Config config = new Config("name1=value1", Gmsec.DataType.KEY_VALUE_DATA);

		cfgFile.addConfig("Cfg1", config);

		Config tmp = cfgFile.lookupConfig("Cfg1");

		check("Unexpected config value", config.getValue("name1").equals(tmp.getValue("name1")));

		//o Off-nominal tests

		// null config name
		try {
			cfgFile.addConfig(null, config);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config name is null or contains an empty string"));
		}

		// empty-string config name
		try {
			cfgFile.addConfig("", config);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config name is null or contains an empty string"));
		}

		// null config
		try {
			cfgFile.addConfig("cfg", null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config object handle is null"));
		}
	}


	private void test_lookup_config()
		throws Exception
	{
		Log.info("Test lookupConfig()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal case
		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));

		Config cfg = cfgFile.lookupConfig("Bolt");
		check("Unexpected mw-id value", cfg.getValue("mw-id").equals("bolt"));
		check("Unexpected server value", cfg.getValue("server").equals("localhost"));

		cfg = cfgFile.lookupConfig("ActiveMQ");
		check("Unexpected mw-id value", cfg.getValue("mw-id").equals("activemq39"));
		check("Unexpected server value", cfg.getValue("server").equals("tcp://localhost:61616"));

		//o Off-nominal cases

		// bogus config name
		try {
			cfgFile.lookupConfig("bogus");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Configuration does not exist for the name bogus"));
		}

		// null config name
		try {
			cfgFile.lookupConfig(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config name is null or contains an empty string"));
		}

		// empty-string config name
		try {
			cfgFile.lookupConfig("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Config name is null or contains an empty string"));
		}
	}


	private void test_remove_config()
		throws Exception
	{
		Log.info("Test removeConfig()");

		ConfigFile cfgFile = new ConfigFile();

		cfgFile.addConfig("Bolt", new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA));

		check("Expected to remove existing config", cfgFile.removeConfig("Bolt"));
		check("Did not expect to remove config", cfgFile.removeConfig("Bolt") == false);
		check("Did not expect to remove config", cfgFile.removeConfig("bogus") == false);
		check("Did not expect to remove config", cfgFile.removeConfig(null) == false);
		check("Did not expect to remove config", cfgFile.removeConfig("") == false);
	}


	private void test_add_message()
		throws Exception
	{
		Log.info("Test addMessage()");

		ConfigFile     cfgFile = new ConfigFile();
		MessageFactory factory = MessageFactory.create();

		Message msg = factory.createMessage("HB");
		msg.addField("FIELD1", "ONE");
		msg.addField("FIELD2", "TWO");

		MessageFactory.destroy(factory);

		//o Nominal test
		cfgFile.addMessage("my-msg", msg);

		Message tmp = cfgFile.lookupMessage("my-msg");

		check("Unexpected message subject", tmp.getSubject().equals(msg.getSubject()));
		check("Unexpected message kind", tmp.getKind() == msg.getKind());
		check("Unexpected message field count", tmp.getFieldCount() == msg.getFieldCount());
		check("Unexpected field value", tmp.getStringValue("FIELD1").equals("ONE"));
		check("Unexpected field value", tmp.getStringValue("FIELD2").equals("TWO"));
		check("Unexpected XML content", tmp.toXML().equals(msg.toXML()));
		check("Unexpected JSON content", tmp.toJSON().equals(msg.toJSON()));

		//o Off-nominal tests

		// null message name
		try {
			cfgFile.addMessage(null, msg);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Message name is null or contains an empty string"));
		}

		// empty-string message name
		try {
			cfgFile.addMessage("", msg);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Message name is null or contains an empty string"));
		}

		// null message
		try {
			cfgFile.addMessage("msg", null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Message object handle is null"));
		}
	}


	private void test_lookup_message()
		throws Exception
	{
		Log.info("Test lookupMessage()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal case
		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));

		Message msg = cfgFile.lookupMessage("Msg1");
		check("Unexpected MESSAGE-TYPE", msg.getStringValue("MESSAGE-TYPE").equals("MSG"));
		check("Unexpected MESSAGE-SUBTYPE", msg.getStringValue("MESSAGE-SUBTYPE").equals("HB"));

		//o Off-nominal cases

		// bogus message name
		try {
			cfgFile.lookupMessage("bogus");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not exist for the name bogus"));
		}

		// null message name
		try {
			cfgFile.lookupMessage(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Message name is null or contains an empty string"));
		}

		// empty-string message name
		try {
			cfgFile.lookupMessage("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Message name is null or contains an empty string"));
		}
	}


	private void test_remove_message()
		throws Exception
	{
		Log.info("Test removeMessage()");

		ConfigFile cfgFile = new ConfigFile();

		MessageFactory msgFactory = MessageFactory.create();

		cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));

		MessageFactory.destroy(msgFactory);

		check("Expected to remove existing message", cfgFile.removeMessage("Heartbeat"));
		check("Did not expect to remove message", cfgFile.removeMessage("Heartbeat") == false);
		check("Did not expect to remove message", cfgFile.removeMessage("bogus") == false);
		check("Did not expect to remove message", cfgFile.removeMessage(null) == false);
		check("Did not expect to remove message", cfgFile.removeMessage("") == false);
	}


	private void test_add_subscription_entry()
		throws Exception
	{
		Log.info("Test addSubscription()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal case
		ConfigFile.SubscriptionEntry entry = new ConfigFile.SubscriptionEntry("Sub1", "GMSEC.>");
		entry.addExcludedPattern("GMSEC.*.*.MSG.HB.>");

		cfgFile.addSubscriptionEntry(entry);

		entry = cfgFile.lookupSubscriptionEntry("Sub1");

		check("Unexpected subscription entry name", entry.getName().equals("Sub1"));
		check("Unexpected subscription entry pattern", entry.getPattern().equals("GMSEC.>"));
		check("Expected an excluded pattern", entry.hasNextExcludedPattern() == true);
		check("Unexpected excluded pattern", entry.nextExcludedPattern().equals("GMSEC.*.*.MSG.HB.>"));
		check("Expected no additional excluded patterns", entry.hasNextExcludedPattern() == false);

		//o Off-nominal cases
		try {
			cfgFile.addSubscriptionEntry(null);
			check("An exception was expected",false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("SubscriptionEntry is null"));
		}
	}


	private void test_lookup_subscription_entry()
		throws Exception
	{
		Log.info("Test lookupSubscriptionEntry()");

		ConfigFile cfgFile = new ConfigFile();

		//o Nominal case
		cfgFile.load(getDataFile(GOOD_CONFIG_FILE));

		ConfigFile.SubscriptionEntry entry = cfgFile.lookupSubscriptionEntry("Sub1");

		check("Unexpected subscription entry name", entry.getName().equals("Sub1"));
		check("Unexpected subscription entry pattern", entry.getPattern().equals("GMSEC.>"));

		//o Off-nominal cases

		// bogus subscription name
		try {
			cfgFile.lookupSubscriptionEntry("bogus");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("SubscriptionEntry does not exist for the name bogus"));
		}

		// null subscription name
		try {
			cfgFile.lookupSubscriptionEntry(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("SubscriptionEntry name is null or contains an empty string"));
		}

		// empty-string subscription name
		try {
			cfgFile.lookupSubscriptionEntry("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("SubscriptionEntry name is null or contains an empty string"));
		}
	}


	private void test_remove_subscription()
		throws Exception
	{
		Log.info("Test removeSubscription()");

		ConfigFile cfgFile = new ConfigFile();

		cfgFile.addSubscriptionEntry( new ConfigFile.SubscriptionEntry("AllMessages", "GMSEC.>") );

		check("Expected to remove existing subscription", cfgFile.removeSubscriptionEntry("AllMessages"));
		check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("AllMessages") == false);
		check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("bogus") == false);
		check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry(null) == false);
		check("Did not expect to remove subscription", cfgFile.removeSubscriptionEntry("") == false);
	}


	private void test_get_iterator()
		throws Exception
	{
		Log.info("Test getIterator()");

		ConfigFile cfgFile = new ConfigFile();

		cfgFile.addSubscriptionEntry( new ConfigFile.SubscriptionEntry("AllMessages", "GMSEC.>") );
		cfgFile.addSubscriptionEntry( new ConfigFile.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

		cfgFile.addConfig("Bolt", new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA));
		cfgFile.addConfig("ActiveMQ", new Config("mw-id=activemq39 server=tcp://localhost:61616", Gmsec.DataType.KEY_VALUE_DATA));

		MessageFactory msgFactory = MessageFactory.create();

		cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));
		cfgFile.addMessage("Log", msgFactory.createMessage("LOG"));

		MessageFactory.destroy(msgFactory);

		ConfigFileIterator iter = cfgFile.getIterator();

		int numSubscriptions = 0;
		int numConfigs = 0;
		int numMessages = 0;

		while (iter.hasNextSubscription()) {
			iter.nextSubscription();
			++numSubscriptions;
		}

		while (iter.hasNextConfig()) {
			iter.nextConfig();
			++numConfigs;
		}

		while (iter.hasNextMessage()) {
			iter.nextMessage();
			++numMessages;
		}

		check("Unexpected number of subscriptions", numSubscriptions == 2);
		check("Unexpected number of configs", numConfigs == 2);
		check("Unexpected number of messages", numMessages == 2);
	}
}

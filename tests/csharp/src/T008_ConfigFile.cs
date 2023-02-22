namespace T008
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.IO;
	using System.Text;


	class T008_ConfigFile : TestCase
	{
		static void Main(string[] args)
		{
			T008_ConfigFile test = new T008_ConfigFile();
			test.SetDescription("Test ConfigFile");
			test.Run(args);
		}


		public override void Run()
		{
			Test_Constructor_1();
			Test_Constructor_2();
			Test_Load();
			Test_Save();
			Test_FromXML();
			Test_ToXML();
			Test_IsLoaded();
			Test_AddConfig();
			Test_LookupConfig();
			Test_RemoveConfig();
			Test_AddMessage();
			Test_LookupMessage();
			Test_RemoveMessage();
			Test_AddSubscriptionEntry();
			Test_LookupSubscriptionEntry();
			Test_RemoveSubscription();
			Test_GetIterator();
		}


		private string GOOD_CONFIG_FILE  = "good_config_file.xml";
		private string BAD_CONFIG_FILE_1 = "bad_config_file1.xml";
		private string BAD_CONFIG_FILE_2 = "bad_config_file2.xml";

		private static string XML = "<DEFINITIONS>\n"
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


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (no args)");

			ConfigFile cfgFile = new ConfigFile();
			Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor (Config arg)");

			try
			{
				ConfigFile cfgFile = new ConfigFile( GetConfig() );
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);

				cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));
				Check("Expected IsLoaded to report false", cfgFile.IsLoaded());

				Message msg = cfgFile.LookupMessage("Msg1");
				Check("Expected C2MS specification", msg.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}

			try
			{
				ConfigFile cfgFile = new ConfigFile( new Config("gmsec-schema-level=2", DataType.KEY_VALUE_DATA) );
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);

				cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));
				Check("Expected IsLoaded to report false", cfgFile.IsLoaded());

				Message msg = cfgFile.LookupMessage("Msg1");
				Check("Expected GMSEC specification", msg.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_2);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}
		}


		private void Test_Load()
		{
			Log.Info("Test Load()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal test
			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));
			Check("Expected IsLoaded to report false", cfgFile.IsLoaded());

			//o Off-nominal test
			try {
				cfgFile.Load(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid or missing config file path"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			try {
				cfgFile.Load("nonexistent.xml");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Error: File not found"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			try {
				cfgFile.Load(GetDataFile(BAD_CONFIG_FILE_1));
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid XML document - missing DEFINITIONS"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			try {
				cfgFile.Load(GetDataFile(BAD_CONFIG_FILE_2));
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Error: Mismatched element"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}
		}


		private void Test_Save()
		{
			Log.Info("Test Save()");

			ConfigFile cfgFile = new ConfigFile();
			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));

			cfgFile.Save("new_config_file.xml");

			ConfigFile cfgFile2 = new ConfigFile();
			cfgFile2.Load("new_config_file.xml");

			Check("Unexpected XML content", cfgFile.ToXML() == cfgFile2.ToXML());

			File.Delete("new_config_file.xml");
		}


		private void Test_FromXML()
		{
			Log.Info("Test FromXML()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal case
			cfgFile.FromXML(XML);
			Check("Expected IsLoaded() to report true", cfgFile.IsLoaded());

			//o Off-nominal cases

			// null content
			try {
				cfgFile.FromXML(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid XML string"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			// empty-string content
			try {
				cfgFile.FromXML("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid XML string"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			// invalid content
			try {
				cfgFile.FromXML("<FOOBAR/>");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Invalid XML document - missing DEFINITIONS"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}

			// incomplete content
			try {
				cfgFile.FromXML("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Unable to parse XML string"));
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}
		}


		private void Test_ToXML()
		{
			Log.Info("Test ToXML()");

			ConfigFile cfgFile = new ConfigFile();

			cfgFile.FromXML(XML);

			Check("Unexpected XML content from config file", cfgFile.ToXML() == XML);
		}


		private void Test_IsLoaded()
		{
			Log.Info("Test IsLoaded()");

			ConfigFile cfgFile = new ConfigFile();
			Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);

			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));
			Check("Expected IsLoaded() to report true", cfgFile.IsLoaded());

			try {
				cfgFile.Load(GetDataFile(BAD_CONFIG_FILE_1));
				Check("An exception was expected", false);
			}
			catch (GmsecException) {
				Check("Expected IsLoaded() to report false", cfgFile.IsLoaded() == false);
			}
		}


		private void Test_AddConfig()
		{
			Log.Info("Test AddConfig()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal test
			Config config = new Config("name1=value1", DataType.KEY_VALUE_DATA);

			cfgFile.AddConfig("Cfg1", config);

			Config tmp = cfgFile.LookupConfig("Cfg1");

			Check("Unexpected config value", config.GetValue("name1") == tmp.GetValue("name1"));

			//o Off-nominal tests

			// null config name
			try {
				cfgFile.AddConfig(null, config);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Configuration name cannot be NULL, nor an empty string"));
			}

			// empty-string config name
			try {
				cfgFile.AddConfig("", config);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Configuration name cannot be NULL, nor an empty string"));
			}

			// null config
			try {
				cfgFile.AddConfig("cfg", null);
				Check("An exception was expected", false);
			}
			catch (ArgumentNullException e) {
				Check(e.ToString(), e.ToString().Contains("gmsec::api5::Config const & type is null"));
			}
		}


		private void Test_LookupConfig()
		{
			Log.Info("Test LookupConfig()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal case
			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));

			Config cfg = cfgFile.LookupConfig("Bolt");
			Check("Unexpected mw-id value", cfg.GetValue("mw-id") == "bolt");
			Check("Unexpected server value", cfg.GetValue("server") == "localhost");

			cfg = cfgFile.LookupConfig("ActiveMQ");
			Check("Unexpected mw-id value", cfg.GetValue("mw-id") == "activemq39");
			Check("Unexpected server value", cfg.GetValue("server") == "tcp://localhost:61616");

			//o Off-nominal cases

			// bogus config name
			try {
				cfgFile.LookupConfig("bogus");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Configuration does not exist for the name bogus"));
			}

			// null config name
			try {
				cfgFile.LookupConfig(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Configuration name is NULL, or is an empty string"));
			}

			// empty-string config name
			try {
				cfgFile.LookupConfig("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Configuration name is NULL, or is an empty string"));
			}
		}


		private void Test_RemoveConfig()
		{
			Log.Info("Test RemoveConfig()");

			ConfigFile cfgFile = new ConfigFile();

			cfgFile.AddConfig("Bolt", new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA));

			Check("Expected to Remove existing config", cfgFile.RemoveConfig("Bolt"));
			Check("Did not expect to Remove config", cfgFile.RemoveConfig("Bolt") == false);
			Check("Did not expect to Remove config", cfgFile.RemoveConfig("bogus") == false);
			Check("Did not expect to Remove config", cfgFile.RemoveConfig(null) == false);
			Check("Did not expect to Remove config", cfgFile.RemoveConfig("") == false);
		}


		private void Test_AddMessage()
		{
			Log.Info("Test AddMessage()");

			ConfigFile     cfgFile = new ConfigFile();
			MessageFactory factory = new MessageFactory();

			SetStandardFields(factory);

			Message msg = factory.CreateMessage("HB");
			msg.AddField("FIELD1", "ONE");
			msg.AddField("FIELD2", "TWO");

			//o Nominal test
			cfgFile.AddMessage("my-msg", msg);

			Message tmp = cfgFile.LookupMessage("my-msg");

			Check("Unexpected message subject", tmp.GetSubject() == msg.GetSubject());
			Check("Unexpected message kind", tmp.GetKind() == msg.GetKind());
			Check("Unexpected message field count", tmp.GetFieldCount() == msg.GetFieldCount());
			Check("Unexpected field value", tmp.GetStringValue("FIELD1") == "ONE");
			Check("Unexpected field value", tmp.GetStringValue("FIELD2") == "TWO");
			Check("Unexpected XML content", tmp.ToXML() == msg.ToXML());
			Check("Unexpected JSON content", tmp.ToJSON() == msg.ToJSON());

			//o Off-nominal tests

			// null message name
			try {
				cfgFile.AddMessage(null, msg);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message name cannot be NULL, nor an empty string"));
			}

			// empty-string message name
			try {
				cfgFile.AddMessage("", msg);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message name cannot be NULL, nor an empty string"));
			}

			// null message
			try {
				cfgFile.AddMessage("msg", null);
				Check("An exception was expected", false);
			}
			catch (ArgumentNullException e) {
				Check(e.ToString(), e.ToString().Contains("gmsec::api5::Message const & type is null"));
			}
		}


		private void Test_LookupMessage()
		{
			Log.Info("Test LookupMessage()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal case
			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));

			Message msg = cfgFile.LookupMessage("Msg1");
			Check("Unexpected MESSAGE-TYPE", msg.GetStringValue("MESSAGE-TYPE") == "MSG");
			Check("Unexpected MESSAGE-SUBTYPE", msg.GetStringValue("MESSAGE-SUBTYPE") == "HB");

			//o Off-nominal cases

			// bogus message name
			try {
				cfgFile.LookupMessage("bogus");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message does not exist for the name bogus"));
			}

			// null message name
			try {
				cfgFile.LookupMessage(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message name cannot be NULL, nor an empty string"));
			}

			// empty-string message name
			try {
				cfgFile.LookupMessage("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Message name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_RemoveMessage()
		{
			Log.Info("Test RemoveMessage()");

			ConfigFile cfgFile = new ConfigFile();

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			cfgFile.AddMessage("Heartbeat", msgFactory.CreateMessage("HB"));

			Check("Expected to Remove existing message", cfgFile.RemoveMessage("Heartbeat"));
			Check("Did not expect to Remove message", cfgFile.RemoveMessage("Heartbeat") == false);
			Check("Did not expect to Remove message", cfgFile.RemoveMessage("bogus") == false);
			Check("Did not expect to Remove message", cfgFile.RemoveMessage(null) == false);
			Check("Did not expect to Remove message", cfgFile.RemoveMessage("") == false);
		}


		private void Test_AddSubscriptionEntry()
		{
			Log.Info("Test AddSubscriptionEntry()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal case
			SubscriptionEntry entry = new SubscriptionEntry("Sub1", "GMSEC.>");
			entry.AddExcludedPattern("GMSEC.*.*.MSG.HB.>");

			cfgFile.AddSubscriptionEntry(entry);

			entry = cfgFile.LookupSubscriptionEntry("Sub1");

			Check("Unexpected subscription entry name", entry.GetName() == "Sub1");
			Check("Unexpected subscription entry pattern", entry.GetPattern() == "GMSEC.>");
			Check("Expected an excluded pattern", entry.HasNextExcludedPattern() == true);
			Check("Unexpected excluded pattern", entry.NextExcludedPattern() == "GMSEC.*.*.MSG.HB.>");
			Check("Expected no additional excluded patterns", entry.HasNextExcludedPattern() == false);

			//o Off-nominal cases
			try {
				cfgFile.AddSubscriptionEntry(null);
				Check("An exception was expected",false);
			}
			catch (ArgumentNullException e) {
				Check(e.ToString(), e.ToString().Contains("gmsec::api5::ConfigFile::SubscriptionEntry const & type is null"));
			}
		}


		private void Test_LookupSubscriptionEntry()
		{
			Log.Info("Test LookupSubscriptionEntry()");

			ConfigFile cfgFile = new ConfigFile();

			//o Nominal case
			cfgFile.Load(GetDataFile(GOOD_CONFIG_FILE));

			SubscriptionEntry entry = cfgFile.LookupSubscriptionEntry("Sub1");

			Check("Unexpected subscription entry name", entry.GetName() == "Sub1");
			Check("Unexpected subscription entry pattern", entry.GetPattern() == "GMSEC.>");

			//o Off-nominal cases

			// bogus subscription name
			try {
				cfgFile.LookupSubscriptionEntry("bogus");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("SubscriptionEntry does not exist for the name bogus"));
			}

			// null subscription name
			try {
				cfgFile.LookupSubscriptionEntry(null);
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("SubscriptionEntry name cannot be NULL, nor an empty string"));
			}

			// empty-string subscription name
			try {
				cfgFile.LookupSubscriptionEntry("");
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("SubscriptionEntry name cannot be NULL, nor an empty string"));
			}
		}


		private void Test_RemoveSubscription()
		{
			Log.Info("Test RemoveSubscription()");

			ConfigFile cfgFile = new ConfigFile();

			cfgFile.AddSubscriptionEntry( new SubscriptionEntry("AllMessages", "GMSEC.>") );

			Check("Expected to Remove existing subscription", cfgFile.RemoveSubscriptionEntry("AllMessages"));
			Check("Did not expect to Remove subscription", cfgFile.RemoveSubscriptionEntry("AllMessages") == false);
			Check("Did not expect to Remove subscription", cfgFile.RemoveSubscriptionEntry("bogus") == false);
			Check("Did not expect to Remove subscription", cfgFile.RemoveSubscriptionEntry(null) == false);
			Check("Did not expect to Remove subscription", cfgFile.RemoveSubscriptionEntry("") == false);
		}


		private void Test_GetIterator()
		{
			Log.Info("Test GetIterator()");

			ConfigFile cfgFile = new ConfigFile();

			cfgFile.AddSubscriptionEntry( new SubscriptionEntry("AllMessages", "GMSEC.>") );
			cfgFile.AddSubscriptionEntry( new SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

			cfgFile.AddConfig("Bolt", new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA));
			cfgFile.AddConfig("ActiveMQ", new Config("mw-id=activemq39 server=tcp://localhost:61616", DataType.KEY_VALUE_DATA));

			MessageFactory msgFactory = new MessageFactory();

			SetStandardFields(msgFactory);

			cfgFile.AddMessage("Heartbeat-1", msgFactory.CreateMessage("HB"));
			cfgFile.AddMessage("Heartbeat-2", msgFactory.CreateMessage("HB"));

			ConfigFileIterator iter = cfgFile.GetIterator();

			int numSubscriptions = 0;
			int numConfigs = 0;
			int numMessages = 0;

			while (iter.HasNextSubscription()) {
				iter.NextSubscription();
				++numSubscriptions;
			}

			while (iter.HasNextConfig()) {
				iter.NextConfig();
				++numConfigs;
			}

			while (iter.HasNextMessage()) {
				iter.NextMessage();
				++numMessages;
			}

			Check("Unexpected number of subscriptions", numSubscriptions == 2);
			Check("Unexpected number of configs", numConfigs == 2);
			Check("Unexpected number of messages", numMessages == 2);
		}
	}
}

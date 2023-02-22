namespace T009
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T009_ConfigFileIterator : TestCase
	{
		private ConfigFile cfgFile = null;
		private Message    hbMsg   = null;
		private Message    logMsg  = null;


		static void Main(string[] args)
		{
			T009_ConfigFileIterator test = new T009_ConfigFileIterator();
			test.SetDescription("Test ConfigFileIterator");
			test.Run(args);
		}


		public override void Run()
		{
			Setup();

			Test_Iterator();
			Test_Reset();
		}


		private void Test_Iterator()
		{
			ConfigFileIterator iter = cfgFile.GetIterator();

			Log.Info("Test HasNextSubscription() and NextSubscription()");

			int numSubscriptions = 0;
			while (iter.HasNextSubscription())
			{
				SubscriptionEntry entry = iter.NextSubscription();

				++numSubscriptions;

				if (numSubscriptions == 1)
				{
					Check("Unexpected subscription entry 1 name", entry.GetName() == "AllMessages");
					Check("Unexpected subscription entry 1 pattern", entry.GetPattern() == "GMSEC.>");
				}
				else
				{
					Check("Unexpected subscription entry 2 name", entry.GetName() == "LogMessages");
					Check("Unexpected subscription entry 2 pattern", entry.GetPattern() == "GMSEC.*.*.MSG.LOG.+");
				}
			}
			Check("Unexpected number of subscription entries", numSubscriptions == 2);


			Log.Info("Test HasNextConfig() and NextConfig()");

			int numConfigs = 0;
			while (iter.HasNextConfig())
			{
				ConfigEntry entry = iter.NextConfig();

				++numConfigs;

				if (numConfigs == 1)
				{
					Check("Unexpected config entry 1 name", entry.GetName() == "ActiveMQ");
					Check("Unexpected config entry mw-id", entry.GetConfig().GetValue("mw-id") == "activemq39");
					Check("Unexpected config entry server", entry.GetConfig().GetValue("server") == "tcp://localhost:61616");
				}
				else
				{
					Check("Unexpected config entry 2 name", entry.GetName() == "Bolt");
					Check("Unexpected config entry mw-id", entry.GetConfig().GetValue("mw-id") == "bolt");
					Check("Unexpected config entry server", entry.GetConfig().GetValue("server") == "localhost");
				}
			}
			Check("Unexpected number of config entries", numConfigs == 2);


			Log.Info("Test HasNextMessage() and NextMessage()");

			int numMessages = 0;
			while (iter.HasNextMessage())
			{
				MessageEntry entry = iter.NextMessage();

				++numMessages;

				if (numMessages == 1)
				{
					Check("Unexpected message entry 1 name", entry.GetName() == "Heartbeat");
					Check("Unexpected message entry 1", entry.GetMessage().ToXML() == hbMsg.ToXML());
				}
				else
				{
					Check("Unexpected message entry 2 name", entry.GetName() == "Log");
					Check("Unexpected message entry 2", entry.GetMessage().ToXML() == logMsg.ToXML());
				}
			}
			Check("Unexpected number of message entries", numMessages == 2);
		}


		private void Test_Reset()
		{
			Log.Info("Test Reset()");

			ConfigFileIterator iter = cfgFile.GetIterator();

			Check("Expected to have subscriptions", iter.HasNextSubscription());
			Check("Expected to have configs", iter.HasNextConfig());
			Check("Expected to have messages", iter.HasNextMessage());

			while (iter.HasNextSubscription()) iter.NextSubscription();
			while (iter.HasNextConfig()) iter.NextConfig();
			while (iter.HasNextMessage()) iter.NextMessage();

			Check("Expected NOT to have additional subscriptions", iter.HasNextSubscription() == false);
			Check("Expected NOT to have additional configs", iter.HasNextConfig() == false);
			Check("Expected NOT to have additional messages", iter.HasNextMessage() == false);

			iter.Reset();

			Check("Expected to have subscriptions", iter.HasNextSubscription());
			Check("Expected to have configs", iter.HasNextConfig());
			Check("Expected to have messages", iter.HasNextMessage());
		}


		private void Setup()
		{
			cfgFile = new ConfigFile();

			cfgFile.AddSubscriptionEntry( new SubscriptionEntry("AllMessages", "GMSEC.>") );
			cfgFile.AddSubscriptionEntry( new SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

			cfgFile.AddConfig("ActiveMQ", new Config("mw-id=activemq39 server=tcp://localhost:61616", DataType.KEY_VALUE_DATA));
			cfgFile.AddConfig("Bolt", new Config("mw-id=bolt server=localhost", DataType.KEY_VALUE_DATA));

			MessageFactory msgFactory = new MessageFactory();
			SetStandardFields(msgFactory);

			hbMsg  = msgFactory.CreateMessage("HB");
			logMsg = msgFactory.CreateMessage("LOG");

			logMsg.AddField("SUBCLASS", "T009");
			logMsg.AddField("OCCURRENCE-TYPE", "TEST");
			logMsg.AddField("SEVERITY", (short) 1);

			cfgFile.AddMessage("Heartbeat", hbMsg);
			cfgFile.AddMessage("Log", logMsg);
		}
	}
}

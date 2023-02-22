import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.ConfigFile;
import gov.nasa.gsfc.gmsec.api5.ConfigFileIterator;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T009_ConfigFileIterator extends TestCase
{
	private static ConfigFile     cfgFile    = null;
	private static MessageFactory msgFactory = null;


	public static void main(String[] args)
	{
		T009_ConfigFileIterator test = new T009_ConfigFileIterator();
		test.setDescription("Test ConfigFileIterator");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			setup();

			test_iterator();
			test_reset();

			teardown();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_iterator()
		throws Exception
	{
		ConfigFileIterator iter = cfgFile.getIterator();

		Log.info("Test hasNextSubscription() and nextSubscription()");

		int numSubscriptions = 0;
		while (iter.hasNextSubscription())
		{
			ConfigFile.SubscriptionEntry entry = iter.nextSubscription();

			++numSubscriptions;

			if (numSubscriptions == 1)
			{
				check("Unexpected subscription entry 1 name", entry.getName().equals("AllMessages"));
				check("Unexpected subscription entry 1 pattern", entry.getPattern().equals("GMSEC.>"));
			}
			else
			{
				check("Unexpected subscription entry 2 name", entry.getName().equals("LogMessages"));
				check("Unexpected subscription entry 2 pattern", entry.getPattern().equals("GMSEC.*.*.MSG.LOG.+"));
			}
		}
		check("Unexpected number of subscription entries", numSubscriptions == 2);


		Log.info("Test hasNextConfig() and nextConfig()");

		int numConfigs = 0;
		while (iter.hasNextConfig())
		{
			ConfigFile.ConfigEntry entry = iter.nextConfig();

			++numConfigs;

			if (numConfigs == 1)
			{
				check("Unexpected config entry 1 name", entry.getName().equals("ActiveMQ"));
				check("Unexpected config entry mw-id", entry.getConfig().getValue("mw-id").equals("activemq39"));
				check("Unexpected config entry server", entry.getConfig().getValue("server").equals("tcp://localhost:61616"));
			}
			else
			{
				check("Unexpected config entry 2 name", entry.getName().equals("Bolt"));
				check("Unexpected config entry mw-id", entry.getConfig().getValue("mw-id").equals("bolt"));
				check("Unexpected config entry server", entry.getConfig().getValue("server").equals("localhost"));
			}
		}
		check("Unexpected number of config entries", numConfigs == 2);


		Log.info("Test hasNextMessage() and nextMessage()");

		int numMessages = 0;
		while (iter.hasNextMessage())
		{
			ConfigFile.MessageEntry entry = iter.nextMessage();

			++numMessages;

			if (numMessages == 1)
			{
				check("Unexpected message entry 1 name", entry.getName().equals("Heartbeat"));
				check("Unexpected message entry 1", entry.getMessage().toXML().equals( msgFactory.createMessage("HB").toXML() ));
			}
			else
			{
				check("Unexpected message entry 2 name", entry.getName().equals("Log"));
				check("Unexpected message entry 2", entry.getMessage().toXML().equals( msgFactory.createMessage("LOG").toXML() ));
			}
		}
		check("Unexpected number of message entries", numMessages == 2);
	}


	private void test_reset()
		throws Exception
	{
		Log.info("Test reset()");

		ConfigFileIterator iter = cfgFile.getIterator();

		check("Expected to have subscriptions", iter.hasNextSubscription());
		check("Expected to have configs", iter.hasNextConfig());
		check("Expected to have messages", iter.hasNextMessage());

		while (iter.hasNextSubscription()) iter.nextSubscription();
		while (iter.hasNextConfig()) iter.nextConfig();
		while (iter.hasNextMessage()) iter.nextMessage();

		check("Expected NOT to have additional subscriptions", iter.hasNextSubscription() == false);
		check("Expected NOT to have additional configs", iter.hasNextConfig() == false);
		check("Expected NOT to have additional messages", iter.hasNextMessage() == false);

		iter.reset();

		check("Expected to have subscriptions", iter.hasNextSubscription());
		check("Expected to have configs", iter.hasNextConfig());
		check("Expected to have messages", iter.hasNextMessage());
	}


	private static void setup()
		throws Exception
	{
		if (cfgFile == null) {
			cfgFile = new ConfigFile();
		}

		if (msgFactory == null) {
			msgFactory = MessageFactory.create();
		}

		cfgFile.addSubscriptionEntry( new ConfigFile.SubscriptionEntry("AllMessages", "GMSEC.>") );
		cfgFile.addSubscriptionEntry( new ConfigFile.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") );

		cfgFile.addConfig("ActiveMQ", new Config("mw-id=activemq39 server=tcp://localhost:61616", Gmsec.DataType.KEY_VALUE_DATA));
		cfgFile.addConfig("Bolt", new Config("mw-id=bolt server=localhost", Gmsec.DataType.KEY_VALUE_DATA));

		cfgFile.addMessage("Heartbeat", msgFactory.createMessage("HB"));
		cfgFile.addMessage("Log", msgFactory.createMessage("LOG"));
	}


	private static void teardown()
	{
		MessageFactory.destroy(msgFactory);
	}
}

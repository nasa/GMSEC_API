/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file configfile.java
 *
 * A java example demonstration of loading pre-defined subscriptions, Config
 * objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 * configuration file used for this example program is "configfile.xml".
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

public class configfile{
	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java configfile <filename>");
			System.exit(-1);
		}

		try
		{
			initializeLogging();

			// load configuration file
			ConfigFile cfgFile = new ConfigFile();
			String cfgFilename = args[0];
			cfgFile.load(cfgFilename);

			// retrieve config objects from the Config file
			Config c1 = cfgFile.lookupConfig("config1");
			Config c2 = cfgFile.lookupConfig("config2");

			// Display log of XML representation of Config objects
			Log.info("Config 1:\n" + c1.toXML());
			Log.info("Config 2:\n" + c2.toXML());

			// lookup subscription topic from configuration file, and any excluded topics associated with such
			try
			{
				ConfigFile.SubscriptionEntry subEntry = cfgFile.lookupSubscriptionEntry("events");
				Log.info("Subscription pattern: " + subEntry.getPattern());

				while (subEntry.hasNextExcludedPattern())
				{
					Log.info("Subscription excluded pattern: " + subEntry.nextExcludedPattern());
				}
			}
			catch (GMSEC_Exception e)
			{
				Log.error("Failed to acquire 'events' subscription entry; reason: " + e.getMessage());
			}

			// lookup a Message from the configuration file
			Message message = cfgFile.lookupMessage("msg1");

			// Display XML representation of the message
			Log.info("Message:\n" + message.toXML());

			// Obtain ConfigFile Iterator to examine all of the various
			// entries defined in the ConfigFile
			ConfigFileIterator iter = cfgFile.getIterator();

			// Acquire and display all Config entries
			while (iter.hasNextConfig())
			{
				ConfigFile.ConfigEntry entry = iter.nextConfig();

				Log.info("\nConfig Name: " + entry.getName() +
								 "\nConfig     :\n" + entry.getConfig().toXML());
			}

			// Acquire and display all Message entries
			while (iter.hasNextMessage())
			{
				ConfigFile.MessageEntry entry = iter.nextMessage();

				Log.info("\nMessage Name: " + entry.getName() +
								 "\nMessage     :\n" + entry.getMessage().toXML());
			}

			// Acquire and display all subscription entries
			while (iter.hasNextMessage())
			{
				ConfigFile.SubscriptionEntry entry = iter.nextSubscription();

				Log.info("\nSubscription Name: " + entry.getName() +
								 "\nSubscription     :\n" + entry.getPattern());
			}

			// Acquire and display all custom XML entries
			while (iter.hasNextCustomElement())
			{
				String element = iter.nextCustomElement();

				Log.info("\nCustom XML Element:\n" + element);
			}
		}
		catch (Exception e)
		{
			Log.error(e.getMessage());
			System.exit(-1);
		}
	}

	public static void initializeLogging()
	{
		// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
		// object, it affects the logging level and output destination for all
		// logging macros in the entire API, regardless of whether that Config
		// object is kept around or not. In this case, we are creating a local
		// Config object just for the sake of initializing logging.
		Config config = new Config();

		try
		{
			String logLevel = config.getValue("LOGLEVEL");
			String logFile = config.getValue("LOGFILE");

			if (logLevel == null)
			{
				config.addValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				config.addValue("LOGFILE", "STDOUT");
			}
		}
		catch(Exception e)
		{
			Log.error(e.toString());
		}
  }
}

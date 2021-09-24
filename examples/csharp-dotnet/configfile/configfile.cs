/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file configfile.cs
 *
 * A C# example demonstration of loading pre-defined subscriptions, Config
 * objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 * configuration file used for this example program is "configfile.xml".
 */

using GMSEC.API;
using GMSEC.API.UTIL;
using GMSEC.API.MIST;
using System;

class publish
{
	static int Main(string[] args)
	{
		if (args.Length < 1)
		{
			Console.WriteLine("usage configfile.exe <filename>");
			return -1;
		}

		try
		{
			InitializeLogging();

			// load configuration file
			ConfigFile cfgFile = new ConfigFile();
			String cfgFilename = args[0];
			cfgFile.Load(cfgFilename);

			// retrieve config objects from the Config file
			Config c1 = cfgFile.LookupConfig("config1");
			Config c2 = cfgFile.LookupConfig("config2");

			// Display log of XML representation of Config objects
			Log.Info("Config 1:\n" + c1.ToXML());
			Log.Info("Config 2:\n" + c2.ToXML());


			// lookup subscription topic from configuration file, including excluded topics
			try
			{
				ConfigFile.SubscriptionEntry subEntry = cfgFile.LookupSubscriptionEntry("events");

				Log.Info("Subscription pattern: " + subEntry.GetPattern());

				while (subEntry.HasNextExcludedPattern())
				{
					Log.Info("Subscription excluded pattern: " + subEntry.NextExcludedPattern());
				}
			}
			catch (Exception e)
			{
				Log.Error("Failed to acquire 'events' subscription entry; reason: " + e.ToString());
			}


			// lookup a Message from the configuration file
			Message message = cfgFile.LookupMessage("msg1");

			// Display XML representation of the message
			Log.Info("Message:\n" + message.ToXML());

			// Obtain ConfigFile Iterator to examine all of the various
			// entries defined in the ConfigFile
			ConfigFileIterator iter = cfgFile.GetIterator();

			// Acquire and display all Config entries
			while (iter.HasNextConfig())
			{
				ConfigFile.ConfigEntry entry = iter.NextConfig();

				Log.Info("\nConfig Name: " + entry.GetName() +
								 "\nConfig     :\n" + entry.GetConfig().ToXML());
			}

			// Acquire and display all Message entries
			while (iter.HasNextMessage())
			{
				ConfigFile.MessageEntry entry = iter.NextMessage();

				Log.Info("\nMessage Name: " + entry.GetName() +
								 "\nMessage     :\n" + entry.GetMessage().ToXML());
			}

			// Acquire and display all Message entries
			while (iter.HasNextSubscription())
			{
				ConfigFile.SubscriptionEntry entry = iter.NextSubscription();

				Log.Info("\nSubscription Name : " + entry.GetName() +
								 "\nSubscription Topic: " + entry.GetSubject());

                while (entry.HasNextExcludedPattern())
                {
                    Log.Info("\nExcluded Pattern: " + entry.NextExcludedPattern());
                }
			}

			while (iter.HasNextCustomElement())
			{
				String element = iter.NextCustomElement();

				Log.Info("\nCustom XML Element:\n" + element);
			}
		}
		catch (Exception e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}

	static void InitializeLogging()
	{
		// Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
		// object, it affects the logging level and output destination for all
		// logging macros in the entire API, regardless of whether that Config
		// object is kept around or not. In this case, we are creating a local
		// Config object just for the sake of initializing logging.
		Config config = new Config();

		String logLevel = config.GetValue("LOGLEVEL");
		String logFile = config.GetValue("LOGFILE");

		if (logLevel == null)
		{
			config.AddValue("LOGLEVEL", "INFO");
		}
		if (logFile == null)
		{
			config.AddValue("LOGFILE", "STDERR");
		}
	}
}

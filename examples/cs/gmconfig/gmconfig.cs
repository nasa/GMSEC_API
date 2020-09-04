/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmconfig.cs
*
*  A C# example program demonstrating the use of a ConfigFile.
*  (Loading form a XML file, Creating a Connection, Subscribing & Publishing)
*/


namespace gmconfig
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
    using System.Collections.Generic;

	class gmconfig : GmsecExample
	{
		private ConfigFile cfgFile;
		private Connection conn;
	    private List<SubscriptionInfo> info = new List<SubscriptionInfo>();

		public gmconfig()
		{
			Config tmpConfig = new Config();
			Initialize(tmpConfig);
		}


		private bool printUsage(int numberArgs)
		{
			if (numberArgs < 1)
			{
				Console.Out.WriteLine("\nUsage: gmconfig_cs <filename>\n");
				return true;
			}
			return false;
		}


		private void Run(string[] args)
		{
			if (printUsage(args.Length))
			{
				return;
			}

			try
			{
				// Create the ConfigFile
				cfgFile = new ConfigFile();

				// Load Config File
				cfgFile.Load(args[0]);

				// Lookup Configs from config file
				Config config1 = cfgFile.LookupConfig("config1");
				Config config2 = cfgFile.LookupConfig("config2");

				// Display XML representation of the Configs
				Log.Info("Config 1:\n" + config1.ToXML());
				Log.Info("Config 2:\n" + config2.ToXML());

				// Create connection and connect
				conn = Connection.Create(config2);
				conn.Connect();

				// Lookup subscription patterns from config file and subscribe 
				string pattern1 = cfgFile.LookupSubscription("events");
				string pattern2 = cfgFile.LookupSubscription("custom1");

				Log.Info("Subscribing to " + pattern1);
                info.Add(conn.Subscribe(pattern1));

				Log.Info("Subscribing to " + pattern2);
                info.Add(conn.Subscribe(pattern2));

				// Lookup Message from config file
				Message msg = cfgFile.LookupMessage("msg1");

				// Display XML representation of the message
				Log.Info("Publishing Message:\n" + msg.ToXML());

				// Publish the message
				conn.Publish(msg);
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("Exception: " + e.ToString());
			}
			finally
			{
				if (conn != null)
				{
                    for (int i = info.Count - 1; i >= 0; i--)
                    {
                        Log.Info("Unsubscribing to " + info[i].GetSubject());
                        var temp = info[i];
                        conn.Unsubscribe(ref temp);
                        info.RemoveAt(i);
                    }
                    
                    // Disconnect connection
					conn.Disconnect();

					// Destroy connection
					Connection.Destroy(ref conn);
				}
			}
        }


		static void Main(string[] args)
		{
			new gmconfig().Run(args);
		}
	}
}

/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/*! \file Recorder.cs
 *  A simple application built with the C# GMSEC API. This
 *  application demonstrates several key components within the GMSEC API
 *  including loading from an xml file, producing a heartbeat, and publishing
*/


using System.Collections.Generic;

namespace Recorder
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;


	class LogCallback : Callback
	{
		public override void OnMessage(Connection conn, Message msg)
		{
			Log.Info("[LogCallback.OnMessage] Received Message:\n" + msg.ToXML());
		}
	}


	class Recorder : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 


		public Recorder()
		{
			Config tmpConfig = new Config();
			Initialize(tmpConfig);
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine("usage: Recorder <filename>\n");
		}


		private void Cleanup()
		{
			if (conn != null)
			{
                for (int i = info.Count - 1; i >= 0; i--)
                {
                    Log.Info("Unsubscribing from " + info[i].GetSubject());
                    var temp = info[i];
                    conn.Unsubscribe(ref temp);
                    info.RemoveAt(i);
                }
                
                conn.Disconnect();

				Connection.Destroy(ref conn);
			}
		}


		private void Run(string[] args)
		{
			if (args.Length < 1)
			{
				PrintUsage();
				return;
			}

			try
			{
				// Create configuration file object
	            ConfigFile cfgFile = new ConfigFile();

				// Load configuration file itself
				cfgFile.Load(args[0]);

				//Look up configuration for the middleware
				Config connConfig = cfgFile.LookupConfig("connection-config");

				// Create connection from configuration
				conn = Connection.Create(connConfig);

				// Connect to the GMSEC bus
				conn.Connect();

				// Output Info
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version= " + conn.GetLibraryVersion());

				// Lookup additional program configuration info
				Config programConfig = cfgFile.LookupConfig("program-config");

				// Get Program Config Values
				short loopTime   = (short) programConfig.GetIntegerValue("loop-time", 30);
				short updateRate = (short) programConfig.GetIntegerValue("update-rate", 30);

				// Create Subscriptions from subscriptions in configFile
				string subject1 = cfgFile.LookupSubscription("RECEIVE-LOG");
				string subject2 = cfgFile.LookupSubscription("SEND-LOG");

				//Subscription Callback
				LogCallback cb = new LogCallback();
				info.Add(conn.Subscribe(subject1, cb));
				info.Add(conn.Subscribe(subject2, cb));

				// Load Heartbeat Message Definition
				Message hbMsg = cfgFile.LookupMessage("C2CX-HEARTBEAT-REC");

				// Obtain publish rate field from heartbeat message
				short pubRate = 30;

				try {
					pubRate = hbMsg.GetI16Field("PUB-RATE").GetValue();
				}
				catch (GMSEC_Exception e) {
					Log.Warning("Unable to find PUB-RATE; will use default publish rate of " + pubRate + " seconds.");
				}

				// Output the program info
				Log.Info("Publishing for " + loopTime + " seconds.");
				Log.Info("Publishing Heartbeat Messages every "+ pubRate + " seconds.");

				// Start auto dispatcher
				conn.StartAutoDispatch();

				Log.Info("Start Time: " + DateTime.Now.ToString());

				// Publish Loop
				short heartbeatCount     = 0;
				short heartbeatCountdown = 0;

				for (; loopTime > 0; --loopTime )
				{
					// When Countdown reaches 0 publish heartbeat & reset
					if (heartbeatCountdown < 1)
					{
						++heartbeatCount;

						// Update Message Counter
						hbMsg.AddField(new I32Field("COUNTER", heartbeatCount));

						// Publish Heartbeat Message
						conn.Publish(hbMsg);

						// Output Heartbeat marker and reset the counter
						Log.Info("Published Heartbeat");

						heartbeatCountdown = pubRate;
					}

					// Decrement the Counters
					heartbeatCountdown -= updateRate;

					//Sleep for 1 second
					System.Threading.Thread.Sleep(updateRate * 1000);
				}

				// EndTime
				Log.Info("End Time: " + DateTime.Now.ToString());

				// Stop the auto dispatcher
				conn.StopAutoDispatch();
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("GMSEC Exception: " + e.ToString());
			}
			finally
			{
				Cleanup();
			}
		}


		static void Main(string[] args)
		{
			new Recorder().Run(args);
		}
	}
}

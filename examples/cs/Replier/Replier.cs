/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file Replier.cs
 * 
 * This a sample application built with the C# GMSEC API. This application
 * demonstrates several key components within the GMSEC API including 
 * loading from a xml file, producing a heartbeat, and request/reply
 * functionality using callbacks. This works in sync with Requestor.cs
 * 
*/


using System.Collections.Generic;

namespace Replier
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;


	class RequestReplyCallback : Callback
	{
		public ConfigFile cfgFile { get; set; }

		public override void OnMessage(Connection conn, Message msg)
		{
			try
			{
				OnMessageAux(conn, msg);
			}
			catch(Exception e)
			{
				Log.Error("[RequestReplyCallback::OnMessage]" + e.ToString());
			}
		}

		private void OnMessageAux(Connection conn, Message msg)
		{
			// We don't care about anything but request messages
		    if (msg.GetKind() != Message.MessageKind.REQUEST)
			{
                return;
            }

			try
			{
				// Obtain the DIRECTIVE-KEYWORD field from the message.  It holds the request count.
				string dirKey = msg.GetStringField("DIRECTIVE-KEYWORD").GetValue();

				Log.Info("[RequestReplyCallback.OnMessage] Received Request (" + dirKey + ")");

				// Lookup and load the config file directive response message definition
				Message tmpMsg = cfgFile.LookupMessage("DIRECTIVE-RESPONSE");

				// Create the reply message
				Message replyMsg = new Message(msg.GetSubject(), Message.MessageKind.REPLY);

				// Copy fields from the directive response template to our reply message
				tmpMsg.CopyFields(replyMsg);

				// Replace the DATA field with the directive key we are responding to
				replyMsg.AddField(new StringField("DATA", dirKey));

				// Replace the TIME-COMPLETE field with the correct time
				replyMsg.AddField(new StringField("TIME-COMPLETED", DateTime.Now.ToString()));

				// Send the reply
				conn.Reply(msg,replyMsg);

				Log.Info("Reply Sent (" + dirKey + ")");

				// Find and load the config file log message definition
				Message logMsg = cfgFile.LookupMessage("LOG-REP");

				// Fill the log message time
				logMsg.AddField(new StringField("EVENT-TIME", DateTime.Now.ToString()));

				// Fill the log message message text
				logMsg.AddField(new StringField("MSG-TEXT", dirKey));

				// Publish the log message
				conn.Publish(logMsg);

			}
			catch (GMSEC_Exception e)
			{
				Log.Error("[RequestReplyCallback.OnMessage] " + e.ToString());
			}
		}
	}


	class Replier : GmsecExample
	{
		private Connection conn;
		private ConfigFile cfgFile;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>();
 

		public Replier()
		{
			Config tmpCfg = new Config();
			Initialize(tmpCfg);
		}


		private void PrintUsage()
		{
				Console.Out.WriteLine("usage: Replier_cs <filename>\n");
		}


		private void Cleanup()
		{
			if (conn != null)
			{
                for (int i = info.Count-1; i >= 0; i--)
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
				// Instantiate Configuration File object
				cfgFile = new ConfigFile();

				// Load Config File
				cfgFile.Load(args[0]);

				//Instantiate Subscription Callback
				RequestReplyCallback cb = new RequestReplyCallback();    
				cb.cfgFile = cfgFile;

				// Look up configuration for the middleware
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
				short updateRate = (short) Get(programConfig, "update-rate", 1);
				short loopTime   = (short) Get(programConfig, "loop-time", 30);

				// Create Subscriptions, using callback, from subscription subject in configFile
				string subject = cfgFile.LookupSubscription("DIRECTIVE-REQUEST");
				info.Add(conn.Subscribe(subject, cb));

				// Load Heartbeat Definition
				Message hbMessage = cfgFile.LookupMessage("C2CX-HEARTBEAT-REP");

				// Obtain publish rate field from heartbeat message
				short pubRate = 30;
				try {
					pubRate = hbMessage.GetI16Field("PUB-RATE").GetValue();
				}
				catch (GMSEC_Exception e) {
					Log.Warning("Could not reference PUB-RATE from C2CX-HEARTBEAT-REP message definition.");
				}

				// Output some Program Info
				Log.Info("Publishing for " + loopTime + " seconds.");
				Log.Info("Publishing Heartbeat Messages every " + pubRate + " seconds.");

				// Start auto dispatcher
				conn.StartAutoDispatch();

				Log.Info("Start Time: " + DateTime.Now.ToString());

				// Publish Loop
				short heartbeatCount = 0;
				short heartbeatCountdown = 0;

				for (; loopTime > 0; --loopTime)
				{
					// When Countdown reaches 0 publish heartbeat & reset
					if (heartbeatCountdown < 1)
					{
						++heartbeatCount;

						// Update Message Counter
						hbMessage.AddField(new I32Field("COUNTER", heartbeatCount));

						// Publish Heartbeat Message
						conn.Publish(hbMessage);

						// Output Heartbeat marker and reset the counter
						Log.Info("Published Heartbeat");

						heartbeatCountdown = pubRate;
					}

					// Decrement the Counters
					heartbeatCountdown -= updateRate;

					// Sleep for updateRate second(s)
					System.Threading.Thread.Sleep(updateRate * 1000);
				}

				Log.Info("End Time: " + DateTime.Now.ToString());

				// Stop auto dispatcher
				conn.StopAutoDispatch();
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("GMSEC_Exception: " + e.ToString());
			}
			finally
			{
				Cleanup();
			}
		}


		static void Main(string[] args)
		{
			new Replier().Run(args);
		}
	}
}

/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmrpl_cb.cs
 *  A C# example demonstrating the reply/request abilities of GMSEC.
 *  This Reply example shows the use of callback functionality, which
 *  allows for code to be performed anytime a Message is recieved
*/


namespace gmrpl_cb
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Diagnostics;
	using System.Collections.Generic;


	class RequestCallback : Callback
	{
		public override void OnMessage(Connection conn, Message message)
		{
			//Check if REQUEST message was received
			if (message.GetKind() == Message.MessageKind.REQUEST)
			{
				try
				{
					string component    = message.GetStringField("COMPONENT").GetValue();
					string statusCode   = "1";
					string replySubject = "GMSEC.MISSION.SAT_ID.RESP." + component + "." + statusCode;

					// Create reply message
					Message reply = new Message(replySubject, Message.MessageKind.REPLY);

					// Add fields to the reply message
					reply.AddField(new StringField("ANSWER",    "Sure looks like it"));
					reply.AddField(new StringField("COMPONENT", "GMRPL_CB"));

					// Display XML representation of the reply message
					Log.Info("[RequestCallback:OnMessage] Prepared Reply Message:\n" + reply.ToXML());

					// Send reply
					conn.Reply(message, reply);
				}
				catch (GMSEC_Exception e)
				{
					Log.Error("[RequestCallback:OnMessage] Exception: " + e.ToString());
				}
			}
		}
	}


	class gmrpl_cb : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 

		public gmrpl_cb()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmrpl_cs_cb connectiontype=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameter 'connectiontype' is required. " + 
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +
				"\n\tsubject=<subject name>" +
				"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
				"\n\tprog_timeout_s=<timeout period (seconds)>" +
				"\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s" +
				"\n\tloglevel=<log level>" +
				"\n\tcfgfile=<config_filename.xml>" +
				"\n\nFor more information, see API User's Guide\n");
		}


		private void CleanUp()
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

                conn.Disconnect();

				Connection.Destroy(ref conn);
			}
		}


		private void Run(string[] args)
		{
			Config config = new Config(args);

			Initialize(config);

			AddToConfigFromFile(config);

			if (IsOptionInValid(config, args, true))
			{
				PrintUsage();
				return;
			}

			// Output GMSEC API version
			Log.Info(Connection.GetAPIVersion());

			try
			{
				// Create the connection
				conn = Connection.Create(config);

				// Connect
				conn.Connect();

				// Get CommandLine Info
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);

				// OutPut MiddlewareVersion
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				//Subscribe
				List<string> subjects = DetermineSubjects(config);

				RequestCallback rcb = new RequestCallback();
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to " + subjects[i]);
					info.Add(conn.Subscribe(subjects[i], rcb));
				}

				//Wait for messages
				bool done = false;
				Stopwatch stopWatch = new Stopwatch();
				while (!done)
				{
					if (prog_timeout_s != (int)GMSECWaitDefs.WAIT_FOREVER)
					{
						if (!stopWatch.IsRunning)
						{
							stopWatch.Start();
						}
						else if(stopWatch.ElapsedMilliseconds >= (long)(prog_timeout_s*1000))
						{
							Log.Info("Program Timeout Reached!");
							stopWatch.Stop();
							break;
						}
					}

					Message message = conn.Receive(msg_timeout_ms);

					if (message == null)
					{
						Log.Warning("timeout occurred");
						continue;
					}

					// Display XML represent of the received message
					Log.Info("Received Message:\n" + message.ToXML());

					// Dispatch the message so that our callback receives it
					conn.Dispatch(message);

					string subject = message.GetSubject();

					if (subject.Equals("GMSEC.TERMINATE"))
					{
						done = true;
					}
				}
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("Exception: " + e.ToString());
			}
			finally
			{
				CleanUp();
			}
		}


		static void Main(string[] args)
		{
			new gmrpl_cb().Run(args);
		}
	}
}

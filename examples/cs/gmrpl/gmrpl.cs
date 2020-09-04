/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmrpl.cs
 *
 * A C# example demonstrating the request/reply functionality.
 * The supplied gmreq_cs will provide the request.
 * gmrpl_cs  should be started before gmreq_cs.
*/


namespace gmrpl
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Diagnostics;
	using System.Collections.Generic;


	class gmrpl : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 

		public gmrpl()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmrpl_cs connectiontype=<middleware> " +
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

			try
			{
				// Output GMSEC API version
				Log.Info(Connection.GetAPIVersion());

				// Create the connection
				conn = Connection.Create(config);

				// Connect
				conn.Connect();

				// Get CommandLine Info
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);

				// OutPut MiddlewareVersion
				Log.Info("Middleware Version = "+conn.GetLibraryVersion());

				//Subscribe
				List<string> subjects = DetermineSubjects(config);

				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to " + subjects[i]);

					info.Add(conn.Subscribe(subjects[i]));
                }

				// Wait for messages
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
						else if (stopWatch.ElapsedMilliseconds >= (long)(prog_timeout_s * 1000))
						{
							Log.Info("Program Timeout Reached!");
							stopWatch.Stop();
							break;
						}
					}

					Message msg = conn.Receive(msg_timeout_ms);

					if (msg == null)
					{
						Log.Warning("timeout occurred");
						continue;
					}

					// Display XML represention of the received message
					Log.Info("Received Message:\n" + msg.ToXML());

					string subject = msg.GetSubject();

					if (subject.Equals("GMSEC.TERMINATE"))
					{
						done = true;
					}

                    // Check if the received message is a request message
					if (msg.GetKind() == Message.MessageKind.REQUEST)
					{
						// Construct a reply message
						string component    = msg.GetStringField("COMPONENT").GetValue();
						string statusCode   = "1";
						string replySubject = "GMSEC.MISSION.SAT_ID.RESP." + component + "." + statusCode;

						Message reply = new Message(replySubject, Message.MessageKind.REPLY);

						//Add fields to the reply message
						reply.AddField(new StringField("ANSWER", "Sure looks like it"));
						reply.AddField(new StringField("COMPONENT", "GMRPL"));

						if (config.GetBooleanValue("ENCRYPT", false))
						{
							reply.AddField(new BooleanField("ENCRYPT", true));
						}

						// Display XML representation of the reply message
						Log.Info("Prepared Reply Message:\n" + reply.ToXML());

						// Send Reply
						conn.Reply(msg, reply);
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
			new gmrpl().Run(args);
		}
	}
}

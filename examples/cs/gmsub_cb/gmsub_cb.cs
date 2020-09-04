/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmsub_cb.cs
 * 
 * A C# example demonstrating the subscription abilities of GMSEC. This
 * example involves the use of callback functionality, which allows for
 * code to be performed anytime a Message is recieved.
 * 
*/


namespace gmsub_cb
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Diagnostics;
	using System.Collections.Generic;


	class PublishCallback : Callback
	{
		public override void OnMessage(Connection conn, Message message)
		{
			// Display XML representation of the received message
			Log.Info("[PublishCallback.OnMessage] Received Message:\n" + message.ToXML());
		}
	}


	class gmsub_cb : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 

		public gmsub_cb()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmsub_cs_cb connectiontype=<middleware> " +
				"[ <parameter>=<value> ]\n" + 
				"\n\tNote that the parameter 'connectiontype' is required. " +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +	
				"\n\tsubject=<subject name>" +
				"\n\titerations=<iterations>" +
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

				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				//Determine the subjects to listen too
				List<string> subjects = DetermineSubjects(config);

				// Get program options
				int iterations     = Get(config, "ITERATIONS", 0);
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);

				if (iterations > 0)
				{
					Log.Info("Waiting for up to " + iterations + " messages");
				}

				// Subscribe using callback
				PublishCallback cb = new PublishCallback();
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to "+subjects[i]);
					info.Add(conn.Subscribe(subjects[i], cb));
				}

				// Wait & Print Out Messages
				bool done = false;
				int count = 0;
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

					Message message = conn.Receive(msg_timeout_ms);

					if (message == null)
					{
						Log.Warning("timeout occurred");
						continue;
					}

					// Received message!
					++count;

					if (iterations > 0 && count >= iterations)
					{
						done = true;
					}

					// Dispatch the message to our callback
					conn.Dispatch(message);

					string subject = message.GetSubject();

					if(subject.Equals("GMSEC.TERMINATE"))
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
			new gmsub_cb().Run(args);
		}
	}
}

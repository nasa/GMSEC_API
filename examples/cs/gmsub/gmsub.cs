/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmsub.cs
 * This file contains a simple example subscriber using the GMSEC .NET API.
 */


namespace gmsub
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Diagnostics;
	using System.Collections.Generic;


	class gmsub : GmsecExample
	{
		private Config     config;
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 

		public gmsub()
		{
		}


		private void PrintUsage()
		{
			Log.Error("\nusage: gmsub_cs connectiontype=<middleware> " +
			          "[ <parameter>=<value> ]\n" + 
			          "\n\tNote that the parameter 'connectiontype' is required. " +
			          "\n\tThe rest of other parameters are optional.\n" +
			          "\n\tserver=<server name> " +
			          "(required if middleware is not bolt/MB locally)" +	
			          "\n\tsubject=<subject name>" +
			          "\n\tsubject.<N>=<subject name>" +
			          "\n\t\tNote: N must be 1 or greater, and be consecutive." +
			          "\n\t\tFor example: subject.1=GMSEC.A subject.2=GMSEC.B ..." +
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
			if(conn != null)
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
			config = new Config(args);

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

				//Determine the subjects to listen too
				List<string> subjects = DetermineSubjects(config);

				// Get example program configurable options
				int iterations     = Get(config, "ITERATIONS", 0);
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);

				if (iterations > 0)
				{
					Log.Info("Waiting for up to " + iterations + " messages");
				}

				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				//Subscribe
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to "+subjects[i]);
					info.Add(conn.Subscribe(subjects[i]));
				}

				//Wait & Print Out Messages
				bool done = false;
				int count = 0;
				Stopwatch stopWatch = new Stopwatch();

				while (!done)
				{
					if (prog_timeout_s != (int) GMSECWaitDefs.WAIT_FOREVER)
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

					++count;

					if (iterations > 0 && count >= iterations)
					{
						done = true;
					}

					// Display XML representation of the received message
					Log.Info("Received:\n" + msg.ToXML());

					string subject = msg.GetSubject();

					if (subject.Equals("GMSEC.TERMINATE"))
					{
						done = true;
					}

					// Destroy message
					conn.Release(ref msg);
				}
			}
			catch (GMSEC_Exception e)
			{
				Log.Error(e.ToString());
			}
			finally
			{
				CleanUp();
			}
		}


		static void Main(string[] args)
		{
			new gmsub().Run(args);
		}
	}
}

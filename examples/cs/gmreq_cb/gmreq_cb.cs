/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




/*! \file gmreq_cb.cs
 * 
 *  A C# example program demostrating the request/reply functionality of
 *  GMSEC. This example shows the use of callback functionality, which allows
 *  for code to be performed anytime a Message is recieved.
 * 
*/


namespace gmreq_cb
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Diagnostics;


	class MyReplyCallback : ReplyCallback
	{
		public override void OnReply(Connection conn, Message request, Message reply)
		{
			++Used;

			Log.Info("[MyReplyCallback::OnReply] Received Reply Message:\n" + reply.ToXML());
		}


		public override void OnEvent(Connection conn, Status status, Connection.ConnectionEvent connEvent)
		{
			if (status.IsError())
			{
				Log.Error("[MyReplyCallback.OnError] Received Error: " + status.Get() + "[event=" + connEvent + "]");
			}
		}


		public int Used { get; set; }
	}


	class gmreq_cb : GmsecExample
	{
		private Config     config;
		private Connection conn;


		public gmreq_cb()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmreq_cs_cb connectiontype=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameter 'connectiontype' is required. " + 
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +
				"\n\tsubject=<subject name>" +
				"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
				"\n\tloglevel=<log level>" +
				"\n\tcfgfile=<config_filename.xml>" +
				"\n\nFor more information, see API User's Guide\n");
		}


		private void CleanUp()
		{
			if (conn != null)
			{
				conn.Disconnect();

				Connection.Destroy(ref conn);
			}
		}


		private void Request(int msg_timeout_ms, string subject)
		{
			// Create request message
			Message request = new Message(subject, Message.MessageKind.REQUEST);

			//Add Fields
			request.AddField(new StringField("QUESTION", "Does the request/reply functionality stil work?"));
			request.AddField(new StringField("COMPONENT", "GMREQ_CB"));

			if (config.GetBooleanValue("ENCRYPT", false))
			{
				request.AddField(new BooleanField("SEC-ENCRYPT", true));
			}

			// Display XML representation of the request message
			Log.Info("Prepared Request Message:\n" + request.ToXML());

			// Send the request message using a callback
			MyReplyCallback rcb = new MyReplyCallback();
            rcb.Used = 0;

			conn.Request(request, msg_timeout_ms, rcb, 0);
			
			// Verify reply was received
			Stopwatch stopWatch = new Stopwatch();

			while (rcb.Used == 0)
			{
				System.Threading.Thread.Sleep(100);

				if (msg_timeout_ms != (int)GMSECWaitDefs.WAIT_FOREVER)
				{
					if (!stopWatch.IsRunning)
					{
						stopWatch.Start();
					}
					else if (stopWatch.ElapsedMilliseconds >= (long) msg_timeout_ms)
					{
						stopWatch.Stop();
						break;
					}
				}
			}

			if (rcb.Used == 1)
			{
				Log.Info("A response was received");
			}
			else
			{
				Log.Warning("A response was NOT received");
			}
		}


		private void Run(string[] args)
		{
			config = new Config(args);

			Initialize(config);

			AddToConfigFromFile(config);

			if (IsOptionInValid(config, args, false))
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
				string subject     = Get(config, "SUBJECT", "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL");
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);

				// OutPut MiddlewareVersion
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// OutPut Info
				Log.Info("Using Subject '" + subject + "'");

				// Create request message and issue such
				Request(msg_timeout_ms, subject);
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
			new gmreq_cb().Run(args);
		}
	}
}

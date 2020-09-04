/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




/*! \file gmreq.cs
 * 
 * A C# example demonstrating the GMSEC request/reply functionality.
 * The associated example gmrpl_cs will provide the reply.
 * gmrpl_cs should be run before gmreq_cs
 * 
*/


namespace gmreq
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;


	class gmreq : GmsecExample
	{
		private Config     config;
		private Connection conn;


		public gmreq()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmreq_cs connectiontype=<middleware> " +
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

			// Add fields to request message
			request.AddField(new StringField("QUESTION", "Does the request/reply functionality still work?"));
			request.AddField(new StringField("COMPONENT", "GMREQ"));

            if (config.GetBooleanValue("ENCRYPT", false))
            {
				request.AddField(new BooleanField("SEC-ENCRYPT", true));
            }

			// Display XML representation of the request message
			Log.Info("Prepared Request Message:\n" + request.ToXML());

            // Issue request message
            Message reply = conn.Request(request, msg_timeout_ms, 0);

			if (reply == null)
			{
				Log.Warning("request timed out");
			}
			else
			{
				// Display XML representation of the reply message
				Log.Info("Received Reply Message:\n" + reply.ToXML());

				// Destroy Message
				conn.Release(ref reply);
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
				Log.Info("Middleware Version = " + conn.GetLibraryVersion());

				// OutPut Info
				Log.Info("Using Subject '" + subject + "'");

				// Prepare request message and issue request
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
			new gmreq().Run(args);
		}
	}
}

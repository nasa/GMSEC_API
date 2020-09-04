/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmsub_disp_rr.cs
 *  A C# example subscriber using the GMSEC API dispatcher. The callback
 *  is registered with the connection. A seperate thread, transmits a request
 *  and waits for a reply. THe Dispatcher is then started, which launches its
 *  own thread.
*/


namespace gmsub_disp_rr
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
			Log.Info("[PublishCallback.OnMessage] Received Message:\n" + message.ToXML());
		}
	}


	class MyReqReplyCallback : ReplyCallback
	{
		public override void OnReply(Connection conn, Message request, Message reply)
		{
			Log.Info("[MyReqReplyCallback.OnReply] Received Reply Message:\n" + reply.ToXML());
		}

		public override void OnEvent(Connection conn, Status status, Connection.ConnectionEvent connEvent)
		{
			if (status.IsError())
			{
				Log.Error("[MyReqReplyCallback:OnEvent] Status=" + status.Get() + " [event=" + connEvent + "]");
			}
		}
	}


	class gmsub_disp_rr : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>();
 

		public gmsub_disp_rr()
		{
		}

		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmsub_cs_disp_rr connectiontype=<middleware> " +
				"[ <parameter>=<value> ]\n" + 
				"\n\tNote that the parameter 'connectiontype' is required. " +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +	
				"\n\tsubject=<subject name>" +
				"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
				"\n\tprog_timeout_s=<timeout period (seconds)>" +
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

			if (IsOptionInValid(config, args, false))
			{
				PrintUsage();
				return;
			}

			config.AddValue("isThreaded", "true");

			// Output GMSEC API version
			Log.Info(Connection.GetAPIVersion());

			try
			{
				// Create the connection
				conn = Connection.Create(config);

				// Connect
				conn.Connect();

				// Output the middleware info
				Log.Info("Middleware version = " + conn.GetLibraryVersion());


				// Determine the subjects to listen too
				List<string> subjects = DetermineSubjects(config);

				//Determine Timeout Periods
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);

				// Subscribe
				PublishCallback cb = new PublishCallback();
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to "+subjects[i]);
					info.Add(conn.Subscribe(subjects[i], cb));
				}

				Log.Info("Starting AutoDispatch");
				conn.StartAutoDispatch();

				string subject = "GMSEC.TEST.PUBLISH.NOREPLY";
				if (subjects[0] != "GMSEC.>")
				{
					subject = subjects[0];
				}

				// Create request message
				Message request = new Message(subject, Message.MessageKind.REQUEST);

				// Display XML representation of the request message
				Log.Info("Sending Request Message:\n" + request.ToXML());

				// Issue request using custom ReplyCallback
				MyReqReplyCallback rcb = new MyReqReplyCallback();
				conn.Request(request, msg_timeout_ms, rcb, 0);

				if (prog_timeout_s == (int)GMSECWaitDefs.WAIT_FOREVER)
				{
					Console.Out.WriteLine("Press the Enter Key to Exit");
					Console.ReadKey();
				}
				else
				{
					System.Threading.Thread.Sleep(prog_timeout_s * 1000);

					Log.Info("Program Timeout Reached!");
				}

				Log.Info("Stopping AutoDispatch");
				conn.StopAutoDispatch();
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("Exception: " + e.ToString());
			}
			catch (Exception e)
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
			new gmsub_disp_rr().Run(args);
		}
	}
}

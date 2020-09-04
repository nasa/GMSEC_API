/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file gmsub_disp.cs
 *  A C# example demonstrating the subscribe capabilities of the
 *  GMSEC API using the Autodispatch routine.
*/


namespace gmsub_disp
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Collections.Generic;


	class PublishCallback : Callback
	{
		public override void OnMessage(Connection conn, Message message)
		{
			Log.Info("[PublishCallback.OnMessage] Received Message:\n" + message.ToXML());

			Console.Out.WriteLine("Press Enter to Exit");
		}
	}


	class gmsub_disp : GmsecExample
	{
		private Connection conn;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 


		public gmsub_disp()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nusage: gmsub_cs_disp connectiontype=<middleware> " +
				"[ <parameter>=<value> ]\n" + 
				"\n\tNote that the parameter 'connectiontype' is required. " +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +	
				"\n\tsubject=<subject name>" +
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

				// Output middleware information
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Determine the subjects to listen too
				List<string> subjects = DetermineSubjects(config);

				//Subscribe
				PublishCallback cb = new PublishCallback();
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to "+subjects[i]);

					info.Add(conn.Subscribe(subjects[i], cb));
				}

				Log.Info("Starting AutoDispatch");
				conn.StartAutoDispatch();

				Console.Out.WriteLine("Press Enter to Exit");

				//Wait for User Input
				Console.ReadKey();

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
			new gmsub_disp().Run(args);
		}
	}
}

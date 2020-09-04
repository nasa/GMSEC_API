/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*! \file Requestor.cs
 * 
 * This a sample application built with the C# GMSEC API. This application
 * demonstrates several key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request/reply functionality
 * using callbacks. This works in sync with Replier.cs
 * 
*/


namespace Requestor
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;


	class RequestReplyCallback : ReplyCallback
	{
		public ConfigFile cfgFile { get; set; }


		public override void OnReply(Connection conn, Message request, Message reply)
		{
			try
			{
            	// Get the status of the response (successful, failed, etc) act accordingly
				short  status = reply.GetI16Field("RESPONSE-STATUS").GetValue();
				string data   = "";

				switch (status)
				{
				case 1:
					Log.Info("\nAcknowledgement");
					break;
				case 2:
					Log.Info("\nWorking/keep alive");
					break;
				case 3:
					// Obtain the DATA field from the reply message
					data = reply.GetStringField("DATA").GetValue();
					Log.Info("\nReceived Reply (DATA=" + data + "): " + DateTime.Now.ToString());
					break;
				case 4:
					Log.Info("\nFailed completion");
					break;
				case 5:
					Log.Info("\nInvalid Request");
					break;
				default:
					break;
				}

				// Lookup and load the log message definition from the config file
		    	Message logMessage = cfgFile.LookupMessage("LOG-REQ");

				// Fill the log message time
				logMessage.AddField(new StringField("EVENT-TIME", DateTime.Now.ToString()));

				// Fill the log message message text
				logMessage.AddField(new StringField("MSG-TEXT", data));

				// Publish the log message
				conn.Publish(logMessage);
			}
			catch (Exception e)
			{
				Log.Error("[RequestReplyCallback::OnReply]" + e.ToString());
			}
		}


		public override void OnEvent(Connection conn, Status status, Connection.ConnectionEvent connEvent)
		{
			if (status.IsError())
			{
				Log.Error("[RequestReplyCallback::OnEvent]" + status.Get() + "[event=" + connEvent + "]");
			}
		}
	}


	class Requestor : GmsecExample
	{
		private Connection conn;


		public Requestor()
		{
			Config tmpCfg = new Config();
			Initialize(tmpCfg);
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine("usage: Requestor_cs <filename>\n");
		}


		private void Cleanup()
		{
			if (conn != null)
			{
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
				// Instantiate ConfigFile
	            ConfigFile cfgFile = new ConfigFile();

				// Load Config File
				cfgFile.Load(args[0]);

				// Instantiate Subscription Callback
				RequestReplyCallback cb = new RequestReplyCallback();    
				cb.cfgFile = cfgFile;

				// Look up connection configuration for the middleware
				Config connConfig = cfgFile.LookupConfig("connection-config");

				// Create connection from configuration
				conn = Connection.Create(connConfig);

				// Connect to the GMSEC bus
				conn.Connect();

				// Output Info
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version= " + conn.GetLibraryVersion());

				// Lookup additional program configuration info
				Config progConfig = cfgFile.LookupConfig("program-config");

				// Get Program Config Values
				short updateRate      = (short) Get(progConfig, "update-rate", 1);
				short loopCountdown   = (short) Get(progConfig, "loop-time", 30);
				short directiveHolder = (short) Get(progConfig, "send-directive-rate", 5);

				// Load Heartbeat Definition
				Message hbMessage = cfgFile.LookupMessage("C2CX-HEARTBEAT-REQ");

				short heartbeatHolder = 30;

				try {
					heartbeatHolder = hbMessage.GetI16Field("PUB-RATE").GetValue();
				}
				catch (GMSEC_Exception) {
					Log.Warning("Unable to find PUB-RATE from C2CX-HEARTBEAT-REQ message template");
				}

				// Create generic message container for the Directive Request Message
				Message directiveRequestMessage = cfgFile.LookupMessage("DIRECTIVE-REQUEST");

				// Output some Program Info
				Log.Info("Publishing for " + loopCountdown + " seconds.");
				Log.Info("Publishing Directive Request Messages every " + directiveHolder + " seconds.");
				Log.Info("Publishing Heartbeat Messages every " + heartbeatHolder + " seconds.");

				Log.Info("Start Time: " + DateTime.Now.ToString());

				//Publish Loop
				short requestCount = 0;
				short heartbeatCount = 0;
				short directiveCountdown = 0;
				short heartbeatCountdown = 0;

				for (; loopCountdown > 0; --loopCountdown)
				{
					// When Countdown reaches 0 publish a directive message & reset
					if (directiveCountdown < 1)
					{
						++requestCount;

						// Alter the directive request to a unique setting
						directiveRequestMessage.AddField(new StringField("DIRECTIVE-KEYWORD", requestCount.ToString()));

						//Alter the directive request to a unique string
						directiveRequestMessage.AddField(new StringField("DIRECTIVE-STRING", "DIRECTIVE REQUEST MESSAGE " + requestCount.ToString()));
                    
						Log.Info("Issuing Directive Request Message (" + requestCount + ")");
						conn.Request(directiveRequestMessage,(int) GMSECWaitDefs.WAIT_FOREVER, cb, 0);
                    
						//Reset the countdown
						directiveCountdown = directiveHolder;
					}

					// When the countdown reaches 0, we publish a heartbeat message & reset
					if (heartbeatCountdown < 1)
					{
						++heartbeatCount;

						// Update Message Counter
						hbMessage.AddField(new StringField("COUNTER", heartbeatCount.ToString()));

						// Publish Heartbeat Message
						conn.Publish(hbMessage);

						// Output Heartbeat marker and reset the counter
						Log.Info("Published Heartbeat");

						heartbeatCountdown = heartbeatHolder;
					}

					//Decrement the Counters
					heartbeatCountdown -= updateRate;
					directiveCountdown -= updateRate;

					//Sleep for update # seconds
					System.Threading.Thread.Sleep(updateRate * 1000);
				}

				// EndTime
				Log.Info("End Time: " + DateTime.Now.ToString());
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("GMSEC_Exception: " + e.ToString());
			}
        }


		static void Main(string[] args)
		{
			new Requestor().Run(args);
		}
	}
}

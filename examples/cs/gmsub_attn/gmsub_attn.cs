/*
 * Copyright 2007-2014 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * All Rights Reserved.
 */

/*! \file gmsub_attn.cs
 *  This file contains a simple example subscriber using the GMSEC .NET API.
 */


namespace gmsub_attn
{
	using GMSEC.API;
	using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;
	using System.Linq;
	using System.IO;
	using System.Diagnostics;
	using System.Collections.Generic;
	using System.Configuration;
	using System.Management;

	using System.Net.Sockets;


	class gmsub_attn : GmsecExample
	{
		private readonly string ATTENTION_DELIMITER_DEFAULT = "***";

		private Connection conn;
		Queue<string>      includeFields;
		string             delimiterString;
        private List<SubscriptionInfo> info = new List<SubscriptionInfo>(); 


		public gmsub_attn()
		{
		}


		private void PrintUsage()
		{
			Console.Out.WriteLine(
				"\nUsage: gmsub_attn_cs connectiontype=<middleware> " +
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
				"\n\tattn-port=<Attention! server port>" +
				"\n\tattn-server=<Attention! server host>" +
				"\n\tinclude-fields=<comma-separated list of field names," +
				"\n\t\tthe content of which will be sent to the Attention!" +
				"\n\t\tserver>" +
				"\n\tdelimiter-string=<Attention! server string delimiter>" +
				"\n\nFor more information, see API User's Guide\n\n");
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

			string server = Get(config, "ATTN-SERVER", "127.0.0.1");
			int    port   = 0;

			try
			{
				string strPort = Get(config, "ATTN-PORT", "4999");
				port = Convert.ToInt32(strPort);
			}
			catch (FormatException fe)
			{
				Log.Error("FormatException : " + fe.ToString());
				Log.Error("Value input for ATTN-PORT is not a valid sequence of digits.");
				Environment.Exit(1);
			}
			catch (OverflowException oe)
			{
				Log.Error("OverflowException : " + oe.ToString());
				Log.Error("The number cannot fit in an Int32.");
				Environment.Exit(1);
			}

			int    connectionAttempts = 0;
			Socket socket = null;

			while (connectionAttempts++ < 3)
			{
				System.Net.IPHostEntry ipHostInfo = System.Net.Dns.GetHostEntry(server);
				System.Net.IPAddress   ipAddress  = ipHostInfo.AddressList[0];
				System.Net.IPEndPoint  ipEndPoint = new System.Net.IPEndPoint(ipAddress, port);

				socket = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

				try
				{
					socket.Connect(ipEndPoint);
					connectionAttempts = 0;
					break;
				}
				catch (ArgumentNullException ae)
				{
					Console.Out.WriteLine("ArgumentNullException : {0}", ae.ToString());
					Environment.Exit(1);
				}
				catch (SocketException se)
				{
					Console.Out.WriteLine("SocketException : {0}", se.ToString());
					Console.Out.WriteLine("Trying to connect again (Attempt " + connectionAttempts + " of 3)\n", se.ToString());
					System.Threading.Thread.Sleep(1000);
					continue;
				}
				catch (Exception e)
				{
					Console.Out.WriteLine("Unexpected exception : {0}", e.ToString());
					Environment.Exit(1);
				}
			}

			if (connectionAttempts >= 3)
			{
				Log.Error("Unable to connect to the Attention Server!");
				Environment.Exit(1);
			}

			Log.Info("Connected to Attention Server: " + server + ":" + port);

			// Output GMSEC API version
			Log.Info(Connection.GetAPIVersion());

			try
			{
				// Create the connection
				conn = Connection.Create(config);

				// Connect
				conn.Connect();

				//Determine the subjects to listen to
				List<string> subjects = DetermineSubjects(config);

				// Determine the message field names to insert into the delimited string
				// in the case of an overflow situation (>1000 characters)
				includeFields   = DetermineIncludeFields(config);
				delimiterString = DetermineDelimiterString(config);

				// Get program configurable settings
				int msg_timeout_ms = Get(config, "MSG_TIMEOUT_MS", (int) GMSECWaitDefs.WAIT_FOREVER);
				int prog_timeout_s = Get(config, "PROG_TIMEOUT_S", (int) GMSECWaitDefs.WAIT_FOREVER);
				int iterations     = Get(config, "ITERATIONS", 0);

				if (iterations > 0)
				{
					Log.Info("Waiting for up to " + iterations + " messages");
				}

				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				//Subscribe
				for (int i = 0; i < subjects.Count; i++)
				{
					Log.Info("Subscribing to " + subjects[i]);
					info.Add(conn.Subscribe(subjects[i]));
				}

				bool done  = false;
				int  count = 0;

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

					Message message = conn.Receive(msg_timeout_ms);

					if (message == null)
					{
						Log.Warning("Timeout Occurred");
						continue;
					}

					// Got a message!

					++count;

					if (iterations > 0 && count >= iterations)
					{
						done = true;
					}

					// Get the subject of the message
					string subject = message.GetSubject();

					if (subject.Equals("GMSEC.GPM.GPM1.MSG.LOG.WDOG", StringComparison.CurrentCultureIgnoreCase) ||
					    subject.Equals("GMSEC.GPM.GPM1.MSG.C2CX.LSIMSS.HB", StringComparison.CurrentCultureIgnoreCase))
					{
						Log.Debug("Received Message:\n" + message.ToXML());
					}

					// Create the delimited string that will be sent to the Attention Generic Agent
					string attnArgs = GetAttnArgString(message);

					byte[] data = System.Text.Encoding.UTF8.GetBytes(attnArgs);
					try
					{
						int bytesSent = 0;

						while (bytesSent < data.Length)
						{
							// Blocks until send returns.
							int result = socket.Send(data, bytesSent, data.Length - bytesSent, SocketFlags.None);

							if (result > 0)
							{
								bytesSent += result;
							}
							else if (result < 0)
							{
								Log.Warning("Tried to write more data than was in the buffer!");
							}
						}

						// Wait for an ack (Operation blocks until data is received, no need for other logic)
						int bytesRec = socket.Receive(data);
					}
					catch (SocketException e)
					{
						Console.Out.WriteLine("{0} Error code: {1}.", e.Message, e.ErrorCode);
						break;
					}

					if (subject.Equals("GMSEC.TERMINATE"))
					{
						done = true;
					}

					// Destroy message
					conn.Release(ref message);
				}
			}
			catch (GMSEC_Exception e)
			{
				Log.Error("GMSEC_Exception: " + e.ToString());
			}
			finally
			{
				CleanUp();
			}
		}


		private string GetAttnArgString(Message message)
		{
			// 5.0;gpm-auto1|<First field>|3***<Rest of the message fields>

			string attnArgs  = "5.0;gpm-auto1|";
			int    counter   = 0;
			bool   firstIter = true;

			foreach (string fieldName in includeFields)
			{
				string fieldValue = "";

				if (fieldName == "SUBJECT")
				{
					fieldValue = message.GetSubject();
				}
				else
				{
					try
					{
						Field field = message.GetField(fieldName);

						switch (field.GetType())
						{
						case Field.FieldType.BOOL_TYPE:
							fieldValue = ((BinaryField) field).GetValue().ToString();
							break;

						case Field.FieldType.CHAR_TYPE:
							fieldValue = ((CharField) field).GetValue().ToString();
							break;

						case Field.FieldType.F32_TYPE:
							fieldValue = ((F32Field) field).GetValue().ToString();
							break;

						case Field.FieldType.F64_TYPE:
							fieldValue = ((F64Field) field).GetValue().ToString();
							break;

						case Field.FieldType.I16_TYPE:
							fieldValue = ((I16Field) field).GetValue().ToString();
							break;

						case Field.FieldType.I32_TYPE:
							fieldValue = ((I32Field) field).GetValue().ToString();
							break;

						case Field.FieldType.I64_TYPE:
							fieldValue = ((I64Field) field).GetValue().ToString();
							break;

						case Field.FieldType.STRING_TYPE:
							fieldValue = ((StringField) field).GetValue();
							break;

						case Field.FieldType.BIN_TYPE:
						case Field.FieldType.I8_TYPE:
						case Field.FieldType.U16_TYPE:
						case Field.FieldType.U32_TYPE:
						case Field.FieldType.U64_TYPE:
						case Field.FieldType.U8_TYPE:
							fieldValue = "NO " + fieldName;
							break;

						// TODO: Unsigned short, int, long, unset, bin, compound
						}
					}
					catch (GMSEC_Exception e)
					{
						Log.Warning("Field with name '" + fieldName + "' does not exist in message. [" + e.ToString() + "]");
						fieldValue = "NO " + fieldName;
					}
				}

				if (!String.IsNullOrEmpty(fieldValue))
				{
					attnArgs += fieldValue;
				}
				else
				{
					attnArgs += "NO " + fieldName;
				}

				if (firstIter)
				{
					attnArgs  += "|3";
					firstIter  = false;
				}

				if (++counter < includeFields.Count)
				{
					attnArgs += delimiterString;
				}
			}

			return attnArgs;
		}


		// Parse out the overflow field names from the comma-delimited string
		public Queue<string> DetermineIncludeFields(Config cfg)
		{
			Queue<string> fields = new Queue<string>();
			string inclFields = Get(cfg, "INCLUDE-FIELDS");
			string[] inclFieldsSplit = inclFields.Split(',');

			Console.Out.WriteLine("The following fields will be concatenated:");
			foreach (string fieldName in inclFieldsSplit)
			{
				if (fieldName.Length != 0)
				{
					Console.Out.WriteLine(fieldName);
					fields.Enqueue(fieldName);
				}
			}
			Console.Out.WriteLine();

			return fields;
		}


		// Determine the delimiter used in the formatted string
		public string DetermineDelimiterString(Config cfg)
		{
			string delimiterString = Get(cfg, "DELIMITER-STRING");

			if (delimiterString == null)
			{
				delimiterString = ATTENTION_DELIMITER_DEFAULT;
			}

			Console.Out.WriteLine("Delimiter string is set to: " + delimiterString + "\n");

			return delimiterString;
		}


		static void Main(string[] args)
		{
			new gmsub_attn().Run(args);
		}
	}
}


/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/*! \file gmpub.cs
 *  This file contains a simple example publisher using the GMSEC.NET API.
 */



namespace gmpub
{
	using GMSEC.API;
    using GMSEC.API.UTIL;
	using GMSEC.API.Example.Common;
	using System;

	class gmpub : GmsecExample
	{
		private Config     config;
		private Connection conn;


		public gmpub()
		{
		}


		private void PrintUsage()
		{
			Log.Error("\nusage: gmpub_cs connectiontype=<middleware> " +
			          "[ <parameter>=<value> ]\n" +
			          "\n\tNote that the parameter 'connectiontype' is required. " +
			          "\n\tThe rest of other parameters are optional.\n" +
			          "\n\tserver=<server name> " +
			          "(required if middleware is not bolt/MB locally)" +
			          "\n\tsubject=<subject name>" +
			          "\n\titerations=<iterations>" +
			          "\n\tinterval_ms=<interval (milliseconds)>" +
			          "\n\tloglevel=<log level>" +
			          "\n\tcfgfile=<config_filename.xml>" +
			          "\n\nFor more information, see API User's Guide\n");
		}


        private void Publish(int iteration, String subject)
        {
            int value = iteration + 1;

            using (Message msg = new Message(subject, Message.MessageKind.PUBLISH))
			{
            	// Add Fields
				msg.AddField(new CharField("CHAR-FIELD", (sbyte) 'c'));

				msg.AddField(new BooleanField("BOOL-FIELD-TRUE", true));
				msg.AddField(new BooleanField("BOOL-FIELD-FALSE", false));

				msg.AddField(new I8Field("I8-FIELD",   (sbyte) value));
				msg.AddField(new I16Field("I16-FIELD", (Int16) value));
				msg.AddField(new I32Field("I32-FIELD",         value));
				msg.AddField(new I64Field("I64-FIELD", (Int64) value));

				msg.AddField(new U8Field("U8-FIELD",   (byte)   value));
				msg.AddField(new U16Field("U16-FIELD", (UInt16) value));
				msg.AddField(new U32Field("U32-FIELD", (UInt32) value));
				msg.AddField(new U64Field("U64-FIELD", (UInt64) value));

				msg.AddField(new F32Field("F32-FIELD", (Single)(1 + 1.0 / value)));
				msg.AddField(new F64Field("F64-FIELD", (Double)(1 + 1.0 / value)));

				msg.AddField(new StringField("STRING-FIELD", "This is a test"));

				msg.AddField(new BinaryField("BIN-FIELD", System.Text.Encoding.ASCII.GetBytes("JLMNOPQ")));

				msg.AddField(new I32Field("COUNT", iteration));

				try {
					if (config.GetBooleanValue("ENCRYPT"))
					{
						msg.AddField(new BooleanField("SEC-ENCRYPT", true));
					}
				}
				catch (GMSEC_Exception e) {
					Log.Debug(e.ToString());
				}

				// Publish Message
				conn.Publish(msg);

				// Display XML representation of the message
				Log.Info("Published GMSEC Message:\n" + msg.ToXML());
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

			// Output GMSEC API version
			Log.Info(Connection.GetAPIVersion());

			try
			{
				// Create the connection
				conn = Connection.Create(config);

				// Connect
				conn.Connect();

				//Get Command Line Information
				String subject  = Get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
				int iterations  = Get(config, "ITERATIONS", 1);
				int interval_ms = Get(config, "INTERVAL_MS", 1000);
            
				// Output middleware version & Info
				Log.Info("Middleware version = " + conn.GetLibraryVersion());
				Log.Info("Using Subject '" + subject + "'");
				Log.Info("Publishing " + iterations + " message(s)");
            
				if (iterations > 1)
				{
					Log.Info("Publishing Interval "+interval_ms+" [ms]");
				}

				for (int i = 0; i < iterations; ++i)
				{
					Publish(i, subject);

					if (i < iterations - 1)
					{
						System.Threading.Thread.Sleep(interval_ms);
					}
				}
			}
			catch (GMSEC_Exception e)
			{
				Log.Error(e.ToString());
			}
			finally
			{
				if (conn != null)
				{
					// Disconnect
					conn.Disconnect();

					// Destroy the Connection
					Connection.Destroy(ref conn);
				}
			}
		}


		static void Main(string[] args)
		{
			new gmpub().Run(args);
		}
	}
}

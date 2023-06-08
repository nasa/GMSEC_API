/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file publish_subject_filter.cs
 * 
 * This file contains an example publishing a few messages with different
 * subjects in order to demonstrate the subject filtering functionality of the
 * API. 
 *
 * This example program is intended to be run after starting the
 * 'subscribe_subject_filter' example program.
 */

using GMSEC.API5;
using System;
using System.Text;


public class publish_subject_filter
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "PUBLISH-SUBJECT-FILTER");

				// Establish connection to the GMSEC Bus.
				conn.Connect();
		
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a valid Log message using the MessageFactory.
				Message logMsg = conn.GetMessageFactory().CreateMessage("LOG");

				// Add required fields
				logMsg.AddField("SUBCLASS", "INFO");
				logMsg.AddField("OCCURRENCE-TYPE", "TEST");
				logMsg.AddField("SEVERITY", (short) 1);
				logMsg.AddField("MSG-TEXT", "Hello subscriber!");

				// And the current (event) time
				logMsg.AddField("EVENT-TIME", TimeUtil.FormatTime( TimeUtil.GetCurrentTime() ));

				// Create a valid Heartbeat message using the MessageFactory.
				Message hbMsg = conn.GetMessageFactory().CreateMessage("HB");

				Log.Info("Publishing two messages -- Log message will be received by the subscriber,"
			   	      + " the Heartbeat message will be filtered out");

				// Publish the Log message
				conn.Publish(logMsg);
				Log.Info("Published LOG message:\n" + logMsg.ToXML());

				// Delay a bit
				TimeUtil.Millisleep(2000);

				// Publish the Heartbeat message
				conn.Publish(hbMsg);
				Log.Info("Published HB message:\n" + hbMsg.ToXML());

				// Disconnect from the GMSEC Bus.
				conn.Disconnect();
			}
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.Error("GmsecException: " + e.ToString());
			return -1;
		}

		return 0;
	}


	private static void InitializeLogLevel(Config config)
	{
		LogLevel level = Log.FromString( config.GetValue("loglevel", "info") );
		Log.SetReportingLevel(level);
	}


	// Convenience function for associating standard (i.e. common) fields
	// with the MessageFactory which will be included automatically within
	// all messages created using the factory.
	private static void SetStandardFields(MessageFactory factory, string component)
	{
		FieldList standardFields = new FieldList()
		{
			new StringField("MISSION-ID", "MY-MISSION", true),
			new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true),
			new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true),
			new StringField("CONSTELLATION-ID", "MY-CNST", true),
			new StringField("FACILITY", "MY-FACILITY", true),
			new StringField("DOMAIN1", "MY-DOMAIN-1", true),
			new StringField("DOMAIN2", "MY-DOMAIN-2", true),
			new StringField("COMPONENT", component, true)
		};

		factory.SetStandardFields(standardFields);
	}
}

/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/** 
 * @file specification_addendum.cs
 * 
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

using GMSEC.API5;
using System;
using System.Text;


public class specification_addendum
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options from the command line.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);


		// Enable Message validation. This setting is "false" by default.
		config.AddValue("GMSEC-MSG-CONTENT-VALIDATE-SEND", "true");


		// Tell the API that there is an additional layer of message schema to
		// validate (The 'EXAMPLE' message definitions). By default, this value
		// is set to 2 (only 'GMSEC' specification validation).
		//
		// Note: These levels for validation are determined by the "LEVEL-X"
		// attributes defined in the DIRECTORY.xml file contained in the XML
		// templates directory. In the case of this example, Level-0 implies
		// C2MS, and Level-1 is our example extension.
		//
		// Note: The GMSEC team envisions using message specifications in a
		// layered hierarchical fashion. For example, the "C2MS" message
		// specification would be 'layer 0', followed by an organization-level
		// message specification at 'layer 1' which builds upon the message
		// specification outlined in C2MS.  This would be followed by a mission
		// level message specification at 'layer 2' (e.g. GMSEC) and so on.
		config.AddValue("GMSEC-SCHEMA-LEVEL", "1");


		// Tell the API where to find all of the schema files.
		//
		// Note: This example only demonstrates a simple usage case of this
		// functionality. When using this functionality, if the intent is
		// to use any of the GMSEC message definitions, then ALL of the XML
		// template files must be contained in the same directory.
		// e.g. GMSEC_API/templates/2019.00 (or the location defined in
		// GMSEC-SCHEMA-PATH)
		config.AddValue("GMSEC-SCHEMA-PATH", "templates");


		// Since this example relies on the 2019.00 version of the templates,
		// we indicate such within the configuration object.
		config.AddValue("GMSEC-SPECIFICATION-VERSION", "201900");


		try
		{
			// Create connection instance using custom configuration.
			using (Connection conn = new Connection(config))
			{
				// Set up standard fields within the MessageFactory associated
				// with the connection object.
				SetStandardFields(conn.GetMessageFactory(), "SPECIFICATION-ADDENDUM");

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Output information regarding the API version and connection.
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Create a LOG Message using a subject defined in the XML template
				// outlining our example addendum message definitions.
				Message logMsg = conn.GetMessageFactory().CreateMessage("LOG");

				// Add all of the required fields to the LOG message.
				logMsg.AddField("NUM-OF-EVENTS", (ushort) 2);
				logMsg.AddField("EVENT.1.DESC", AddTimeToString("AOS occurred at: "));
				logMsg.AddField("EVENT.2.DESC", AddTimeToString("Telemetry downlink began at: "));

				// Publish the LOG message to the GMSEC Bus.
				conn.Publish(logMsg);

				// Display the XML string representation of the LOG message just published.
				Log.Info("Published custom LOG Message:\n" + logMsg.ToXML());


				// Setup a new LOG message without the NUM-OF-EVENTS required field and
				// attempt to publish it (i.e. trigger a validation failure)
				Message badLogMsg = conn.GetMessageFactory().CreateMessage("LOG");

				try
				{
					conn.Publish(badLogMsg);
					Log.Warning("Surprisingly was able to publish an invalid message:\n" + badLogMsg.ToXML());
				}
				catch (GmsecException e)
				{
					Log.Info("This validation error is expected for the incomplete LOG message:\n" + e.ToString());
				}

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


	// Convenience method for associating standard (i.e. common) fields
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


	private static string AddTimeToString(string preface)
	{
		return preface + TimeUtil.FormatTime(TimeUtil.GetCurrentTime());
	}
}
